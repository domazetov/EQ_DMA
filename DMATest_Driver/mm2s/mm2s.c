#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/device.h>

#include <linux/io.h>			   //iowrite ioread
#include <linux/slab.h>			   //kmalloc kfree
#include <linux/platform_device.h> //platform driver
#include <linux/of.h>			   //of_match_table
#include <linux/ioport.h>		   //ioremap

#include <linux/dma-mapping.h> //dma access
#include <linux/mm.h>		   //dma access
#include <linux/interrupt.h>   //interrupt handlers

MODULE_AUTHOR("FTN");
MODULE_DESCRIPTION("Test Driver for mm2s controller IP.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:mm2s_dma controller");

#define DEVICE_NAME "mm2s_dma"
#define DRIVER_NAME "mm2s_dma_driver"
#define BUFF_SIZE 20
#define MAX_PKT_LEN 1024 * 4

//*******************FUNCTION PROTOTYPES************************************
static int mm2s_dma_probe(struct platform_device *pdev);
static int mm2s_dma_open(struct inode *i, struct file *f);
static int mm2s_dma_close(struct inode *i, struct file *f);
static ssize_t mm2s_dma_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t mm2s_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static ssize_t mm2s_dma_mmap(struct file *f, struct vm_area_struct *vma_s);
static int __init mm2s_dma_init(void);
static void __exit mm2s_dma_exit(void);
static int mm2s_dma_remove(struct platform_device *pdev);

//static irqreturn_t dma_isr(int irq, void *dev_id);
int dma_init(void __iomem *base_address);
u32 dma_simple_write(dma_addr_t TxBufferPtr, u32 max_pkt_len, void __iomem *base_address);

//*********************GLOBAL VARIABLES*************************************
struct mm2s_dma_info
{
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
	//int irq_num;
};

static struct cdev *my_cdev;
static dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct mm2s_dma_info *vp = NULL;

static struct file_operations my_fops =
	{
		.owner = THIS_MODULE,
		.open = mm2s_dma_open,
		.release = mm2s_dma_close,
		.read = mm2s_dma_read,
		.write = mm2s_dma_write,
		.mmap = mm2s_dma_mmap};

static struct of_device_id mm2s_dma_of_match[] = {
	{
		.compatible = "xlnx,axi-dma-mm2s-channel",
	},
	{.compatible = "axi_dma_mm2s"},
	{/* end of list */},
};
MODULE_DEVICE_TABLE(of, mm2s_dma_of_match);

static struct platform_driver mm2s_dma_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = mm2s_dma_of_match,
	},
	.probe = mm2s_dma_probe,
	.remove = mm2s_dma_remove,
};

dma_addr_t tx_phy_buffer;
u32 *tx_vir_buffer;

//***************************************************************************
// PROBE AND REMOVE
static int mm2s_dma_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;

	printk(KERN_INFO "mm2s_dma_probe: Probing\n");
	// Get phisical register adress space from device tree
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem)
	{
		printk(KERN_ALERT "mm2s_dma_probe: Failed to get reg resource\n");
		return -ENODEV;
	}
	// Get memory for structure mm2s_dma_info
	vp = (struct mm2s_dma_info *)kmalloc(sizeof(struct mm2s_dma_info), GFP_KERNEL);
	if (!vp)
	{
		printk(KERN_ALERT "mm2s_dma_probe: Could not allocate memory for structure mm2s_dma_info\n");
		return -ENOMEM;
	}
	// Put phisical adresses in timer_info structure
	vp->mem_start = r_mem->start;
	vp->mem_end = r_mem->end;

	// Reserve that memory space for this driver
	if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
	{
		printk(KERN_ALERT "mm2s_dma_probe: Could not lock memory region at %p\n", (void *)vp->mem_start);
		rc = -EBUSY;
		goto error1;
	}
	// Remap phisical to virtual adresses

	vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	if (!vp->base_addr)
	{
		printk(KERN_ALERT "mm2s_dma_probe: Could not allocate memory for remapping\n");
		rc = -EIO;
		goto error2;
	}

	// Get irq num
	/*
	vp->irq_num = platform_get_irq(pdev, 0);
	if (!vp->irq_num)
	{
		printk(KERN_ERR "mm2s_dma_probe: Could not get IRQ resource\n");
		rc = -ENODEV;
		goto error2;
	}
	*/
	/*
	if (request_irq(vp->irq_num, dma_isr, 0, DEVICE_NAME, NULL))
	{
		printk(KERN_ERR "mm2s_dma_probe: Could not register IRQ %d\n", vp->irq_num);
		return -EIO;
		goto error3;
	}
	else
	{
		printk(KERN_INFO "mm2s_dma_probe: Registered IRQ %d\n", vp->irq_num);
	}
*/
	/* INIT DMA */
	dma_init(vp->base_addr);
	dma_simple_write(tx_phy_buffer, MAX_PKT_LEN, vp->base_addr); // helper function, defined later

	printk(KERN_NOTICE "mm2s_dma_probe: mm2s platform driver registered\n");
	return 0; //ALL OK

