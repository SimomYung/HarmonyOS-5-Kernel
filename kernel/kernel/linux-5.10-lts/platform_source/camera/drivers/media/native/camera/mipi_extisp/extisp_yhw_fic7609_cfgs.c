
#include "extisp_driver.h"
#include <linux/gpio.h>

#define fic7609_err(fmt, args...) \
do { \
	pr_err("[fic7609 err] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define fic7609_info(fmt, args...) \
do { \
	pr_info("[fic7609 info] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define fic7609_warn(fmt, args...) \
do { \
	pr_warn("[fic7609 warn] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define YHW_FIC7609_SLAVE_ADDR 0x76
#define MSGS_MAX_LENGTH 8192

static int validate_message(struct i2c_msg *msg)
{
    if (!msg || (msg->len > MSGS_MAX_LENGTH))
        return -EINVAL;

    if (msg->flags & I2C_M_RECV_LEN) {
        if (!(msg->flags & I2C_M_RD) ||
            msg->len < 1 || msg->buf[0] < 1 ||
            msg->len < msg->buf[0] + I2C_SMBUS_BLOCK_MAX) {
            return -EINVAL;
        }
        msg->len = msg->buf[0];
    }

    return 0;
}

static int prepare_message(struct i2c_msg *msg, u8 __user **data_ptr)
{
    if(!msg || msg->len <= 0)
        return -EINVAL;

    *data_ptr = (u8 __user *)msg->buf;
    msg->buf = memdup_user(*data_ptr, msg->len);
    if (IS_ERR(msg->buf))
        return PTR_ERR(msg->buf);

    msg->flags |= I2C_M_DMA_SAFE;
    return 0;
}

static void cleanup_messages(struct i2c_msg *msgs, unsigned nmsgs)
{
    unsigned i;
    for (i = 0; i < nmsgs; i++) {
        kfree(msgs[i].buf);
    }
}

static int yhw_fic7609_i2c_transfer(struct i2c_client *client,
        unsigned nmsgs, struct i2c_msg *msgs)
{
    u8 __user **data_ptrs = NULL;
    int i, res;

    data_ptrs = kmalloc_array(nmsgs, sizeof(u8 __user *), GFP_KERNEL);
    if (ZERO_OR_NULL_PTR(data_ptrs))
        return -ENOMEM;

    res = 0;
    for (i = 0; i < nmsgs; i++) {
        res = prepare_message(&msgs[i], &data_ptrs[i]); // Prepare the message, including copying data from user space and setting DMA flags.
        if (res < 0)
            break;

        res = validate_message(&msgs[i]); // Verify the message length and receive length flag.
        if (res < 0)
            break;
    }

    if (res < 0) {
        cleanup_messages(msgs, i); // Clear the allocated memory.
        kfree(data_ptrs);
        return res;
    }

    res = i2c_transfer(client->adapter, msgs, nmsgs); // I2C transmission and processing of results
    while (i > 0) {
        i--;
        if (res >= 0 && (msgs[i].flags & I2C_M_RD)) {
            if (copy_to_user(data_ptrs[i], msgs[i].buf,
				    msgs[i].len))
                res = -EFAULT;
        }
        kfree(msgs[i].buf);
    }

    kfree(data_ptrs);
    return res;
}

static int yhw_fic7609_rw_des_reg(struct i2c_client *i2client, extisp_reg_rw_cfg_t *reg_cfg)
{
    int ret = -1;
    struct i2c_rdwr_ioctl_data rdwr_arg;
    struct i2c_msg *rdwr_msg = NULL;

    fic7609_info("enter.");

    if (!i2client || !reg_cfg)
        return -EINVAL;
	rdwr_arg = reg_cfg->rdwr;

    i2client->addr = YHW_FIC7609_SLAVE_ADDR;

    if (!rdwr_arg.msgs ||
        rdwr_arg.nmsgs <= 0 ||
        rdwr_arg.nmsgs > I2C_RDWR_IOCTL_MAX_MSGS)
        return -EINVAL;

    rdwr_msg = memdup_user(rdwr_arg.msgs,
                    rdwr_arg.nmsgs * sizeof(struct i2c_msg));
    if (rdwr_msg == NULL)
        return -EINVAL;

    ret = yhw_fic7609_i2c_transfer(i2client, rdwr_arg.nmsgs, rdwr_msg);
	if (ret < 0)
		fic7609_info("exit, ret:%d i2c_client addr:0x%x, nmsgs:%d", ret, i2client->addr, rdwr_arg.nmsgs);
	kfree(rdwr_msg);

    return ret;
}

static int yhw_fic7609_match_id(extisp_data_ctrl_t *ctrl)
{
    int ret, idx_gpio_val = -1;
    int isp_index_gpiono;

    if (!ctrl)
        return -EINVAL;

    isp_index_gpiono = ctrl->isp_index_gpiono;

    fic7609_info("exter, isp_index_gpiono:%d ", ctrl->isp_index_gpiono);

    if (!gpio_is_valid(isp_index_gpiono)) {
        fic7609_err("isp index gpiono:%d is invalid.", isp_index_gpiono);
        return -EINVAL;
    }
    ret = gpio_request(isp_index_gpiono, "isp_index");
    if (ret) {
        fic7609_err("request gpio%d name:isp_index  fail %d", isp_index_gpiono, ret);
        return ret;
    }
    ret = gpio_direction_input(isp_index_gpiono);
    if (ret) {
        fic7609_err("set gpio%d isp index direction input low  fail %d", isp_index_gpiono, ret);
        gpio_free(isp_index_gpiono);
        return ret;
    } else {
        idx_gpio_val = gpio_get_value(isp_index_gpiono);
        fic7609_info("gpio%d get idx_gpio_val:%d.", isp_index_gpiono, idx_gpio_val);
        gpio_free(isp_index_gpiono);
    }

    if (ctrl->isp_idx == idx_gpio_val)
        ret = EOK;
    else
        ret = -EINVAL;

    return ret;
}

static exitisp_reg_ops_t yhw_fic7609_reg_ops =
{
    .match_id = yhw_fic7609_match_id,
    .rw_des_reg = yhw_fic7609_rw_des_reg,
};

exitisp_reg_ops_t *get_fic7609_reg_ops(void)
{
    return &yhw_fic7609_reg_ops;
}
