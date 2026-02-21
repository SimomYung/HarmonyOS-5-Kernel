/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Contexthub 9030 driver.
 * Create: 2022-11-30
 */

#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/io.h>
#include <linux/of_irq.h>
#include "inputhub_api/inputhub_api.h"
#include <platform_include/smart/linux/iomcu_status.h>
#include <platform_include/smart/linux/iomcu_ipc.h>
#include <securec.h>
#include <linux/string.h>
#include <linux/workqueue.h>
#include <platform_include/smart/drivers/io_die/int_parse.h>

#define INTR_CLR_REE             0x3000
#define INTR_MASK_REE            0x3004
#define INTR_STAT_REE            0x300C
#define MAX_WQ_ACTIVE            0x8
#define INT_PARSE_REE_MASK       0xFFFFFFFF
#define IODIE_STATUS_MASK        0x80000000
#define IODIE_STATUS_OFFSET      0x46C

struct iodie_intr_parse_work {
	struct work_struct worker;
	enum iodie_ree_int_type int_type;
};

struct intr_parse_info {
	iodie_intr_parse_notify_func func;
	void *data;
};

struct iodie_int_parse_priv {
	int irqnum;
	struct workqueue_struct *int_wq;
	struct intr_parse_info notify_info[IODIE_INT_REE_END];
};

static struct iodie_int_parse_priv g_int_parse_priv;
static void __iomem *g_intr_base_addr;
static void __iomem *g_iodie_status_addr;

int iodie_ree_int_parse_register(enum iodie_ree_int_type int_type, iodie_intr_parse_notify_func notify, void *data)
{
	pr_info("%s: enter\n", __func__);

	if (int_type >= IODIE_INT_REE_END) {
		pr_err("%s:invalid int type %x\n", __func__, (uint32_t)int_type);
		return -EINVAL;
	}

	if (notify == NULL) {
		pr_err("%s:notify is null, int type %x\n", __func__, (uint32_t)int_type);
		return -EINVAL;
	}

	pr_info("%s: int type %x\n", __func__, (uint32_t)int_type);

	g_int_parse_priv.notify_info[int_type].func = notify;
	g_int_parse_priv.notify_info[int_type].data = data;

	return 0;
}

int iodie_ree_int_parse_unregister(enum iodie_ree_int_type int_type)
{
	pr_info("%s: enter\n", __func__);

	if (int_type >= IODIE_INT_REE_END) {
		pr_err("%s:invalid int type %x\n", __func__, (uint32_t)int_type);
		return -EINVAL;
	}

	g_int_parse_priv.notify_info[int_type].func = NULL;
	g_int_parse_priv.notify_info[int_type].data = NULL;

	return 0;
}

static void iodie_intr_parse_dispatch(struct work_struct *work)
{
	struct iodie_intr_parse_work *p = container_of(work, struct iodie_intr_parse_work, worker);

	if (p->int_type >= IODIE_INT_REE_END) {
		pr_err("[%s] int_type is invalid\n", __func__);
		kfree(p);
		return;
	}

	if (g_int_parse_priv.notify_info[p->int_type].func != NULL) {
		pr_info("[%s] work exec, int type 0x%x.\n", __func__, p->int_type);
		g_int_parse_priv.notify_info[p->int_type].func(g_int_parse_priv.notify_info[p->int_type].data);
	} else{
		pr_err("[%s] func is null, int type 0x%x.\n", __func__, p->int_type);
	}

	kfree(p);
}

static void iodie_intr_parse_exec(uint32_t int_type)
{
	if (int_type >= IODIE_INT_REE_END) {
		pr_err("[%s] int_type is invalid\n", __func__);
		return;
	}

	if (g_int_parse_priv.notify_info[int_type].func != NULL) {
		pr_info("[%s] int type 0x%x.\n", __func__, int_type);
		g_int_parse_priv.notify_info[int_type].func(g_int_parse_priv.notify_info[int_type].data);
	} else{
		pr_err("[%s] func is null, int type 0x%x.\n", __func__, int_type);
	}
}

