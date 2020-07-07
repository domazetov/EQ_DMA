#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

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

#include <linux/io.h> //iowrite ioread
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/ioport.h> //ioremap

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for eq_in ouvput");
#define DEVICE_NAME "eq_in"
#define DRIVER_NAME "eq_in_driver"
#define BUFF_SIZE 20

//*******************FUNCTION PROTOTYPES************************************

static int eq_in_probe(struct platform_device *pdev);
static int eq_in_open(struct inode *i, struct file *f);
static int eq_in_close(struct inode *i, struct file *f);
static ssize_t eq_in_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t eq_in_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static int __init eq_in_init(void);
static void __exit eq_in_exit(void);
static int eq_in_remove(struct platform_device *pdev);

//*********************GLOBAL VARIABLES*************************************
struct eq_in_info
{
  unsigned long mem_start;
  unsigned long mem_end;
  void __iomem *base_addr;
};
static struct cdev *my_cdev;
static dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static int int_cnt;
static struct eq_in_info *vp = NULL;

static struct file_operations my_fops =
    {
        .owner = THIS_MODULE,
        .open = eq_in_open,
        .release = eq_in_close,
        .read = eq_in_read,
        .write = eq_in_write};
static struct of_device_id eq_in_of_match[] = {
    {
        .compatible = "eq_in",
    },
    {/* end of list */},
};

static struct platform_driver eq_in_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = eq_in_of_match,
    },
    .probe = eq_in_probe,
    .remove = eq_in_remove,
};

MODULE_DEVICE_TABLE(of, eq_in_of_match);

//***************************************************************************

// PROBE AND REMOVE

static int eq_in_probe(struct platform_device *pdev)
{
  struct resource *r_mem;
  int rc = 0;

  printk(KERN_INFO "Probing\n");
  // Get phisical register adress space from device tree
  r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  if (!r_mem)
  {
    printk(KERN_ALERT "eq_in_probe: Failed to get reg resource\n");
    return -ENODEV;
  }
  // Get memory for structure eq_in_info
  vp = (struct eq_in_info *)kmalloc(sizeof(struct eq_in_info), GFP_KERNEL);
  if (!vp)
  {
    printk(KERN_ALERT "eq_in_probe: Could not allocate timer device\n");
    return -ENOMEM;
  }
  // Put phisical adresses in timer_info structure
  vp->mem_start = r_mem->start;
  vp->mem_end = r_mem->end;

  // Reserve that memory space for this driver
  if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
  {
    printk(KERN_ALERT "eq_in_probe: Could not lock memory region at %p\n", (void *)vp->mem_start);
    rc = -EBUSY;
    goto error1;
  }
  // Remap phisical to virtual adresses

  vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
  if (!vp->base_addr)
  {
    printk(KERN_ALERT "eq_in_probe: Could not allocate memory\n");
    rc = -EIO;
    goto error2;
  }

  printk(KERN_NOTICE "eq_in_probe: eq_in platform driver registered\n");
  return 0; //ALL OK
error2:
  release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
error1:
  return rc;
}

static int eq_in_remove(struct platform_device *pdev)
{
  // Exit Device Module
  iowrite32(0, vp->base_addr + 8);
  iowrite32(0, vp->base_addr);
  printk(KERN_INFO "eq_in_remove: eq_in remove in process");
  iounmap(vp->base_addr);
  release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
  kfree(vp);
  printk(KERN_INFO "eq_in_remove: eq_in driver removed");
  return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS

static int eq_in_open(struct inode *i, struct file *f)
{
  //printk(KERN_INFO "eq_in opened\n");
  return 0;
}
static int eq_in_close(struct inode *i, struct file *f)
{
  //printk(KERN_INFO "eq_in closed\n");
  return 0;
}
static ssize_t eq_in_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  //printk("eq_in read\n");
  return 0;
}
static ssize_t eq_in_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
  char buff[BUFF_SIZE];
  int ret = 0;
  unsigned int pos = 0b00001;
  unsigned int val = 0;
  printk("EQ: Write.\n");
  ret = copy_from_user(buff, buf, length);
  if (ret)
  {
    printk("copy from user failed \n");
    return -EFAULT;
  }
  buff[length] = '\0';

  sscanf(buff, "%d", &val);

  if (ret != -EINVAL) //checking for parsing error
  {
    iowrite32(pos, vp->base_addr);
    iowrite32(val, vp->base_addr + 8);
  }
  else
  {
    printk(KERN_WARNING "eq_in_write: Wrong write format, expected \"xpos,ypos,rgb\"\n");
    // return -EINVAL;//parsing error
  }
  printk(KERN_INFO "EQ Write done.\n");
  return length;
}

//***************************************************
// HELPER FUNCTIONS (STRING TO INTEGER)

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init eq_in_init(void)
{

  int ret = 0;
  int_cnt = 0;

  printk(KERN_INFO "eq_in_init: Initialize Module \"%s\"\n", DEVICE_NAME);
  ret = alloc_chrdev_region(&my_dev_id, 0, 1, "eq_in_region");
  if (ret)
  {
    printk(KERN_ALERT "<1>Failed CHRDEV!.\n");
    return -1;
  }
  printk(KERN_INFO "Succ CHRDEV!.\n");
  my_class = class_create(THIS_MODULE, "eq_in_drv");
  if (my_class == NULL)
  {
    printk(KERN_ALERT "<1>Failed class create!.\n");
    goto fail_0;
  }
  printk(KERN_INFO "Succ class chardev1 create!.\n");
  my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "eq_in");
  if (my_device == NULL)
  {
    goto fail_1;
  }

  printk(KERN_INFO "Device created.\n");

  my_cdev = cdev_alloc();
  my_cdev->ops = &my_fops;
  my_cdev->owner = THIS_MODULE;
  ret = cdev_add(my_cdev, my_dev_id, 1);
  if (ret)
  {
    printk(KERN_ERR "eq_in_init: Failed to add cdev\n");
    goto fail_2;
  }
  printk(KERN_INFO "eq_in Device init.\n");

  return platform_driver_register(&eq_in_driver);

fail_2:
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
fail_1:
  class_destroy(my_class);
fail_0:
  unregister_chrdev_region(my_dev_id, 1);
  return -1;
}

static void __exit eq_in_exit(void)
{

  platform_driver_unregister(&eq_in_driver);
  cdev_del(my_cdev);
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
  class_destroy(my_class);
  unregister_chrdev_region(my_dev_id, 1);
  printk(KERN_INFO "eq_in_exit: Exit Device Module \"%s\".\n", DEVICE_NAME);
}

module_init(eq_in_init);
module_exit(eq_in_exit);

MODULE_AUTHOR("FTN");
MODULE_DESCRIPTION("Test Driver for eq_in output.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("custom:eq_in");
