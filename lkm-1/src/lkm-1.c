#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init lkm_init(void)
{
    //printk(KERN_INFO "Hello from LKM\n");
    pr_info("Hello from LKM\n");
    return 0;
}

static void __exit lkm_exit(void)
{
    //printk(KERN_INFO "Bye, LKM\n");
    pr_info("Say Bye to LKM\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_INFO(name, "LKM-1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("LKM-1");
