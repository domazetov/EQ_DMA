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
MODULE_DESCRIPTION("Driver for DAE output");
#define DEVICE_NAME "DAE"
#define DRIVER_NAME "DAE_driver"
#define BUFF_SIZE 20

int counter = 0;
int endRead = 0;

//*******************FUNCTION PROTOTYPES************************************

static int DAE_probe(struct platform_device *pdev);
static int DAE_open(struct inode *i, struct file *f);
static int DAE_close(struct inode *i, struct file *f);
static ssize_t DAE_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t DAE_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static int __init DAE_init(void);
static void __exit DAE_exit(void);
static int DAE_remove(struct platform_device *pdev);

//*********************GLOBAL VARIABLES*************************************
struct DAE_info
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
static struct DAE_info *eq_in = NULL;  //FIRST
static struct DAE_info *eq_out = NULL; //SECOND

static struct file_operations my_fops =
    {
        .owner = THIS_MODULE,
        .open = DAE_open,
        .release = DAE_close,
        .read = DAE_read,
        .write = DAE_write};
static struct of_device_id DAE_of_match[] = {
    {
        .compatible = "eq_in",
    },
    {
        .compatible = "eq_out",
    },
    {/* end of list */},
};

static struct platform_driver DAE_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .owner = THIS_MODULE,
        .of_match_table = DAE_of_match,
    },
    .probe = DAE_probe,
    .remove = DAE_remove,
};

MODULE_DEVICE_TABLE(of, DAE_of_match);

//***************************************************************************

// PROBE AND REMOVE
static int DAE_probe(struct platform_device *pdev)
{

  struct resource *r_mem;
  int rc = 0;
  r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  if (!r_mem)
  {
    printk(KERN_ALERT "Failed to get resource\n");
    return -ENODEV;
  }

  switch (counter)
  {

  case 0:

    eq_in = (struct DAE_info *)kmalloc(sizeof(struct DAE_info), GFP_KERNEL);
    if (!eq_in)
    {
      printk(KERN_ALERT "Could not allocate memory\n");
      return -ENOMEM;
    }

    eq_in->mem_start = r_mem->start;
    eq_in->mem_end = r_mem->end;
    printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);

    if (!request_mem_region(eq_in->mem_start, eq_in->mem_end - eq_in->mem_start + 1, DRIVER_NAME))
    {
      printk(KERN_ALERT "Could not lock memory region at %p\n", (void *)eq_in->mem_start);
      rc = -EBUSY;
      goto error1;
    }

    eq_in->base_addr = ioremap(eq_in->mem_start, eq_in->mem_end - eq_in->mem_start + 1);

    if (!eq_in->base_addr)
    {
      printk(KERN_ALERT "Could not allocate memory\n");
      rc = -EIO;
      goto error2;
    }

    counter++;
    printk(KERN_NOTICE "DAE_probe: DAE platform driver registered\n");
    return 0; //ALL OK

  error2:
    release_mem_region(eq_in->mem_start, eq_in->mem_end - eq_in->mem_start + 1);
  error1:
    return rc;

  case 1:

    eq_out = (struct DAE_info *)kmalloc(sizeof(struct DAE_info), GFP_KERNEL);
    if (!eq_out)
    {
      printk(KERN_ALERT "Could not allocate memory\n");
      return -ENOMEM;
    }

    eq_out->mem_start = r_mem->start;
    eq_out->mem_end = r_mem->end;
    printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);

    if (!request_mem_region(eq_out->mem_start, eq_out->mem_end - eq_out->mem_start + 1, DRIVER_NAME))
    {
      printk(KERN_ALERT "Could not lock memory region at %p\n", (void *)eq_out->mem_start);
      rc = -EBUSY;
      goto error4;
    }

    eq_out->base_addr = ioremap(eq_out->mem_start, eq_out->mem_end - eq_out->mem_start + 1);

    if (!eq_out->base_addr)
    {
      printk(KERN_ALERT "Could not allocate memory\n");
      rc = -EIO;
      goto error3;
    }

    counter++;
    printk(KERN_NOTICE "DAE_probe: DAE platform driver registered\n");
    return 0; //ALL OK

  error3:
    release_mem_region(eq_out->mem_start, eq_out->mem_end - eq_out->mem_start + 1);
  error4:
    return rc;
  }

  return 0;
}

