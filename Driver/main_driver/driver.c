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
#include <linux/delay.h>
#include <linux/io.h>			   //iowrite ioread
#include <linux/slab.h>			   //kmalloc kfree
#include <linux/platform_device.h> //platform driver
#include <linux/of.h>			   //of_match_table
#include <linux/ioport.h>		   //ioremap
#include <linux/miscdevice.h>

#include <linux/dma-mapping.h> //dma access
#include <linux/mm.h>		   //dma access
#include <linux/interrupt.h>   //interrupt handlers

MODULE_AUTHOR("Zero");
MODULE_DESCRIPTION("AXI DMA RX & TX Driver");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:dma controller");

#define DEVICE_NAME "axi_dma"
#define DRIVER_NAME "dma_driver"
#define BUFF_SIZE 20
#define MAX_PKT_LEN 1024 * 4
#define NUM_OF_PKT 1292 //3min

int num_of_wr = 0;
int endRead = 0;
unsigned int mmap_length = 0;

//*******************FUNCTION PROTOTYPES************************************
static int axi_dma_probe(struct platform_device *pdev);
static int axi_dma_open(struct inode *i, struct file *f);
static int axi_dma_close(struct inode *i, struct file *f);
static ssize_t axi_dma_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t axi_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off);
static ssize_t dma_mmap(struct file *f, struct vm_area_struct *vma_s);
static int __init axi_dma_init(void);
static void __exit axi_dma_exit(void);
static int axi_dma_remove(struct platform_device *pdev);

//static irqreturn_t rx_dma_isr(int irq, void *dev_id);
int rx_dma_init(void __iomem *base_address);
u32 rx_dma_simple_write(dma_addr_t RxBufferPtr, u32 max_pkt_len, void __iomem *base_address);

//static irqreturn_t tx_dma_isr(int irq, void *dev_id);
int tx_dma_init(void __iomem *base_address);
u32 tx_dma_simple_write(dma_addr_t TxBufferPtr, u32 max_pkt_len, void __iomem *base_address);

//*********************GLOBAL VARIABLES*************************************
struct axi_dma_info
{
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
	int irq_num_tx;
	int irq_num_rx;
};

static struct cdev *dma_cdev;
static dev_t dma_dev_id;
static struct class *dma_class;
static struct device *dma_device;

static struct axi_dma_info *vp = NULL;

static struct file_operations dma_fops =
	{
		.owner = THIS_MODULE,
		.open = axi_dma_open,
		.release = axi_dma_close,
		.read = axi_dma_read,
		.write = axi_dma_write,
		.mmap = dma_mmap};

static struct of_device_id axi_dma_of_match[] = {
	{
		.compatible = "axi_dma",
	},
	{/* end of list */},
};
MODULE_DEVICE_TABLE(of, axi_dma_of_match);

static struct platform_driver dma_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = axi_dma_of_match,
	},
	.probe = axi_dma_probe,
	.remove = axi_dma_remove,
};

dma_addr_t tx_phy_buffer;
u32 *tx_vir_buffer;

dma_addr_t rx_phy_buffer;
u32 *rx_vir_buffer;

//***************************************************************************
// PROBE AND REMOVE

