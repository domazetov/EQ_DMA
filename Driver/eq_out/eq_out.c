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
MODULE_DESCRIPTION("Driver for eq_out ouvput");
#define DEVICE_NAME "eq_out"
#define DRIVER_NAME "eq_out_driver"
#define BUFF_SIZE 20

int endRead = 0;
//*******************FUNCTION PROTOTYPES************************************

static int eq_out_probe(struct platform_device *pdev);
static int eq_out_open(struct inode *i, struct file *f);
static int eq_out_close(struct inode *i, struct file *f);
static ssize_t eq_out_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t eq_out_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static int __init eq_out_init(void);
static void __exit eq_out_exit(void);
static int eq_out_remove(struct platform_device *pdev);

//*********************GLOBAL VARIABLES*************************************
struct eq_out_info
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
static struct eq_out_info *vp = NULL;

static struct file_operations my_fops =
    {
        .owner = THIS_MODULE,
        .open = eq_out_open,
        .release = eq_out_close,
        .read = eq_out_read,
        .write = eq_out_write};
static struct of_device_id eq_out_of_match[] = {
    {
        .compatible = "eq_out",
    },
    {/* end of list */},
};

static struct platform_driver eq_out_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = eq_out_of_match,
    },
    .probe = eq_out_probe,
    .remove = eq_out_remove,
};

MODULE_DEVICE_TABLE(of, eq_out_of_match);

//***************************************************************************

// PROBE AND REMOVE

static int eq_out_probe(struct platform_device *pdev)
{
  struct resource *r_mem;
  int rc = 0;

  printk(KERN_INFO "Probing\n");
  // Get phisical register adress space from device tree
  r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  if (!r_mem)
  {
    printk(KERN_ALERT "eq_out_probe: Failed to get reg resource\n");
    return -ENODEV;
  }
  // Get memory for structure eq_out_info
  vp = (struct eq_out_info *)kmalloc(sizeof(struct eq_out_info), GFP_KERNEL);
  if (!vp)
  {
    printk(KERN_ALERT "eq_out_probe: Could not allocate timer device\n");
    return -ENOMEM;
  }
  // Put phisical adresses in timer_info structure
  vp->mem_start = r_mem->start;
  vp->mem_end = r_mem->end;

  // Reserve that memory space for this driver
  if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
  {
    printk(KERN_ALERT "eq_out_probe: Could not lock memory region at %p\n", (void *)vp->mem_start);
    rc = -EBUSY;
    goto error1;
  }
  // Remap phisical to virtual adresses

  vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
  if (!vp->base_addr)
  {
    printk(KERN_ALERT "eq_out_probe: Could not allocate memory\n");
    rc = -EIO;
    goto error2;
  }

  printk(KERN_NOTICE "eq_out_probe: eq_out platform driver registered\n");
  return 0; //ALL OK
error2:
  release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
error1:
  return rc;
}

static int eq_out_remove(struct platform_device *pdev)
{
  // Exit Device Module
  iowrite32(0, vp->base_addr + 8);
  iowrite32(0, vp->base_addr);
  printk(KERN_INFO "eq_out_remove: eq_out remove in process");
  iounmap(vp->base_addr);
  release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
  kfree(vp);
  printk(KERN_INFO "eq_out_remove: eq_out driver removed");
  return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS

static int eq_out_open(struct inode *i, struct file *f)
{
  //printk(KERN_INFO "eq_out opened\n");
  return 0;
}
static int eq_out_close(struct inode *i, struct file *f)
{
  //printk(KERN_INFO "eq_out closed\n");
  return 0;
}
static ssize_t eq_out_read(struct file *f, char __user *buf, size_t length, loff_t *off)
{
  int ret = 0;
  char buff[BUFF_SIZE];
  unsigned int pos = 0;
  int value = 0;
  if (endRead)
  {
    endRead = 0;
    return 0;
  }
  printk("EQ: Read.\n");
  for (pos = 0; pos < 19; pos++)
  {
    iowrite32(pos, vp->base_addr);
    value = ioread32(vp->base_addr + 8);
    printk(KERN_INFO "%d\n", value);
  }

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
static ssize_t eq_out_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
  return 0;
}

//***************************************************
// HELPER FUNCTIONS (STRING TO INTEGER)

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init eq_out_init(void)
{

  int ret = 0;
  int_cnt = 0;

  printk(KERN_INFO "eq_out_init: Initialize Module \"%s\"\n", DEVICE_NAME);
  ret = alloc_chrdev_region(&my_dev_id, 0, 1, "eq_out_region");
  if (ret)
  {
    printk(KERN_ALERT "<1>Failed CHRDEV!.\n");
    return -1;
  }
  printk(KERN_INFO "Succ CHRDEV!.\n");
  my_class = class_create(THIS_MODULE, "eq_out_drv");
  if (my_class == NULL)
  {
    printk(KERN_ALERT "<1>Failed class create!.\n");
    goto fail_0;
  }
  printk(KERN_INFO "Succ class chardev1 create!.\n");
  my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "eq_out");
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
    printk(KERN_ERR "eq_out_init: Failed to add cdev\n");
    goto fail_2;
  }
  printk(KERN_INFO "eq_out Device init.\n");

  return platform_driver_register(&eq_out_driver);

fail_2:
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
fail_1:
  class_destroy(my_class);
fail_0:
  unregister_chrdev_region(my_dev_id, 1);
  return -1;
}

static void __exit eq_out_exit(void)
{

  platform_driver_unregister(&eq_out_driver);
  cdev_del(my_cdev);
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
  class_destroy(my_class);
  unregister_chrdev_region(my_dev_id, 1);
  printk(KERN_INFO "eq_out_exit: Exit Device Module \"%s\".\n", DEVICE_NAME);
}

module_init(eq_out_init);
module_exit(eq_out_exit);

MODULE_AUTHOR("FTN");
MODULE_DESCRIPTION("Test Driver for eq_out output.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("custom:eq_out");
