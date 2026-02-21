/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved. \n
 *
 * Description:  \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2023-12-26, Create file. \n
 */
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/suspend.h>
#include <linux/delay.h>

#include "tiot_service_interface.h"
#include "tiot_service_interface_ext.h"
#include "tiot_dts_node.h"
#include "tiot_board_log.h"
#include "tiot_types.h"
#include "tiot_osal.h"

#define OPEN_FLAGS_FLOWCTRL_BIT   0

#define MISC_IOCTL_FLOWCTRL_SET     _IOW('t', 0x1, unsigned int)

#define SLE_IOCTL_MAGIC             'S'
#define SLE_IOCTL_OFF_FIND_CMD      _IOW(SLE_IOCTL_MAGIC, 1, int)

#define DTS_NODE_COMPATIBLE    "tiot,bsxx"

#define READ_TIMEOUT   0xFFFFFFF
#define TRX_DATA_MAX   9300   /* 按实际单包数据大小最大值修改. */

#define DEV_MAX_OPEN_COUNT  10240   /* 单个节点的最大同时打开次数 */

#define SUBSYS_SLE   0
#define SUBSYS_OM    1

typedef enum {
    DEV_DEFAULT,
    DEV_OM,
    DEV_OFF_FIND,
    DEV_MAX
} device_type;

enum callback_cmd {
    OFF_FIND_ENABLE = 0x1,
    OFF_FIND_DISABLE = 0x2,
    OFF_FIND_CMD_BUTT
};

struct off_find_para {
    uint32_t cmd;        //
    uint32_t data_len;   // 可扩展的数据长度
    uint32_t data[1];     // 可扩展的数据地址
};

struct dev_buff {
    uint8_t *data;  /* data pointer */
    uint32_t len;   /* data len */
    uint32_t size;  /* buff size (readonly) */
    uint8_t head[]; /* buff head */
};

static uint32_t g_open_flags = 1UL << (uint32_t)OPEN_FLAGS_FLOWCTRL_BIT;  /* 默认4线. */

struct tiot_dev {
    osal_mutex dev_mutex;
    struct miscdevice misc;
    struct dev_buff *recv_buff;
    uint32_t open_count;
    tiot_handle handle;
};

typedef struct {
    struct tiot_dev dev;
    const struct file_operations *const fops;
    const char *prop;
    const char *name;
} tiot_dev_entry;

static const char *g_device_node_name = NULL;

static bool g_off_find_enable = false;

static struct device_node *misc_find_device_node(void)
{
    struct device_node *np = NULL;
    struct device_node *dev_np = NULL;
    /* 获取 top dts节点 */
    np = of_find_compatible_node(NULL, NULL, DTS_NODE_TIOT);
    if (np == NULL) {
        printk("no compatible node.\r\n");
        return NULL;
    }
    /* 获取子dts节点 */
    dev_np = of_find_compatible_node(np, NULL, DTS_NODE_COMPATIBLE);
    if (dev_np == NULL) {
        printk("no device compatible node.\r\n");
        return NULL;
    }
    return dev_np;
}

/* ------------------dev_buff------------------ */
static struct dev_buff *dev_buff_alloc(uint32_t size)
{
    struct dev_buff *b = NULL;

    b = kmalloc(sizeof(struct dev_buff) + size, GFP_KERNEL);
    if (b == NULL) {
        tiot_print_err("alloc dev_buff fail.\n");
        return NULL;
    }

    b->data = b->head;
    b->size = size;
    b->len = 0;
    return b;
}

static void dev_buff_free(struct dev_buff *b)
{
    kfree(b);
}

static void dev_buff_clear(struct dev_buff *b)
{
    b->data = b->head;
    b->len = 0;
}

static void dev_buff_put(struct dev_buff *b, uint32_t len)
{
    b->len += len;
    /* 上层调用方会进行合法性校验 */
    WARN_ON(b->data + b->len > b->head + b->size);
}

static void dev_buff_pull(struct dev_buff *b, uint32_t len)
{
    /* 上层调用方会进行合法性校验 */
    WARN_ON(len > b->len);
    b->data += len;
    b->len -= len;
    if (b->len == 0) { /* reset data pointer */
        b->data = b->head;
    }
}
/* -------------------------------------------- */

