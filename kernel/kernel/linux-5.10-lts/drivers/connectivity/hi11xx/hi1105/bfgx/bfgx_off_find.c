/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: bfgx sle functional module
 * Author: @CompanyNameTag
 */

/* Include Head file */
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/power_supply.h>
#include "plat_debug.h"
#include "plat_pm.h"
#include "plat_uart.h"
#include "bfgx_data_parse.h"
#include "bfgx_dev.h"
#include "bfgx_bt.h"
#include "bfgx_sle.h"
#include "bfgx_core.h"
#include "bfgx_off_find.h"

#define DTS_NODE_HISI_OFF_FIND_EN "hi110x,off_find_en"
#define DTS_NODE_HISI_OFF_FIND_CTRL "hi110x,off_find_ctrl"
#define DTS_NODE_HISI_OFF_FIND_GPIO "hi110x,off_find_gpio"
#define OFF_FIND_TIME_DEFAULT  1    // 默认关机找回时长为1小时

// 关机找回全局功能使能配置
int32_t g_off_find_en = OAL_FALSE;
int32_t g_off_find_ctrl_gpio = -1;
struct regulator *g_off_find_ctrl = NULL;
int32_t g_off_find_status = OAL_FALSE;
oal_completion g_off_find_thread_wait_opened;
oal_completion g_off_find_thread_wait_closed;

/* 调试使用:
* 防漏电命令完全不处理：       echo 0x1 > /sys/module/plat_1105/parameters/g_avoid_leakage_bypass
* 防漏单命令发送但驱动不关闭： echo 0x2> /sys/module/plat_1105/parameters/g_avoid_leakage_bypass
*/
ulong g_avoid_leakage_bypass = 0;
oal_debug_module_param(g_avoid_leakage_bypass, ulong, S_IRUGO | S_IWUSR);
#define AVL_BYPASS_CMD_SEND      0      // bypass防漏电命令的所有处理
#define AVL_BYPASS_CLOSE_DRV     1      // bypass防漏电命令下发后关闭驱动的处理

static int32_t off_find_set_flag(uint32_t flag);
static int32_t off_find_get_flag(void);
static int32_t off_find_set_time(uint32_t time_hour);
static int32_t off_find_get_time(void);

int32_t off_find_get_status(void)
{
    return  g_off_find_status;
}

/*
 * Prototype    : off_find_dts_init
 * Description  : 关机找回初始化读DTS配置,
 */
static int32_t off_find_dts_init(struct platform_device *pdev)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    struct device_node *np = NULL;
    int32_t gpio_nr;

    ret = get_board_dts_node(&np, DTS_NODE_HISI_MPXX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return -OAL_FAIL;
    }
    /* 获取关机找回功能是否支持 */
    ret = of_property_read_u32(np, DTS_NODE_HISI_OFF_FIND_EN, &g_off_find_en);
    if (ret == BOARD_SUCC) {
        ps_print_info("off_find_en:0x%x\n", g_off_find_en);
    } else {
        ps_print_info("off_find_init, get %s fail\n", DTS_NODE_HISI_OFF_FIND_EN);
        return -OAL_FAIL;
    }

    if (g_off_find_en == OAL_FALSE) {
        return OAL_SUCC;
    }
    /* 获取关机找回功能的不下电flag控制GPIO */
    ret = of_property_read_u32(np, DTS_NODE_HISI_OFF_FIND_GPIO, &gpio_nr);
    if (ret == BOARD_SUCC) {
        g_off_find_ctrl_gpio = gpio_nr;
        if (gpio_request(g_off_find_ctrl_gpio, "off_find_gpio") != OAL_SUCC) {
            ps_print_err("off_find_gpio request [%d] fail\n", g_off_find_ctrl_gpio);
            return -OAL_FAIL;
        }
        ps_print_info("succ to get off_find_gpio %s=%d\n", DTS_NODE_HISI_OFF_FIND_GPIO, g_off_find_ctrl_gpio);
    } else {
        ps_print_info("fail to get off_find_gpio prop: %s\n", DTS_NODE_HISI_OFF_FIND_GPIO);
        return -OAL_FAIL;
    }
    /* 获取关机找回功能的控制结构：时间设置节点 */
    g_off_find_ctrl = regulator_get(&pdev->dev, DTS_NODE_HISI_OFF_FIND_CTRL);
    if (oal_is_err_or_null(g_off_find_ctrl)) {
        ps_print_err("fail to get %s\n", DTS_NODE_HISI_OFF_FIND_CTRL);
        return -OAL_FAIL;
    }
    ps_print_info("succ to get %s\n", DTS_NODE_HISI_OFF_FIND_CTRL);
    return OAL_SUCC;