error3:
	iounmap(vp->base_addr);
error2:
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
error1:
	return rc;
}

static int mm2s_dma_remove(struct platform_device *pdev)
{
	u32 reset = 0x00000004;
	// writing to MM2S_DMACR register. Seting reset bit (3. bit)
	printk(KERN_INFO "mm2s_dma_probe: resseting");
	iowrite32(reset, vp->base_addr);

	//free_irq(vp->irq_num, NULL);
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
	printk(KERN_INFO "mm2s_dma_probe: mm2s DMA removed");
	return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS
static int mm2s_dma_open(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "mm2s_dma opened\n");
	return 0;
}

static int mm2s_dma_close(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "mm2s_dma closed\n");
	return 0;
}

static ssize_t mm2s_dma_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	//printk("mm2s_dma read\n");
	return 0;
}

static ssize_t mm2s_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
	char buff[BUFF_SIZE];
	int ret = 0;
	int value;

	ret = copy_from_user(buff, buf, length);
	if (ret)
	{
		printk("MM2S copy from user failed \n");
		return -EFAULT;
	}
	buff[length] = '\0';

	sscanf(buff, "%d", &value);

	tx_vir_buffer[0] = (u32)value;
	printk(KERN_INFO "MM2S Succesfully wrote value %d", value);
	return length;
}

static ssize_t mm2s_dma_mmap(struct file *f, struct vm_area_struct *vma_s)
{
	int ret = 0;
	long length = vma_s->vm_end - vma_s->vm_start;

	//printk(KERN_INFO "DMA TX Buffer is being memory mapped\n");

	if (length > MAX_PKT_LEN)
	{
		return -EIO;
		printk(KERN_ERR "Trying to mmap more space than it's allocated\n");
	}

	ret = dma_mmap_coherent(NULL, vma_s, tx_vir_buffer, tx_phy_buffer, length);
	if (ret < 0)
	{
		printk(KERN_ERR "memory map failed\n");
		return ret;
	}
	return 0;
}

/****************************************************/
// IMPLEMENTATION OF DMA related functions
/*
static irqreturn_t dma_isr(int irq, void *dev_id)
{
	u32 IrqStatus;
	// Read pending interrupts
	IrqStatus = ioread32(vp->base_addr + 4);			  //read irq status from MM2S_DMASR register
	iowrite32(IrqStatus | 0x00007000, vp->base_addr + 4); //clear irq status in MM2S_DMASR register
	//(clearing is done by writing 1 on 13. bit in MM2S_DMASR (IOC_Irq)

	//Send a transaction
	//dma_simple_write(tx_phy_buffer, MAX_PKT_LEN, vp->base_addr); //My function that starts a DMA transaction
	printk(KERN_INFO "MM2S DMA Interrupt Detected");
	return IRQ_HANDLED;
	;
}
*/
int dma_init(void __iomem *base_address)
{
	u32 reset = 0x00000004;
	//u32 IOC_IRQ_EN;
	//u32 ERR_IRQ_EN;
	//u32 MM2S_DMACR_reg;
	//u32 en_interrupt;

	//IOC_IRQ_EN = 1 << 12; // this is IOC_IrqEn bit in MM2S_DMACR register
	//ERR_IRQ_EN = 1 << 14; // this is Err_IrqEn bit in MM2S_DMACR register

	iowrite32(reset, base_address); // writing to MM2S_DMACR register. Seting reset bit (3. bit)

	//MM2S_DMACR_reg = ioread32(base_address);				 // Reading from MM2S_DMACR register inside DMA
	//en_interrupt = MM2S_DMACR_reg | IOC_IRQ_EN | ERR_IRQ_EN; // seting 13. and 15.th bit in MM2S_DMACR
	//iowrite32(en_interrupt, base_address);					 // writing to MM2S_DMACR register
	return 0;
}

