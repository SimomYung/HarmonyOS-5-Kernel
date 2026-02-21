/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Init of dw8250 serial device
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 16 09:57:12 2021
 */

#include <linux/of.h>
#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>

#include "serial.h"
#include "serial_core.h"
#include "serial_8250.h"
#include "../tty_wrap.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zipeng Zhang");
MODULE_DESCRIPTION("simple linux uart driver for dw8250");
MODULE_VERSION("0.01");

#define UART_DEV_NAME "uart"
#define DW8250_DEV_NAME UART_DEV_NAME
#define DW8250_COMPATIBLE "user,snps,dw-apb-uart"
#define DW8250_DRV_NAME "serial_dw8250"

#define SERIAL_DW8250_TTY_OPTION (TTY_OPTION_DEFAULT | TTY_OPTION_HANDLE_DEBUG_SHORTCUT)

static struct serial_device *g_dw8250_dev = NULL;

static const struct uart_ops g_uops = {
	.getc			= dw8250_serial_getc,
	.rx_empty		= dw8250_rx_empty,
	.handle_error		= dw8250_handle_error,
	.interrupt_state	= dw8250_interrupt_state,
	.tx_async		= dw8250_tx_async,
};

static const struct tty_backend_operations g_bops = {
	.putc		= dw8250_serial_putc,
	.flush		= dw8250_serial_flush,
	.set_speed	= dw8250_serial_set_speed,
	.get_speed	= dw8250_serial_get_speed,
	.stop_tx	= dw8250_serial_stop_tx,
	.start_tx	= dw8250_serial_start_tx,
	.stop_rx	= dw8250_serial_stop_rx,
	.start_rx	= dw8250_serial_start_rx,
	.ioctl		= NULL,
	.set_termios	= dw8250_serial_set_termios,
	.get_termios	= dw8250_serial_get_termios,
	.poll_notify	= NULL,
	.get_icount	= dw8250_serial_get_icount,
	.break_ctl	= dw8250_serial_break_ctl,
};

static int dw8250_device_init(struct serial_device *serdev,
			      struct platform_device *pdev,
			      unsigned int *init_io_state)
{
	int err;
	int irq = 0;
	unsigned int input_mode;
	unsigned int uart_clock = 0;

	/* parse dts to get some basic info */
	err = of_property_read_u32(pdev->dev.of_node, "input-mode",
				   &input_mode);
	if (err < 0) {
		input_mode = UART_INPUT_MODE_NONE;
	}
	if (input_mode == UART_INPUT_MODE_IRQ) {
		irq = platform_get_irq(pdev, 0);
		if (irq <= 0) {
			tty_warn("tty: get irq failed: %d\n", irq);
			return -ENXIO;
		}
	}
	err = of_property_read_u32(pdev->dev.of_node, "output-mode",
				   &serdev->output_mode);
	if (err < 0) {
		serdev->output_mode = UART_OUTPUT_MODE_SYNC;
	}
	/* if enable fifo in dts */
	if (of_property_read_bool(pdev->dev.of_node, "fifo-enabled")) {
		serdev->fifo_enabled = UART_FIFO_ENABLED;
	} else {
		serdev->fifo_enabled = UART_FIFO_DISABLED;
	}
	tty_info("tty: input mode: %d, output mode: %d, fifo enabled: %d\n",
		 input_mode, serdev->output_mode, serdev->fifo_enabled);

	/* ioremap registers to base addr */
	serdev->mem_base = of_iomap(pdev->dev.of_node, 0);
	if (serdev->mem_base == NULL) {
		return -ENXIO;
	}

	/* archieve uart clock on platform from its dts now */
	err = of_property_read_u32(pdev->dev.of_node, "clock_frequence", &uart_clock);
	if (err == 0) {
		serdev->uart_clk = uart_clock;
	} else {
		tty_info("tty: this platform cannot change baudrate uart_clock: %u\n", uart_clock);
	}

	/* Read the serial port initial enable config from dts */
	err = of_property_read_u32(pdev->dev.of_node, "flow_state", init_io_state);
	if (err < 0) {
		*init_io_state = UART_OUTPUT_STATE_ON | UART_INPUT_STATE_ON;
	}

	/* set basic info to serdev */
	serial_device_setup(serdev, &g_bops, &g_uops, irq, input_mode);
	return 0;
}

static void serial_cleanup(struct serial_device *serdev)
{
	if (serdev == NULL) {
		return;
	}

	if (serdev->tty != NULL) {
		uart_unregister_tty(serdev);
	}
	if (serdev->mem_base != NULL) {
		iounmap(serdev->mem_base);
		serdev->mem_base = NULL;
	}
	serial_device_remove(serdev);
}

static int dw8250_uart_probe(struct platform_device *pdev)
{
	int err;
	unsigned int init_io_state;
	struct serial_device *serdev = NULL;

	tty_info("tty: probe dw8250 serial driver\n");
	/* alloc for serial_device and init it */
	serdev = serial_device_create(DW8250_DEV_NAME);
	if (serdev == NULL) {
		return -ENOMEM;
	}

	err = dw8250_device_init(serdev, pdev, &init_io_state);
	if (err < 0) {
		tty_err("tty: dw8250_device_init failed, err: %s\n", strerror(-err));
		serial_cleanup(serdev);
		return err;
	}

	/* register uart as one tty */
	err = uart_register_tty(serdev, SERIAL_DW8250_TTY_OPTION);
	if (err < 0) {
		tty_err("tty: uart_register_tty failed, err: %s\n", strerror(-err));
		serial_cleanup(serdev);
		return err;
	}

	/* uart register irq */
	err = uart_register_irq(serdev);
	if (err < 0) {
		tty_err("tty: uart_register_irq failed, err: %s\n", strerror(-err));
		serial_cleanup(serdev);
		return err;
	}

	/*
	 * Setting serial port configration here is not compatible with the
	 * serial port setting in VM scenario. Therefore it is not configured here
	 * currently
	 */
	g_dw8250_dev = serdev;

	return dw8250_startup(serdev, init_io_state);
}

static int dw8250_uart_remove(struct platform_device *pdev)
{
	serial_cleanup(g_dw8250_dev);
	g_dw8250_dev = NULL;

	return 0;
}

static struct of_device_id dw8250_dt_ids[] = {
	{
		.compatible	= DW8250_COMPATIBLE,
		.name		= DW8250_DEV_NAME,
	},
	{
		/* sentinel */
	},
};

static struct platform_driver arm_dw8250_uart_platform_driver = {
	.probe		= dw8250_uart_probe,
	.remove		= dw8250_uart_remove,
	.shutdown	= NULL,
	.suspend	= NULL,
	.resume		= NULL,
	.driver = {
		.name = DW8250_DRV_NAME,
		.of_match_table = dw8250_dt_ids,
	},
	.id_table	= NULL,
	.prevent_deferred_probe = false,
};

static int __init serial_init(void)
{
	tty_info("tty init dw8250 serial driver\n");
	if (platform_driver_register(&arm_dw8250_uart_platform_driver)) {
		tty_err("tty serial: could not register arm_dw8250_uart_platform_driver\n");
		return -1;
	}
	tty_info("tty: end of init dw8250 serial driver\n");
	return 0;
}

module_init(serial_init);
