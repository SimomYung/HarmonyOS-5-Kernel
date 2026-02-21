/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description: I2C Function Implementation
 * Author: @CompanyNameTag
 */

#include "plat_i2c.h"
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include "board.h"
#include "plat_gpio.h"
#include "plat_debug.h"
#include "oal_schedule.h"

#define WRITE_TIME_MAX              30
#define MAX_RECV_DATA_LEN           1024
#define MAX_SEND_DATA_LEN           1024
#define I2C_WRITE_RETRY_DELAY       1
#define I2C_WRITE_DATA_LEN_BYTE_LEN 2

#define I2C_SWITCH_SLAVE_RX_TL_CMD   0xA1
#define I2C_TX_DATA_CMD              0xA2
#define I2C_GET_RX_LEN_CMD           0x91
#define I2C_GET_RX_DATA_CMD          0x92

#define I2C_CHR_DEV_NUM              1
#define MPXX_I2C_DEV_NAME            "mpxx_i2c"
typedef struct {
    struct i2c_client *client;
} conn_i2c_info;

typedef struct {
    dev_t dev_id;
    uint8_t chr_dev_num;
    struct cdev *cdev;
    struct class *cls;
    struct device *dev;
} i2c_dev;

oal_mutex_stru g_i2c_read_mutex;
oal_mutex_stru g_i2c_dev_mutex;       // 访问deivce的互斥锁
oal_wait_queue_head_stru g_i2c_read_wq;

conn_i2c_info g_i2c_info;

OAL_STATIC int32_t i2c_board_read(char *buf, int32_t count)
{
    return (int32_t)i2c_master_recv(g_i2c_info.client, buf, count);
}

OAL_STATIC int32_t i2c_board_write(char *buf, int32_t count)
{
    return (int32_t)i2c_master_send(g_i2c_info.client, buf, count);
}

OAL_STATIC int32_t i2c_device_read(char *buf, int32_t count)
{
    int32_t ret;

    if (count > MAX_RECV_DATA_LEN) {
        ps_print_err("max read len = %d, less than data_len = %d\n", MAX_RECV_DATA_LEN, count);
        return 0;
    }

    if (count == 0) {
        ps_print_err("expect count is 0\n");
        return 0;
    }

    ret = i2c_board_read(buf, count);
    if (ret < 0) {
        ps_print_err("i2c read data fail, ret: %d, expect:%d\n", ret, count);
        return ret;
    }

    return ret;
}

OAL_STATIC int32_t i2c_device_write(char *buf, int32_t count)
{
    int32_t ret;
    int32_t retry;

    for (retry = 0; retry < WRITE_TIME_MAX; retry++) {
        ret = i2c_board_write(buf, count);
        if (ret == count) {
            break;
        }

        ps_print_err("send data try =%d returned %d\n", retry, ret);
        oal_msleep(I2C_WRITE_RETRY_DELAY);
    }

    if (retry == WRITE_TIME_MAX) {
        ps_print_err("i2c_device_write send fail.\n");
    }

    return ret;
}

OAL_STATIC int32_t i2c_priv_read_data_len(void)
{
    int32_t ret;
    uint8_t cmd = I2C_GET_RX_LEN_CMD;
    uint16_t data_len;
    ret = i2c_device_write(&cmd, sizeof(cmd));
    if (ret != sizeof(cmd)) {
        ps_print_err("i2c send read data len cmd fail, ret = %d\n", ret);
        return 0;
    }

    ret = i2c_device_read((char*)&data_len, sizeof(data_len));
    if (ret != sizeof(data_len)) {
        ps_print_err("i2c read data len fail, ret = %d\n", ret);
        return 0;
    }

    return (int32_t)data_len;
}

OAL_STATIC int32_t i2c_priv_read_data(char *buf, int32_t count)
{
    int32_t ret;
    uint8_t cmd = I2C_GET_RX_DATA_CMD;
    ret = i2c_device_write(&cmd, sizeof(cmd));
    if (ret != sizeof(cmd)) {
        ps_print_err("i2c send read data cmd fail, ret = %d\n", ret);
        return 0;
    }

    ret = i2c_device_read(buf, count);
    return ret;
}