#else
    ps_print_info("not support dts\n");
    return OAL_SUCC;
#endif
}

/*
 * Prototype    : off_find_init
 * Description  : 关机找回初始化
 */
void off_find_init(struct platform_device *pdev)
{
    oal_init_completion(&g_off_find_thread_wait_opened);
    oal_init_completion(&g_off_find_thread_wait_closed);
    if (off_find_dts_init(pdev) != OAL_SUCC) {
        return;
    }
    /* 重启后，如果是关机找回状态，读取维测信息, 由于regulator的原生机制会检测启动kernel的时候如果没有组件使用电源，
    就会关闭这个电源，所以flag会在启动时候自动清理掉，不能用于检查是否关机找回，只能通过检查电源管脚是否拉高间接判断 */
    if ((g_off_find_en == OAL_TRUE) && (conn_get_gpio_level(CHIP_POWR) == GPIO_LEVEL_HIGH)) {
        ps_print_info("get off_find debug info:\n");
        ssi_dump_device_regs(SSI_MODULE_MASK_OFF_FIND);
    }

    ps_print_info("current off_find flag %d:\n", off_find_get_flag());
    /* 清除flag和时间配置 */
    off_find_set_flag(OAL_FALSE);
    off_find_set_time(OFF_FIND_TIME_DEFAULT);

    /* 芯片下电 */
    ps_print_info("CHIP_POWR[%d], W_EN[%d], B_EN[%d], G_EN[%d]\n",
                  conn_get_gpio_level(CHIP_POWR), conn_get_gpio_level(W_POWER),
                  conn_get_gpio_level(BFGX_POWER), conn_get_gpio_level(G_POWER));
    conn_set_gpio_level(BFGX_POWER, GPIO_LEVEL_LOW);
    conn_set_gpio_level(G_POWER, GPIO_LEVEL_LOW);
    conn_set_gpio_level(W_POWER, GPIO_LEVEL_LOW);
    conn_set_gpio_level(CHIP_POWR, GPIO_LEVEL_LOW);
    g_off_find_status = OAL_FALSE;
}

/*
 * Prototype    : off_find_set_flag
 * Description  : 关机找回配置PMU不下电flag
 *                flag：OAL_TRUE/OAL_FALSE
 */
static int32_t off_find_set_flag(uint32_t flag)
{
    int32_t ret = OAL_SUCC;
    enum gpio_level level_set;

    if (g_off_find_ctrl_gpio < 0) {
        ps_print_err("g_off_find_ctrl_gpio not init\n");
        return -OAL_FAIL;
    }

    level_set = (flag == OAL_TRUE) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;

    ret = gpio_direction_output(g_off_find_ctrl_gpio, level_set);

    ps_print_info("off_find_set_flag %d ret %d\n", level_set, ret);

    return ret;
}

/*
 * Prototype    : off_find_get_flag
 * Description  : 关机找回获取PMU不下电flag
 */
static int32_t off_find_get_flag(void)
{
    if (g_off_find_ctrl_gpio < 0) {
        ps_print_err("g_off_find_ctrl_gpio not init\n");
        return -OAL_FAIL;
    }
    return gpio_get_value(g_off_find_ctrl_gpio);
}

