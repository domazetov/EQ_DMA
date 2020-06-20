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
MODULE_DESCRIPTION("Test Driver for test controller IP.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:dma controller");

#define DEVICE_NAME "test_dma"
#define DRIVER_NAME "dma_driver"
#define MAX_PKT_LEN 1024 * 4

//*******************FUNCTION PROTOTYPES************************************
static int test_dma_probe(struct platform_device *pdev);
static int test_dma_open(struct inode *i, struct file *f);
static int test_dma_close(struct inode *i, struct file *f);
static ssize_t test_dma_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t test_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static ssize_t rx_dma_mmap(struct file *f, struct vm_area_struct *vma_s);
static ssize_t tx_dma_mmap(struct file *f, struct vm_area_struct *vma_s);
static int __init test_dma_init(void);
static int rx_init(void);
static int tx_init(void);
static void __exit test_dma_exit(void);
static int test_dma_remove(struct platform_device *pdev);

static irqreturn_t dma_isr(int irq, void *dev_id);
int dma_init(void __iomem *base_address);
u32 dma_simple_write(dma_addr_t TxBufferPtr, dma_addr_t RxBufferPtr, u32 max_pkt_len, void __iomem *base_address);

//*********************GLOBAL VARIABLES*************************************
struct test_dma_info
{
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
	int irq_num;
};

static struct cdev *tx_cdev;
static dev_t tx_dev_id;
static struct class *tx_class;
static struct device *tx_device;

static struct cdev *rx_cdev;
static dev_t rx_dev_id;
static struct class *rx_class;
static struct device *rx_device;

static struct test_dma_info *vp = NULL;

static struct file_operations rx_fops =
	{
		.owner = THIS_MODULE,
		.open = test_dma_open,
		.release = test_dma_close,
		.read = test_dma_read,
		.write = test_dma_write,
		.mmap = rx_dma_mmap};

static struct file_operations tx_fops =
	{
		.owner = THIS_MODULE,
		.open = test_dma_open,
		.release = test_dma_close,
		.read = test_dma_read,
		.write = test_dma_write,
		.mmap = tx_dma_mmap};

static struct of_device_id test_dma_of_match[] = {
	{
		.compatible = "axi_dma",
	},
	{/* end of list */},
};
MODULE_DEVICE_TABLE(of, test_dma_of_match);

static struct platform_driver test_dma_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = test_dma_of_match,
	},
	.probe = test_dma_probe,
	.remove = test_dma_remove,
};

dma_addr_t tx_phy_buffer;
u32 *tx_vir_buffer;

dma_addr_t rx_phy_buffer;
u32 *rx_vir_buffer;
//***************************************************************************
// PROBE AND REMOVE
static int test_dma_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;

	printk(KERN_INFO "test_dma_probe: Probing\n");
	// Get phisical register adress space from device tree
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem)
	{
		printk(KERN_ALERT "test_dma_probe: Failed to get reg resource\n");
		return -ENODEV;
	}
	// Get memory for structure test_dma_info
	vp = (struct test_dma_info *)kmalloc(sizeof(struct test_dma_info), GFP_KERNEL);
	if (!vp)
	{
		printk(KERN_ALERT "test_dma_probe: Could not allocate memory for structure test_dma_info\n");
		return -ENOMEM;
	}
	// Put phisical adresses in timer_info structure
	vp->mem_start = r_mem->start;
	vp->mem_end = r_mem->end;

	// Reserve that memory space for this driver
	if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
	{
		printk(KERN_ALERT "test_dma_probe: Could not lock memory region at %p\n", (void *)vp->mem_start);
		rc = -EBUSY;
		goto error1;
	}
	// Remap phisical to virtual adresses

	vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	if (!vp->base_addr)
	{
		printk(KERN_ALERT "test_dma_probe: Could not allocate memory for remapping\n");
		rc = -EIO;
		goto error2;
	}

	// Get irq num
	vp->irq_num = platform_get_irq(pdev, 0);
	if (!vp->irq_num)
	{
		printk(KERN_ERR "test_dma_probe: Could not get IRQ resource\n");
		rc = -ENODEV;
		goto error2;
	}

	if (request_irq(vp->irq_num, dma_isr, 0, DEVICE_NAME, NULL))
	{
		printk(KERN_ERR "test_dma_probe: Could not register IRQ %d\n", vp->irq_num);
		return -EIO;
		goto error3;
	}
	else
	{
		printk(KERN_INFO "test_dma_probe: Registered IRQ %d\n", vp->irq_num);
	}

	/* INIT DMA */
	dma_init(vp->base_addr);
	dma_simple_write(tx_phy_buffer, rx_phy_buffer, MAX_PKT_LEN, vp->base_addr); // helper function, defined later

	printk(KERN_NOTICE "test_dma_probe: test platform driver registered\n");
	return 0; //ALL OK