OAL_STATIC int32_t i2c_priv_read_ext(char *buf, int32_t count)
{
    int32_t ret;
    int32_t data_len;

    if (!conn_get_gpio_level(I2C_WKUP_HOST)) {
        ps_print_err("wakeup gpio is low, device no data to report\n");
        return 0;
    }

    ret = i2c_priv_read_data_len();
    data_len = ret;
    if (data_len <= 0) {
        return ret;
    }
    if (data_len > count) {
        ps_print_err("buf_len = %d, less than data_len = %d\n", count, data_len);
        return 0;
    }

    ret = i2c_priv_read_data(buf, data_len);
    return ret;
}

OAL_STATIC int32_t i2c_priv_read(char *buf, int32_t count)
{
    int32_t ret;

    oal_mutex_lock(&g_i2c_dev_mutex); // device访问互斥
    ret = i2c_priv_read_ext(buf, count);
    oal_mutex_unlock(&g_i2c_dev_mutex);

    return ret;
}

OAL_STATIC int32_t i2c_switch_slave_rx_waterline(void)
{
    int32_t ret;
    uint8_t cmd = I2C_SWITCH_SLAVE_RX_TL_CMD;
    uint8_t status;
    ret = i2c_device_write(&cmd, sizeof(cmd));
    if (ret != sizeof(cmd)) {
        ps_print_err("i2c send set device rx waterline cmd fail, ret = %d\n", ret);
        return -OAL_FAIL;
    }

    ret = i2c_device_read(&status, sizeof(status));
    if (ret != sizeof(status)) {
        ps_print_err("i2c read device rx status fail, ret = %d\n", ret);
        return -OAL_FAIL;
    }

    if (status != 0) {
        ps_print_err("i2c device rx status is busy\n, status = %d", status);
        return -OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t i2c_priv_write_data_len(uint16_t len)
{
    int32_t ret;
    uint8_t data_len[I2C_WRITE_DATA_LEN_BYTE_LEN];
    uint8_t cmd = I2C_TX_DATA_CMD;

    ret = i2c_device_write(&cmd, sizeof(cmd));
    if (ret != sizeof(cmd)) {
        ps_print_err("i2c send tx data cmd fail, ret = %d\n", ret);
        return 0;
    }

    data_len[0] = (uint8_t)(len & 0x00FF);           /* 取2字节数据的低8位 */
    data_len[1] = (uint8_t)((len & 0xFF00) >> 8);    /* 取2字节数据的高8位 */
    ret = i2c_device_write(data_len, I2C_WRITE_DATA_LEN_BYTE_LEN);
    if (ret != I2C_WRITE_DATA_LEN_BYTE_LEN) {
        ps_print_err("i2c send tx data len fail, ret = %d\n", ret);
        return 0;
    }

    return ret;
}

OAL_STATIC int32_t i2c_priv_write_data(char *buf, uint16_t len)
{
    int32_t ret;
    uint8_t cmd = I2C_TX_DATA_CMD;

    ret = i2c_device_write(&cmd, sizeof(cmd));
    if (ret != sizeof(cmd)) {
        ps_print_err("i2c send tx data cmd fail, ret = %d\n", ret);
        return 0;
    }

    ret = i2c_device_write(buf, len);
    if (ret != len) {
        ps_print_err("i2c send tx data fail, ret = %d\n", ret);
        return 0;
    }

    return ret;
}

OAL_STATIC int32_t i2c_priv_write(char *buf, int32_t count)
{
    int32_t ret;
    ret = i2c_switch_slave_rx_waterline();
    if (ret != OAL_SUCC) {
        return 0;
    }

    ret = i2c_priv_write_data_len((uint16_t)count);
    if (ret != sizeof(uint16_t)) {
        return ret;
    }

    ret = i2c_switch_slave_rx_waterline();
    if (ret != OAL_SUCC) {
        return 0;
    }

    ret = i2c_priv_write_data(buf, count);
    
    return ret;
}

int32_t i2c_conn_read(char *buf, int32_t count)
{
    int32_t ret;
    if (count == 0) {
        ps_print_warning("read count is 0\n");
        return 0;
    }

    oal_mutex_lock(&g_i2c_read_mutex); // 防止多个线程同时读
    ret = i2c_priv_read(buf, count);
    oal_mutex_unlock(&g_i2c_read_mutex);

    return ret;
}

int32_t i2c_conn_write(char *buf, int32_t count)
{
    int32_t ret;
    oal_mutex_lock(&g_i2c_dev_mutex);
    ret = i2c_priv_write(buf, count);
    oal_mutex_unlock(&g_i2c_dev_mutex);
    return ret;
}

STATIC ssize_t mpxx_i2c_read(struct file *filp, char __user *buf,
        size_t count, loff_t *offset)
{
    struct i2c_dev *plat_dev;
    char tmp[MAX_RECV_DATA_LEN];
    int32_t ret;

    plat_dev = filp->private_data;
    if (plat_dev == NULL) {
        ps_print_err("plat_dev is err.\n");
        return -EFAULT;
    }

    if (filp->f_flags & O_NONBLOCK) {
        ps_print_warning("read will block anyway\n");
    }

    ret = i2c_conn_read(tmp, count);

    if (ret <= 0) {
        ps_print_err("i2c read data len: %d, expect:%lu\n", ret, count);
        return (size_t)ret;
    }

    if (copy_to_user(buf, tmp, ret)) {
        ps_print_err("failed to copy to user space\n");
        return -EFAULT;
    }
    return (size_t)ret;
}

STATIC ssize_t mpxx_i2c_write(struct file *filp, const char __user *buf,
        size_t count, loff_t *offset)
{
    struct i2c_dev *plat_dev;
    char tmp[MAX_SEND_DATA_LEN];
    int32_t ret;

    plat_dev = filp->private_data;

    if (plat_dev == NULL) {
        ps_print_err("plat_dev is err.\n");
        return -EFAULT;
    }

    if (count > MAX_SEND_DATA_LEN) {
        ps_print_err("write len:%lu, change to %d\n", count, MAX_SEND_DATA_LEN);
        count = MAX_SEND_DATA_LEN;
    }

    if (copy_from_user(tmp, buf, count)) {
        ps_print_err("copy_from_user fail\n");
        return -EFAULT;
    }

    ret = i2c_conn_write(tmp, count);
    if (ret != (int32_t)count) {
        ps_print_err("i2c write data len: %d, expect:%lu\n", ret, count);
        return -EIO;
    }
    return ret;
}

STATIC uint32_t mpxx_i2c_poll(struct file *filp, poll_table *wait)
{
    uint32_t mask = 0;
    ps_print_dbg("%s\n", __func__);

    poll_wait(filp, &g_i2c_read_wq, wait);

    if (conn_get_gpio_level(I2C_WKUP_HOST)) {
        mask |= POLLIN | POLLRDNORM;
    } else {
        conn_gpio_intr_enable(I2C_WKUP_HOST, OAL_IRQ_ENABLE);
    }

    return mask;
}

STATIC int32_t mpxx_i2c_open(struct inode *inode, struct file *filp)
{
    i2c_dev *dev = NULL;

    dev = container_of(filp->private_data, i2c_dev, cdev);
    filp->private_data = dev;
    ps_print_info("imajor:%d, iminor:%d\n", imajor(inode), iminor(inode));
    ps_print_info("mpxx i2c dev open\n");
    return 0;
}

STATIC int32_t mpxx_i2c_close(struct inode *inode, struct file *filp)
{
    ps_print_info("mpxx i2c dev close\n");
    return 0;
}

STATIC int32_t conn_driver_init(void)
{
    oal_wait_queue_init_head(&g_i2c_read_wq);
    mutex_init(&g_i2c_read_mutex);
    mutex_init(&g_i2c_dev_mutex);
    return OAL_SUCC;
}

void conn_driver_deinit(void)
{
    mutex_destroy(&g_i2c_read_mutex);
    mutex_destroy(&g_i2c_dev_mutex);
}

STATIC void i2c_wakeup_host_gpio_isr(void)
{
    if (conn_get_gpio_level(I2C_WKUP_HOST) != 0) {
        ps_print_info("This is true of wakeup_host_gpio_isr.\n");
    } else {
        ps_print_err("wakeup_host_gpio_isr false positives, and set read event also.\n");
    }
    oal_wait_queue_wake_up_interrupt(&g_i2c_read_wq);
}

irqreturn_t mpxx_i2c_dev_irq_handler(int32_t irq, void *dev_id)
{
    conn_gpio_intr_enable(I2C_WKUP_HOST, OAL_IRQ_FORBIDDEN);
    i2c_wakeup_host_gpio_isr();

    return IRQ_HANDLED;
}

STATIC const struct file_operations mpxx_i2c_fops = {
    .owner	 = THIS_MODULE,
    .llseek	 = no_llseek,
    .read	 = mpxx_i2c_read,
    .write 	 = mpxx_i2c_write,
    .poll    = mpxx_i2c_poll,
    .open	 = mpxx_i2c_open,
    .release = mpxx_i2c_close,
};

STATIC int32_t register_i2c_chr_dev(i2c_dev *node)
{
    int32_t ret = 0;

    node->chr_dev_num = I2C_CHR_DEV_NUM;
    ret = alloc_chrdev_region(&node->dev_id, 0, node->chr_dev_num, MPXX_I2C_DEV_NAME);
    if (ret < 0) {
        return ret;
    }

    node->cdev = cdev_alloc();
    if (node->cdev == NULL) {
        return -OAL_FAIL;
    }
    cdev_init(node->cdev, &mpxx_i2c_fops);
    node->cdev->owner = THIS_MODULE;

    ret = cdev_add(node->cdev, node->dev_id, node->chr_dev_num);
    if (ret < 0) {
        return ret;
    }

    node->cls = class_create(THIS_MODULE, "mpxx");
    if (node->cls == NULL) {
        return -OAL_FAIL;
    }

    node->dev = device_create(node->cls, NULL, node->dev_id, NULL, MPXX_I2C_DEV_NAME);
    if (node->dev == NULL) {
        return -OAL_FAIL;
    }
    return ret;
}

STATIC int32_t mpxx_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int32_t ret = 0;
    i2c_dev *dev = NULL;
    ps_print_info("mpxx board i2c probe\n");

    if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
        ps_print_err("need I2C_FUNC_I2C\n");
        return  -OAL_FAIL;
    }

    dev = kzalloc(sizeof(i2c_dev), GFP_KERNEL);
    if (dev == NULL) {
        ps_print_err("alloc error\n");
        dev_err(&client->dev, "failed to allocate memory for module data\n");
        return 0;
    }

    g_i2c_info.client = client;

    ret = register_i2c_chr_dev(dev);
    if (ret != 0) {
        dev_err(&client->dev, "%s: chrdev register err %d\n", __func__, ret);
        goto err_dev_init;
    }

    conn_driver_init();

    ret = conn_irq_register(I2C_WKUP_HOST, mpxx_i2c_dev_irq_handler, NULL);
    if (ret != 0) {
        ps_print_err("request irq fail\n");
        goto err_irq_init;
    }
    conn_gpio_intr_enable(I2C_WKUP_HOST, OAL_IRQ_FORBIDDEN);

    i2c_set_clientdata(client, dev);

    return OAL_SUCC;
err_irq_init:
    conn_driver_deinit();
err_dev_init:
    kfree(dev);
    return -OAL_FAIL;
}