/*
 * Prototype    : off_find_set_time
 * Description  : 关机找回配置PMU不下电最大时间
 *                ctrl：控制节点
 *                time：以小时计的时间
 */
static int32_t off_find_set_time(uint32_t time_hour)
{
    int32_t ret;
    uint32_t time_minute = time_hour * 60 ; // 一小时60分钟

    if (g_off_find_ctrl == NULL) {
        ps_print_err("g_off_find_ctrl is NULL\n");
        return -OAL_FAIL;
    }
    /* 复用电压接口 */
    ret = regulator_set_voltage(g_off_find_ctrl, time_minute, time_minute);
    ps_print_info("off_find_set_time %d ret %d\n", time_minute, ret);
    off_find_get_time();
    return ret;
}

/*
 * Prototype    : off_find_get_time
 * Description  : 关机找回获取配置的PMU不下电时间
 *                ret: 负值：失败；正值：以小时为单位的时长
 */
static int32_t off_find_get_time(void)
{
    int32_t ret;

    if (g_off_find_ctrl == NULL) {
        ps_print_err("g_off_find_ctrl is NULL\n");
        return -OAL_FAIL;
    }
    /* 复用电压接口 */
    ret = regulator_get_voltage(g_off_find_ctrl);
    ret = ret / 60;  // 1小时60分钟
    ps_print_info("off_find_get_time ret %d\n", ret);
    return ret;
}

/*
 * Prototype    : off_find_close_other_subsys
 * Description  : 关机找回关闭关机找回命令节点外的其它的子系统
 */
static void off_find_close_other_subsys(struct ps_core_s *ps_core_d)
{
    struct ps_plat_s *ps_plat_i = NULL;
    struct ps_core_s *ps_core_i = NULL;
    struct list_head *ps_plat_list = ps_get_plat_list();
    struct pm_top* pm_top_data = pm_get_top();

    ps_print_info("close all other systems\n");
    wlan_pm_close_dev(HCC_EP_WIFI_DEV);

    mutex_lock(&(pm_top_data->host_mutex));
    list_for_each_entry(ps_plat_i, ps_plat_list, node) {
        ps_core_i = ps_plat_i->core_data;
        /* 非关机找回所属域下子系统关闭，拉en下电子系统 */
        if (ps_core_i != ps_core_d) {
            hw_bfgx_close_all_service(ps_core_i->pm_data, OAL_TRUE);
        }
    }
    mutex_unlock(&(pm_top_data->host_mutex));
}
/*
 * Prototype    : off_find_close_all_subsys
 * Description  : 关机找回关闭所有的子系统
 *              selfclose: ps_core_d所在的子系统是否下电
 */
static void off_find_close_all_subsys(struct ps_core_s *ps_core_d, uint32_t self_close)
{
    struct pm_top* pm_top_data = pm_get_top();

    off_find_close_other_subsys(ps_core_d);

    /* 关机找回所属域下子系统关闭 */
    mutex_lock(&(pm_top_data->host_mutex));
    hw_bfgx_close_all_service(ps_core_d->pm_data, self_close);
    mutex_unlock(&(pm_top_data->host_mutex));
}

/*
 * Prototype    : off_find_avoid_leakage_cmd
 * Description  : 关机找回防漏电命令处理。
 */