error3:
	iounmap(vp->base_addr);
error2:
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
error1:
	return rc;
}

static int test_dma_remove(struct platform_device *pdev)
{
	u32 reset = 0x00000004;
	// writing to MM2S_DMACR register. Seting reset bit (3. bit)
	printk(KERN_INFO "test_dma_probe: resseting");
	iowrite32(reset, vp->base_addr);

	free_irq(vp->irq_num, NULL);
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
	printk(KERN_INFO "test_dma_probe: test DMA removed");
	return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS
static int test_dma_open(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "test_dma opened\n");
	return 0;
}

static int test_dma_close(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "test_dma closed\n");
	return 0;
}

static ssize_t test_dma_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	//printk("test_dma read\n");
	return 0;
}

static ssize_t test_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
	//printk("test_dma write\n");
	return 0;
}

static ssize_t rx_dma_mmap(struct file *f, struct vm_area_struct *vma_s)
{
	int ret = 0;
	long length = vma_s->vm_end - vma_s->vm_start;

	//printk(KERN_INFO "DMA TX Buffer is being memory mapped\n");

	if (length > MAX_PKT_LEN)
	{
		return -EIO;
		printk(KERN_ERR "S2MM Trying to mmap more space than it's allocated\n");
	}

	ret = dma_mmap_coherent(NULL, vma_s, rx_vir_buffer, rx_phy_buffer, length);
	if (ret < 0)
	{
		printk(KERN_ERR "MMAP failed for S2MM\n");
		return ret;
	}
	return 0;
}

static ssize_t tx_dma_mmap(struct file *f, struct vm_area_struct *vma_s)
{
	int ret = 0;
	long length = vma_s->vm_end - vma_s->vm_start;

	//printk(KERN_INFO "DMA TX Buffer is being memory mapped\n");

	if (length > MAX_PKT_LEN)
	{
		return -EIO;
		printk(KERN_ERR "MM2S Trying to mmap more space than it's allocated\n");
	}

	ret = dma_mmap_coherent(NULL, vma_s, tx_vir_buffer, tx_phy_buffer, length);
	if (ret < 0)
	{
		printk(KERN_ERR "MMAP failed for MM2S\n");
		return ret;
	}
	return 0;
}

/****************************************************/
// IMPLEMENTATION OF DMA related functions

static irqreturn_t dma_isr(int irq, void *dev_id)
{
	u32 IrqStatus;
	/* Read pending interrupts */
	IrqStatus = ioread32(vp->base_addr + 4 + 48);			   //read irq status from S2MM_DMASR register
	iowrite32(IrqStatus | 0x00007000, vp->base_addr + 4 + 48); //clear irq status in S2MM_DMASR register

	IrqStatus = ioread32(vp->base_addr + 4);			  //read irq status from MM2S_DMASR register
	iowrite32(IrqStatus | 0x00007000, vp->base_addr + 4); //clear irq status in MM2S_DMASR register
	//(clearing is done by writing 1 on 13. bit in MM2S_DMASR (IOC_Irq)

	/*Send a transaction*/
	dma_simple_write(tx_phy_buffer, rx_phy_buffer, MAX_PKT_LEN, vp->base_addr); //My function that starts a DMA transaction

	printk(KERN_INFO "DMA ISR: IRQ cleared and starting DMA transaction!\n");
	return IRQ_HANDLED;
}

