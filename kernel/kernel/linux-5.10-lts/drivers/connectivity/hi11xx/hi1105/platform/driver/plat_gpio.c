/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: GPIO abstraction layer encapsulation
 * Author: @CompanyNameTag
 */

#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>

#include "oal_ext_if.h"
#include "board.h"
#include "plat_debug.h"
#include "plat_gpio.h"

#define MAP_DEFAULT_VALUE 0xFFFFFFFF

struct plat_gpio_info {
    uint32_t gpio_num;
    uint32_t inited;
    int32_t *search_map;
    struct conn_gpio_node *node;
    uint32_t node_num;
    struct conn_gpio_ops *pub_ops;
    struct usr_gpio_ops *usr_ops;
    void *dev;
};

STATIC struct plat_gpio_info g_conn_gpio;

void* conn_gpio_dev_get(void)
{
    return g_conn_gpio.dev;
}

#ifdef _PRE_CONFIG_USE_DTS
STATIC int32_t get_gpio_nr_from_node(struct conn_gpio_node *conn_node, const char *dts_node,
                                     const char *gpio_prop, int32_t *gpio_nr)
{
    int32_t ret;
    struct device_node *np = NULL;
    uint32_t cfg_mode = conn_node->attr.mode;

    ret = get_board_dts_node(&np, dts_node);
    if (ret != OAL_SUCC) {
        return -OAL_FAIL;
    }

    if ((cfg_mode & MODE_DTS) == MODE_DTS) {
        ret = of_get_named_gpio(np, gpio_prop, 0);
        if (ret >= 0) {
            *gpio_nr = ret;
            ps_print_suc("gpio prop value: %s=%d\n", gpio_prop, ret);
            return BOARD_SUCC;
        }
    }

    if ((cfg_mode & MODE_PMU_IO) == MODE_PMU_IO) {
        ret = of_property_read_u32(np, gpio_prop, gpio_nr);
        if (ret == BOARD_SUCC) {
            conn_node->pin.init.req_sts = REQ_ONLY;
            ps_print_suc("gpio prop value: %s=%d\n", gpio_prop, *gpio_nr);
            return BOARD_SUCC;
        }
    }
    ps_print_warning("get dts_prop=%s fail,ret=%d,is_key_gpio=%u\n", gpio_prop, ret, conn_node->pin.is_key_gpio);
    return BOARD_FAIL;
}

int32_t conn_get_gpio_nr_from_dts(struct conn_gpio_node *gpio_node)
{
    int32_t ret;
    int32_t gpio_nr;
    struct conn_gpio_dts_attr *dts = NULL;

    dts = &gpio_node->attr.u.dts;
    if (dts->compatibale == NULL || dts->label_name == NULL) {
        return -OAL_FAIL;
    }

    ret = get_gpio_nr_from_node(gpio_node, dts->compatibale, dts->label_name, &gpio_nr);
    if (ret != BOARD_SUCC) {
        ps_print_info("gpio %s from dts failed, ret = %d\n", dts->label_name, ret);
        return -OAL_FAIL;
    }

    gpio_node->pin.number = gpio_nr;
    gpio_node->pin.main_type = SOC_PIN;
    return OAL_SUCC;
}
#endif

STATIC uint32_t is_conn_key_pin(struct conn_gpio_node *gpio_node)
{
#ifdef _PRE_CONFIG_USE_DTS
    struct device_node *np = NULL;
    struct conn_gpio_dts_attr *dts = NULL;

    if ((gpio_node->attr.mode & MODE_DTS) == MODE_DTS) {
        dts = &gpio_node->attr.u.dts;
        if (dts->compatibale == NULL || dts->label_name == NULL) {
            goto label_node_is_key;
        }

        np = of_find_compatible_node(NULL, NULL, dts->compatibale);
        if (np == NULL) {
            goto label_node_is_key;
        }

        // 如果该属性没有配置, 不作为关键key
        if (of_find_property(np, dts->label_name, NULL) == NULL) {
            return OAL_FALSE;
        }
    }

label_node_is_key:
#endif
    if (gpio_node->pin.is_key_gpio == OAL_TRUE) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}

