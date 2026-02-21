#ifndef VCODEC_MCU_HAL_H
#define VCODEC_MCU_HAL_H
#include "vcodec_types.h"

struct vcodec_dev_mcu;

int32_t vcodec_mcu_init_hal(struct vcodec_dev_mcu* dev, uint8_t *reg_base);
void vcodec_mcu_deinit_hal(struct vcodec_dev_mcu* dev);
void vcodec_mcu_hal_show_status(struct vcodec_dev_mcu *dev);
int32_t vcodec_mcu_hal_wait_suspend_done(struct vcodec_dev_mcu *dev);
int32_t vcodec_mcu_hal_poweron(struct vcodec_dev_mcu *dev, uint32_t load_image_flag);
void vcodec_mcu_hal_poweroff(struct vcodec_dev_mcu *dev);

#endif
