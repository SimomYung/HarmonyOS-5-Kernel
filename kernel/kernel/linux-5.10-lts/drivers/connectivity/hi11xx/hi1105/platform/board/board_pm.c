/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: gpio board-level implementation
 * Author: @CompanyNameTag
 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/notifier.h>
#include <linux/suspend.h>
#include <linux/pm_wakeup.h>
#endif
#ifdef CONFIG_SYS_PM_ULSR
#include <platform_include/cee/linux/ulsr.h>
#endif
#include "plat_debug.h"
#include "board.h"
#include "board_gpio.h"
#include "board_pm.h"

#define BOARD_SR_NOTIFIER_PRIORITY   1

static LIST_HEAD(g_pm_notify_list);

static pm_notify_param g_board_pm_param;

static int32_t g_hibernate = OAL_FALSE;

/* S4睡眠前检查power on管脚是否拉低状态，芯片要在S4睡眠前下电，否则主芯片GPIO失效，connectivity芯片行为不可预期 */
static int32_t board_hibernate_check(void)
{
    int32_t power_on_s4_state = 0;
    int32_t wl_en_s4_state = 0;
    int32_t bt_en_s4_state = 0;
    int32_t gle_en_s4_state = 0;

    power_on_s4_state = conn_get_gpio_level(CHIP_POWR);
    wl_en_s4_state = conn_get_gpio_level(W_POWER);
    bt_en_s4_state = conn_get_gpio_level(BFGX_POWER);
    gle_en_s4_state = conn_get_gpio_level(G_POWER);
    ps_print_info("CHIP_POWR[%d], W_EN[%d], B_EN[%d], G_EN[%d]\n",
                  power_on_s4_state, wl_en_s4_state, bt_en_s4_state, gle_en_s4_state);
    if (power_on_s4_state == GPIO_LEVEL_HIGH) {
        ps_print_err("should not s4 hibernate when chip power on");
        declare_dft_trace_key_info("s4 hibernate when powe on", OAL_DFT_TRACE_EXCEP);
        /* NOTIFY_BAD才能阻止内核pm流程 */
        return OAL_NOTIFY_BAD;
    }
    return OAL_SUCC;
}

/*
 * Function: board_pm_hibernate
 * Description: s4 hibernate process, release irq & gpio
 * Ruturn: 0 -- success
 */
static int32_t board_pm_hibernate(pm_notify_param* param, void* data)
{
    struct platform_device *pdev = (struct platform_device*)data;
    int32_t ret = OAL_SUCC;

    if (pdev == NULL) {
        ps_print_err("pdev NULL");
        return ret;
    }
    ps_print_info("+\n");
    if (g_hibernate == OAL_TRUE) {
        ps_print_err("already in hibernate state, need not process again");
        return OAL_SUCC;
    }

    conn_irq_save_all();
    conn_board_gpio_remove(pdev);
    g_hibernate = OAL_TRUE;
    ps_print_info("-\n");
    return ret;
}

/*
 * Function: board_pm_restore
 * Description: s4 restore process， reinit gpio & irq
 * Ruturn: 0 -- success
 */
static int32_t board_pm_restore(pm_notify_param* param, void* data)
{
    struct platform_device *pdev = (struct platform_device*)data;

    if (pdev == NULL) {
        ps_print_err("pdev NULL");
        return OAL_SUCC;
    }
    ps_print_info("+\n");
    if (g_hibernate == OAL_FALSE) {
        ps_print_err("NOT in hibernate state, need not restore");
        return OAL_SUCC;
    }
    conn_board_gpio_init(pdev);
    conn_irq_restore_all();
    g_hibernate = OAL_FALSE;
    ps_print_info("-\n");
    return OAL_SUCC;
}

/*
 * Function: board_pm_suspend_proc
 * Description: s3 suspend process
 * Ruturn: 0 -- success
 */
static int32_t board_pm_suspend(pm_notify_param* param, void* data)
{
    int32_t ret = OAL_SUCC;
    ps_print_info("+\n");
#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
    /* 继承收编ARMPC项目处理逻辑, s3和s4一样GPIO都会下电，需要保存恢复 */
    ret = board_pm_hibernate(param, data);
#endif
    ps_print_info("-\n");
    return ret;
}

/*
 * Function: board_pm_resume_proc
 * Description: s3 resume process
 * Ruturn: 0 -- success
 */
static int32_t board_pm_resume(pm_notify_param* param, void* data)
{
    int32_t ret = OAL_SUCC;
    ps_print_info("+\n");
#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
    /* 继承收编ARMPC项目处理逻辑, s3和s4一样GPIO都会下电，需要保存恢复 */
    ret = board_pm_restore(param, data);
#endif
    ps_print_info("-\n");
    return ret;
}