static int axi_dma_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;

	printk(KERN_INFO "DMA Probe: Probing\n");
	// Get phisical register adress space from device tree
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!r_mem)
	{
		printk(KERN_ALERT "DMA Probe: Failed to get reg resource\n");
		return -ENODEV;
	}
	// Get memory for structure axi_dma_info
	vp = (struct axi_dma_info *)kmalloc(sizeof(struct axi_dma_info), GFP_KERNEL);
	if (!vp)
	{
		printk(KERN_ALERT "DMA Probe: Could not allocate memory for structure axi_dma_info\n");
		return -ENOMEM;
	}
	// Put phisical adresses in timer_info structure
	vp->mem_start = r_mem->start;
	vp->mem_end = r_mem->end;

	// Reserve that memory space for this driver
	if (!request_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1, DRIVER_NAME))
	{
		printk(KERN_ALERT "DMA Probe: Could not lock memory region at %p\n", (void *)vp->mem_start);
		rc = -EBUSY;
		goto error1;
	}
	// Remap phisical to virtual adresses

	vp->base_addr = ioremap(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	if (!vp->base_addr)
	{
		printk(KERN_ALERT "DMA Probe: Could not allocate memory for remapping\n");
		rc = -EIO;
		goto error2;
	}

	// Get irq num
	/*
	vp->irq_num_tx = platform_get_irq(pdev, 0);
	if (!vp->irq_num_tx)
	{
		printk(KERN_ERR "DMA Probe: Could not get TX IRQ resource\n");
		rc = -ENODEV;
		goto error2;
	}

	if (request_irq(vp->irq_num_tx, tx_dma_isr, 0, DEVICE_NAME, NULL))
	{
		printk(KERN_ERR "DMA Probe: Could not register TX IRQ %d\n", vp->irq_num_tx);
		return -EIO;
		goto error3;
	}
	else
	{
		printk(KERN_INFO "DMA Probe: Registered TX IRQ %d\n", vp->irq_num_tx);
	}

	vp->irq_num_rx = platform_get_irq(pdev, 1);
	if (!vp->irq_num_rx)
	{
		printk(KERN_ERR "DMA Probe: Could not get RX IRQ resource\n");
		rc = -ENODEV;
		goto error2;
	}

	if (request_irq(vp->irq_num_rx, rx_dma_isr, 0, DEVICE_NAME, NULL))
	{
		printk(KERN_ERR "DMA Probe: Could not register RX IRQ %d\n", vp->irq_num_rx);
		return -EIO;
		goto error3;
	}
	else
	{
		printk(KERN_INFO "DMA Probe: Registered RX IRQ %d\n", vp->irq_num_rx);
	}
*/
	/* INIT DMA */
	rx_dma_init(vp->base_addr);
	tx_dma_init(vp->base_addr);
	rx_dma_simple_write(rx_phy_buffer, MAX_PKT_LEN, vp->base_addr); // helper function, defined later
	tx_dma_simple_write(tx_phy_buffer, MAX_PKT_LEN, vp->base_addr); // helper function, defined later

	printk(KERN_NOTICE "DMA Probe: DMA platform driver registered\n");
	return 0; //ALL OK

//error3:
//	iounmap(vp->base_addr);
error2:
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
error1:
	return rc;
}

static int axi_dma_remove(struct platform_device *pdev)
{
	u32 reset = 0x00000004;
	// writing to MM2S_DMACR register. Seting reset bit (3. bit)
	printk(KERN_INFO "DMA Probe: resseting");
	iowrite32(reset, vp->base_addr);

	free_irq(vp->irq_num_tx, NULL);
	free_irq(vp->irq_num_rx, NULL);
	iounmap(vp->base_addr);
	release_mem_region(vp->mem_start, vp->mem_end - vp->mem_start + 1);
	kfree(vp);
	printk(KERN_INFO "DMA Probe: DMA removed");
	return 0;
}

//***************************************************
// IMPLEMENTATION OF FILE OPERATION FUNCTIONS
static int axi_dma_open(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "DMA opened\n");
	return 0;
}

static int axi_dma_close(struct inode *i, struct file *f)
{
	//printk(KERN_INFO "DMA closed\n");
	return 0;
}

static ssize_t axi_dma_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	/*
	int ret = 0;
	int length = 0;
	u32 value = 0;
	char buff[BUFF_SIZE];
	printk("DMA Read.\n");
	if (endRead)
	{
		endRead = 0;
		return 0;
	}
	rx_dma_simple_write(rx_phy_buffer, MAX_PKT_LEN, vp->base_addr);

	value = rx_vir_buffer[0];

	length = scnprintf(buff, BUFF_SIZE, "%d ", value);
	ret = copy_to_user(buf, buff, length);
	if (ret)
	{
		printk(KERN_INFO "DMA Read: Copy to user failed.\n");
		return -EFAULT;
	}
	printk(KERN_INFO "DMA Read: Succesfully read.\n");
	endRead = 1;
	return length;
	*/
	int ret = 0;
	char buff[BUFF_SIZE];
	printk("DMA Read.\n");
	if (endRead)
	{
		endRead = 0;
		return 0;
	}

	len = scnprintf(buff, BUFF_SIZE, "%c", '1');
	ret = copy_to_user(buf, buff, len);
	if (ret)
		return -EFAULT;

	if (num_of_wr == 1)
	{
		len = scnprintf(buff, BUFF_SIZE, "%c", '1');
		//printk(KERN_INFO "############ 1\n");
		ret = copy_to_user(buf, buff, len);
		if (ret)
			return -EFAULT;
	}
	else
	{
		len = scnprintf(buff, BUFF_SIZE, "%c", '0');
		//printk(KERN_INFO "############ 0\n");
		ret = copy_to_user(buf, buff, len);
		if (ret)
			return -EFAULT;
	}
	endRead = 1;
	return len;
}

