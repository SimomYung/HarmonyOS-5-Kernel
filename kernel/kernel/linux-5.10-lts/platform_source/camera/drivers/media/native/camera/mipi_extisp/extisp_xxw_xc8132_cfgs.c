
#include "extisp_driver.h"
#include <linux/gpio.h>

#define xc8132_err(fmt, args...) \
do { \
	pr_err("[xc8132 err] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define xc8132_info(fmt, args...) \
do { \
	pr_info("[xc8132 info] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define xc8132_warn(fmt, args...) \
do { \
	pr_warn("[xc8132 warn] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define BYTE_LENGTH  3
#define XXW_XC8132_SLAVE_ADDR 0x1B

static int  xc8132_read_regs(struct i2c_client *i2client, uint32_t reg, uint8_t *val, int len)
{
    int ret = -1;
    uint8_t b0[BYTE_LENGTH] = {0};
    uint8_t b1[BYTE_LENGTH] = {0};
    uint8_t b2[BYTE_LENGTH] = {0};
    struct i2c_msg msg[4];

    if (!i2client || !val)
        return -EINVAL;

    xc8132_info("enter. xc8132 i2c read begin.");

    /* b0[0], b0[1], and b0[2] indicate the shift operation performed when the address of the first byte is taken;
     * b1[0], b1[1], and b1[2] indicate the shift operation performed when the address of the second byte is taken,
     * b2[0] and b2[1] indicate the shift operation performed when the addresses of the third and fourth bytes are obtained.
     * msg[0], msg[1], msg[2], and msg[3] respectively indicate a message.
     */
    b0[0] = 0xff; // 0xfffd is the most significant eight-bit identifier of the address of the ISP internal register.
    b0[1] = 0xfd;
    b0[2] = reg >> 24; // '24' Shift rightwards by 24 bits

    msg[0].addr = i2client->addr;
    msg[0].flags = 0;
    msg[0].buf = b0;
    msg[0].len = 3; // The length of the '3' buffer is 3.

    b1[0] = 0xff; // 0xfffe is the second most significant eight-bit identifier of the ISP internal register address.
    b1[1] = 0xfe;
    b1[2] = reg >> 16; // '16' shift rightwards by 16 bits

    msg[1].addr = i2client->addr;
    msg[1].flags = 0;
    msg[1].buf = b1;
    msg[1].len = 3; // The length of the '3' buffer is 3.

    b2[0] = reg >> 8; // '8' shifts 8 bits to the right.
    b2[1] = reg;

    msg[2].addr = i2client->addr;
    msg[2].flags = 0;
    msg[2].buf = b2;
    msg[2].len = 2; // '2'The buffer length is 2.

    msg[3].addr = i2client->addr;
    msg[3].flags = I2C_M_RD;
    msg[3].buf = val;
    msg[3].len = len;

    ret = i2c_transfer(i2client->adapter, msg, 4);
    if(ret == 4) {
        ret = 0;
    } else {
        xc8132_err("i2c rd failed=%d reg=0x%08x len=%d ",ret, reg, len);
        ret = -EREMOTEIO;
    }
    xc8132_info("exit, reg: %08x val: %02x.", reg, *val);
    return ret;
}

static s32 xc8132_write_regs(struct i2c_client *i2client, uint32_t reg, uint8_t *val, int len)
{
    xc8132_info(" enter.");
    uint8_t b0[BYTE_LENGTH] = {0};
    uint8_t b1[BYTE_LENGTH] = {0};
    struct i2c_msg msg[3];
    int ret = -1;

    if (!i2client || !val)
        return -EINVAL;

    uint8_t *b2 = kmalloc(len + 2, GFP_KERNEL);
    if (ZERO_OR_NULL_PTR(b2))
        return -ENOMEM;

    xc8132_info("xc8132 i2c write begin reg: %08x val: %02x  \n", reg,*(uint8_t *)val);

    /* b0[0], b0[1], and b0[2] indicate the shift operation performed when the address of the first byte is taken;
     * b1[0], b1[1], and b1[2] indicate the shift operation performed when the address of the second byte is taken,
     * b2[0] and b2[1] indicate the shift operation performed when the addresses of the third and fourth bytes are obtained.
     * msg[0], msg[1], msg[2], and msg[3] respectively indicate a message.
     */
    b0[0] = 0xff; // 0xfffd is the upper eight bits identifier of the ISP internal register address.
    b0[1] = 0xfd;
    b0[2] = reg >> 24; // '24'Shift rightwards by 24 bits

    msg[0].addr = i2client->addr;
    msg[0].flags = 0;
    msg[0].buf = b0;
    msg[0].len = 3; // '3'The buffer length is 3.

    b1[0] = 0xff; // 0xfffe is the second upper eight-bit identifier of the ISP internal register address.
    b1[1] = 0xfe;
    b1[2] = reg >> 16; // '16'Shift right by 16 bits

    msg[1].addr = i2client->addr;
    msg[1].flags = 0;
    msg[1].buf = b1;
    msg[1].len = 3; // '3'The buffer length is 3.

    b2[0] = reg >> 8; // '8'Shift right by 8 bits
    b2[1] = reg;

    ret = memcpy_s(&b2[2], len, val, len);
    if (ret != 0) {
        xc8132_err("memcpy_s fail");
        kfree(b2);
        return ret;
    }
    msg[2].addr = i2client->addr;
    msg[2].flags = 0;
    msg[2].buf = b2;
    msg[2].len = len + 2;

    ret = i2c_transfer(i2client->adapter, msg, 3);
    kfree(b2);
    xc8132_info(" exit");
    return ret;
}

static int xxw_xc8132_rw_des_reg(struct i2c_client *i2client, extisp_reg_rw_cfg_t *reg_cfg)
{
    int ret = -1;
    struct reg_rw_data *reg_data = NULL;
    int len = 0;
    uint8_t *buffer = NULL;

    if (!i2client || !reg_cfg)
        return -EINVAL;

    // check reg_data invalid
    reg_data = &reg_cfg->reg_data;
    if (!reg_data || reg_data->len <= 0)
        return -EINVAL;

    // override i2c slave device addr
    i2client->addr = XXW_XC8132_SLAVE_ADDR;
    xc8132_info("enter, i2c_client addr:0x%x, reg_read_or_write:%d(1-read,0-write), len:%d.",
		    i2client->addr, reg_cfg->reg_read_or_write, reg_data->len);

    len = reg_data->len;
    buffer = kmalloc(len, GFP_KERNEL);
    if (ZERO_OR_NULL_PTR(buffer))
        return -ENOMEM;

    if (reg_cfg->reg_read_or_write == I2C_RW_OPS_READ) {
        ret = xc8132_read_regs(i2client, reg_data->reg_addr, buffer, len);
        if (ret < 0) {
            xc8132_err("xc8132_read_regs fail.");
            kfree(buffer);
            return ret;
        }
        if (copy_to_user(reg_data->data, buffer, len)) {
            xc8132_err("copy_to_user fail.");
            kfree(buffer);
            return -EFAULT;
        }
    } else if (reg_cfg->reg_read_or_write == I2C_RW_OPS_WRITE) {
        if (copy_from_user(buffer, reg_data->data, len)) {
            xc8132_err("copy_from_user fail.");
            kfree(buffer);
            return -EFAULT;
        }
        ret = xc8132_write_regs(i2client, reg_data->reg_addr, buffer, len);
        if (ret < 0) {
            xc8132_err("xc8132_write_regs fail.");
            kfree(buffer);
            return ret;
        }
    }

    kfree(buffer);
    return 0;
}

static int xxw_xc8132_match_id(extisp_data_ctrl_t *ctrl)
{
    int ret, idx_gpio_val = -1;
    int isp_index_gpiono;

    if (!ctrl)
        return -EINVAL;

    isp_index_gpiono = ctrl->isp_index_gpiono;

    xc8132_info("exter, isp_index_gpiono:%d ", ctrl->isp_index_gpiono);

    if (!gpio_is_valid(isp_index_gpiono)) {
        xc8132_err("isp index gpiono:%d is invalid.", isp_index_gpiono);
        return -EINVAL;
    }
    ret = gpio_request(isp_index_gpiono, "isp_index");
    if (ret) {
        xc8132_err("request gpio%d name:isp_index  fail %d", isp_index_gpiono, ret);
        return ret;
    }
    ret = gpio_direction_input(isp_index_gpiono);
    if (ret) {
        xc8132_err("set gpio%d isp index direction input low  fail %d", isp_index_gpiono, ret);
        gpio_free(isp_index_gpiono);
        return ret;
    } else {
        idx_gpio_val = gpio_get_value(isp_index_gpiono);
        xc8132_info("gpio%d get idx_gpio_val:%d.", isp_index_gpiono, idx_gpio_val);
        gpio_free(isp_index_gpiono);
    }

    if (ctrl->isp_idx == idx_gpio_val) {
        ret = EOK;
    } else {
        ret = -EINVAL;
    }

    return ret;
}


static exitisp_reg_ops_t xxw_xc8132_reg_ops =
{
    .match_id = xxw_xc8132_match_id,
    .rw_des_reg = xxw_xc8132_rw_des_reg,
};

exitisp_reg_ops_t *get_xc8132_reg_ops(void)
{
    return &xxw_xc8132_reg_ops;
}