static int board_pm_notify_func(unsigned long event, pm_notify_param* param, void *data)
{
    int32_t ret = OAL_SUCC;
    switch (event) {
        case PM_SUSPEND_PREPARE:
            ret = board_pm_suspend(param, data);
            break;
        case PM_HIBERNATION_PREPARE:
            ret = board_hibernate_check();
            if (ret != OAL_SUCC) {
                return ret;
            }
            ret = board_pm_hibernate(param, data);
            break;
        case PM_POST_SUSPEND:
            ret = board_pm_resume(param, data);
            break;
        case PM_POST_HIBERNATION:
            ret = board_pm_restore(param, data);
            break;
        default:
            break;
    }
    return ret;
}

static pm_notify_node g_board_pm_nb = {
    .name = "board_pm_nb",
    .priority = PLAT_NOITFY_PRIO,
    .cb_func = board_pm_notify_func,
    .data = NULL,
};

/*
 * Prototype    : board_pm_register_notify_cb
 * Description  : mpxx驱动子模块注册PM通知回调处理
 */
void board_pm_register_notify_cb(pm_notify_node* my_cb)
{
    pm_notify_node* node_i = NULL;
    pm_notify_node* node_insert = NULL;

    if (my_cb == NULL) {
        ps_print_err("no mem to allocate\n");
        return ;
    }
    ps_print_info("register pm notify cb %s\n", my_cb->name);
    list_for_each_entry(node_i, &g_pm_notify_list, node) {
        if (my_cb == node_i) {
            ps_print_err("double register %s detected\n", my_cb->name);
            return;
        }

        if (my_cb->priority >= node_i->priority) {
            node_insert = node_i;
            break;
        }
    }
    /* Insert a new entry before the specified head */
    if (node_insert != NULL) {
        list_add_tail(&my_cb->node, &node_insert->node);
    } else {
        list_add_tail(&my_cb->node, &g_pm_notify_list);
    }
}
EXPORT_SYMBOL_GPL(board_pm_register_notify_cb);

/*
 * Prototype    : board_pm_unregister_notify_cb
 * Description  : mpxx驱动子模块去注册PM通知回调处理
 */
void board_pm_unregister_notify_cb(pm_notify_node* my_cb)
{
    list_del(&my_cb->node);
}
EXPORT_SYMBOL_GPL(board_pm_unregister_notify_cb);

/*
 * Prototype    : board_pm_notify_chain_rollback
 * Description  : suspend遇到失败回滚处理：当前失败节点之前的节点回滚, 当前节点应该自行处理失败
 */
static int board_pm_notify_chain_rollback(unsigned long mode, pm_notify_node* node_fail)
{
    pm_notify_node* node_i = NULL;
    unsigned long rollback_mode = (mode == PM_SUSPEND_PREPARE) ? PM_POST_SUSPEND : PM_POST_HIBERNATION;
    uint16_t start = OAL_FALSE;
    int ret;

    if ((mode != PM_SUSPEND_PREPARE) && (mode != PM_HIBERNATION_PREPARE)) {
        return BOARD_SUCC;
    }

    /* 反向找到失败节点，往前回滚 */
    list_for_each_entry_reverse(node_i, &g_pm_notify_list, node) {
        if (start == OAL_TRUE) {
            if (node_i->cb_func != NULL) {
                ps_print_info("rollback call %s, priority %d\n", node_i->name, node_i->priority);
                ret = node_i->cb_func(rollback_mode, &g_board_pm_param, node_i->data);
                if (ret != BOARD_SUCC) {
                    ps_print_err("rollback %s error, ret %d\n", node_i->name, ret);
                }
            }
        }
        /* 找到,下一个节点开始rollback */
        if (node_i == node_fail) {
            start = OAL_TRUE;
            continue;
        }
    }
    return BOARD_SUCC;
}
/*
 * Prototype    : board_pm_notify_chain_call
 * Description  : 子模块注册的PM通知链回调处理
 */
