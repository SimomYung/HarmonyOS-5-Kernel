/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Init of dw8250 serial device
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 07 17:13:18 2019
 */

#include <udk/types.h>
#include <udk/driver.h>

#include "serial_8250.h"
#include "serial_core.h"
#include "../tty_wrap.h"

#define DW8250_DEV_NAME "uart"
#define DW8250_DRV_NAME "udk_serial_8250"
#define DW8250_DRV_COMPAT "user,snps,dw-apb-uart"
#define DTS_INPUT_MODE_PROP "input-mode"
#define DTS_FIFO_ENABLE_PROP "fifo-enabled"
#define DTS_CLOCK_FREQUENCY "clock_frequency"

#define SERIAL_8250_TTY_OPTION TTY_OPTION_DEFAULT

/* g_dw8250_dev is initialized when probe, and freed when removed */
static struct serial_device *g_dw8250_dev = NULL;

static const struct uart_ops g_uops __read_mostly = {
	.rx_empty = dw8250_rx_empty,
	.getc = dw8250_serial_getc,
	.handle_error = dw8250_handle_error,
	.interrupt_state = dw8250_interrupt_state,
};

static const struct tty_backend_operations g_bops = {
	.putc = dw8250_serial_putc,
	.flush = dw8250_serial_flush,
	.stop_tx = dw8250_serial_stop_tx,
	.start_tx = dw8250_serial_start_tx,
	.stop_rx = dw8250_serial_stop_rx,
	.start_rx = dw8250_serial_start_rx,
#ifdef CONFIG_PLAT_SD62XX
	.set_speed = NULL,
	.get_speed = NULL,
#else
	.set_speed = dw8250_serial_set_speed,
	.get_speed = dw8250_serial_get_speed,
#endif
	.set_termios = dw8250_serial_set_termios,
	.get_termios = dw8250_serial_get_termios,
	.ioctl = NULL,
	.poll_notify = NULL,
	.get_icount = dw8250_serial_get_icount,
	.break_ctl = dw8250_serial_break_ctl,
};

static int dw8250_device_set_base_addr(struct serial_device *serdev,
				       const struct udk_device *udk_dev)
{
	unsigned long long mem_base;
	size_t mem_size;
	struct udk_resource *res = NULL;
	void *base_addr = NULL;

	res = udk_device_get_resource(udk_dev, IORESOURCE_REG, 0);
	if (res == NULL) {
		tty_err("tty: fail to get mem base by parsing dts\n");
		return -EIO;
	}
	mem_base = res->addr;
	mem_size = res->size;

	/* ioremmap dts mem_base to dev_base*/
	base_addr = udk_ioremap(mem_base, mem_size);
	if (base_addr == NULL) {
		tty_err("tty: fail to ioremmap uart, membase=%llx\n", mem_base);
		return -EIO;
	}
	serdev->mem_base = base_addr;
	serdev->mem_size = mem_size;
	return 0;
}

static void dw8250_device_free_base_addr(struct serial_device *serdev)
{
	if (serdev == NULL || serdev->mem_base == NULL) {
		return;
	}

	udk_iounmap(serdev->mem_base, serdev->mem_size);
	serdev->mem_base = NULL;
	serdev->mem_size = 0;
}

/* the init_io_state parameter is an output parameter */
static int dw8250_device_init(struct udk_device *udk_dev,
			      struct serial_device *serdev, u32 *init_io_state)
{
	int err;
	int irq;
	unsigned int input_mode;
	u32 clk_frequency;

	if (udk_dev == NULL || serdev == NULL) {
		return -EIO;
	}

	/* init uart_clk in serdev according to configuration in dts */
	err = udk_device_prop_read_u32(udk_dev, DTS_CLOCK_FREQUENCY, &clk_frequency);
	if (err < 0) {
		tty_info("tty: serial parse clock frequency failed, set it as 0\n");
		serdev->uart_clk = 0;
	} else {
		serdev->uart_clk = clk_frequency;
	}