static int dev_open(struct file *filp)
{
    tiot_handle handle = 0;
    tiot_service_open_param param = { 0 };
    /* current filp->private_data pointer to the registered miscdevice. */
    struct tiot_dev *dev = container_of(filp->private_data, struct tiot_dev, misc);

    param.flags = g_open_flags;
    osal_mutex_lock(&dev->dev_mutex);
    if (dev->open_count == DEV_MAX_OPEN_COUNT) {
        osal_mutex_unlock(&dev->dev_mutex);
        tiot_print_err("dev open reach max count.\n");
        return -1;
    }
    handle = tiot_service_open(g_device_node_name, &param);
    if (handle == 0) {
        osal_mutex_unlock(&dev->dev_mutex);
        tiot_print_err("dev open fail.\n");
        return -1;
    }
    if (dev->open_count++ == 0) {
        dev->handle = handle;
    }
    osal_mutex_unlock(&dev->dev_mutex);
    /*
     * Each open returns a new fd (struct file),
     * so we need to set filp->private_data each time.
     */
    filp->private_data = dev;
    return 0;
}

static void dev_close(struct tiot_dev *dev)
{
    tiot_service_close(dev->handle);
    osal_mutex_lock(&dev->dev_mutex);
    if (dev->open_count == 0) {
        tiot_print_warning("close dev node but not open.\n");
        osal_mutex_unlock(&dev->dev_mutex);
        return;
    }
    if (--dev->open_count == 0) {
        /* 清掉残留数据 */
        dev_buff_clear(dev->recv_buff);
        dev->handle = 0;
    }
    osal_mutex_unlock(&dev->dev_mutex);
}

static int misc_dev_open(struct inode *inode, struct file *filp)
{
    tiot_print_info("%s enter.\n", __func__);
    if (g_off_find_enable == true) {
        return -EINVAL;
    }
    if (dev_open(filp) != 0) {
        return -1;
    }
    tiot_print_info("%s ok.\n", __func__);
    return 0;
}

static int misc_dev_close(struct inode *inode, struct file *filp)
{
    struct tiot_dev *dev = filp->private_data;
    tiot_print_info("%s enter.\n", __func__);
    dev_close(dev);
    tiot_print_info("%s ok.\n", __func__);
    return 0;
}

static ssize_t dev_read(struct tiot_dev *dev, char __user *buf, size_t count,
                        const tiot_service_rx_param *param)
{
    int32_t read_len;
    size_t read_count = count;
    struct dev_buff *b = dev->recv_buff;
    uint32_t buff_size = b->size;

    if (count > buff_size) {
        return -ERANGE;
    }

    osal_mutex_lock(&dev->dev_mutex);
    /* recv_buff is empty */
    if (b->len == 0) {
        read_len = tiot_service_read_ext(dev->handle, b->data, buff_size, param);
        if (read_len <= 0) {
            osal_mutex_unlock(&dev->dev_mutex);
            return 0;
        }
        dev_buff_put(b, read_len);
    }
    if (read_count > b->len) {
        read_count = b->len;
    }
    if (copy_to_user(buf, b->data, read_count)) {
        osal_mutex_unlock(&dev->dev_mutex);
        return -ERANGE;
    }
    dev_buff_pull(b, read_count);
    osal_mutex_unlock(&dev->dev_mutex);
    return read_count;
}

static ssize_t misc_dev_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    struct tiot_dev *dev = filp->private_data;
    /* 这里将timeout_ms设置为0使用非阻塞读 */
    const tiot_service_rx_param param = { 0, SUBSYS_SLE, NULL, NULL };

    if (g_off_find_enable == true) {
        return -EINVAL;
    }
    return dev_read(dev, buf, count, &param);
}

static uint32_t dev_poll(struct file *filp, poll_table *wait,
                         const tiot_service_rx_param *param)
{
    uint32_t mask = 0;
    struct tiot_dev *dev = NULL;
    bool has_data = false;

    if ((filp == NULL) || (wait == NULL)) {
        tiot_print_dbg("invalid filp or poll_table.\n");
        return mask;
    }

    dev = filp->private_data;
    osal_mutex_lock(&dev->dev_mutex);
    has_data = (dev->recv_buff->len > 0);
    osal_mutex_unlock(&dev->dev_mutex);

    if (has_data) {
        mask |= POLLIN | POLLRDNORM;
        return mask;
    }
    /* param->filp, param->poll_wait参数用于poll接口，用来查询是否有数据可读而非实际的读出操作 */
    if (tiot_service_read_ext(dev->handle, NULL, 0, param) != 0) {
        mask |= POLLIN | POLLRDNORM;
    }

    return mask;
}