static int32_t off_find_avoid_leakage_cmd(struct bfgx_dev_node *dev_node)
{
    struct ps_core_s *ps_core_d = NULL;
    struct pm_drv_data *pm_data = NULL;
    int32_t prepare_result;
    uint32_t max_check_cnt = 100;
    uint32_t cur_cnt = 0;

    if (test_bit(AVL_BYPASS_CMD_SEND, &g_avoid_leakage_bypass)) {
        ps_print_info("bypass send cmd");
        return OAL_SUCC;
    }

    if (dev_node == NULL || dev_node->core == NULL) {
        ps_print_err("%s dev_node or core is null", __func__);
        return -EINVAL;
    }

    ps_core_d = dev_node->core;

    /* 防漏命令下发前，关闭其它子系统 */
    off_find_close_other_subsys(ps_core_d);

    pm_data = ps_core_d->pm_data;
    if (prepare_to_visit_node(ps_core_d, &prepare_result) != BFGX_POWER_SUCCESS) {
        ps_print_err("[%s]wake up device fail\n", index2name(pm_data->index));
        return -OAL_FAIL;
    }
    ps_print_info("[%s]curr wakeup host gpio level[%d]\n", index2name(pm_data->index),
                  conn_get_gpio_level(pm_data->ps_plat_data->host_wkup_io_type));

    if (ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_OFF_FIND) != 0) {
        ps_print_err("[%s]SYS_CFG_OFF_FIND MSG send fail\n", index2name(pm_data->index));
        post_to_visit_node(ps_core_d, prepare_result);
        return -OAL_FAIL;
    }

    /* 等待芯片处理完成。芯片需要同时disable uart唤醒使能, 避免soc下电时误唤醒芯片 */
    do {
        cur_cnt++;
        msleep(10); // 10ms后检查GPIO是否拉低确认命令发送成功
        if (cur_cnt >= max_check_cnt) {
            ps_print_err("[%s]SYS_CFG_OFF_FIND wait ack fail\n", index2name(pm_data->index));
            post_to_visit_node(ps_core_d, prepare_result);
            return -OAL_FAIL;
        }
    } while (conn_get_gpio_level(pm_data->ps_plat_data->host_wkup_io_type) == GPIO_LEVEL_HIGH);

    if (test_bit(AVL_BYPASS_CLOSE_DRV, &g_avoid_leakage_bypass)) {
        ps_print_info("bypass close all service");
        post_to_visit_node(ps_core_d, prepare_result);
        return OAL_SUCC;
    }

    pm_data->bfgx_lowpower_enable = BFGX_PM_DISABLE;
    /* 关机找回所属域下子系统关闭，清理drvier侧的资源，所有业务设置为shutdown状态，关闭tty，但芯片不下电 */
    mutex_lock(&(pm_get_top()->host_mutex));
    hw_bfgx_close_all_service(ps_core_d->pm_data, OAL_FALSE);
    /* 设置off find status, 防止后续其它子系统再open, 以及业务节点读写数据 */
    g_off_find_status = OAL_TRUE;
    mutex_unlock(&(pm_get_top()->host_mutex));

    post_to_visit_node(ps_core_d, prepare_result);

    return OAL_SUCC;
}

static int32_t off_find_thread_open(struct ps_core_s *ps_core_d)
{
    int32_t prepare_result;
    unsigned long timeleft;
    struct pm_drv_data *pm_data = NULL;

    pm_data = ps_core_d->pm_data;
    if (prepare_to_visit_node(ps_core_d, &prepare_result) != BFGX_POWER_SUCCESS) {
        ps_print_err("[%s]wake up device fail\n", index2name(pm_data->index));
        return -OAL_FAIL;
    }

    oal_reinit_completion(g_off_find_thread_wait_opened);
    if (ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_OPEN_OFF_FIND_THREAD) != 0) {
        ps_print_err("[%s]SYS_CFG_OPEN_OFF_FIND_THREAD MSG send fail\n", index2name(pm_data->index));
        post_to_visit_node(ps_core_d, prepare_result);
        return -OAL_FAIL;
    }
    ps_print_info("[%s]SYS_CFG_OPEN_OFF_FIND_THREAD MSG send\n", index2name(pm_data->index));

    timeleft = oal_wait_for_completion_timeout(&g_off_find_thread_wait_opened,
                                               msecs_to_jiffies(WAIT_OPEN_SLE_HOST_TIME));
    if (!timeleft) {
        ps_uart_state_dump(ps_core_d);
        ps_print_err("wait SYS_CFG_OPEN_OFF_FIND_THREAD open ack timeout\n");
        ssi_dump_device_regs(SSI_MODULE_MASK_AON | SSI_MODULE_MASK_ARM_REG | SSI_MODULE_MASK_GTRACE |
                             SSI_MODULE_MASK_BTRACE);
        post_to_visit_node(ps_core_d, prepare_result);
        return -ETIMEDOUT;
    }

    post_to_visit_node(ps_core_d, prepare_result);

    return OAL_SUCC;
}

