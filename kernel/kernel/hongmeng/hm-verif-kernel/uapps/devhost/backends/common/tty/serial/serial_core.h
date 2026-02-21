/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of serial device functions
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:42:56 2019
 */

#ifndef SERIAL_CORE_H
#define SERIAL_CORE_H

#include "serial.h"

int uart_register_tty(struct serial_device *serdev, unsigned int options);

void uart_unregister_tty(struct serial_device *serdev);

struct serial_device *serial_device_create(const char *dev_name);

void serial_device_remove(struct serial_device *serdev);

int uart_register_irq(struct serial_device *serdev);

void serial_device_setup(struct serial_device *serdev,
			 const struct tty_backend_operations *bops,
			 const struct uart_ops *uops,
			 int irq,
			 unsigned int input_mode);

#endif