static ssize_t axi_dma_write(struct file *f, const char __user *buf, size_t length, loff_t *off)
{
	char buff[BUFF_SIZE];
	char start[] = "start";
	int ret = 0;
	int i;

	printk("DMA Write.\n");
	ret = copy_from_user(buff, buf, length);
	if (ret)
	{
		printk(KERN_INFO "DMA Write: Copy from user failed.\n");
		return -EFAULT;
	}
	buff[length - 1] = '\0';

	ret = strcmp(start, buff);

	//printk(KERN_INFO "%s %s %d\n", start, buff, ret);

	if (!ret)
	{
		printk(KERN_INFO "DMA Write: Started.\n");
		num_of_wr = NUM_OF_PKT + 2;
		for (i = 0; i <= NUM_OF_PKT; i++)
		{
			tx_dma_simple_write(tx_phy_buffer + i * MAX_PKT_LEN, MAX_PKT_LEN, vp->base_addr);
			udelay(2000);
			rx_dma_simple_write(rx_phy_buffer + i * MAX_PKT_LEN, MAX_PKT_LEN, vp->base_addr);
			udelay(2000);
			num_of_wr--;
		}
	}
	else
		printk(KERN_INFO "DMA Write: Wrong command format.\n");

	return length;
}

static ssize_t dma_mmap(struct file *f, struct vm_area_struct *vma_s)
{
	int minor = MINOR(f->f_inode->i_rdev);
	int ret = 0;
	long length = vma_s->vm_end - vma_s->vm_start;

	if (length > NUM_OF_PKT * MAX_PKT_LEN)
	{
		return -EIO;
		printk(KERN_ERR "Trying to mmap more space than it's allocated.\n");
	}
	mmap_length = length;
	printk(KERN_INFO "MMAP length %d\n", mmap_length);

	switch (minor)
	{
	case 0: //dma_rx
		printk(KERN_INFO "DMA RX Buffer is being memory mapped.\n");

		ret = dma_mmap_coherent(NULL, vma_s, rx_vir_buffer, rx_phy_buffer, length);
		break;
	case 1: //dma_tx
		printk(KERN_INFO "DMA TX Buffer is being memory mapped.\n");

		ret = dma_mmap_coherent(NULL, vma_s, tx_vir_buffer, tx_phy_buffer, length);
		break;
	default:
		printk(KERN_INFO "MMAP: Invalid minor.\n");
	}

	if (ret < 0)
	{
		printk(KERN_ERR "MMAP failed. [%d].\n", minor);
		return ret;
	}

	printk(KERN_INFO "MMAP done, length: %ld.\n", length);
	return 0;
}