static uint32_t misc_dev_poll(struct file *filp, poll_table *wait)
{
    const tiot_service_rx_param param = { 0, SUBSYS_SLE, filp, wait };

    tiot_print_dbg("misc_dev_poll enter.\n");
    if (g_off_find_enable == true) {
        return 0;
    }
    return dev_poll(filp, wait, &param);
}

static ssize_t dev_write(struct tiot_dev *dev, const char __user *buf, size_t count)
{
    ssize_t ret;
    char *tx_data = NULL;

    if (count > TRX_DATA_MAX) {
        return -ERANGE;
    }
    tx_data = (char *)kmalloc(count, GFP_KERNEL);
    if (tx_data == NULL) {
        return -ENOMEM;
    }
    if (copy_from_user(tx_data, buf, count)) {
        kfree(tx_data);
        return -ERANGE;
    }
    ret = tiot_service_write(dev->handle, tx_data, count);
    kfree(tx_data);
    return ret;
}

static ssize_t misc_dev_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    struct tiot_dev *dev = filp->private_data;

    if (g_off_find_enable == true) {
        return -EINVAL;
    }
    return dev_write(dev, buf, count);
}

static long misc_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    volatile unsigned long flags = 0;
    struct tiot_dev *dev = filp->private_data;
    if (g_off_find_enable == true) {
        return -EINVAL;
    }
    flags = g_open_flags;
    tiot_print_info("misc_dev_ioctl cmd 0x%x, arg 0x%x\n", cmd, arg);
    switch (cmd) {
        case MISC_IOCTL_FLOWCTRL_SET:
            if (arg == 0) {
                clear_bit(OPEN_FLAGS_FLOWCTRL_BIT, &flags);
            } else if (arg == 1) {
                set_bit(OPEN_FLAGS_FLOWCTRL_BIT, &flags);
            } else {
                ret = -EINVAL;
            }
            break;
        default:
            ret = -ENOIOCTLCMD;
            break;
    }
    g_open_flags = (uint32_t)flags;
    return ret;
}

static int misc_dev_om_open(struct inode *inode, struct file *filp)
{
    tiot_print_info("%s enter.\n", __func__);

    if (dev_open(filp) != 0) {
        return -1;
    }
    tiot_print_info("%s ok.\n", __func__);
    return 0;
}

static int misc_dev_om_close(struct inode *inode, struct file *filp)
{
    struct tiot_dev *dev = filp->private_data;
    tiot_print_info("%s enter.\n", __func__);
    dev_close(dev);
    tiot_print_info("%s ok.\n", __func__);
    return 0;
}

static ssize_t misc_dev_om_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    struct tiot_dev *dev = filp->private_data;
    const tiot_service_rx_param param = { 0, SUBSYS_OM, NULL, NULL };

    return dev_read(dev, buf, count, &param);
}

static uint32_t misc_dev_om_poll(struct file *filp, poll_table *wait)
{
    const tiot_service_rx_param param = { 0, SUBSYS_OM, filp, wait };

    tiot_print_dbg("misc_dev_om_poll enter.\n");
    return dev_poll(filp, wait, &param);
}

static ssize_t misc_dev_om_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    /* OM is for read only. */
    return 0;
}

static int misc_dev_off_find_open(struct inode *inode, struct file *filp)
{
    tiot_print_info("%s enter.\n", __func__);

    if (dev_open(filp) != 0) {
        return -1;
    }
    tiot_print_info("%s ok.\n", __func__);
    return 0;
}

static int misc_dev_off_find_close(struct inode *inode, struct file *filp)
{
    struct tiot_dev *dev = filp->private_data;
    tiot_print_info("%s enter.\n", __func__);
    dev_close(dev);
    tiot_print_info("%s ok.\n", __func__);
    return 0;
}

static ssize_t misc_dev_off_find_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    struct tiot_dev *dev = filp->private_data;
    /* 这里将timeout_ms设置为0使用非阻塞读 */
    const tiot_service_rx_param param = { 0, SUBSYS_SLE, NULL, NULL };

    return dev_read(dev, buf, count, &param);
}

static uint32_t misc_dev_off_find_poll(struct file *filp, poll_table *wait)
{
    const tiot_service_rx_param param = { 0, SUBSYS_SLE, filp, wait };

    tiot_print_dbg("misc_dev_poll enter.\n");
    return dev_poll(filp, wait, &param);
}

