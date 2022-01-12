#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/device.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#include <linux/sched/signal.h>
#endif


// cpu wait
static const int delay_HZ = HZ;
static struct task_struct *thread_wait_busy;
static struct task_struct *thread_wait_easy;
int thread_wait_fn_busy(void * p);
int thread_wait_fn_easy(void * p);


static int  __init waitdev_init(void)
{
	printk(KERN_INFO "===================\n");
	printk(KERN_INFO "Hello from Wait Dev\n");
	
	// cpu wait thread
	thread_wait_busy = kthread_run(thread_wait_fn_busy, NULL, "thread_wait_busy");
	thread_wait_easy = kthread_run(thread_wait_fn_easy, NULL, "thread_wait_easy");

	return 0;
}

static void __exit waitdev_exit(void)
{
	if (thread_wait_easy) {
		kthread_stop(thread_wait_easy);
	}
	if (thread_wait_busy) {
		kthread_stop(thread_wait_busy);
	}
	
	printk(KERN_INFO "Say Bye to Wait Dev\n");
	printk(KERN_INFO "===================\n");
}


int thread_wait_fn_busy(void * p)
{
	static unsigned long jiffies_now  = 0;
	static unsigned long jiffies_end  = 0;

	allow_signal(SIGKILL);

	while(!kthread_should_stop()) {
		jiffies_now = jiffies;
		jiffies_end = jiffies_now + delay_HZ;

		while (time_before(jiffies, jiffies_end)) {
			cpu_relax();  	// busy wait
			// schedule();   	// easy wait
		}

		printk(KERN_INFO "busy wait: %lu .. %lu jiffies\n", jiffies_now, jiffies_end);
		jiffies_now = jiffies;

		if (signal_pending(current)) {
			break;
		}
	}
	
	printk(KERN_INFO "thread_wait *busy* stopped\n");
	thread_wait_busy = NULL;
	do_exit(0);

	// return 0;
}

int thread_wait_fn_easy(void * p)
{
	static unsigned long jiffies_now  = 0;
	static unsigned long jiffies_end  = 0;

	allow_signal(SIGKILL);

	while(!kthread_should_stop()) {
		jiffies_now = jiffies;

		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(delay_HZ);

		jiffies_end = jiffies;

		printk(KERN_INFO "easy wait: %lu .. %lu jiffies\n", jiffies_now, jiffies_end);
		jiffies_now = jiffies;

		if (signal_pending(current)) {
			break;
		}
	}
	
	printk(KERN_INFO "thread_wait *easy* stopped\n");
	thread_wait_easy = NULL;
	do_exit(0);

	// return 0;
}


module_init(waitdev_init);
module_exit(waitdev_exit);


MODULE_INFO(name, "CharDev");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("my-CharDev");
