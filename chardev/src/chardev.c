#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <asm/io.h>


// timer
#define TIMEOUT_MS (3000)
static struct timer_list char_timer;
void timer_callback(struct timer_list * timer);
void timer_callback(struct timer_list * timer)
{
	printk(KERN_INFO "timer_callback at %lu jiffies == %d ms\n", 
		jiffies, jiffies_to_msecs(jiffies));
	mod_timer(&char_timer, jiffies+msecs_to_jiffies(TIMEOUT_MS));

}

 
// kthread
static struct task_struct *char_thread1;
int char_thread_fn1(void * p);


// spinlock
DEFINE_SPINLOCK(char_spinlock);
static struct task_struct *char_thread2;
static struct task_struct *char_thread3;
int char_thread_fn2(void * p);
int char_thread_fn3(void * p);


// tasklet
void tasklet_fn1(struct tasklet_struct * data);
void tasklet_fn2(unsigned long data);
DECLARE_TASKLET(tasklet1, tasklet_fn1);
struct tasklet_struct * tasklet2;
void tasklet_fn1(struct tasklet_struct * data) 
{
	printk(KERN_INFO "Executing tasklet 1: data = %ld\n", data->data);
}
void tasklet_fn2(unsigned long data)
{
	printk(KERN_INFO "Executing tasklet 2: data = %ld\n", data);
}


// IRQ
// cat /proc/interrupts
#define IRQ_NO (1)
static irqreturn_t irq_handler(int irq, void* dev_id) 
{
	static uint8_t i = 0;
	++i;
	if (i % 10 == 0) {
		printk(KERN_INFO "Keyboard IRQ occured %d times\n", i);
		tasklet_schedule(&tasklet1);
	}
	return IRQ_HANDLED;
}
volatile int chr_val = 0;


// ioctl code
#define WR_DATA _IOW('a', 'a', int32_t*)
#define RD_DATA _IOR('a', 'b', int32_t*)

int32_t val = 0;


// device methods
static int  __init chardrv_init(void);
static void __exit chardrv_exit(void);
static int my_open( struct inode * inode, struct file * file);
static int my_close(struct inode * inode, struct file * file);
static ssize_t my_read( struct file * file, char __user * buf, size_t len, loff_t * off);
static ssize_t my_write(struct file * file, const char  * buf, size_t len, loff_t * off);
static long my_ioctl(struct file * file, unsigned int cmd, unsigned long arg);

// static int procfs_open( struct inode * inode, struct file * file);
// static int procfs_close(struct inode * inode, struct file * file);
// static ssize_t procfs_read( struct file * file, char __user * buf, size_t len, loff_t * off);
// static ssize_t procfs_write(struct file * file, const char  * buf, size_t len, loff_t * off);


#define MEM_SIZE (1024)


dev_t dev = 0;
static struct class * dev_class;
static struct cdev my_cdev;
static struct file_operations fops = {
	.owner          = THIS_MODULE,
	.read           = my_read,
	.write          = my_write,
	.open           = my_open,
	.release        = my_close,
	.unlocked_ioctl = my_ioctl,
};
uint8_t * kernel_buffer;


static int  __init chardrv_init(void)
{
	printk(KERN_INFO "===================\n");
	printk(KERN_INFO "Hello from Char Dev\n");

	alloc_chrdev_region(&dev, 0, 1, "My Char Dev");
	printk(KERN_INFO "Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

	cdev_init(&my_cdev, &fops);

	if (cdev_add(&my_cdev, dev, 1) < 0) {
		printk(KERN_ERR "\tERROR adding Char Dev\n");
		goto r_class;
	}

	dev_class = class_create(THIS_MODULE, "my_class");
	if (dev_class == NULL) {
		printk(KERN_ERR "\tERROR creating struct class\n");
		goto r_class;
	}

	if (device_create(dev_class, NULL, dev, NULL, "my_device") == NULL) {
		printk(KERN_ERR "\tERROR creating Device\n");
		goto r_device;
	}

	if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "my_char_dev", (void*)(irq_handler)) != 0) {
		printk(KERN_ERR "\tERROR registering IRQ\n");
		goto r_irq;
	}


	// timer
	timer_setup(&char_timer, timer_callback, 0);
	mod_timer(&char_timer, msecs_to_jiffies(TIMEOUT_MS));


  	// tasklet - dynamic method
	tasklet2 = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
	if (tasklet2 == NULL) {
		printk(KERN_ERR "\tERROR creating tasklet dynamic\n");
		goto r_class;
	}
	tasklet_init(tasklet2, tasklet_fn2, 234567UL);
	tasklet_schedule(tasklet2);


	// kthread
	// char_thread1 = kthread_run(char_thread_fn1, NULL, "char_thread1");
	char_thread1 = kthread_create(char_thread_fn1, NULL, "char_thread1");
	if (char_thread1) {
		wake_up_process(char_thread1);
	} else {
		printk(KERN_ERR "\tERROR creating kthread 1\n");
		goto r_device;
	}

	// kthread w/ spinlock
	// char_thread2 = kthread_run(char_thread_fn2, NULL, "char_thread2");
	char_thread2 = kthread_create(char_thread_fn2, NULL, "char_thread2");
	if (char_thread2) {
		wake_up_process(char_thread2);
	} else {
		printk(KERN_ERR "\tERROR creating kthread 2\n");
		goto r_device;
	}
	// char_thread3 = kthread_run(char_thread_fn3, NULL, "char_thread3");
	char_thread3 = kthread_create(char_thread_fn3, NULL, "char_thread3");
	if (char_thread3) {
		wake_up_process(char_thread3);
	} else {
		printk(KERN_ERR "\tERROR creating kthread 3\n");
		goto r_device;
	}


	return 0;

