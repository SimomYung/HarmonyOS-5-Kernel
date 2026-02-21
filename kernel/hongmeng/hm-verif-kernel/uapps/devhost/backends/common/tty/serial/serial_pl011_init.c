/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Init of pl011 serial device
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 11 13:24:18 2019
 */

#include <linux/of.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "serial_pl011.h"
#include "serial_core.h"
#include "../tty_wrap.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peitao Shi");
MODULE_DESCRIPTION("simple linux uart driver for pl011");
MODULE_VERSION("0.01");

#define UART_DEV_NAME "uart"
#define PL011_COMPATIBLE "arm,pl011,uart"
#define PL011_DEV_NAME UART_DEV_NAME
#define PL011_DRV_NAME "serial_pl011"

#define PL011_UART_REG_SIZE 0x1000

#define SERIAL_PL011_TTY_OPTION TTY_OPTION_DEFAULT

/* g_pl011_dev is initialized when probe, and freed when removed */
static struct serial_device *g_pl011_dev = NULL;

static const struct uart_ops g_uops = {
	.rx_empty = pl011_rx_fifo_empty,
	.getc = pl011_serial_getc,
	.handle_error = pl011_handle_error,
	.interrupt_state = pl011_interrupt_state,
};

static const struct tty_backend_operations g_bops = {
	.putc = pl011_serial_putc,
	.flush = pl011_serial_flush,
	.set_speed = pl011_set_speed,
	.get_speed = pl011_get_speed,
	.stop_tx = pl011_serial_stop_tx,
	.start_tx = pl011_serial_start_tx,
	.stop_rx = pl011_serial_stop_rx,
	.start_rx = pl011_serial_start_rx,
	.get_icount = pl011_serial_get_icount,
	.break_ctl = pl011_serial_break_ctl,
};

static int pl011_device_init(struct serial_device *serdev,
			     struct platform_device *pdev)
{
	unsigned int uart_clock = 0;
	unsigned int input_mode;
	int irq = 0;
	int err;

	/* parse dts to get some basic info */
	err = of_property_read_u32(pdev->dev.of_node, "input-mode",
				   &input_mode);
	if (err < 0) {
		input_mode = UART_INPUT_MODE_NONE;
	}
	if (input_mode == UART_INPUT_MODE_IRQ) {
		irq = platform_get_irq(pdev, 0);
		if (irq < 0) {
			tty_warn("tty: get irq failed\n");
			return -EIO;
		}
	}
	/* if enable fifo in dts */
	if (of_property_read_bool(pdev->dev.of_node, "fifo-enabled")) {
		serdev->fifo_enabled = UART_FIFO_ENABLED;
	} else {
		serdev->fifo_enabled = UART_FIFO_DISABLED;
	}
	tty_info("tty: input mode: %d, output mode: %d, fifo enabled: %d\n",
		 input_mode, serdev->output_mode, serdev->fifo_enabled);
	err = of_property_read_u32(pdev->dev.of_node, "output-mode",
				   &serdev->output_mode);
	if (err < 0) {
		serdev->output_mode = UART_OUTPUT_MODE_SYNC;
	}

	/* ioremmap registers to base addr */
	serdev->mem_base = of_iomap(pdev->dev.of_node, 0);
	if (serdev->mem_base == NULL) {
		return -ENXIO;
	}

	/* achieve uart clock on platform from its dts now */
	err = of_property_read_u32(pdev->dev.of_node, "clock_frequency", &uart_clock);
	if (err == 0) {
		serdev->uart_clk = uart_clock;
	} else {
		tty_info("tty: this platform cannot change baudrate uart_clock:%u\n", uart_clock);
	}

	/* set basic info to serdev */
	serial_device_setup(serdev, &g_bops, &g_uops, irq, input_mode);
	return 0;
}

static struct of_device_id pl011_dt_ids[] = {
	{ .compatible = PL011_COMPATIBLE, .name = PL011_DEV_NAME, },
	{ }, /* sentinel */
};

static int pl011_uart_probe(struct platform_device *pdev)
{
	int err;
	struct serial_device *serdev = NULL;

	tty_info("tty: probe PL011 serial driver\n");
	/* alloc for serial_device and init it */
	serdev = serial_device_create(PL011_DEV_NAME);
	if (serdev == NULL) {
		return -ENOMEM;
	}

	err = pl011_device_init(serdev, pdev);
	if (err < 0) {
		tty_err("tty: pl011_device_init failed, err: %s\n", strerror(-err));
		serial_device_remove(serdev);
		return err;
	}

	/* register uart as one tty */
	err = uart_register_tty(serdev, SERIAL_PL011_TTY_OPTION);
	if (err < 0) {
		tty_err("tty: uart_register_tty failed, err: %s\n", strerror(-err));
		iounmap(serdev->mem_base);
		serial_device_remove(serdev);
		return err;
	}

	/* uart register irq */
	err = uart_register_irq(serdev);
	if (err < 0) {
		tty_err("tty: uart_register_irq failed, err: %s\n", strerror(-err));
		uart_unregister_tty(serdev);
		iounmap(serdev->mem_base);
		serial_device_remove(serdev);
		return err;
	}

	/* Finally start up uart */
	(void)pl011_startup(serdev);
	g_pl011_dev = serdev;
	return 0;
}

static int pl011_uart_remove(struct platform_device *pdev)
{
	uart_unregister_tty(g_pl011_dev);

	iounmap(g_pl011_dev->mem_base);
	g_pl011_dev->mem_base = NULL;

	serial_device_remove(g_pl011_dev);
	g_pl011_dev = NULL;
	return 0;
}

static struct platform_driver arm_pl011_uart_platform_driver = {
	.probe = pl011_uart_probe,
	.remove = pl011_uart_remove,
	.driver = {
		.name = PL011_DRV_NAME,
		.of_match_table = pl011_dt_ids,
	}
};

static int __init serial_init(void)
{
	tty_info("tty: init PL011 serial driver\n");
	if (platform_driver_register(&arm_pl011_uart_platform_driver)) {
		tty_err("tty serial: could not register arm_pl011_uart_platform_driver\n");
		return -1;
	}
	tty_info("tty: end of init PL011 serial driver\n");
	return 0;
}

module_init(serial_init);
