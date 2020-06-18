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
MODULE_DESCRIPTION("Test Driver for s2mm controller IP.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:s2mm_dma controller");

#define DEVICE_NAME "s2mm_dma"
#define DRIVER_NAME "s2mm_dma_driver"
#define BUFF_SIZE 20
#define MAX_PKT_LEN 1024 * 4

//*******************FUNCTION PROTOTYPES************************************
static int s2mm_dma_probe(struct platform_device *pdev);
static int s2mm_dma_open(struct inode *i, struct file *f);
static int s2mm_dma_close(struct inode *i, struct file *f);
//static ssize_t s2mm_dma_read(struct file *f, char __user *buf, size_t length, loff_t *off);
//static ssize_t s2mm_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static ssize_t s2mm_dma_mmap(struct file *f, struct vm_area_struct *vma_s);
static int __init s2mm_dma_init(void);
static void __exit s2mm_dma_exit(void);
static int s2mm_dma_remove(struct platform_device *pdev);

//static irqreturn_t dma_isr(int irq, void *dev_id);
int dma_init(void __iomem *base_address);
u32 dma_simple_read(dma_addr_t RxBufferPtr, u32 max_pkt_len, void __iomem *base_address);

//*********************GLOBAL VARIABLES*************************************
struct s2mm_dma_info
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
static struct s2mm_dma_info *vp = NULL;

static struct file_operations my_fops =
	{
		.owner = THIS_MODULE,
		.open = s2mm_dma_open,
		.release = s2mm_dma_close,
		//.read = s2mm_dma_read,
		//.write = s2mm_dma_write,
		.mmap = s2mm_dma_mmap};

static struct of_device_id s2mm_dma_of_match[] = {
	{
		.compatible = "xlnx,axi-dma-s2mm-channel",
	},
	{.compatible = "axi_dma_s2mm"},
	{/* end of list */},
};
MODULE_DEVICE_TABLE(of, s2mm_dma_of_match);

static struct platform_driver s2mm_dma_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = s2mm_dma_of_match,
	},
	.probe = s2mm_dma_probe,
	.remove = s2mm_dma_remove,
};

dma_addr_t rx_phy_buffer;
u32 *rx_vir_buffer;

//***************************************************************************
// PROBE AND REMOVE
static int s2mm_dma_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;

	printk(KERN_INFO "S2MM Probe: Probing\n");
	// Get phisical register adress space from device tree
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem)
	{
		printk(KERN_ALERT "S2MM Probe: Failed to get reg resource\n");
		return -ENODEV;
	}
	// Get memory for structure s2mm_dma_info
	vp = (struct s2mm_dma_info *)kmalloc(sizeof(struct s2mm_dma_info), GFP_KERNEL);
	if (!vp)
	{
		printk(KERN_ALERT "S2MM Probe: Could not allocate memory for structure s2mm_dma_info\n");
		return -ENOMEM;
	}
	// Put phisical adresses in timer_info structure
	vp->mem_start = r_mem->start;
	vp->mem_end = r_mem->end;

	// Reserve that memory space for this driver
	if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
	{
		printk(KERN_ALERT "S2MM Probe: Could not lock memory region at %p\n", (void *)vp->mem_start);
		rc = -EBUSY;
		goto error1;
	}
	// Remap phisical to virtual adresses

	vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	if (!vp->base_addr)
	{
		printk(KERN_ALERT "S2MM Probe: Could not allocate memory for remapping\n");
		rc = -EIO;
		goto error2;
	}

	// Get irq num
	/*
	vp->irq_num = platform_get_irq(pdev, 0);
	if (!vp->irq_num)
	{
		printk(KERN_ERR "s2mm_dma_probe: Could not get IRQ resource\n");
		rc = -ENODEV;
		goto error2;
	}
	*/
	/*
	if (request_irq(vp->irq_num, dma_isr, 0, DEVICE_NAME, NULL))
	{
		printk(KERN_ERR "s2mm_dma_probe: Could not register IRQ %d\n", vp->irq_num);
		return -EIO;
		goto error3;
	}
	else
	{
		printk(KERN_INFO "s2mm_dma_probe: Registered IRQ %d\n", vp->irq_num);
	}
*/
	/* INIT DMA */
	dma_init(vp->base_addr);
	dma_simple_read(rx_phy_buffer, MAX_PKT_LEN, vp->base_addr); // helper function, defined later

	printk(KERN_NOTICE "S2MM Probe: Platform driver registered\n");
	return 0; //ALL OK
/*
error3:
	iounmap(vp->base_addr);
	*/
error2:
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
error1:
	return rc;
}

static int s2mm_dma_remove(struct platform_device *pdev)
{
	u32 reset = 0x00000004;
	// writing to S2MM_DMACR register. Seting reset bit (3. bit)
	printk(KERN_INFO "S2MM Remove: Reset\n");
	iowrite32(reset, vp->base_addr);

	//free_irq(vp->irq_num, NULL);
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
	printk(KERN_INFO "S2MM Removed\n");
	return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS
static int s2mm_dma_open(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "s2mm_dma opened\n");
	return 0;
}