int32_t conn_gpio_request(struct conn_gpio_node *gpio_node)
{
    int32_t ret;
    struct conn_gpio_pin_info *pin_info;

    pin_info = &gpio_node->pin;
    if (pin_info->init.req_sts == REQ_ONE) {
        ret = gpio_request_one(pin_info->number, pin_info->init.flags, pin_info->name);
        if (ret != OAL_SUCC) {
            ps_print_err("gpio request one [%s-%d] fail ret %d\n",
                         pin_info->name, pin_info->number, ret);
            return ret;
        }
        pin_info->requested = OAL_TRUE;
    } else if (pin_info->init.req_sts == REQ_ONLY) {
        ret = gpio_request(pin_info->number, pin_info->name);
        if (ret != OAL_SUCC) {
            ps_print_err("gpio request[%s-%d] fail ret %d\n",
                         pin_info->name, pin_info->number, ret);
            return ret;
        }
        pin_info->requested = OAL_TRUE;
    }

    return OAL_SUCC;
}

void conn_gpio_unrequest(struct conn_gpio_node *node)
{
    struct conn_gpio_pin_info *pin_info = NULL;
    pin_info = &node->pin;

    if (pin_info->requested != OAL_TRUE) {
        return;
    }

    if (pin_info->init.req_sts == REQ_ONE || pin_info->init.req_sts == REQ_ONLY) {
        ps_print_info("gpio_free type[%d],number[%d]\n", pin_info->type, pin_info->number);
        gpio_free(pin_info->number);
    }
}

STATIC void gpio_uninit(void)
{
    int32_t i;

    for (i = (int32_t)g_conn_gpio.gpio_num - 1; i >= 0; i--) {
        if (g_conn_gpio.node[i].ops != NULL && g_conn_gpio.node[i].ops->gpio_uninit != NULL) {
            g_conn_gpio.node[i].ops->gpio_uninit(&g_conn_gpio.node[i]);
        } else {
            conn_gpio_unrequest(&g_conn_gpio.node[i]);
        }
    }
}

STATIC int32_t gpio_init(struct conn_gpio_node *gpio_node)
{
    int32_t ret;

    if (gpio_node->ops != NULL && gpio_node->ops->gpio_init != NULL) {
        ret = gpio_node->ops->gpio_init(gpio_node);
        if (ret != OAL_SUCC) {
            ps_print_err("gpio ops cust init fail ret %d\n", ret);
        }
        return ret;
    }
#ifdef _PRE_CONFIG_USE_DTS
    if (((gpio_node->attr.mode & MODE_DTS) == MODE_DTS) ||
        ((gpio_node->attr.mode & MODE_PMU_IO) == MODE_PMU_IO)) {
        if (conn_get_gpio_nr_from_dts(gpio_node) < 0) {
            return -OAL_FAIL;
        }
    }
#endif
    ret = conn_gpio_request(gpio_node);
    if (ret < 0) {
        return ret;
    }

    return OAL_SUCC;
}

int32_t conn_gpio_init(struct conn_gpio_chip *chip)
{
    uint32_t i;
    int32_t ret;

    if (chip->node == NULL || chip->node_num == 0) {
        return -OAL_FAIL;
    }

    g_conn_gpio.node = chip->node;
    g_conn_gpio.node_num = chip->node_num;
    g_conn_gpio.pub_ops = chip->pub_ops;
    g_conn_gpio.usr_ops = chip->usr_ops;
    g_conn_gpio.dev = chip->dev;
    g_conn_gpio.search_map = (int32_t *)kmalloc(sizeof(int32_t) * GPIO_TYPE_BUFF, GFP_KERNEL);
    if (g_conn_gpio.search_map == NULL) {
        return -OAL_FAIL;
    }

    memset_s(g_conn_gpio.search_map, sizeof(int32_t) * GPIO_TYPE_BUFF, MAP_DEFAULT_VALUE,
             sizeof(int32_t) * GPIO_TYPE_BUFF);
    for (i = 0; i < g_conn_gpio.node_num; i++) {
        ret = gpio_init(&g_conn_gpio.node[i]);
        if (ret < 0) {
            if (is_conn_key_pin(&g_conn_gpio.node[i]) == OAL_TRUE) {
                gpio_uninit();
                kfree(g_conn_gpio.search_map);
                return ret;
            }
            continue;
        }
        ps_print_info("pin type[%d] = number[%d]\n", g_conn_gpio.node[i].pin.type, g_conn_gpio.node[i].pin.number);
        if (g_conn_gpio.search_map[g_conn_gpio.node[i].pin.type] != MAP_DEFAULT_VALUE) {
            ps_print_err("pin type[%d] already register\n", g_conn_gpio.node[i].pin.type);
            gpio_uninit();
            kfree(g_conn_gpio.search_map);
            return -OAL_FAIL;
        }
        /* GPIO ops不指定，默认继承自pub_ops, pub_ops不指定，则采用默认操作接口 */
        if (g_conn_gpio.node[i].ops == NULL) {
            g_conn_gpio.node[i].ops = g_conn_gpio.pub_ops;
        }
        g_conn_gpio.search_map[g_conn_gpio.node[i].pin.type] = (int32_t)i;
        g_conn_gpio.gpio_num++;
    }

    g_conn_gpio.inited = OAL_TRUE;
    return OAL_SUCC;
}