int dma_init(void __iomem *base_address)
{
	u32 reset = 0x00000004;
	u32 IOC_IRQ_EN;
	u32 ERR_IRQ_EN;
	u32 MM2S_DMACR_reg;
	u32 S2MM_DMACR_reg;
	u32 en_interrupt;

	IOC_IRQ_EN = 1 << 12; // this is IOC_IrqEn bit in MM2S_DMACR register
	ERR_IRQ_EN = 1 << 14; // this is Err_IrqEn bit in MM2S_DMACR register

	iowrite32(reset, base_address); // writing to MM2S_DMACR register. Seting reset bit (3. bit) Resets whole DMA

	S2MM_DMACR_reg = ioread32(base_address + 48);
	MM2S_DMACR_reg = ioread32(base_address); // Reading from MM2S_DMACR register inside DMA

	en_interrupt = S2MM_DMACR_reg | IOC_IRQ_EN | ERR_IRQ_EN; // seting 13. and 15.th bit in S2MM_DMACR
	iowrite32(en_interrupt, base_address);					 // writing to S2MM_DMACR register

	en_interrupt = MM2S_DMACR_reg | IOC_IRQ_EN | ERR_IRQ_EN; // seting 13. and 15.th bit in MM2S_DMACR
	iowrite32(en_interrupt, base_address + 48);				 // writing to MM2S_DMACR register

	printk(KERN_INFO "DMA Init: Reset and interrupts set!\n");
	return 0;
}

u32 dma_simple_write(dma_addr_t TxBufferPtr, dma_addr_t RxBufferPtr, u32 max_pkt_len, void __iomem *base_address)
{
	u32 MM2S_DMACR_reg;
	u32 S2MM_DMACR_reg;

	S2MM_DMACR_reg = ioread32(base_address + 48); // READ from S2MM_DMACR register
	MM2S_DMACR_reg = ioread32(base_address);	  // READ from MM2S_DMACR register

	iowrite32(0x1 | S2MM_DMACR_reg, base_address + 48); // set RS bit in S2MM_DMACR register (this bit starts the DMA)
	iowrite32(0x1 | MM2S_DMACR_reg, base_address);		// set RS bit in MM2S_DMACR register (this bit starts the DMA)

	iowrite32((u32)RxBufferPtr, base_address + 24 + 48); // Write into S2MM_SA register the value of RxBufferPtr.
	iowrite32((u32)TxBufferPtr, base_address + 24);		 // Write into MM2S_SA register the value of TxBufferPtr.
	// With this, the DMA knows from where to start.

	iowrite32(max_pkt_len, base_address + 40 + 48); // Write into S2MM_LENGTH register. This is the length of a tranaction.
	iowrite32(max_pkt_len, base_address + 40);		// Write into MM2S_LENGTH register. This is the length of a tranaction.

	printk(KERN_INFO "DMA Init: DMACR SA and LENGTH registers set!\n");
	return 0;
}

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int rx_init(void)
{
	int ret = 0;
	int i = 0;

	printk(KERN_INFO "RX DMA Init: Initialize Module \"%s\"\n", DEVICE_NAME);
	ret = alloc_chrdev_region(&rx_dev_id, 0, 1, "dma_driver");
	if (ret)
	{
		printk(KERN_ALERT "RX DMA Init: Failed CHRDEV!\n");
		return -1;
	}
	printk(KERN_INFO "RX DMA Init: Successful CHRDEV!\n");
	rx_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (rx_class == NULL)
	{
		printk(KERN_ALERT "RX DMA Init: Failed class create!\n");
		goto fail_0;
	}
	printk(KERN_INFO "RX DMA Init: Successful class chardev create!\n");
	rx_device = device_create(rx_class, NULL, MKDEV(MAJOR(rx_dev_id), 0), NULL, "rx_dma");
	if (rx_device == NULL)
	{
		goto fail_1;
	}

	printk(KERN_INFO "RX DMA Init: Device created\n");

	rx_cdev = cdev_alloc();
	rx_cdev->ops = &rx_fops;
	rx_cdev->owner = THIS_MODULE;
	ret = cdev_add(rx_cdev, rx_dev_id, 1);

	if (ret)
	{
		printk(KERN_ERR "RX DMA Init: Failed to add cdev\n");
		goto fail_2;
	}
	printk(KERN_INFO "RX DMA Init: Module init done\n");

	rx_vir_buffer = dma_alloc_coherent(NULL, MAX_PKT_LEN, &rx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if (!rx_vir_buffer)
	{
		printk(KERN_ALERT "RX DMA Init: Could not allocate dma_alloc_coherent");
		goto fail_3;
	}
	else
		printk("RX DMA Init: Successfully allocated memory for dma transaction buffer\n");
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		rx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "RX DMA Init: DMA memory reset.\n");
	return platform_driver_register(&test_dma_driver);

fail_3:
	cdev_del(rx_cdev);
fail_2:
	device_destroy(rx_class, MKDEV(MAJOR(rx_dev_id), 0));
fail_1:
	class_destroy(rx_class);
fail_0:
	unregister_chrdev_region(rx_dev_id, 1);
	return -1;
}