static int DAE_remove(struct platform_device *pdev)
{
  switch (counter)
  {

  case 0: //FIRST
    int pos;
    for (pos = 0; pos < 19; pos++)
    {
      iowrite32(pos, eq_in->base_addr);
      iowrite32(0, eq_in->base_addr + 8);
    }
    printk(KERN_INFO "eq_in_remove: eq_in remove in process");
    iounmap(eq_in->base_addr);
    release_mem_region(eq_in->mem_start, eq_in->mem_end - eq_in->mem_start + 1);
    kfree(eq_in);
    printk(KERN_INFO "eq_in_remove: eq_in driver removed");

    break;

  case 1: //SECOND
    iowrite32(0, eq_out->base_addr);
    printk(KERN_INFO "eq_out_remove: eq_out remove in process");
    iounmap(eq_out->base_addr);
    release_mem_region(eq_out->mem_start, eq_out->mem_end - eq_out->mem_start + 1);
    kfree(eq_out);
    printk(KERN_INFO "eq_out_remove: eq_out driver removed");
    counter--;
    break;
  }

  return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS

static int DAE_open(struct inode *i, struct file *f)
{
  //printk(KERN_INFO "DAE opened\n");
  return 0;
}
static int DAE_close(struct inode *i, struct file *f)
{
  //printk(KERN_INFO "DAE closed\n");
  return 0;
}
static ssize_t DAE_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
  char buff[BUFF_SIZE];
  int ret = 0;
  unsigned int pos = 0;
  unsigned int val = 0;
  printk("DAE: Write start.\n");
  ret = copy_from_user(buff, buf, length);
  if (ret)
  {
    printk("copy from user failed \n");
    return -EFAULT;
  }
  buff[length] = '\0';

  sscanf(buff, "%d,%d", &pos, &val);

  if (ret != -EINVAL) //checking for parsing error
  {
    iowrite32(pos, eq_in->base_addr);
    iowrite32(val, eq_in->base_addr + 8);
    printk(KERN_INFO "DAE: Writing value: %d at position %d.\n", val, pos);
  }
  else
  {
    printk(KERN_WARNING "DAE_write: Wrong write format, expected \"xpos,ypos,rgb\"\n");
    // return -EINVAL;//parsing error
  }
  printk(KERN_INFO "DAE: Write done.\n");
  return length;
}

static ssize_t DAE_read(struct file *f, char __user *buf, size_t length, loff_t *off)
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
  printk("DAE: Read.\n");
  for (pos = 0; pos < 28; pos++)
  {
    iowrite32(pos, eq_out->base_addr);
    value = ioread32(eq_out->base_addr + 8);
    printk(KERN_INFO "%d\n", value);
  }

  length = scnprintf(buff, BUFF_SIZE, "%d", value);
  ret = copy_to_user(buf, buff, BUFF_SIZE);

  if (ret)
  {
    printk(KERN_INFO "DAE Read: Copy to user failed.\n");
    return -EFAULT;
  }

  printk(KERN_INFO "DAE Read done.\n");
  endRead = 1;
  return length;
}

//***************************************************
// HELPER FUNCTIONS (STRING TO INTEGER)

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init DAE_init(void)
{

  int ret = 0;
  int_cnt = 0;

  printk(KERN_INFO "DAE_init: Initialize Module \"%s\"\n", DEVICE_NAME);
  ret = alloc_chrdev_region(&my_dev_id, 0, 1, "DAE_region");
  if (ret)
  {
    printk(KERN_ALERT "<1>Failed CHRDEV!.\n");
    return -1;
  }
  printk(KERN_INFO "Succ CHRDEV!.\n");
  my_class = class_create(THIS_MODULE, "DAE_drv");
  if (my_class == NULL)
  {
    printk(KERN_ALERT "<1>Failed class create!.\n");
    goto fail_0;
  }
  printk(KERN_INFO "Succ class chardev1 create!.\n");
  my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "dae");
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
    printk(KERN_ERR "DAE_init: Failed to add cdev\n");
    goto fail_2;
  }
  printk(KERN_INFO "DAE Device init.\n");

  return platform_driver_register(&DAE_driver);

fail_2:
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
fail_1:
  class_destroy(my_class);
fail_0:
  unregister_chrdev_region(my_dev_id, 1);
  return -1;
}

static void __exit DAE_exit(void)
{

  platform_driver_unregister(&DAE_driver);
  cdev_del(my_cdev);
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
  class_destroy(my_class);
  unregister_chrdev_region(my_dev_id, 1);
  printk(KERN_INFO "DAE_exit: Exit Device Module \"%s\".\n", DEVICE_NAME);
}

module_init(DAE_init);
module_exit(DAE_exit);

MODULE_AUTHOR("FTN");
MODULE_DESCRIPTION("Test Driver for DAE output.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("custom:DAE");