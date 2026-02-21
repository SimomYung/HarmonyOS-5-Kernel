/*
* Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
* Description: provide 9030 gpio access function interfaces.
* Create: 2023-12-5
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/bitops.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/pinctrl/consumer.h>
#include <linux/workqueue.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/gpio/driver.h>
#include <gpiolib.h>
#include <platform_include/basicplatform/linux/hi9030_gpio.h>
#include <platform_include/smart/drivers/io_die/int_parse.h>
#include <platform_include/smart/drivers/io_die/iodie_pcie.h>

#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>

#define SCTRL_BASE_ADDR 0xFDB23000
#define HW_SW_SEL 4
#define HW_SW_SEL_MASK 20
#define GPIO_PER_REG 16
#define SET_MASK 0x00010001
#define HIGH_LEVEL_ENABLE 0x00080008
#define LOW_LEVEL_ENABLE  0x00080000
#define RISING_EDGE_ENABLE  0x00060002
#define FALLING_EDGE_ENABLE  0x00060000
#define BOTH_EDGE_ENABLE  0x00060006
#define LEVEL_TRIGGER_ENABLE 0x00010001
#define EDGE_TRIGGER_ENABLE 0x00010000

#define intr_ctrl_bit(x) (0x08 + 0x04 * (x))
#define config_mask(x) (0x1 << (x))
#define data_offset(x) (((x) > 15) ? 0x0 : 0x04)
#define mask_offset(x) (((x) > 15) ? (x) : (x) + 16)

#define GPIO_NUM_32 32
#define GPIO_NUM_08 8
#define AO_GPIO_BASE 448
#define AO_GPIO_MAX_NUM (AO_GPIO_BASE + GPIO_NUM_08)
#define PERI_DOMAIN_NUM 2
#define GPIO_PREE_IRQ 21
#define PERI_GPIO_BASE 384

#define GPIO_MASK_0 0x088
#define GPIO_INTR_CLEAR  0x0A8
#define GPIO_STATUS_0 0x0B0
#define SOURCE_DATA 0x0DC
#define IRQ_MASK 0xFFFFFFFF
#define CLEAR_TRQ_TYPE 0xFFFFFFF0
#define AO_IRQ_GPIO_NUM 2064
#define IODIE_STATUS_MASK 0x80000000
#define IODIE_STATUS_OFFSET 0x46c
#define MAP_SIZE 0x1000

#define HI9030_AO_READ 0x0
#define HI9030_AO_WRITE 0x1

struct hi9030 {
	void __iomem *base;
	struct gpio_chip gc;
	struct platform_device *pdev;
	uint32_t irqarray;
	struct irq_domain *domain;
};

typedef void(* handler_single)(int, void*);

typedef struct {
	handler_single handler;
	void *driver_data;
	int gpio;
} gpio_handler_group;

static gpio_handler_group hi9030_ao_handlers[GPIO_NUM_08];

int hi9030_config_iomg(unsigned int gpio_id, enum gpiomux_mode mode)
{
	int ret;
	struct ffa_send_direct_data args = {0};
	args.data0 = SECURE_IODIE_IOMG_CONFIG;
	args.data1 = gpio_id;
	args.data2 = mode;

	ret = iodie_normal_request(IODIE_USER_GPIO);
	if(ret != 0){
		pr_err("%s:iodie_normal_request fail! ret:%d\n",__func__, ret);
		return ret;
	}

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("%s:ffa_platdrv_send_msg fail! ret:%d\n", __func__, ret);

	ret = iodie_normal_release(IODIE_USER_GPIO);
	if(ret != 0){
		pr_err("%s:iodie_normal_release fail! ret:%d\n",__func__, ret);
		return ret;
	}

	return args.data1;
}

static uint32_t hi9030_config_ao_gpio(uint32_t addr, int tag, uint32_t v)
{
	int ret;
	struct ffa_send_direct_data args = {0};
	args.data0 = SECURE_IODIE_GPIO_CONFIG;
	args.data1 = addr;
	args.data2 = tag;
	args.data3 = v;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("%s:hi9030 gpio:ffa send fail:%d\n", __func__, ret);
	return args.data1;
}

static uint32_t hi9030_readl(struct hi9030 *chip, uint32_t offset)
{
	int ret;
	uint32_t value;

	ret = iodie_normal_request(IODIE_USER_GPIO);
	if (ret) {
		pr_err("%s: iodie status err\n", __func__);
		return 0;
	}

	if (chip->irqarray < PERI_DOMAIN_NUM)
		value = readl(chip->base + offset);
	else
		value = hi9030_config_ao_gpio(chip->base + offset, HI9030_AO_READ, 0);

	ret = iodie_normal_release(IODIE_USER_GPIO);
	if (ret) {
		pr_err("%s: iodie status err\n", __func__);
		return 0;
	}

	return value;
}

static void hi9030_writel(struct hi9030 *chip, uint32_t v, uint32_t offset)
{
	int ret;

	ret = iodie_normal_request(IODIE_USER_GPIO);
	if (ret) {
		pr_err("%s: iodie status err\n", __func__);
		return;
	}

	if (chip->irqarray < PERI_DOMAIN_NUM)
		writel(v, chip->base + offset);
	else
		(void)hi9030_config_ao_gpio(chip->base + offset, HI9030_AO_WRITE, v);

	ret = iodie_normal_release(IODIE_USER_GPIO);
	if (ret) {
		pr_err("%s: iodie status err\n", __func__);
		return;
	}
}

static int hi9030_get_direction(struct gpio_chip *gc, uint32_t offset)
{
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	return !(hi9030_readl(hi9030, intr_ctrl_bit(offset)) & BIT(HW_SW_SEL));
}

static int hi9030_direction_input(struct gpio_chip *gc, uint32_t offset)
{
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	uint32_t gpiodir = 0;

	if (offset >= gc->ngpio)
		return -EINVAL;

	gpiodir |= BIT(HW_SW_SEL_MASK);
	hi9030_writel(hi9030, gpiodir, (intr_ctrl_bit(offset)));

	return 0;
}

static int hi9030_direction_output(struct gpio_chip *gc, uint32_t offset,
		int value)
{
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	uint32_t gpiodir = 0;
	uint32_t gpioval = 0;

	if (offset >= gc->ngpio)
		return -EINVAL;

	gpioval |= BIT(mask_offset(offset));
	gpioval |= (!!value << (offset % GPIO_PER_REG));
	hi9030_writel(hi9030, gpioval, (data_offset(offset)));
	gpiodir |= (SET_MASK << HW_SW_SEL);
	hi9030_writel(hi9030, gpiodir, (intr_ctrl_bit(offset)));

	return 0;
}

static int hi9030_get_value(struct gpio_chip *gc, uint32_t offset)
{
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	return (config_mask(offset) & (hi9030_readl(hi9030, SOURCE_DATA))) >> offset;
}

static void hi9030_set_value(struct gpio_chip *gc, uint32_t offset,
		int value)
{
	uint32_t gpioval = 0;
	struct hi9030 *hi9030 = gpiochip_get_data(gc);

	gpioval |= BIT(mask_offset(offset));
	gpioval |= (!!value << (offset % GPIO_PER_REG));
	hi9030_writel(hi9030, gpioval, (data_offset(offset)));
}

static int hi9030_gpio_to_irq(struct gpio_chip *gc, unsigned int offset)
{
	uint32_t virq;
	uint32_t group = (gc->base - PERI_GPIO_BASE) / GPIO_NUM_32;
	struct hi9030 *hi9030 = gpiochip_get_data(gc);

	virq = irq_create_mapping(hi9030->domain, offset);
	if (!virq) {
		pr_err("%s: Failed mapping hwirq\n", __func__);
		return -1;
	}

	return virq;
}

static int hi9030_irq_type(struct irq_data *d, unsigned trigger)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	irq_hw_number_t offset = irqd_to_hwirq(d);
	uint32_t intr_ctrl;

	if (offset >= GPIO_NUM_32)
		return -EINVAL;

	if ((trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) &&
		(trigger & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_EDGE_FALLING))) {
		pr_err("%s:trigger type error\n", __func__);
		return -EINVAL;
	}

	intr_ctrl = hi9030_readl(hi9030, intr_ctrl_bit(offset));
	intr_ctrl &= CLEAR_TRQ_TYPE;

	if (trigger & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) {
		bool polarity = trigger & IRQ_TYPE_LEVEL_HIGH;
		intr_ctrl |= LEVEL_TRIGGER_ENABLE;
		if (polarity)
			intr_ctrl |= HIGH_LEVEL_ENABLE;
		else
			intr_ctrl |= LOW_LEVEL_ENABLE;
		irq_set_handler_locked(d, handle_level_irq);
	} else if ((trigger & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH) {
		intr_ctrl |= EDGE_TRIGGER_ENABLE;
		intr_ctrl |= BOTH_EDGE_ENABLE;
		irq_set_handler_locked(d, handle_edge_irq);
	} else if ((trigger & IRQ_TYPE_EDGE_RISING) ||
		(trigger & IRQ_TYPE_EDGE_FALLING)) {
		bool rising = trigger & IRQ_TYPE_EDGE_RISING;
		intr_ctrl |= EDGE_TRIGGER_ENABLE;
		if (rising)
			intr_ctrl |= RISING_EDGE_ENABLE;
		else
			intr_ctrl |= FALLING_EDGE_ENABLE;
		irq_set_handler_locked(d, handle_edge_irq);
	} else {
		irq_set_handler_locked(d, handle_bad_irq);
	}

	hi9030_writel(hi9030, intr_ctrl, (intr_ctrl_bit(offset)));
	return 0;
}

static void hi9030_irq_unmask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	uint32_t mask = BIT(irqd_to_hwirq(d) % GPIO_NUM_32);
	uint32_t gpioim;

	gpioim = hi9030_readl(hi9030, GPIO_MASK_0) & ~mask;
	hi9030_writel(hi9030, gpioim, GPIO_MASK_0);
}

static void hi9030_irq_mask(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	uint32_t mask = BIT(irqd_to_hwirq(d) % GPIO_NUM_32);
	uint32_t gpioim;

	gpioim = hi9030_readl(hi9030, GPIO_MASK_0) | mask;
	hi9030_writel(hi9030, gpioim, GPIO_MASK_0);
}

static void hi9030_irq_ack(struct irq_data *d)
{
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct hi9030 *hi9030 = gpiochip_get_data(gc);
	uint32_t mask = BIT(irqd_to_hwirq(d) % GPIO_NUM_32);

	hi9030_writel(hi9030, mask, GPIO_INTR_CLEAR);
}

static struct irq_chip hi9030_irqchip = {
	.name = "hi9030-gpio",
	.irq_ack = hi9030_irq_ack,
	.irq_mask = hi9030_irq_mask,
	.irq_unmask = hi9030_irq_unmask,
	.irq_disable = hi9030_irq_mask,
	.irq_enable = hi9030_irq_unmask,
	.irq_set_type = hi9030_irq_type,
};

static void hi9030_irq_handler(void *irq_array)
{
	unsigned long pending;
	irq_hw_number_t offset;
	uint32_t gpio, gpio_group;
	struct gpio_desc *desc;
	struct gpio_chip *gc;
	struct hi9030 *hi9030;

	if (irq_array == NULL)
		return;

	gpio_group = *(int *)irq_array;
	if (gpio_group < 0 || gpio_group >= PERI_DOMAIN_NUM)
		return;

	gpio = gpio_group * GPIO_NUM_32 + PERI_GPIO_BASE;
	desc = gpio_to_desc(gpio);
	gc = desc->gdev->chip;
	hi9030 = gpiochip_get_data(gc);

	pending = hi9030_readl(hi9030, GPIO_STATUS_0);
	if (pending) {
		for_each_set_bit(offset, &pending, GPIO_NUM_32)
			generic_handle_irq(irq_find_mapping(hi9030->domain, offset));
	}
}

static int hi9030_irq_map(struct irq_domain *d, uint32_t virq, irq_hw_number_t hw)
{
	int ret;
	struct gpio_chip *chip = d->host_data;

	irq_set_chip_data(virq, chip);
	irq_set_chip_and_handler(virq, &hi9030_irqchip, handle_simple_irq);
	ret = irq_set_irq_type(virq, IRQ_TYPE_NONE);
	if (ret) {
		pr_err("%s: set irq type failed\n", __func__);
		return ret;
	}

	return 0;
}

static const struct irq_domain_ops hi9030_domain_ops = {
	.map	= hi9030_irq_map,
	.xlate	= irq_domain_xlate_twocell,
};

static int hi9030_setup_gpio(struct device_node *np, struct hi9030 *hi9030, struct device *dev)
{
	int ret;

	hi9030->gc.request = gpiochip_generic_request;
	hi9030->gc.free = gpiochip_generic_free;
	hi9030->gc.get_direction = hi9030_get_direction;
	hi9030->gc.direction_input = hi9030_direction_input;
	hi9030->gc.direction_output = hi9030_direction_output;
	hi9030->gc.get = hi9030_get_value;
	hi9030->gc.set = hi9030_set_value;
	hi9030->gc.to_irq = hi9030_gpio_to_irq;
	hi9030->gc.label = dev_name(dev);
	hi9030->gc.parent = dev;
	hi9030->gc.owner = THIS_MODULE;

	ret = of_property_read_u32(np, "base,offset", (u32 *)(&(hi9030->gc.base)));
	if (ret) {
		pr_err("%s no base,offset set\n", __func__);
		return ret;
	}

	ret = of_property_read_u32(np, "gpio_nums", (u32 *)(&(hi9030->gc.ngpio)));
	if (ret) {
		pr_err("%s no gpio_nums set\n", __func__);
		return ret;
	}

	return 0;
}

static int hi9030_setup_irq(struct device_node *np, struct hi9030 *hi9030)
{
	int ret = 0;
	struct irq_domain *domain = NULL;

	domain = irq_domain_add_simple(np, hi9030->gc.ngpio, 0, &hi9030_domain_ops, &hi9030->gc);
	if (!domain) {
		pr_err("%s: failed irq domain add simple!\n", __func__);
		return -ENODEV;
	}

	hi9030->domain = domain;
	ret = iodie_ree_int_parse_register(hi9030->irqarray + GPIO_PREE_IRQ,
			(iodie_intr_parse_notify_func)hi9030_irq_handler, (void *)&(hi9030->irqarray));
	if (ret) {
		pr_err("%s: register gpio irq fail!\n", __func__);
		return -ENODEV;
	}

	return 0;
}

static int get_hi9030_device_tree_data(struct device_node *np, struct hi9030 *hi9030)
{
	int ret, reg_prop_cnt;
	uint32_t *reg_prop = NULL;

	reg_prop_cnt = of_property_count_u32_elems(np, "reg");
	if (reg_prop_cnt < 0) {
		pr_err("%s: register reg not specified\n", __func__);
		return -ENOMEM;
	}

	reg_prop = kmalloc_array(reg_prop_cnt, sizeof(*reg_prop), GFP_KERNEL);
	if (!reg_prop)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "reg", reg_prop, reg_prop_cnt);
	if (ret) {
		pr_err("%s: register reg base not specified\n", __func__);
		goto free_prop;
	}

	ret = of_property_read_u32_array(np, "hi9030-irq-array", &(hi9030->irqarray), 1);
	if (ret) {
		pr_err("%s: no hi9030-irq-array property set\n", __func__);
		goto free_prop;
	}

	if (hi9030->irqarray < PERI_DOMAIN_NUM)
		hi9030->base = ioremap(reg_prop[1], reg_prop[3]);
	else
		hi9030->base = reg_prop[1];

free_prop:
	kfree(reg_prop);
	return ret;
}

static int get_iodie_status()
{
	void __iomem *virAddr = NULL;
	int status;

	virAddr = ioremap(SCTRL_BASE_ADDR, MAP_SIZE);
	if (virAddr == NULL)
		return 0;

	status = readl(virAddr + IODIE_STATUS_OFFSET);
	iounmap(virAddr);
	return status & IODIE_STATUS_MASK;
}

static int hi9030_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct hi9030 *hi9030 = NULL;
	struct device_node *np = NULL;
	int ret;

	if (get_iodie_status() == 0) {
		pr_err("%s: iodie status is error, non probe\n", __func__);
		return 0;
	}

	np = pdev->dev.of_node;
	if (!np)
		return -EINVAL;

	hi9030 = devm_kzalloc(dev, sizeof(*hi9030), GFP_KERNEL);
	if (!hi9030)
		return -ENOMEM;

	ret = get_hi9030_device_tree_data(np, hi9030);
	if (ret) {
		pr_err("%s: Error get hi9030 dts\n", __func__);
		return ret;
	}

	ret = hi9030_setup_gpio(np, hi9030, dev);
	if (ret) {
		pr_err("%s: Error setup hi9030 gpio %d\n", __func__, ret);
		return ret;
	}

	ret = gpiochip_add_data(&hi9030->gc, hi9030);
	if (ret) {
		pr_err("%s: gpio 9030 add data failed, fail err_no is %d!\n", __func__, ret);
		return ret;
	}

	hi9030_writel(hi9030, IRQ_MASK, GPIO_INTR_CLEAR);
	hi9030_writel(hi9030, IRQ_MASK, GPIO_MASK_0);

	if (hi9030->irqarray < PERI_DOMAIN_NUM) {
		ret = hi9030_setup_irq(np, hi9030);
		if (ret)
			pr_err("%s: hi9030 setup irq failed, err_no is %d!\n", __func__, ret);
	}

	platform_set_drvdata(pdev, hi9030);
	pr_err("%s: Hi9030 GPIO chip @0x%x registered\n", __func__, hi9030->base);
	return 0;
}

static void hi9030_ao_gpio_handler(void *data)
{
	int offset;

	if (data == NULL)
		return;

	offset = *(int *)data;
	if ((offset < 0) || (offset >= GPIO_NUM_08))
		return;

	if (hi9030_ao_handlers[offset].handler)
		hi9030_ao_handlers[offset].handler(hi9030_ao_handlers[offset].gpio, hi9030_ao_handlers[offset].driver_data);

	if (iodie_gpio_enable(AO_IRQ_GPIO_NUM + offset))
		pr_err("%s: gpio %d irq enable fail\n", __func__, hi9030_ao_handlers[offset].gpio);
}

int hi9030_gpio_irq_free(uint32_t gpio)
{
	int ret, offset;

	if (get_iodie_status() == 0) {
		pr_err("%s: iodie status is error, free irq fail\n", __func__);
		return -EFAULT;
	}

	if (gpio < AO_GPIO_BASE)
		return -EINVAL;

	offset = gpio - AO_GPIO_BASE;
	if ((offset >= GPIO_NUM_08) || (hi9030_ao_handlers[offset].handler == NULL))
		return -EINVAL;

	ret = iodie_gpio_intr_unregister(AO_IRQ_GPIO_NUM + offset);
	if (ret) {
		pr_err("%s: iodie unregister gpio irq err:%d\n", __func__, ret);
		return ret;
	}

	hi9030_ao_handlers[offset].handler = NULL;
	hi9030_ao_handlers[offset].driver_data = NULL;
	hi9030_ao_handlers[offset].gpio = 0;

	return ret;
}

int hi9030_gpio_set_irq_type(uint32_t gpio, int irqflag)
{
	int ret, offset;

	if (get_iodie_status() == 0) {
		pr_err("%s: iodie status is error, free irq fail\n", __func__);
		return -EFAULT;
	}

	if (gpio < AO_GPIO_BASE)
		return -EINVAL;

	offset = gpio - AO_GPIO_BASE;
	if ((offset >= GPIO_NUM_08) || (hi9030_ao_handlers[offset].handler == NULL))
		return -EINVAL;

	ret = iodie_gpio_intr_unregister(AO_IRQ_GPIO_NUM + offset);
	if (ret) {
		pr_err("%s: iodie unregister gpio irq err:%d\n", __func__, ret);
		return ret;
	}

	ret = iodie_gpio_intr_register(AO_IRQ_GPIO_NUM + offset, irqflag,
		(iodie_gpio_intr_func)hi9030_ao_gpio_handler, NULL);
	if (ret) {
		pr_err("%s: gpio id %d register irq fail\n", __func__, gpio);
		return ret;
	}

	return ret;
}

int hi9030_gpio_irq_request(uint32_t gpio, void (*handler)(int, void *), uint32_t irqflags, void *data)
{
	int ret, offset;

	if (get_iodie_status() == 0) {
		pr_err("%s: iodie status is error, request irq fail\n", __func__);
		return -EFAULT;
	}

	if (gpio < AO_GPIO_BASE)
		return -EINVAL;

	offset = gpio - AO_GPIO_BASE;
	if ((offset >= GPIO_NUM_08) || (hi9030_ao_handlers[offset].handler))
		return -EINVAL;

	ret = iodie_gpio_intr_register(AO_IRQ_GPIO_NUM + offset, irqflags,
		(iodie_gpio_intr_func)hi9030_ao_gpio_handler, NULL);
	if (ret) {
		pr_err("%s: gpio id %d register irq fail\n", __func__, gpio);
		return ret;
	}

	hi9030_ao_handlers[offset].handler = handler;
	hi9030_ao_handlers[offset].driver_data = data;
	hi9030_ao_handlers[offset].gpio = gpio;
	return ret;
}

static const struct of_device_id hi9030_gpio_of_match[] = {
	{
		.compatible = "hisilicon,hi9030-gpio",
		.data = NULL
	},
	{},
};
MODULE_DEVICE_TABLE(of, hi9030_gpio_of_match);

static struct platform_driver hi9030_gpio_driver = {
	.probe = hi9030_probe,
	.driver = {
		.name = "hi9030-gpio",
		.owner = THIS_MODULE,
		.of_match_table = hi9030_gpio_of_match,
	},
};

static int __init hi9030_gpio_init(void)
{
	return platform_driver_register(&hi9030_gpio_driver);
}

static void __exit hi9030_gpio_exit(void)
{
	platform_driver_unregister(&hi9030_gpio_driver);
}

subsys_initcall_sync(hi9030_gpio_init);
module_exit(hi9030_gpio_exit);

MODULE_DESCRIPTION("hi9030 GPIO driver");
MODULE_LICENSE("GPL");