void off_find_thread_completion_handle(uint8_t msg)
{
    if (msg == SYS_OFF_FIND_INIT) {
        ps_print_info("OFF_FIND thread init success\n");
        oal_complete(&g_off_find_thread_wait_opened);
    }

    if (msg == SYS_OFF_FIND_EXIT) {
        ps_print_info("OFF_FIND thread exit success\n");
        oal_complete(&g_off_find_thread_wait_closed);
    }
}

static int32_t off_find_thread_close(struct ps_core_s *ps_core_d)
{
    int32_t prepare_result;
    unsigned long timeleft;
    struct pm_drv_data *pm_data = NULL;

    pm_data = ps_core_d->pm_data;
    if (prepare_to_visit_node(ps_core_d, &prepare_result) != BFGX_POWER_SUCCESS) {
        ps_print_err("[%s]wake up device fail\n", index2name(pm_data->index));
        return -OAL_FAIL;
    }

    oal_reinit_completion(g_off_find_thread_wait_closed);
    if (ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_CLOSE_OFF_FIND_THREAD) != 0) {
        ps_print_err("[%s]SYS_CFG_CLOSE_OFF_FIND_THREAD MSG send fail\n", index2name(pm_data->index));
        post_to_visit_node(ps_core_d, prepare_result);
        return -OAL_FAIL;
    }
    ps_print_info("[%s]SYS_CFG_CLOSE_OFF_FIND_THREAD MSG send\n", index2name(pm_data->index));

    timeleft = oal_wait_for_completion_timeout(&g_off_find_thread_wait_closed,
                                               msecs_to_jiffies(WAIT_CLOSE_SLE_HOST_TIME));
    if (!timeleft) {
        ps_uart_state_dump(ps_core_d);
        ps_print_err("wait SYS_CFG_CLOSE_OFF_FIND_THREAD close ack timeout\n");
        if (ps_core_d->uart_index == BUART) {
            ssi_dump_device_regs(SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_BTRACE);
        } else if (ps_core_d->uart_index == GUART) {
            ssi_dump_device_regs(SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_GTRACE);
        }
        post_to_visit_node(ps_core_d, prepare_result);
        return -ETIMEDOUT;
    }

    post_to_visit_node(ps_core_d, prepare_result);

    return OAL_SUCC;
}

/*
 * Prototype    : off_find_enable_cmd
 * Description  : 关机找回使能命令处理。
 */
static int32_t off_find_enable_cmd(struct bfgx_dev_node *dev_node)
{
    int32_t ret = OAL_SUCC;
    struct ps_core_s *ps_core_d = NULL;
    struct pm_top* pm_top_data = pm_get_top();

    if (dev_node == NULL || dev_node->core == NULL) {
        ps_print_err("%s dev_node or core is null", __func__);
        return -EINVAL;
    }
    ps_core_d = dev_node->core;

    ps_print_info("current miscdevice[%s]\n", dev_node->c.name);
    if (strcmp(dev_node->c.name, HW_OFF_FIND_DEV_NAME) == 0) {
        /* 关闭所有打开的子系统，芯片下电 */
        off_find_close_all_subsys(ps_core_d, OAL_TRUE);

        /* 重新打开SLE */
        mutex_lock(&(pm_top_data->host_mutex));
        ret = hw_bfgx_open(ps_core_d, BFGX_SLE);
        if (ret != 0) {
            ps_print_err("open sle fail\n");
            oal_atomic_set(&dev_node->open_cnt, 0);
            if (dev_node->dev_data != NULL) {
                struct bfgx_sle_private_data *priv_data = (struct bfgx_sle_private_data *)(dev_node->dev_data);
                mutex_destroy(&priv_data->data_combination.comb_mutex);
                os_mem_kfree(priv_data);
                dev_node->dev_data = NULL;
            }
            mutex_unlock(&(pm_top_data->host_mutex));
            return -OAL_FAIL;
        }
        /* 设置off find status, 防止后续其它子系统再open, 以及业务节点读写数据 */
        g_off_find_status = OAL_TRUE;
        mutex_unlock(&(pm_top_data->host_mutex));

        ret = off_find_thread_open(ps_core_d);
        if (ret != OAL_SUCC) {
            ps_print_err("open SYS_CFG_OPEN_OFF_FIND_THREAD fail ret[%d]\n", ret);
            return -OAL_FAIL;
        }
    }

    off_find_set_flag(OAL_TRUE);

    return ret;
}