void conn_gpio_remove(void)
{
    if (g_conn_gpio.inited != OAL_TRUE) {
        return;
    }
    g_conn_gpio.inited = OAL_FALSE;
    gpio_uninit();
    kfree(g_conn_gpio.search_map);
    g_conn_gpio.search_map = NULL;
    memset_s(&g_conn_gpio, sizeof(g_conn_gpio), 0, sizeof(g_conn_gpio));
}

STATIC struct conn_gpio_node* get_spec_from_type(enum gpio_type type)
{
    int32_t index;
    struct conn_gpio_node *node = g_conn_gpio.node;

    if (type >= GPIO_TYPE_BUFF) {
        return NULL;
    }
    if (g_conn_gpio.search_map == NULL) {
        return NULL;
    }
    index = g_conn_gpio.search_map[type];
    if (index == MAP_DEFAULT_VALUE) {
        return NULL;
    }

    return &node[index];
}

STATIC void gpio_to_irq_init(struct conn_gpio_node *gpio_node)
{
    struct conn_gpio_irq_attr *irq_attr = NULL;
    struct conn_gpio_pin_info *pin_info = NULL;

    pin_info = &gpio_node->pin;
    irq_attr = &gpio_node->irq;
    if (irq_attr->name == NULL) {
        return;
    }

    if (gpio_node->ops != NULL && gpio_node->ops->get_irq_num != NULL) {
        irq_attr->number = (uint32_t)gpio_node->ops->get_irq_num(pin_info->number);
    } else {
        irq_attr->number = (uint32_t)gpio_to_irq(pin_info->number);
    }
}

OAL_STATIC void gpio_intr_enable(struct conn_gpio_irq_attr *irq_attr, uint32_t en)
{
    oal_uint flags;

    if (irq_attr->number == 0 || irq_attr->inited == OAL_FALSE) {
        ps_print_warning("irq [%s] not initialize", irq_attr->name);
        return;
    }

    oal_spin_lock_irq_save(&irq_attr->lock, &flags);
    if (en == OAL_TRUE) {
        /* 避免中断多次使能 */
        if (irq_attr->is_enabled == OAL_FALSE) {
            enable_irq(irq_attr->number);
            oal_print_mpxx_log(MPXX_LOG_DBG, "gpio enable irq: %s[nr %u] enable",
                               irq_attr->name, irq_attr->number);
            irq_attr->is_enabled = OAL_TRUE;
        }
    } else {
        if (irq_attr->is_enabled == OAL_TRUE) {
            disable_irq_nosync(irq_attr->number);
            oal_print_mpxx_log(MPXX_LOG_DBG, "gpio nosync irq: %s[nr %u] disable",
                               irq_attr->name, irq_attr->number);
            irq_attr->is_enabled = OAL_FALSE;
        }
    }
    oal_spin_unlock_irq_restore(&irq_attr->lock, &flags);
}

void conn_gpio_intr_enable(enum gpio_type type, uint32_t en)
{
    struct conn_gpio_node *gpio_node = NULL;
    struct conn_gpio_irq_attr *irq_attr = NULL;

    gpio_node = get_spec_from_type(type);
    if (gpio_node == NULL) {
        return;
    }

    irq_attr = &gpio_node->irq;

    if (gpio_node->ops != NULL && gpio_node->ops->irq_enable != NULL) {
        gpio_node->ops->irq_enable(irq_attr, en);
    } else {
        gpio_intr_enable(irq_attr, en);
    }
}

OAL_STATIC int32_t irq_register(struct conn_gpio_node *gpio_node, oal_irq_handler_t handler, void *dev)
{
    int32_t ret;
    struct conn_gpio_irq_attr *irq_attr = NULL;

    irq_attr = &gpio_node->irq;
    if (irq_attr->number == 0) {
        ps_print_warning("gpio type [%d] irq not initialize", gpio_node->pin.type);
        return -OAL_FAIL;
    }

    ret = request_irq(irq_attr->number, handler, irq_attr->flags,
                      irq_attr->name, dev);
    if (ret < 0) {
        ps_print_err("gpio index [%d] irq request fail ret = %d\n", gpio_node->pin.number, ret);
        return -OAL_FAIL;
    }

    oal_spin_lock_init(&irq_attr->lock);
    if (irq_attr->init_set_enable == OAL_TRUE) {
        irq_attr->is_enabled = OAL_TRUE;
    } else {
        irq_attr->is_enabled = OAL_FALSE;
        disable_irq_nosync(irq_attr->number);
    }

    irq_attr->inited = OAL_TRUE;
    ps_print_info("success to register %s intr, irq_num[%u]", irq_attr->name, irq_attr->number);
    return OAL_SUCC;
}


