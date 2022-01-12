#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/init.h>

static char __iomem * mapped;

static const unsigned long iostart  = 0x200UL;
static const unsigned long ioextend = 0x40UL;
static const unsigned long ioend    = iostart + ioextend;

static int  __init ioregion_init(void);
static void __exit ioregion_exit(void);

// test:
// sudo cat /proc/ioports | grep my_ioport

static int  __init ioregion_init(void)
{
    printk(KERN_INFO "IO region: ==================================\n");

    if(!request_region(iostart, ioextend, "my_ioport")) {
        printk(KERN_ERR "IO region: busy\n");
        return -EBUSY;
    } else {
        printk(KERN_INFO "IO region: requested from 0x%lx to 0x%lx\n", iostart, ioend);
    }
    
    // IO region
    unsigned long iotest = 0x404UL;
    printk(KERN_INFO "IO region: write value 0x%08lx\n", iotest);
    outl(iotest, iostart);
    iotest = inl(iostart);
    printk(KERN_INFO "IO region: read  value 0x%08lx\n", iotest);

    // MMIO
    mapped = ioport_map(iostart, ioextend);
    printk(KERN_INFO "IO region: mapped at 0x%p\n", mapped);
    iotest = 0x404UL;
    printk(KERN_INFO "IO region: write value 0x%08lx into mapped\n", iotest);
    iowrite32(iotest, mapped);
    iotest = ioread32(mapped);
    printk(KERN_INFO "IO region: read  value 0x%08lx from mapped\n", iotest);

    return 0;
}

static void __exit ioregion_exit(void)
{
    printk(KERN_INFO "IO region: release\n");
    release_region(iostart, ioextend);
    printk(KERN_INFO "IO region: ==================================\n");
}

module_init(ioregion_init);
module_exit(ioregion_exit);

MODULE_INFO(name, "my_ioport");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("my_ioport");