static ssize_t misc_dev_off_find_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    struct tiot_dev *dev = filp->private_data;

    return dev_write(dev, buf, count);
}

static long misc_dev_off_find_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    struct off_find_para cmd_data;
    struct tiot_dev *dev = filp->private_data;

    tiot_print_info("misc_dev_off_find_ioctl cmd 0x%x, arg 0x%x\n", cmd, arg);
    switch (cmd) {
        case SLE_IOCTL_OFF_FIND_CMD:
            if (copy_from_user(&cmd_data, arg, sizeof(cmd_data))) {
                tiot_print_err("get data head from user fail\n");
                return -EINVAL;
            }
            if (cmd_data.cmd == OFF_FIND_ENABLE) {
                g_off_find_enable = true;
            } else if (cmd_data.cmd == OFF_FIND_DISABLE) {
                g_off_find_enable = false;
            } else {
                tiot_print_warning("cmd = %d not find\n", cmd_data.cmd);
                return -EINVAL;
            }
            break;
        default:
            ret = -ENOIOCTLCMD;
            break;
    }
    return ret;
}

static const struct file_operations misc_fops = {
    .owner = THIS_MODULE,
    .read = misc_dev_read,
    .poll = misc_dev_poll,
    .write = misc_dev_write,
    .unlocked_ioctl = misc_dev_ioctl,
    .open = misc_dev_open,
    .release = misc_dev_close
};

static const struct file_operations misc_om_fops = {
    .owner = THIS_MODULE,
    .read = misc_dev_om_read,
    .poll = misc_dev_om_poll,
    .write = misc_dev_om_write,
    .open = misc_dev_om_open,
    .release = misc_dev_om_close
};

static const struct file_operations misc_off_find_fops = {
    .owner = THIS_MODULE,
    .read = misc_dev_off_find_read,
    .poll = misc_dev_off_find_poll,
    .write = misc_dev_off_find_write,
    .unlocked_ioctl = misc_dev_off_find_ioctl,
    .open = misc_dev_off_find_open,
    .release = misc_dev_off_find_close
};

static tiot_dev_entry g_dev_entrys[DEV_MAX] = {
    [DEV_DEFAULT] = { .fops = &misc_fops, .prop = DTS_PROP_MISC_DEV_NAME, .name = NULL },
    [DEV_OM] = { .fops = &misc_om_fops, .prop = DTS_PROP_OM_MISC_DEV_NAME, .name = NULL },
    [DEV_OFF_FIND] = { .fops = &misc_off_find_fops, .prop = DTS_PROP_FIND_MISC_DEV_NAME, .name = NULL }
};

static int misc_pm_suspend(void)
{
    int ret;
    tiot_print_info("suspend notify+++\r\n");
    ret = tiot_service_pm_ctrl(g_dev_entrys[DEV_DEFAULT].dev.handle, TIOT_PM_CTRL_SUSPEND_PREPARE);
    if (ret != 0) {
        tiot_print_err("suspend notify error\r\n");
    }
    return ret;
}

static int misc_pm_resume(void)
{
    int ret;
    tiot_print_info("resume notify---\r\n");
    ret = tiot_service_pm_ctrl(g_dev_entrys[DEV_DEFAULT].dev.handle, TIOT_PM_CTRL_RESUME_PREPARE);
    if (ret != 0) {
        tiot_print_err("resume notify error\r\n");
    }
    return ret;
}

static int misc_pm_callback(struct notifier_block *nb, unsigned long mode, void *_unused)
{
    tiot_unused(nb);
    tiot_unused(_unused);
    tiot_print_info("misc_pm_callback mode:%lu\r\n", mode);
    switch (mode) {
        case PM_SUSPEND_PREPARE:
            misc_pm_suspend();
            break;
        case PM_POST_SUSPEND:
            misc_pm_resume();
            break;
        default:
            break;
    }
    return NOTIFY_OK;
}

static struct notifier_block misc_pm_notifier = {
    .notifier_call = misc_pm_callback,
};