r_irq:
	free_irq(IRQ_NO, (void*)(irq_handler));
r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	cdev_del(&my_cdev);
	return -1;
}

static void __exit chardrv_exit(void)
{
	kthread_stop(char_thread1);
	kthread_stop(char_thread2);
	kthread_stop(char_thread3);
	tasklet_kill(tasklet2);
	del_timer(&char_timer);
	free_irq(IRQ_NO, (void*)(irq_handler));
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Say Bye to Char Dev\n");
	printk(KERN_INFO "===================\n");
}

static int my_open( struct inode * inode, struct file * file)
{
	kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);
	int result = 0;
	if (kernel_buffer == NULL) {
		printk(KERN_INFO "my_open  fail\n");
		result = -1;
	} else {
		printk(KERN_INFO "my_open  OK \n");
	}
	return result;
}

static int my_close(struct inode * inode, struct file * file) 
{
	kfree(kernel_buffer);
	printk(KERN_INFO "my_close OK \n");
	return 0;
}

static ssize_t my_read( struct file * file, char __user * buf, size_t len, loff_t * off)
{
	copy_to_user(buf, kernel_buffer, MEM_SIZE);
	printk(KERN_INFO "copy_to_user %u bytes\n", MEM_SIZE);
	return MEM_SIZE;
}

static ssize_t my_write(struct file * file, const char  * buf, size_t len, loff_t * off)
{
	copy_from_user(kernel_buffer, buf, len);
	printk(KERN_INFO "copy_from_user %lu bytes\n", len);
	return len;
}

static long my_ioctl(struct file * file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case WR_DATA:
		copy_from_user(&val, (uint32_t*)arg, sizeof(val));
		printk(KERN_INFO "WR_DATA val = %d\n", val);
		break;

	case RD_DATA:
		copy_to_user((uint32_t*)arg, &val, sizeof(val));
		printk(KERN_INFO "RD_DATA val = %d\n", val);
		break;

	default:
		break;
	}

	return 0;
}

int char_thread_fn1(void * p)
{
	int i = 0;
	while(!kthread_should_stop()) {
		printk(KERN_INFO "Inside kthread ... %d\n", i);
		++i;
		msleep(1000);
	}
	return 0;
}

int char_thread_fn2(void * p)
{
	while(!kthread_should_stop()) {
		if (!spin_is_locked(&char_spinlock)) {
			printk(KERN_INFO "spin: unlocked\n");
			spin_lock(&char_spinlock);
			if (spin_is_locked(&char_spinlock)) {
				static uint32_t spinlock_var = 0;
				printk(KERN_INFO "spin: locked\n");
				printk(KERN_INFO "spinlock_var 2 = %d\n", spinlock_var);
				++spinlock_var;
				spin_unlock(&char_spinlock);
				msleep(1000);
			}
		}
	}

	return 0;
}

int char_thread_fn3(void * p)
{
	while(!kthread_should_stop()) {
		if (!spin_is_locked(&char_spinlock)) {
			printk(KERN_INFO "spin: unlocked\n");
			spin_lock(&char_spinlock);
			if (spin_is_locked(&char_spinlock)) {
				static uint32_t spinlock_var = 0;
				printk(KERN_INFO "spin: locked\n");
				printk(KERN_INFO "spinlock_var 3 = %d\n", spinlock_var);
				++spinlock_var;
				spin_unlock(&char_spinlock);
				msleep(1000);
			}
		}
	}

	return 0;
}


module_init(chardrv_init);
module_exit(chardrv_exit);


MODULE_INFO(name, "CharDev");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("my-CharDev");