static int tx_init(void)
{
	int ret = 0;
	int i = 0;

	printk(KERN_INFO "TX DMA Init: Initialize Module \"%s\"\n", DEVICE_NAME);
	ret = alloc_chrdev_region(&tx_dev_id, 0, 1, "dma_driver");
	if (ret)
	{
		printk(KERN_ALERT "TX DMA Init: Failed CHRDEV!\n");
		return -1;
	}
	printk(KERN_INFO "TX DMA Init: Successful CHRDEV!\n");
	tx_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (tx_class == NULL)
	{
		printk(KERN_ALERT "TX DMA Init: Failed class create!\n");
		goto fail_0;
	}
	printk(KERN_INFO "TX DMA Init: Successful class chardev create!\n");
	tx_device = device_create(tx_class, NULL, MKDEV(MAJOR(tx_dev_id), 0), NULL, "tx_dma");
	if (tx_device == NULL)
	{
		goto fail_1;
	}

	printk(KERN_INFO "TX DMA Init: Device created\n");

	tx_cdev = cdev_alloc();
	tx_cdev->ops = &tx_fops;
	tx_cdev->owner = THIS_MODULE;
	ret = cdev_add(tx_cdev, tx_dev_id, 1);

	if (ret)
	{
		printk(KERN_ERR "TX DMA Init: Failed to add cdev\n");
		goto fail_2;
	}
	printk(KERN_INFO "TX DMA Init: Module init done\n");

	tx_vir_buffer = dma_alloc_coherent(NULL, MAX_PKT_LEN, &tx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if (!tx_vir_buffer)
	{
		printk(KERN_ALERT "TX DMA Init: Could not allocate dma_alloc_coherent");
		goto fail_3;
	}
	else
		printk("TX DMA Init: Successfully allocated memory for dma transaction buffer\n");
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		tx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "TX DMA Init: DMA memory reset.\n");
	return platform_driver_register(&test_dma_driver);

fail_3:
	cdev_del(tx_cdev);
fail_2:
	device_destroy(tx_class, MKDEV(MAJOR(tx_dev_id), 0));
fail_1:
	class_destroy(tx_class);
fail_0:
	unregister_chrdev_region(tx_dev_id, 1);
	return -1;
}

static int __init test_dma_init(void)
{
	rx_init();
	tx_init();
}

static void rx_dma_exit(void)
{
	//Reset DMA memory
	int i = 0;
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		rx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "RX DMA EXIT: DMA memory reset\n");

	// Exit Device Module
	platform_driver_unregister(&test_dma_driver);
	cdev_del(rx_cdev);
	device_destroy(rx_class, MKDEV(MAJOR(rx_dev_id), 0));
	class_destroy(rx_class);
	unregister_chrdev_region(rx_dev_id, 1);
	dma_free_coherent(NULL, MAX_PKT_LEN, rx_vir_buffer, rx_phy_buffer);
	printk(KERN_INFO "RX DMA EXIT: Exit device module finished\"%s\".\n", DEVICE_NAME);
}

static void tx_dma_exit(void)
{
	//Reset DMA memory
	int i = 0;
	for (i = 0; i < MAX_PKT_LEN / 4; i++)
		tx_vir_buffer[i] = 0x00000000;
	printk(KERN_INFO "TX DMA EXIT: DMA memory reset\n");

	// Exit Device Module
	platform_driver_unregister(&test_dma_driver);
	cdev_del(tx_cdev);
	device_destroy(tx_class, MKDEV(MAJOR(tx_dev_id), 0));
	class_destroy(tx_class);
	unregister_chrdev_region(tx_dev_id, 1);
	dma_free_coherent(NULL, MAX_PKT_LEN, tx_vir_buffer, tx_phy_buffer);
	printk(KERN_INFO "TX DMA EXIT: Exit device module finished\"%s\".\n", DEVICE_NAME);
}

static void __exit test_dma_exit(void)
{
	rx_dma_exit();
	tx_dma_exit();
}

module_init(test_dma_init);
module_exit(test_dma_exit);