int32_t conn_irq_register(enum gpio_type type, oal_irq_handler_t handler, void *dev)
{
    struct conn_gpio_node *gpio_node = NULL;
    int32_t ret;

    gpio_node = get_spec_from_type(type);
    if (gpio_node == NULL) {
        ps_print_warning("gpio type %d not find", type);
        return -OAL_FAIL;
    }

    gpio_to_irq_init(gpio_node);

    if (gpio_node->ops != NULL && gpio_node->ops->irq_register != NULL) {
        return gpio_node->ops->irq_register(gpio_node, handler, dev);
    } else {
        ret = irq_register(gpio_node, handler, dev);
    }

    if (ret == OAL_SUCC) {
        gpio_node->irq.handler = handler;
        gpio_node->irq.dev = dev;
    }
    return ret;
}

/* 重新申请注册所有的GPIO中断 */
void conn_irq_restore_all(void)
{
    uint32_t i;
    struct conn_gpio_node *gpio_node = NULL;
    struct conn_gpio_irq_attr *irq_attr = NULL;

    for (i = 0; i < g_conn_gpio.node_num; i++) {
        gpio_node = &g_conn_gpio.node[i];
        irq_attr = &gpio_node->irq;
        if (irq_attr->name == NULL) {
            continue;
        }
        /* 使用初始化注册时记录的handler和dev重新注册中断回调 */
        if (irq_attr->handler != NULL) {
            if (conn_irq_register(gpio_node->pin.type, irq_attr->handler, irq_attr->dev) != OAL_SUCC) {
                ps_print_err("gpio type %d register irq fail", gpio_node->pin.type);
            }
        }
    }
}

STATIC void irq_unregister(struct conn_gpio_node *gpio_node, void *dev)
{
    struct conn_gpio_irq_attr *irq_attr = NULL;

    irq_attr = &gpio_node->irq;
    if (irq_attr->number == 0) {
        ps_print_warning("gpio index [%d] irq not initialize", gpio_node->pin.number);
        return;
    }

    gpio_intr_enable(irq_attr, OAL_FALSE);
    free_irq(irq_attr->number, dev);
    irq_attr->inited = OAL_FALSE;
}

void conn_irq_unregister(enum gpio_type type, void *dev)
{
    struct conn_gpio_node *gpio_node = NULL;

    gpio_node = get_spec_from_type(type);
    if (gpio_node == NULL) {
        ps_print_warning("gpio type %d not find", type);
        return;
    }

    if (gpio_node->ops != NULL && gpio_node->ops->irq_unregister != NULL) {
        gpio_node->ops->irq_unregister(gpio_node, dev);
    } else {
        irq_unregister(gpio_node, dev);
    }
}

/* 释放并保存所有的GPIO中断 */
void conn_irq_save_all(void)
{
    uint32_t i;
    struct conn_gpio_node *gpio_node;
    struct conn_gpio_irq_attr *irq_attr;

    for (i = 0; i < g_conn_gpio.node_num; i++) {
        gpio_node = &g_conn_gpio.node[i];
        irq_attr = &gpio_node->irq;
        if (irq_attr->number != 0 && irq_attr->inited == OAL_TRUE) {
            ps_print_info("irq %s unregister %u", irq_attr->name, irq_attr->number);
            conn_irq_unregister(gpio_node->pin.type, irq_attr->dev);
        }
    }
}

int32_t default_set_gpio_level(struct conn_gpio_node *gpio_node, enum gpio_level level)
{
    struct conn_gpio_pin_info *pin_info = NULL;
    enum gpio_level level_set = level;

    pin_info = &gpio_node->pin;
    if (pin_info->invert == OAL_TRUE) {
        level_set = (level == GPIO_LEVEL_LOW) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;
    }

    return gpio_direction_output(pin_info->number, level_set);
}

