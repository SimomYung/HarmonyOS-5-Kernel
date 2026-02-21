/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Contexthub 9030 driver.
 * Create: 2022-11-30
 */
#ifndef __INT_PARSE_H__
#define __INT_PARSE_H__

enum iodie_ree_int_type {
	IODIE_INT_DMA_TX_I2C8_REE,  // bit0
	IODIE_INT_DMA_RX_I2C8_REE,  // bit1
	IODIE_INT_DMA_TX_I2C9_REE,  // bit2
	IODIE_INT_DMA_RX_I2C9_REE,  // bit3
	IODIE_INT_DMA_TX_SPI4_REE,  // bit4
	IODIE_INT_DMA_RX_SPI4_REE,  // bit5
	IODIE_INT_DMA_TX_SPI5_REE,  // bit6
	IODIE_INT_DMA_RX_SPI5_REE,  // bit7
	IODIE_INT_DMA_TX_SPI6_REE,  // bit8
	IODIE_INT_DMA_RX_SPI6_REE,  // bit9
	IODIE_INT_DMA_TX_UART0_REE, // bit10
	IODIE_INT_DMA_RX_UART0_REE, // bit11
	IODIE_INT_DMA_TX_UART1_REE, // bit12
	IODIE_INT_DMA_RX_UART1_REE, // bit13
	IODIE_INT_I2C8_REE,         // bit14
	IODIE_INT_I2C9_REE,         // bit15
	IODIE_INT_SPI4_REE,         // bit16
	IODIE_INT_SPI5_REE,         // bit17
	IODIE_INT_SPI6_REE,         // bit18
	IODIE_INT_UART0_REE,        // bit19
	IODIE_INT_UATR1_REE,        // bit20
	IODIE_INT_GPIO2_REE,        // bit21
	IODIE_INT_GPIO3_REE,        // bit22
	IODIE_INT_REE_END,
};

typedef void (*iodie_intr_parse_notify_func)(void *data);

int iodie_ree_int_parse_register(enum iodie_ree_int_type int_type, iodie_intr_parse_notify_func notify, void *data);
int iodie_ree_int_parse_unregister(enum iodie_ree_int_type int_type);

#endif