/*
 * Prototype    : off_find_disable_cmd
 * Description  : 关机找回去使能命令处理。
 */
static int32_t off_find_disable_cmd(struct bfgx_dev_node *dev_node)
{
    int32_t ret = OAL_SUCC;

    if (dev_node == NULL || dev_node->core == NULL) {
        ps_print_err("%s dev_node or core is null", __func__);
        return -EINVAL;
    }

    ps_print_info("current miscdevice[%s]\n", dev_node->c.name);

    off_find_set_flag(OAL_FALSE);

    mutex_lock(&(pm_get_top()->host_mutex));
    /* 已处于关机找回使能状态,关掉使能时开启的sle */
    if ((strcmp(dev_node->c.name, HW_OFF_FIND_DEV_NAME) == 0) && (g_off_find_status == OAL_TRUE)) {
        /* 关掉SLE */
        ret = off_find_thread_close(dev_node->core);
        if (ret != OAL_SUCC) {
            ps_print_err("close SYS_CFG_CLOSE_OFF_FIND_THREAD fail, ret[%d]\n", ret);
        }
        hw_bfgx_close(dev_node->core, BFGX_SLE);
    }
    g_off_find_status = OAL_FALSE;
    mutex_unlock(&(pm_get_top()->host_mutex));
    return OAL_SUCC;
}

/*
 * Prototype    : off_find_battery_reserve
 * Description  : 关机找回配置电量预留
 *                0: 关闭预留 1：使能预留
 */
static int32_t off_find_battery_reserve(int32_t enable)
{
#ifdef CONFIG_BATTERY_OFF_FIND
    int ret;
    struct power_supply *psy;
    union power_supply_propval val = { 0 };

    val.intval = enable;
    psy = power_supply_get_by_name("Battery");
    if (psy == NULL) {
        ps_print_err("psy name null!\n");
        return -EINVAL;
    }

    ret = power_supply_set_property(psy, POWER_SUPPLY_PROP_OFF_FIND, &val);
    ps_print_info("set POWER_SUPPLY_PROP_OFF_FIND[%d],ret[%d]\n", enable, ret);
    power_supply_put(psy); // 注意：put和上面的get对应，get成功后，需要且仅需要put一次
    return ret;
#else
    ps_print_info("CONFIG_BATTERY_OFF_FIND NOT SUPPORT\n");
    return OAL_SUCC;
#endif
}

/*
 * Prototype    : off_find_cmd_parse
 * Description  : 关机找回命令解析总入口。
 */
