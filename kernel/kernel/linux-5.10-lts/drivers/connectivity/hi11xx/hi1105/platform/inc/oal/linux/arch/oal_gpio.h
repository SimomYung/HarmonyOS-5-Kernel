/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: GPIO interface package related to the working platform.
 *              Precompiled macro control is required
 * Author: @CompanyNameTag
 */

#ifndef OAL_LINUX_GPIO_H
#define OAL_LINUX_GPIO_H

OAL_STATIC OAL_INLINE int32_t oal_gpio_is_valid(int32_t i_number)
{
    return gpio_is_valid(i_number);
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_request(uint32_t ul_gpio, const char *pc_label)
{
    return gpio_request(ul_gpio, pc_label);
}

OAL_STATIC OAL_INLINE void oal_gpio_free(uint32_t ul_gpio)
{
    if (ul_gpio) {
        gpio_free(ul_gpio);
    }
}

OAL_STATIC OAL_INLINE int oal_gpio_direction_output(uint32_t ul_gpio, int l_level)
{
#ifdef _PRE_HI_DRV_GPIO
    hitv_gpio_request(ul_gpio, 0);
    return hitv_gpio_direction_output(ul_gpio, l_level);
#else
    return gpio_direction_output(ul_gpio, l_level);
#endif
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_to_irq(uint32_t ul_gpio)
{
    return gpio_to_irq(ul_gpio);
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_get_value(uint32_t ul_gpio)
{
    return gpio_get_value(ul_gpio);
}

OAL_STATIC OAL_INLINE void oal_gpio_set_value(uint32_t ul_gpio, int32_t value)
{
    gpio_set_value(ul_gpio, value);
}

OAL_STATIC OAL_INLINE int32_t oal_gpio_request_one(uint32_t ul_gpio, unsigned long flags, const char *label)
{
    return gpio_request_one(ul_gpio, flags, label);
}

#endif /* end of OAL_LINUX_GPIO_H */