int32_t conn_set_gpio_level(enum gpio_type type, enum gpio_level level)
{
    struct conn_gpio_node *gpio_node = NULL;

    gpio_node = get_spec_from_type(type);
    if (gpio_node == NULL) {
        ps_print_warning("gpio type %d not find", type);
        return -OAL_FAIL;
    }

    if (gpio_node->ops != NULL && gpio_node->ops->set_gpio_level != NULL) {
        return gpio_node->ops->set_gpio_level(gpio_node, level);
    } else {
        return default_set_gpio_level(gpio_node, level);
    }
}

int32_t conn_set_gpio_input(enum gpio_type type)
{
    struct conn_gpio_node *gpio_node = NULL;

    gpio_node = get_spec_from_type(type);
    if (gpio_node == NULL) {
        ps_print_warning("gpio type %d not find", type);
        return -OAL_FAIL;
    }

    if (gpio_node->ops != NULL && gpio_node->ops->set_gpio_input != NULL) {
        return gpio_node->ops->set_gpio_input(gpio_node);
    } else {
        return gpio_direction_input(gpio_node->pin.number);
    }
}

int32_t default_get_gpio_level(struct conn_gpio_node *gpio_node)
{
    int32_t level;
    struct conn_gpio_pin_info *pin_info = NULL;
    pin_info = &gpio_node->pin;
    level = gpio_get_value(pin_info->number);
    if (level < 0) {
        return level;
    }

    if (pin_info->invert == OAL_TRUE) {
        level = !level;
    }

    return level;
}

int32_t conn_get_gpio_level(enum gpio_type type)
{
    struct conn_gpio_node *gpio_node = NULL;

    gpio_node = get_spec_from_type(type);
    if (gpio_node == NULL) {
        ps_print_warning("gpio type %d not find", type);
        return -OAL_FAIL;
    }

    if (gpio_node->ops != NULL && gpio_node->ops->get_gpio_level != NULL) {
        return gpio_node->ops->get_gpio_level(gpio_node);
    } else {
        return default_get_gpio_level(gpio_node);
    }
}

int32_t conn_get_gpio_type_by_number(int32_t gpio_nr)
{
    uint32_t i;
    struct conn_gpio_node *node = g_conn_gpio.node;
    struct conn_gpio_pin_info *pin_info = NULL;

    for (i = 0; i < g_conn_gpio.node_num; i++) {
        pin_info = &node[i].pin;
        if (pin_info->number != gpio_nr) {
            continue;
        }

        if (get_spec_from_type(pin_info->type) == NULL) {
            ps_print_warning("gpio type %d, number %d not register", pin_info->type, pin_info->number);
            return -OAL_FAIL;
        }

        return pin_info->type;
    }

    return -OAL_FAIL;
}

int32_t conn_get_gpio_number_by_type(enum gpio_type type)
{
    struct conn_gpio_node *node = NULL;
    struct conn_gpio_pin_info *pin_info = NULL;

    node = get_spec_from_type(type);
    if (node != NULL) {
        pin_info = &node->pin;
        return pin_info->number;
    }

    return -OAL_FAIL;
}

int32_t conn_gpio_valid(enum gpio_type type)
{
    if (get_spec_from_type(type) == NULL) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

void conn_dump_info(enum dump_type dump_type)
{
}

int32_t conn_user_gpio_config(uint8_t OAL_USER *data)
{
    int32_t ret;
    uint32_t data_len;
    struct conn_gpio_userdata gpio_user;
    struct conn_gpio_userdata *ptr = NULL;

    if (g_conn_gpio.usr_ops == NULL || g_conn_gpio.usr_ops->user_set == NULL) {
        return -OAL_FAIL;
    }

    if (copy_from_user(&gpio_user, data, sizeof(gpio_user))) {
        ps_print_err("get data head from user fail\n");
        return -OAL_FAIL;
    }

    if (gpio_user.data_len > 0) {
        // 限制大小，避免后续大内存申请或者bit翻转
        if (gpio_user.data_len > SZ_4K) {
            return -OAL_FAIL;
        }

        data_len = sizeof(struct conn_gpio_userdata) + gpio_user.data_len;
        ptr = (struct conn_gpio_userdata *)kmalloc(data_len, GFP_KERNEL);
        if (ptr == NULL) {
            return -ENOMEM;
        }

        if (copy_from_user(ptr, data, data_len)) {
            kfree(ptr);
            ps_print_err("get data payload from user fail, len %u\n", data_len);
            return -OAL_FAIL;
        }

        ret = g_conn_gpio.usr_ops->user_set(ptr, data);
        kfree(ptr);
    } else {
        ret = g_conn_gpio.usr_ops->user_set(&gpio_user, data);
    }

    return ret;
}