/****************************************************/
// IMPLEMENTATION OF DMA related functions
/*
static irqreturn_t rx_dma_isr(int irq, void *dev_id)
{
	u32 IrqStatus;
	// Read pending interrupts
	IrqStatus = ioread32(vp->base_addr + 4 + 48);			   //read irq status from S2MM_DMASR register
	iowrite32(IrqStatus | 0x00007000, vp->base_addr + 4 + 48); //clear irq status in S2MM_DMASR register
	//(clearing is done by writing 1 on 13. bit in MM2S_DMASR (IOC_Irq)
	//IrqStatus = ioread32(vp->base_addr + 4 + 48);
	//Send a transaction
	rx_dma_simple_write(rx_phy_buffer, MAX_PKT_LEN, vp->base_addr); //My function that starts a DMA transaction

	printk(KERN_INFO "DMA ISR RX: IRQ cleared and starting DMA transaction!\n"); //IRQ RX Status: 0x%x\n", (int)IrqStatus);
	return IRQ_HANDLED;
}

static irqreturn_t tx_dma_isr(int irq, void *dev_id)
{
	u32 IrqStatus;
	//Read pending interrupts
	IrqStatus = ioread32(vp->base_addr + 4);			  //read irq status from MM2S_DMASR register
	iowrite32(IrqStatus | 0x00007000, vp->base_addr + 4); //clear irq status in MM2S_DMASR register
	//(clearing is done by writing 1 on 13. bit in MM2S_DMASR (IOC_Irq)
	//IrqStatus = ioread32(vp->base_addr + 4);
	//Send a transaction
	tx_dma_simple_write(tx_phy_buffer, MAX_PKT_LEN, vp->base_addr); //My function that starts a DMA transaction

	printk(KERN_INFO "DMA ISR TX: IRQ cleared and starting DMA transaction!\n"); //IRQ TX Status: 0x%x\n", (int)IrqStatus);
	return IRQ_HANDLED;
}
*/
int rx_dma_init(void __iomem *base_address)
{
	u32 reset = 0x00000004;
	//u32 IOC_IRQ_EN;
	//u32 ERR_IRQ_EN;
	//u32 S2MM_DMACR_reg;
	//u32 en_interrupt;

	//IOC_IRQ_EN = 1 << 12; // this is IOC_IrqEn bit in S2MM_DMACR register
	//ERR_IRQ_EN = 1 << 14; // this is Err_IrqEn bit in S2MM_DMACR register
	//S2MM_DMACR_reg = ioread32(base_address + 48);
	//printk(KERN_INFO "S2MM_DMACR Before reset: 0x%x\n", (int)S2MM_DMACR_reg);
	iowrite32(reset, base_address + 48); // writing to S2MM_DMACR register. Seting reset bit (3. bit) Resets whole DMA

	//S2MM_DMACR_reg = ioread32(base_address + 48);

	//en_interrupt = S2MM_DMACR_reg | IOC_IRQ_EN | ERR_IRQ_EN; // seting 13. and 15.th bit in S2MM_DMACR
	//iowrite32(en_interrupt, base_address + 48);				 // writing to S2MM_DMACR register

	//S2MM_DMACR_reg = ioread32(base_address + 48);
	//printk(KERN_INFO "S2MM_DMACR After reset: 0x%x\n", (int)S2MM_DMACR_reg);

	return 0;
}

int tx_dma_init(void __iomem *base_address)
{
	u32 reset = 0x00000004;
	//u32 IOC_IRQ_EN;
	//u32 ERR_IRQ_EN;
	//u32 MM2S_DMACR_reg;
	//u32 en_interrupt;

	//IOC_IRQ_EN = 1 << 12; // this is IOC_IrqEn bit in MM2S_DMACR register
	//ERR_IRQ_EN = 1 << 14; // this is Err_IrqEn bit in MM2S_DMACR register
	//MM2S_DMACR_reg = ioread32(base_address);
	//printk(KERN_INFO "MM2S_DMACR Before reset: 0x%x\n", (int)MM2S_DMACR_reg);
	iowrite32(reset, base_address); // writing to MM2S_DMACR register. Seting reset bit (3. bit) Resets whole DMA

	//MM2S_DMACR_reg = ioread32(base_address); // Reading from MM2S_DMACR register inside DMA

	//en_interrupt = MM2S_DMACR_reg | IOC_IRQ_EN | ERR_IRQ_EN; // seting 13. and 15.th bit in MM2S_DMACR
	//iowrite32(en_interrupt, base_address);					 // writing to MM2S_DMACR register

	//MM2S_DMACR_reg = ioread32(base_address);
	//printk(KERN_INFO "MM2S_DMACR After reset: 0x%x\n", (int)MM2S_DMACR_reg);

	return 0;
}