int32_t off_find_cmd_parse(struct bfgx_dev_node *dev_node, uint8_t OAL_USER *data)
{
    struct off_find_para cmd_data;
    int32_t ret = OAL_SUCC;

    if (dev_node == NULL) {
        ps_print_err("%s dev_node is null", __func__);
        return -EINVAL;
    }

    if (data == NULL) {
        ps_print_err("user data is NULL\n");
        return -EINVAL;
    }

    if (copy_from_user(&cmd_data, data, sizeof(cmd_data))) {
        ps_print_err("get data head from user fail\n");
        return -OAL_FAIL;
    }
    ps_print_info("cmd=%d\n", cmd_data.cmd);

    switch (cmd_data.cmd) {
        case OFF_FIND_ENABLE:
            ps_print_info("enable cmd data_len[%d],data[%d]\n", cmd_data.data_len, cmd_data.data[0]);
            if ((cmd_data.data_len != 0) && (cmd_data.data[0] != 0)) {
                off_find_set_time(cmd_data.data[0]);
            }
            ret = off_find_enable_cmd(dev_node);
            break;
        case OFF_FIND_DISABLE:
            ret = off_find_disable_cmd(dev_node);
            break;
        case OFF_FIND_AVOID_LEAKAGE:
            ret = off_find_avoid_leakage_cmd(dev_node);
            break;
        case OFF_FIND_BATTERY_RSV:
            ps_print_info("battery reserve cmd data_len[%d],data[%d]\n", cmd_data.data_len, cmd_data.data[0]);
            if (cmd_data.data_len != 0) {
                ret = off_find_battery_reserve(cmd_data.data[0]);
            }
            break;
        default:
            ps_print_warning("cmd = %d not find\n", cmd_data.cmd);
            return -EINVAL;
    }

    return ret;
}

/*
 * Prototype    : hw_off_find_open
 * Description  : 关机找回节点open, 仅申请私有数据初始化, 不对子系统上电。
 */