static int s2mm_dma_close(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "s2mm_dma closed\n");
	return 0;
}
/*
static ssize_t s2mm_dma_read(struct file *f, char __user *buf, size_t length, loff_t *off)
{
	int ret;
	char buff[BUFF_SIZE];
	long int len = 0;

	len = scnprintf(buff, BUFF_SIZE, "%d ", rx_vir_buffer[0]);
	ret = copy_to_user(buf, buff, len);
	if (ret)
	{
		printk("MM2S copy from user failed \n");
		return -EFAULT;
	}
	printk(KERN_INFO "S2MM Succesfully read value %s\n", buff);

	return len;
}

static ssize_t s2mm_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
	//printk("s2mm_dma write\n");
	return 0;
}
*/
static ssize_t s2mm_dma_mmap(struct file *f, struct vm_area_struct *vma_s)
{
	int ret = 0;
	long length = vma_s->vm_end - vma_s->vm_start;

	printk(KERN_INFO "S2MM: RX Buffer is being memory mapped\n");

	if (length > MAX_PKT_LEN)
	{
		return -EIO;
		printk(KERN_ERR "S2MM: Trying to mmap more space than it's allocated\n");
	}

	ret = dma_mmap_coherent(NULL, vma_s, rx_vir_buffer, rx_phy_buffer, length);
	if (ret < 0)
	{
		printk(KERN_ERR "S2MM: Memory map failed\n");
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
	//Read pending interrupts
	IrqStatus = ioread32(vp->base_addr + 4);			  //read irq status from S2MM_DMASR register
	iowrite32(IrqStatus | 0x00007000, vp->base_addr + 4); //clear irq status in S2MM_DMASR register
	//(clearing is done by writing 1 on 13. bit in S2MM_DMASR (IOC_Irq)

	//Send a transaction
	//dma_simple_read(rx_phy_buffer, MAX_PKT_LEN, vp->base_addr); //My function that starts a DMA transaction
	printk(KERN_INFO "S2MM DMA Interrupt Detected");
	return IRQ_HANDLED;
	;
}*/
int dma_init(void __iomem *base_address)
{
	u32 reset = 0x00000004;
	//u32 IOC_IRQ_EN;
	//u32 ERR_IRQ_EN;
	//u32 S2MM_DMACR_reg;
	//u32 en_interrupt;

	//IOC_IRQ_EN = 1 << 12; // this is IOC_IrqEn bit in S2MM_DMACR register
	//ERR_IRQ_EN = 1 << 14; // this is Err_IrqEn bit in S2MM_DMACR register

	iowrite32(reset, base_address); // writing to S2MM_DMACR register. Seting reset bit (3. bit)
	printk(KERN_INFO "S2MM Init: Reset\n");
	//S2MM_DMACR_reg = ioread32(base_address);				 // Reading from S2MM_DMACR register inside DMA
	//en_interrupt = S2MM_DMACR_reg | IOC_IRQ_EN | ERR_IRQ_EN; // seting 13. and 15.th bit in S2MM_DMACR
	//iowrite32(en_interrupt, base_address);					 // writing to S2MM_DMACR register
	return 0;
}

u32 dma_simple_read(dma_addr_t RxBufferPtr, u32 max_pkt_len, void __iomem *base_address)
{
	u32 S2MM_DMACR_reg;

	S2MM_DMACR_reg = ioread32(base_address); // READ from S2MM_DMACR register

	iowrite32(0x1 | S2MM_DMACR_reg, base_address); // set RS bit in S2MM_DMACR register (this bit starts the DMA)

	iowrite32((u32)RxBufferPtr, base_address + 24); // Write into S2MM_DA register the value of RxBufferPtr.
	// With this, the DMA knows from where to start.

	iowrite32(max_pkt_len, base_address + 40); // Write into S2MM_LENGTH register. This is the length of a tranaction.
	// In our case this is the size of the image (640*480*4)
	printk(KERN_INFO "S2MM Simple Read\n");
	return 0;
}

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init s2mm_dma_init(void)
{

	int ret = 0;
	int i = 0;

	printk(KERN_INFO "S2MM DMA Init: Initialize Module \"%s\"\n", DEVICE_NAME);
	ret = alloc_chrdev_region(&my_dev_id, 0, 1, "s2mm_region");
	if (ret)
	{
		printk(KERN_ALERT "S2MM DMA Init: Failed CHRDEV!\n");
		return -1;
	}
	printk(KERN_INFO "S2MM DMA Init: Successful CHRDEV!\n");
	my_class = class_create(THIS_MODULE, "s2mm_drv");
	if (my_class == NULL)
	{
		printk(KERN_ALERT "S2MM DMA Init: Failed class create!\n");
		goto fail_0;
	}
	printk(KERN_INFO "S2MM DMA Init: Successful class chardev1 create!\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "s2mm_dma");
	if (my_device == NULL)
	{
		goto fail_1;
	}

	printk(KERN_INFO "S2MM DMA Init: Device created\n");

	my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
		printk(KERN_ERR "S2MM DMA Init: Failed to add cdev\n");
		goto fail_2;
	}
	printk(KERN_INFO "S2MM DMA Init: Module init done\n");

	rx_vir_buffer = dma_alloc_coherent(NULL, MAX_PKT_LEN, &rx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if (!rx_vir_buffer)
	{
		printk(KERN_ALERT "S2MM DMA Init: Could not allocate dma_alloc_coherent for img");
		goto fail_3;
	}
	else
		printk("S2MM DMA Init: Successfully allocated memory for dma transaction buffer\n");
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		rx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "S2MM DMA Init: Memory reset.\n");
	return platform_driver_register(&s2mm_dma_driver);

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

static void __exit s2mm_dma_exit(void)
{
	//Reset DMA memory
	int i = 0;
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		rx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "S2MM DMA Exit: Memory reset\n");

	// Exit Device Module
	platform_driver_unregister(&s2mm_dma_driver);
	cdev_del(my_cdev);
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id), 0));
	class_destroy(my_class);
	unregister_chrdev_region(my_dev_id, 1);
	dma_free_coherent(NULL, MAX_PKT_LEN, rx_vir_buffer, rx_phy_buffer);
	printk(KERN_INFO "S2MM DMA Exit: Exit device module finished\"%s\".\n", DEVICE_NAME);
}

module_init(s2mm_dma_init);
module_exit(s2mm_dma_exit);