u32 rx_dma_simple_write(dma_addr_t RxBufferPtr, u32 max_pkt_len, void __iomem *base_address)
{
	u32 S2MM_DMACR_reg;
	S2MM_DMACR_reg = ioread32(base_address + 48); // READ from S2MM_DMACR register
	//printk(KERN_INFO "S2MM_DMACR Before RS: 0x%x", (int)S2MM_DMACR_reg);
	iowrite32(0x1 | S2MM_DMACR_reg, base_address + 48); // set RS bit in S2MM_DMACR register (this bit starts the DMA)

	//S2MM_DMACR_reg = ioread32(base_address + 48 + 24);
	//printk(KERN_INFO "S2MM_DA Before: 0x%x", (int)S2MM_DMACR_reg);
	iowrite32((u32)RxBufferPtr, base_address + 24 + 48); // Write into S2MM_DA register the value of RxBufferPtr.
														 // With this, the DMA knows from where to start.

	//S2MM_DMACR_reg = ioread32(base_address + 48 + 40);
	//printk(KERN_INFO "S2MM_LENGTH Before: 0x%x", (int)S2MM_DMACR_reg);
	iowrite32(max_pkt_len, base_address + 40 + 48); // Write into S2MM_LENGTH register. This is the length of a tranaction.

	//S2MM_DMACR_reg = ioread32(base_address + 48);
	//printk(KERN_INFO "S2MM_DMACR After RS: 0x%x", (int)S2MM_DMACR_reg);

	//S2MM_DMACR_reg = ioread32(base_address + 48 + 24);
	//printk(KERN_INFO "S2MM_DA After: 0x%x", (int)S2MM_DMACR_reg);

	//S2MM_DMACR_reg = ioread32(base_address + 48 + 40);
	//printk(KERN_INFO "S2MM_LENGTH After: 0x%x", (int)S2MM_DMACR_reg);
	return 0;
}

u32 tx_dma_simple_write(dma_addr_t TxBufferPtr, u32 max_pkt_len, void __iomem *base_address)
{
	u32 MM2S_DMACR_reg;
	MM2S_DMACR_reg = ioread32(base_address); // READ from MM2S_DMACR register
	//printk(KERN_INFO "MM2S_DMACR Befre RS: 0x%x", (int)MM2S_DMACR_reg);
	iowrite32(0x1 | MM2S_DMACR_reg, base_address); // set RS bit in MM2S_DMACR register (this bit starts the DMA)

	//MM2S_DMACR_reg = ioread32(base_address + 24);
	//printk(KERN_INFO "MM2S_SA Before: 0x%x", (int)MM2S_DMACR_reg);
	iowrite32((u32)TxBufferPtr, base_address + 24); // Write into MM2S_SA register the value of TxBufferPtr.
	// With this, the DMA knows from where to start.
	//MM2S_DMACR_reg = ioread32(base_address + 40);
	//printk(KERN_INFO "MM2S_LENGTH Before: 0x%x", (int)MM2S_DMACR_reg);
	iowrite32(max_pkt_len, base_address + 40); // Write into MM2S_LENGTH register. This is the length of a tranaction.

	//MM2S_DMACR_reg = ioread32(base_address);
	//printk(KERN_INFO "MM2S_DMACR After RS: 0x%x", (int)MM2S_DMACR_reg);

	//MM2S_DMACR_reg = ioread32(base_address + 24);
	//printk(KERN_INFO "MM2S_SA After: 0x%x", (int)MM2S_DMACR_reg);

	//MM2S_DMACR_reg = ioread32(base_address + 40);
	//printk(KERN_INFO "MM2S_LENGTH After: 0x%x", (int)MM2S_DMACR_reg);
	return 0;
}

//***************************************************
// INIT AND EXIT FUNCTIONS OF THE DRIVER