static int misc_dev_entry_init(struct platform_device *pdev, tiot_dev_entry *dev_entry)
{
    struct tiot_dev *dev = &dev_entry->dev;
    if (dev_entry->name == NULL) {
        tiot_print_info("ignore dts node[%s].\r\n", dev_entry->prop);
        return 0;
    }
    dev->open_count = 0;
    dev->recv_buff = dev_buff_alloc(TRX_DATA_MAX);
    if (dev->recv_buff == NULL) {
        return -1;
    }
    osal_mutex_init(&dev->dev_mutex);
    dev->misc.minor = MISC_DYNAMIC_MINOR;
    dev->misc.name = dev_entry->name;
    dev->misc.fops = dev_entry->fops;
    dev->misc.parent = &pdev->dev;
    if (misc_register(&dev->misc) != 0) {
        tiot_print_err("%s register fail.\r\n", dev_entry->name);
        osal_mutex_destroy(&dev->dev_mutex);
        dev_buff_free(dev->recv_buff);
        dev->recv_buff = NULL;
        return -1;
    }
    return 0;
}

static inline void misc_dev_entry_destroy(tiot_dev_entry *dev_entry)
{
    struct tiot_dev *dev = &dev_entry->dev;
    if (dev_entry->name) {
        misc_deregister(&dev->misc);
        osal_mutex_destroy(&dev->dev_mutex);
        dev_buff_free(dev->recv_buff);
        dev->recv_buff = NULL;
    }
}

static int bsxx_probe(struct platform_device *pdev)
{
    int ret;
    int i;
    int j;
    struct device_node *dev_node = NULL;
    tiot_dev_entry *dev_entry = NULL;
    tiot_print_info("probe enter++\r\n");
    dev_node = misc_find_device_node();
    if (dev_node == NULL) {
        return -1;
    }
    g_device_node_name = dev_node->name;
    ret = tiot_service_init();
    if (ret != 0) {
        of_node_put(dev_node);
        return -1;
    }
    for (i = 0; i < DEV_MAX; i++) {
        dev_entry = &g_dev_entrys[i];
        of_property_read_string(dev_node, dev_entry->prop, &dev_entry->name);
        ret = misc_dev_entry_init(pdev, dev_entry);
        if (ret != 0) {
            ret = -1;
            break;
        }
    }
    of_node_put(dev_node);
    if (ret != 0) {
        for (j = 0; j < i; j++) {
            misc_dev_entry_destroy(&g_dev_entrys[j]);
        }
        return ret;
    }
    register_pm_notifier(&misc_pm_notifier);
    return 0;
}

static int bsxx_remove(struct platform_device *pdev)
{
    int i;
    unregister_pm_notifier(&misc_pm_notifier);
    for (i = 0; i < DEV_MAX; i++) {
        misc_dev_entry_destroy(&g_dev_entrys[i]);
    }
    tiot_service_deinit();
    return 0;
}

#ifdef CONFIG_PM_SLEEP
static int misc_suspend(struct device *dev)
{
    int ret;
    tiot_unused(dev);
    tiot_print_info("platform suspend+++\r\n");
    ret = tiot_service_pm_ctrl(g_dev_entrys[DEV_DEFAULT].dev.handle, TIOT_PM_CTRL_SUSPEND);
    if (ret != 0) {
        tiot_print_err("platform suspend error\r\n");
    }
    return 0;
}

static int misc_resume(struct device *dev)
{
    int ret;
    tiot_unused(dev);
    tiot_print_info("platform resume---\r\n");
    ret = tiot_service_pm_ctrl(g_dev_entrys[DEV_DEFAULT].dev.handle, TIOT_PM_CTRL_RESUME);
    if (ret != 0) {
        tiot_print_err("platform resume error\r\n");
    }
    return 0;
}

static SIMPLE_DEV_PM_OPS(misc_pm_ops, misc_suspend, misc_resume);
#else
static SIMPLE_DEV_PM_OPS(misc_pm_ops, NULL, NULL);
#endif

/* not use of match for now. */
static struct platform_driver bsxx_driver = {
    .probe  = bsxx_probe,
    .remove    = bsxx_remove,
    .driver = {
        .name = "tiot",
        .pm = &misc_pm_ops
    }
};

static int __init tiot_module_init(void)
{
    int ret;
    ret = platform_driver_register(&bsxx_driver);
    if (ret) {
        tiot_print_err("pdrv register failed\r\n");
    } else {
        tiot_print_err("pdrv register ok\r\n");
    }
    return ret;
}

static void __exit tiot_module_exit(void)
{
    platform_driver_unregister(&bsxx_driver);
}

module_init(tiot_module_init);
module_exit(tiot_module_exit);
MODULE_LICENSE("GPL v2");