static int board_pm_notify_chain_call(unsigned long mode)
{
    pm_notify_node* node_i = NULL;
    int ret;

#ifdef CONFIG_SYS_PM_ULSR
    /* uslr模式下，connecitvity S3行为等同于S4 */
    if (is_ulsr() == OAL_TRUE) {
        if (mode == PM_SUSPEND_PREPARE) {
            ps_print_info("is_ulsr, treate PM_SUSPEND_PREPARE as PM_HIBERNATION_PREPARE\n");
            mode = PM_HIBERNATION_PREPARE;
        } else if (mode == PM_POST_SUSPEND) {
            ps_print_info("is_ulsr, treate PM_POST_SUSPEND as PM_POST_HIBERNATION\n");
            mode = PM_POST_HIBERNATION;
        }
    }
#endif
    if ((mode == PM_SUSPEND_PREPARE) || (mode == PM_HIBERNATION_PREPARE)) {
        /* suspend正向遍历 */
        list_for_each_entry(node_i, &g_pm_notify_list, node) {
            if (node_i->cb_func != NULL) {
                ps_print_info("suspend call %s, priority %d\n", node_i->name, node_i->priority);
                ret = node_i->cb_func(mode, &g_board_pm_param, node_i->data);
                if (ret != BOARD_SUCC) {
                    ps_print_info("suspend %s return error %d, rollback\n", node_i->name, ret);
                    board_pm_notify_chain_rollback(mode, node_i);
                    return ret;
                }
            }
        }
    } else if ((mode == PM_POST_SUSPEND) || (mode == PM_POST_HIBERNATION)) {
        /* resume方向遍历 */
        list_for_each_entry_reverse(node_i, &g_pm_notify_list, node) {
            if (node_i->cb_func != NULL) {
                ps_print_info("resume call %s, priority %d\n", node_i->name, node_i->priority);
                ret = node_i->cb_func(mode, &g_board_pm_param, node_i->data);
                if (ret != BOARD_SUCC) {
                    // resume链上有失败, 继续进行
                    ps_print_info("resume %s ret error %d\n", node_i->name, ret);
                }
            }
        }
    }

    return BOARD_SUCC;
}

int board_pm_notify_handler(struct notifier_block *notify_block, unsigned long mode, void *unused)
{
    int ret = 0;
    ps_print_info("mode[%lu][%s]\n", mode, PM_NOTIFY_STR);

    ret = board_pm_notify_chain_call(mode);
    if (ret != BOARD_SUCC) {
        /* NOTIFY_BAD才能阻止内核pm流程 */
        ps_print_info("mode[%lu] return [0x%x]\n", mode, OAL_NOTIFY_BAD);
        return OAL_NOTIFY_BAD;
    }
    return 0;
}

static struct notifier_block g_board_sr_notifier = {
    .notifier_call = board_pm_notify_handler,
    .priority = BOARD_SR_NOTIFIER_PRIORITY,
};

int board_pm_notifier_register(void)
{
    int32_t ret;
    int32_t cfg_value = 0;

    /* 获取ini的强制睡眠配置 */
    ret = get_cust_conf_int32(INI_MODU_PLAT, "force_sleep", &cfg_value);
    if (ret == INI_FAILED) {
        ps_print_info("fail to get force_sleep ini, keep disable\n");
    }
    g_board_pm_param.force_sleep = (uint32_t)cfg_value;
    ps_print_info("force_sleep support[%d]\n", g_board_pm_param.force_sleep);

    ret = register_pm_notifier(&g_board_sr_notifier);
    if (ret < 0) {
        ps_print_warning("register pm_notifier failed, ret = %d!\n", ret);
        return ret;
    }
    /* 注册首个节点，处理board公共资源：gpio & irq */
    g_board_pm_nb.data = (void*)(get_hi110x_board_info()->dev);
    board_pm_register_notify_cb(&g_board_pm_nb);

    return BOARD_SUCC;
}

uint32_t board_pm_force_sleep_get(void)
{
    return g_board_pm_param.force_sleep;
}

int board_pm_notifier_unregister(void)
{
    int ret;
    board_pm_unregister_notify_cb(&g_board_pm_nb);
    ret = unregister_pm_notifier(&g_board_sr_notifier);
    if (ret < 0) {
        ps_print_warning("unregister pm_notifier failed, ret = %d!\n", ret);
    }
    return ret;
}

#ifdef PM_NOTIFY_TEST
int test_notify_func(unsigned long event, pm_notify_param* param, void *data)
{
    return BOARD_SUCC;
}

pm_notify_node g_test_node1 = {
    .name = "test_node1",
    .priority = 3,
    .cb_func = test_notify_func,
};

pm_notify_node g_test_node2 = {
    .name = "test_node2",
    .priority = 4,
    .cb_func = test_notify_func,
};

pm_notify_node g_test_node3 = {
    .name = "test_node3",
    .priority = 2,
    .cb_func = test_notify_func,
};

void board_pm_register_notify_test(void)
{
    board_pm_register_notify_cb(&g_test_node1);
    board_pm_register_notify_cb(&g_test_node2);
    board_pm_register_notify_cb(&g_test_node3);
    board_pm_register_notify_cb(&g_test_node3);
}
#endif