static int __init axi_dma_init(void)
{

	int ret = 0;
	int i = 0;

	printk(KERN_INFO "DMA INIT: Initialize Module \"%s\".\n", DEVICE_NAME);
	ret = alloc_chrdev_region(&dma_dev_id, 0, 2, "dma_region");
	if (ret)
	{
		printk(KERN_ALERT "DMA INIT: Failed CHRDEV!\n");
		return -1;
	}
	printk(KERN_INFO "DMA INIT: Successful CHRDEV!\n");
	dma_class = class_create(THIS_MODULE, "dma_drv");
	if (dma_class == NULL)
	{
		printk(KERN_ALERT "DMA INIT: Failed class create!\n");
		goto fail_0;
	}
	printk(KERN_INFO "DMA INIT: Successful class chardevs create!\n");

	dma_device = device_create(dma_class, NULL, MKDEV(MAJOR(dma_dev_id), MINOR(dma_dev_id)), NULL, "dma_rx");
	if (dma_device == NULL)
	{
		goto fail_1;
	}

	dma_device = device_create(dma_class, NULL, MKDEV(MAJOR(dma_dev_id), MINOR(dma_dev_id) + 1), NULL, "dma_tx");
	if (dma_device == NULL)
	{
		goto fail_2;
	}
	printk(KERN_INFO "DMA INIT: Device created.\n");

	dma_cdev = cdev_alloc();
	dma_cdev->ops = &dma_fops;
	dma_cdev->owner = THIS_MODULE;
	ret = cdev_add(dma_cdev, dma_dev_id, 2);
	if (ret)
	{
		printk(KERN_ERR "DMA INIT: Failed to add cdev.\n");
		goto fail_3;
	}

	//printk(KERN_INFO "DMA INIT: Module init done.\n");

	rx_vir_buffer = dma_alloc_coherent(NULL, NUM_OF_PKT * MAX_PKT_LEN, &rx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if (!rx_vir_buffer)
	{
		printk(KERN_ALERT "DMA INIT: Could not allocate dma_alloc_coherent for RX");
		goto fail_4;
	}
	else
		printk("DMA INIT: Successfully allocated memory for dma RX buffer.\n");
	for (i = 0; i < NUM_OF_PKT * MAX_PKT_LEN / 4; i++)
		rx_vir_buffer[i] = 0x00000000;

	tx_vir_buffer = dma_alloc_coherent(NULL, NUM_OF_PKT * MAX_PKT_LEN, &tx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if (!tx_vir_buffer)
	{
		printk(KERN_ALERT "DMA INIT: Could not allocate dma_alloc_coherent for TX");
		goto fail_4;
	}
	else
		printk("DMA INIT: Successfully allocated memory for dma TX buffer.\n");
	for (i = 0; i < NUM_OF_PKT * MAX_PKT_LEN / 4; i++)
		tx_vir_buffer[i] = 0x00000000;

	printk(KERN_INFO "DMA INIT: DMA memory reset.\n");
	return platform_driver_register(&dma_driver);

fail_4:
	cdev_del(dma_cdev);
fail_3:
	device_destroy(dma_class, MKDEV(MAJOR(dma_dev_id), 1));
fail_2:
	device_destroy(dma_class, MKDEV(MAJOR(dma_dev_id), 0));
fail_1:
	class_destroy(dma_class);
fail_0:
	unregister_chrdev_region(dma_dev_id, 1);
	return -1;

	return ret;
}

static void __exit axi_dma_exit(void)
{
	//Reset DMA memory
	int i = 0;
	for (i = 0; i < NUM_OF_PKT * MAX_PKT_LEN / 4; i++)
	{
		rx_vir_buffer[i] = 0x00000000;
		tx_vir_buffer[i] = 0x00000000;
	}
	printk(KERN_INFO "DMA EXIT: DMA memory reset.\n");

	// Exit Device Module
	platform_driver_unregister(&dma_driver);
	cdev_del(dma_cdev);
	device_destroy(dma_class, MKDEV(MAJOR(dma_dev_id), 0));
	device_destroy(dma_class, MKDEV(MAJOR(dma_dev_id), 1));
	class_destroy(dma_class);
	unregister_chrdev_region(dma_dev_id, 1);
	dma_free_coherent(NULL, NUM_OF_PKT * MAX_PKT_LEN, rx_vir_buffer, rx_phy_buffer);
	dma_free_coherent(NULL, NUM_OF_PKT * MAX_PKT_LEN, tx_vir_buffer, tx_phy_buffer);
	printk(KERN_INFO "DMA EXIT: Exit device module finished \"%s\".\n", DEVICE_NAME);
}

module_init(axi_dma_init);
module_exit(axi_dma_exit);
