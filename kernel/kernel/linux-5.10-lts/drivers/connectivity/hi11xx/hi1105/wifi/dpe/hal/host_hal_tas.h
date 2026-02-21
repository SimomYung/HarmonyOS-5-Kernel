/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL DEVICE
 * 作    者 : wifi
 * 创建日期 : 2020年03月17日
 */


#ifndef HOST_HAL_TAS_H
#define HOST_HAL_TAS_H
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define WIFI_TAS_DISABLE 0
#define WIFI_TAS_ENABLE  1

#define DTS_PROP_MPXX_WIFI_TAS_STATE "hi110x,gpio_wifi_tas_state"
#define DTS_PROP_WIFI_TAS_EN    "hi110x,wifi_tas_enable"
#define PROC_NAME_GPIO_WIFI_TAS "wifi_tas"
#define DTS_PROP_GPIO_WIFI_TAS  "hi110x,gpio_wifi_tas"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
void hal_wifi_tas_init(void);
void hal_wifi_tas_deinit(void);
int32_t hal_wifi_tas_set(int value);
int32_t hal_get_wifi_tas_gpio_state(void);
#else
OAL_STATIC OAL_INLINE void hal_wifi_tas_init(void)
{
    return;
}

OAL_STATIC OAL_INLINE void hal_wifi_tas_deinit(void)
{
    return;
}

OAL_STATIC OAL_INLINE int32_t hal_wifi_tas_set(int value)
{
    return 0;
}

OAL_STATIC OAL_INLINE int32_t hal_get_wifi_tas_gpio_state(void)
{
    return 0;
}
#endif // (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
