#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/io.h>

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for equalizer output");
#define DEVICE_NAME "equalizer"
#define DRIVER_NAME "eq_driver"
#define BUFF_SIZE 20

//*************************************************************************//
static int eq_probe(struct platform_device *pdev);
static int eq_open(struct inode *i, struct file *f);
static int eq_close(struct inode *i, struct file *f);
static ssize_t eq_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t eq_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static int __init eq_init(void);
static void __exit eq_exit(void);
static int eq_remove(struct platform_device *pdev);

int endRead = 0;
//*********************GLOBAL VARIABLES*************************************//
static struct file_operations eq_fops =
	{
		.owner = THIS_MODULE,
		.open = eq_open,
		.release = eq_close,
		.read = eq_read,
		.write = eq_write};
static struct of_device_id eq_of_match[] = {
	{
		.compatible = "equalizer",
	},
	{/* end of list */},
};

static struct platform_driver eq_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = eq_of_match,
	},
	.probe = eq_probe,
	.remove = eq_remove,
};

struct eq_info
{
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
};

static struct eq_info *vp = NULL;

MODULE_DEVICE_TABLE(of, eq_of_match);

static struct cdev c_dev;
static dev_t first;
static struct class *cl;
static int int_cnt;

//***************************************************//
// PROBE AND REMOVE

static int eq_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;

	printk(KERN_INFO "EQ: Probing.\n");

	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem)
	{
		printk(KERN_ALERT "EQ: Invalid address.\n");
		return -ENODEV;
	}
	vp = (struct eq_info *)kmalloc(sizeof(struct eq_info), GFP_KERNEL);
	if (!vp)
	{
		printk(KERN_ALERT "EQ: Cound not allocate eq device.\n");
		return -ENOMEM;
	}

	vp->mem_start = r_mem->start;
	vp->mem_end = r_mem->end;

	if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
	{
		printk(KERN_ALERT "EQ: Couldn't lock memory region at %p.\n", (void *)vp->mem_start);
		rc = -EBUSY;
		goto error1;
	}
	else
	{
		printk(KERN_INFO "EQ: Successfully allocated memory region for equalizer.\n");
	}
	/* 
	 * Map Physical address to Virtual address
	 */

	vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	if (!vp->base_addr)
	{
		printk(KERN_ALERT "EQ: Could not allocate iomem.\n");
		rc = -EIO;
		goto error2;
	}

	printk(KERN_INFO "EQ: Probing done.\n");
	return 0; // ALL OK

error2:
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);

error1:
	return rc;
}

static int eq_remove(struct platform_device *pdev)
{
	// Exit Device Module
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
	printk(KERN_INFO "EQ: Removed.\n");
	return 0;
}

//***************************************************//
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS

static int eq_open(struct inode *i, struct file *f)
{
	printk("EQ: Opened.\n");
	return 0;
}
static int eq_close(struct inode *i, struct file *f)
{
	printk("EQ: Closed.\n");
	return 0;
}
static ssize_t eq_read(struct file *f, char __user *buf, size_t length, loff_t *off)
{
	int ret = 0;
	char buff[BUFF_SIZE];
	int value = 0;
	if (endRead)
	{
		endRead = 0;
		return 0;
	}
	printk("EQ: Read.\n");
	value = ioread32(vp->base_addr);
	length = scnprintf(buff, BUFF_SIZE, "%d", value);
	ret = copy_to_user(buf, buff, BUFF_SIZE);
	if (ret)
	{
		printk(KERN_INFO "EQ Read: Copy to user failed.\n");
		return -EFAULT;
	}

	printk(KERN_INFO "EQ Read done.\n");
	endRead = 1;
	return length;
}
static ssize_t eq_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{

	char buff[length];
	int ret = 0;
	unsigned int pos = 0;
	unsigned long long value = 0;
	unsigned char parameter[10];
	ret = copy_from_user(buff, buf, length);
	if (ret)
	{
		printk(KERN_INFO "EQ: copy from user failed \n");
		return -EFAULT;
	}
	buff[length] = '\0';

	sscanf(buff, "%d,%s", &pos, parameter);
	ret = kstrtoull(parameter, 0, &value);

	if (ret != -EINVAL) //checking for parsing error
	{
		if (pos > 18)
		{
			printk(KERN_WARNING "EQ: Position exceeded, maximum is 18 and minimum 0 \n");
		}
		else
		{

			iowrite32(value, vp->base_addr + pos);
			printk(KERN_INFO "EQ: Wrote: %llu, at Address: %d\n", value, pos);
		}
	}
	else
	{
		printk(KERN_WARNING "EQ: wrong write format, expected \"pos,paramter\"\n");
		// return -EINVAL;//parsing error
	}
	return length;
}

//***************************************************//
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init eq_init(void)
{

	int_cnt = 0;

	printk(KERN_INFO "EQ Init: Initialize Module \"%s\"\n", DEVICE_NAME);

	if (alloc_chrdev_region(&first, 0, 1, "eq_region") < 0)
	{
		printk(KERN_ALERT "EQ Init: <1>Failed CHRDEV!.\n");
		return -1;
	}
	printk(KERN_INFO "EQ Init: Succ CHRDEV!.\n");

	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		printk(KERN_ALERT "EQ Init: <1>Failed class create!.\n");
		goto fail_0;
	}
	printk(KERN_INFO "EQ Init: Succ class chardev1 create!.\n");
	if (device_create(cl, NULL, MKDEV(MAJOR(first), 0), NULL, "eq") == NULL)
	{
		goto fail_1;
	}

	printk(KERN_INFO "EQ Init: Device created.\n");

	cdev_init(&c_dev, &eq_fops);
	if (cdev_add(&c_dev, first, 1) == -1)
	{
		goto fail_2;
	}

	printk(KERN_INFO "EQ Init: Device init.\n");

	return platform_driver_register(&eq_driver);

fail_2:
	device_destroy(cl, MKDEV(MAJOR(first), 0));
fail_1:
	class_destroy(cl);
fail_0:
	unregister_chrdev_region(first, 1);
	return -1;
}

static void __exit eq_exit(void)
{

	platform_driver_unregister(&eq_driver);
	cdev_del(&c_dev);
	device_destroy(cl, MKDEV(MAJOR(first), 0));
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "EQ Exit: Exit Device Module \"%s\".\n", DEVICE_NAME);
}

module_init(eq_init);
module_exit(eq_exit);

MODULE_AUTHOR("FTN");
MODULE_DESCRIPTION("Test Driver for equalizer output.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("custom:equalizer");