	/* Read the serial port initial enable config from dts. */
	err = udk_device_prop_read_u32(udk_dev, "flow_state", init_io_state);
	if (err < 0) {
		/* The input and output of the serial are enabled by default. */
		*init_io_state = UART_OUTPUT_STATE_ON | UART_INPUT_STATE_ON;
	}

	/* search fifo-enabled in dts, enable fifo if found */
	if (udk_device_prop_read_bool(udk_dev, DTS_FIFO_ENABLE_PROP)) {
		serdev->fifo_enabled = UART_FIFO_ENABLED;
	} else {
		serdev->fifo_enabled = UART_FIFO_DISABLED;
	}

	/* ioremmap registers to base addr */
	err = dw8250_device_set_base_addr(serdev, udk_dev);
	if (err < 0) {
		tty_err("tty: serial: Set base addr failed, err: %s\n", strerror(-err));
		return -EIO;
	}

	irq = udk_device_get_irq(udk_dev, 0);
	err = udk_device_prop_read_u32(udk_dev, DTS_INPUT_MODE_PROP, &input_mode);
	if (err < 0) {
		input_mode = UART_INPUT_MODE_NONE;
	}

	serdev->output_disabled = false; /* default output is on */
	serial_device_setup(serdev, &g_bops, &g_uops, irq, input_mode);
	return 0;
}

static int dw8250_uart_probe(void *ctx, struct device_info *devinfo)
{
	int err;
	struct serial_device *serdev = NULL;
	struct udk_device *udk_dev = NULL;
	u32 init_io_state;
	u32 dw8250_option = SERIAL_8250_TTY_OPTION;

	tty_info("tty: serial probe dw8250 serial driver\n");
	UNUSED(ctx);
	udk_dev = to_udk_device(devinfo);
	if (udk_dev == NULL) {
		return -EIO;
	}

	/* alloc and init for serial_device */
	serdev = serial_device_create(DW8250_DEV_NAME);
	if (serdev == NULL) {
		return -ENOMEM;
	}
	err = dw8250_device_init(udk_dev, serdev, &init_io_state);
	if (err < 0) {
		tty_warn("dw8250 device init failed with io state: %u\n", init_io_state);
		serial_device_remove(serdev);
		return err;
	}

	err = uart_register_tty(serdev, dw8250_option);
	if (err < 0) {
		dw8250_device_free_base_addr(serdev);
		serial_device_remove(serdev);
		return err;
	}

	/* uart register irq*/
	err = uart_register_irq(serdev);
	if (err < 0) {
		tty_err("tty: uart register irq failed, err: %s\n", strerror(-err));
		uart_unregister_tty(serdev);
		dw8250_device_free_base_addr(serdev);
		serial_device_remove(serdev);
		return err;
	}

	g_dw8250_dev = serdev;
	return 0;
}

static const struct hm_driver_ops g_driver_ops = {
	.bind = dw8250_uart_probe,
	.create = NULL,
	.owner = NULL,
};

static int serial_init(void)
{
	int err;

	tty_info("tty: init dw8250 serial driver\n");
	err = udk_register_driver(DW8250_DRV_NAME, DW8250_DRV_COMPAT, strlen(DW8250_DRV_COMPAT) + 1,
				  (struct hm_driver_ops *)&g_driver_ops,
				  NULL);
	if (err < 0) {
		tty_err("tty: fail to register driver, err: %s\n", strerror(-err));
		return err;
	}
	tty_info("tty: end of init dw8250 serial driver\n");
	return 0;
}

static int serial_exit(void)
{
	if (g_dw8250_dev == NULL) {
		return -EPERM;
	}
	tty_info("tty: exit dw8250 serial driver\n");
	uart_unregister_tty(g_dw8250_dev);
	dw8250_device_free_base_addr(g_dw8250_dev);
	serial_device_remove(g_dw8250_dev);
	g_dw8250_dev = NULL;
	return 0;
}

udk_module_init(serial_init);
udk_module_exit(serial_exit);
udk_module_compat(DW8250_DRV_COMPAT);