static int32_t hw_off_find_open(struct inode *inode, struct file *filp)
{
    struct bfgx_dev_node *dev_node = NULL;
    struct bfgx_sle_private_data *priv_data = NULL;
    struct pm_top* pm_top_data = pm_get_top();

    ps_print_info("hw_off_find_open");
    if (g_off_find_en == OAL_FALSE) {
        ps_print_warning("off_find not support!");
        return -EINVAL;
    }

    if (unlikely((filp == NULL) || (filp->private_data == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    dev_node = oal_container_of((struct miscdevice *)filp->private_data, struct bfgx_dev_node, c);

    mutex_lock(&(pm_top_data->host_mutex));
    if (oal_atomic_read(&dev_node->open_cnt) != 0) {
        ps_print_err("hwoff_find dev already opened\n");
        mutex_unlock(&(pm_top_data->host_mutex));
        return -EINVAL;
    }

    if (dev_node->dev_data == NULL) {
        priv_data = (struct bfgx_sle_private_data *)os_kzalloc_gfp(sizeof(struct bfgx_sle_private_data));
        if (priv_data == NULL) {
            ps_print_err("SLE private data memory alloc err\n");
            mutex_unlock(&(pm_top_data->host_mutex));
            return -ENOMEM;
        }
        dev_node->dev_data = priv_data;
    } else {
        priv_data = (struct bfgx_sle_private_data *)(dev_node->dev_data);
    }

    mutex_init(&priv_data->data_combination.comb_mutex);
    oal_atomic_set(&dev_node->open_cnt, 1);

    mutex_unlock(&(pm_top_data->host_mutex));

    return OAL_SUCC;
}

/*
 * Prototype    : hw_off_find_release
 * Description  : 关机找回节点, 仅释放私有数据, 不对子系统下电。
 */
static int32_t hw_off_find_release(struct inode *inode, struct file *filp)
{
    struct bfgx_dev_node *dev_node = NULL;
    struct bfgx_sle_private_data *priv_data = NULL;
    struct bt_data_combination *data_comb = NULL;
    struct pm_top* pm_top_data = pm_get_top();

    ps_print_info("hw_off_find_release");
    if (g_off_find_en == OAL_FALSE) {
        ps_print_warning("off_find not support!");
        return -EINVAL;
    }

    if (unlikely((filp == NULL) || (filp->private_data == NULL))) {
        ps_print_err("%s param is error", __func__);
        return -EINVAL;
    }

    dev_node = oal_container_of((struct miscdevice *)filp->private_data,
                                struct bfgx_dev_node, c);

    mutex_lock(&(pm_top_data->host_mutex));
    if (oal_atomic_read(&dev_node->open_cnt) != 1) {
        ps_print_err("hwoff_find not opened\n");
        mutex_unlock(&(pm_top_data->host_mutex));
        return -EINVAL;
    }
    priv_data = dev_node->dev_data;
    data_comb = &priv_data->data_combination;

    oal_atomic_set(&dev_node->open_cnt, 0);
    mutex_destroy(&data_comb->comb_mutex);
    os_mem_kfree(dev_node->dev_data);
    dev_node->dev_data = NULL;

    mutex_unlock(&(pm_top_data->host_mutex));
    return OAL_SUCC;
}

static ssize_t hw_off_find_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    if (g_off_find_en == OAL_FALSE) {
        ps_print_warning("off_find not support!");
        return -EINVAL;
    }
    return hw_sle_write(filp, buf, count, f_pos);
}

static ssize_t hw_off_find_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    if (g_off_find_en == OAL_FALSE) {
        ps_print_warning("off_find not support!");
        return -EINVAL;
    }
    return hw_sle_read(filp, buf, count, f_pos);
}

static uint32_t hw_off_find_poll(struct file *filp, poll_table *wait)
{
    if (g_off_find_en == OAL_FALSE) {
        ps_print_warning("off_find not support!");
        return -EINVAL;
    }
    return hw_sle_poll(filp, wait);
}

static long hw_off_find_ioctl(struct file *filp, uint32_t cmd, unsigned long arg)
{
    if (g_off_find_en == OAL_FALSE) {
        ps_print_warning("off_find not support!");
        return -EINVAL;
    }
    return hw_sle_ioctl(filp, cmd, arg);
}

static const struct file_operations g_default_off_find_fops = {
    .owner = THIS_MODULE,
    .open = hw_off_find_open,
    .write = hw_off_find_write,
    .read = hw_off_find_read,
    .poll = hw_off_find_poll,
    .unlocked_ioctl = hw_off_find_ioctl,
    .release = hw_off_find_release,
};

static struct bfgx_subsys_driver_desc g_default_off_find_driver_desc = {
    .file_ops = &g_default_off_find_fops,
    .groups = NULL
};

struct bfgx_subsys_driver_desc *get_off_find_default_drv_desc(void)
{
    return &g_default_off_find_driver_desc;
}

/*
 * Prototype    : test_off_find_case1
 * Description  : 测试函数, 模拟业务场景从off_find节点使能关机找回
 */
void test_off_find_case1(void)
{
    off_find_enable_cmd(bfgx_get_dev_node_by_name(HW_OFF_FIND_DEV_NAME));
    off_find_avoid_leakage_cmd(bfgx_get_dev_node_by_name(HW_OFF_FIND_DEV_NAME));
}

/*
 * Prototype    : test_off_find_case2
 * Description  : 测试函数, 模拟产线场景从BT节点使能关机找回
 */
void test_off_find_case2(void)
{
    struct bfgx_dev_node *dev_node = bfgx_get_dev_node_by_name(HW_BT1_DEV_NAME);

    if (dev_node == NULL || dev_node->core == NULL) {
        ps_print_err("%s dev_node or core is null", __func__);
        return;
    }

    hw_bfgx_open(dev_node->core, BFGX_BT);
    off_find_enable_cmd(dev_node);
    off_find_avoid_leakage_cmd(dev_node);
    off_find_get_time();
}

/*
 * Prototype    : test_off_find_case3
 * Description  : 测试函数, 关机找回enable后再disable
 */
void test_off_find_case3(void)
{
    off_find_enable_cmd(bfgx_get_dev_node_by_name(HW_OFF_FIND_DEV_NAME));
    off_find_disable_cmd(bfgx_get_dev_node_by_name(HW_OFF_FIND_DEV_NAME));
}

MODULE_DESCRIPTION("Public Driver for huawei chips Off find");
MODULE_LICENSE("GPL");