u32 dma_simple_write(dma_addr_t TxBufferPtr, u32 max_pkt_len, void __iomem *base_address)
{
	u32 MM2S_DMACR_reg;

	MM2S_DMACR_reg = ioread32(base_address); // READ from MM2S_DMACR register

	iowrite32(0x1 | MM2S_DMACR_reg, base_address); // set RS bit in MM2S_DMACR register (this bit starts the DMA)

	iowrite32((u32)TxBufferPtr, base_address + 24); // Write into MM2S_SA register the value of TxBufferPtr.
	// With this, the DMA knows from where to start.

	iowrite32(max_pkt_len, base_address + 40); // Write into MM2S_LENGTH register. This is the length of a tranaction.
	// In our case this is the size of the image (640*480*4)
	return 0;
}

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init mm2s_dma_init(void)
{

	int ret = 0;
	int i = 0;

	printk(KERN_INFO "mm2s_dma_init: Initialize Module \"%s\"\n", DEVICE_NAME);
	ret = alloc_chrdev_region(&my_dev_id, 0, 1, "mm2s_region");
	if (ret)
	{
		printk(KERN_ALERT "mm2s_dma_init: Failed CHRDEV!\n");
		return -1;
	}
	printk(KERN_INFO "mm2s_dma_init: Successful CHRDEV!\n");
	my_class = class_create(THIS_MODULE, "mm2s_drv");
	if (my_class == NULL)
	{
		printk(KERN_ALERT "mm2s_dma_init: Failed class create!\n");
		goto fail_0;
	}
	printk(KERN_INFO "mm2s_dma_init: Successful class chardev1 create!\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "mm2s_dma");
	if (my_device == NULL)
	{
		goto fail_1;
	}

	printk(KERN_INFO "mm2s_dma_init: Device created\n");

	my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
		printk(KERN_ERR "mm2s_dma_init: Failed to add cdev\n");
		goto fail_2;
	}
	printk(KERN_INFO "mm2s_dma_init: Module init done\n");

	tx_vir_buffer = dma_alloc_coherent(NULL, MAX_PKT_LEN, &tx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if (!tx_vir_buffer)
	{
		printk(KERN_ALERT "mm2s_dma_init: Could not allocate dma_alloc_coherent for img");
		goto fail_3;
	}
	else
		printk("mm2s_dma_init: Successfully allocated memory for dma transaction buffer\n");
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		tx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "mm2s_dma_init: DMA memory reset.\n");
	return platform_driver_register(&mm2s_dma_driver);

fail_3:
	cdev_del(my_cdev);
fail_2:
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
fail_1:
	class_destroy(my_class);
fail_0:
	unregister_chrdev_region(my_dev_id, 1);
	return -1;
}

static void __exit mm2s_dma_exit(void)
{
	//Reset DMA memory
	int i = 0;
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		tx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "mm2s_dma_exit: DMA memory reset\n");

	// Exit Device Module
	platform_driver_unregister(&mm2s_dma_driver);
	cdev_del(my_cdev);
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
	class_destroy(my_class);
	unregister_chrdev_region(my_dev_id, 1);
	dma_free_coherent(NULL, MAX_PKT_LEN, tx_vir_buffer, tx_phy_buffer);
	printk(KERN_INFO "mm2s_dma_exit: Exit device module finished\"%s\".\n", DEVICE_NAME);
}

module_init(mm2s_dma_init);
module_exit(mm2s_dma_exit);