static irqreturn_t iodie_intr_parse_handler(int irq, void *data)
{
	struct iodie_intr_parse_work *int_work = NULL;
	uint8_t i;
	u32 val;

	/* Reverse lookup interrupt source */
	val = readl(g_intr_base_addr + INTR_STAT_REE);
	pr_info("[%s] recv INTR_STAT_REE 0x%x.\n", __func__, val);

	/* clear interrupt */
	writel(val, g_intr_base_addr + INTR_CLR_REE);

	for (i = 0; i < IODIE_INT_REE_END; i++) {
		if ((BIT(i) & val) == 0)
			continue;

		pr_info("[%s] dispatch interrupt %hhu.\n", __func__, i);

		/* direct execution, more efficient */
		if (i == IODIE_INT_I2C9_REE) {
			iodie_intr_parse_exec(i);
			continue;
		}

		int_work = kzalloc(sizeof(struct iodie_intr_parse_work), GFP_ATOMIC);
		if (!int_work) {
			pr_err("[%s] alloc work failed.\n", __func__);
			continue;
		}

		int_work->int_type = i;

		INIT_WORK(&int_work->worker, iodie_intr_parse_dispatch);
		queue_work(g_int_parse_priv.int_wq, &int_work->worker);
	}

	return IRQ_HANDLED;
}

static int get_iodie_dts_status(void)
{
	int len = 0;
	struct device_node *node = NULL;
	const char *status = NULL;
	int ret;

	node = of_find_compatible_node(NULL, NULL, "hisilicon,io-die");
	if (node != NULL) {
		status = of_get_property(node, "status", &len);
		if (status == NULL) {
			pr_err("[%s]of_get_property status err\n", __func__);
			return -EINVAL;
		}
		
		pr_info("[%s] status %s\n", __func__, status);
		if (strstr(status, "ok"))
			return EOK;
	}

	return -EINVAL;
}

static int get_int_parse_dts_info(void)
{
	struct device_node *node = NULL;
	uint32_t reg_addr;
	uint32_t reg_size;
	int ret;

	node = of_find_compatible_node(NULL, NULL, "hisilicon,io-die");
	if (node == NULL) {
		pr_err("can not find hisilicon,io-die node !\n");
		return -EFAULT;
	}

	g_int_parse_priv.irqnum = of_irq_get(node, 0);
	if (g_int_parse_priv.irqnum < 0) {
		pr_err("%s get irq num failed, irqnum %d\n", __func__, g_int_parse_priv.irqnum);
		return -EFAULT;
	}

	ret = of_property_read_u32_index(node, "intr_reg", 0, &reg_addr);
	if (ret) {
		pr_err("%s failed to get reg addr! ret=%d\n", __func__, ret);
		return -EFAULT;
	}

	ret = of_property_read_u32_index(node, "intr_reg", 1, &reg_size);
	if (ret) {
		pr_err("%s failed to get reg size! ret=%d\n", __func__, ret);
		return -EFAULT;
	}

	g_intr_base_addr = ioremap(reg_addr, reg_size);

	ret = of_property_read_u32_index(node, "iodie_status_reg", 0, &reg_addr);
	if (ret) {
		pr_err("%s failed to get reg addr! ret=%d\n", __func__, ret);
		return -EFAULT;
	}

	ret = of_property_read_u32_index(node, "iodie_status_reg", 1, &reg_size);
	if (ret) {
		pr_err("%s failed to get reg size! ret=%d\n", __func__, ret);
		return -EFAULT;
	}

	g_iodie_status_addr = ioremap(reg_addr, reg_size);

	return EOK;
}

void ioide_int_parse_ree_config(uint32_t val)
{
	uint32_t status;

	status = readl(g_iodie_status_addr + IODIE_STATUS_OFFSET);
	if ((status & IODIE_STATUS_MASK) == 0) {
		pr_err("%s iodie link err, status 0x%x\n", __func__, status);
		return;
	}

	writel(val, g_intr_base_addr + INTR_MASK_REE);
}

static int __init iodie_int_parse_init(void)
{
	int ret;

	if (get_iodie_dts_status() != EOK) {
		pr_err("%s: iodie node is not enable\n", __func__);
		return -1;
	}

	ret = get_int_parse_dts_info();
	if (ret != EOK) {
		pr_err("%s get int parse dts info failed, ret %d\n", __func__, ret);
		return -1;
	}

	if (request_irq(g_int_parse_priv.irqnum, iodie_intr_parse_handler, 0, "int_parse", NULL)) {
		pr_err("%s request int parse irq fail\n", __func__);
		return -1;
	}

	g_int_parse_priv.int_wq = create_workqueue("iodie_int_parse");
	workqueue_set_max_active(g_int_parse_priv.int_wq, MAX_WQ_ACTIVE);

	ioide_int_parse_ree_config(INT_PARSE_REE_MASK);

	pr_info("%s success\n", __func__);
	return 0;
}

static void __exit iodie_int_parse_exit(void)
{
	ioide_int_parse_ree_config(0);
	free_irq(g_int_parse_priv.irqnum, NULL);

	pr_info("exit %s\n", __func__);
}

arch_initcall_sync(iodie_int_parse_init);
module_exit(iodie_int_parse_exit);