STATIC int32_t mpxx_i2c_remove(struct i2c_client *client)
{
    i2c_dev *dev = NULL;

    ps_print_info("mpxx_board_i2c_remove begin!\n");
    dev_info(&client->dev, "%s ++\n", __func__);

    dev = i2c_get_clientdata(client);
    device_destroy(dev->cls, dev->dev_id);
    class_destroy(dev->cls);
    cdev_del(dev->cdev);
    unregister_chrdev_region(dev->dev_id, dev->chr_dev_num);
    conn_irq_unregister(I2C_WKUP_HOST, NULL);
    conn_driver_deinit();
    kfree(dev);
    return 0;
}

static struct of_device_id g_mpxx_i2c_match_table[] = {
    {
        .compatible = DTS_NODE_MPXX_I2C,
    },
    {},
};

STATIC struct i2c_driver g_mpxx_i2c_driver = {
    .probe     = mpxx_i2c_probe,
    .remove    = mpxx_i2c_remove,
    .driver    = {
        .owner = THIS_MODULE,
        .name  = "mpxx_i2c",
        .of_match_table	= g_mpxx_i2c_match_table,
    },
};

int32_t mpxx_i2c_init(void)
{
    int32_t ret;

    ret = i2c_add_driver(&g_mpxx_i2c_driver);
    if (ret) {
        ps_print_err("Unable to register hisi connectivity board i2c driver.\n");
        return ret;
    }

    ps_print_info("mpxx_i2c probe succ\n");
    return OAL_SUCC;
}

void mpxx_i2c_exit(void)
{
    i2c_del_driver(&g_mpxx_i2c_driver);
}
