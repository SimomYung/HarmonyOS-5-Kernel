/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: UDK spi subsystem framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 21 17:18:26 2022
 */

#ifndef __UDK_SPI_H__
#define __UDK_SPI_H__

/* Modes, the bitmask of the following values */
#define UDK_SPI_CPHA                0x0001       /* Clock phase */
#define UDK_SPI_CPOL                0x0002       /* Clock polarity */

#define UDK_SPI_MODE_0              (0)
#define UDK_SPI_MODE_1              (0 | UDK_SPI_CPHA)
#define UDK_SPI_MODE_2              (UDK_SPI_CPOL | 0)
#define UDK_SPI_MODE_3              (UDK_SPI_CPOL | UDK_SPI_CPHA)
#define UDK_SPI_MODE_X_MASK         (UDK_SPI_CPOL | UDK_SPI_CPHA)

#define UDK_SPI_CS_HIGH             0x0004       /* Chipselect active is high signal */
#define LOWEST_SPEED                1000

/* These flags are for udk_spi_xfer: masked 8-bit value is to skip that number of first read bytes */
#define UDK_SPI_XFER_FLAG_SKIP_READ_SHIFT 24
#define UDK_SPI_XFER_FLAG_SKIP_READ_MASK 0xFF

#define udk_spi_xfer_flags_to_skip_read(flags) \
	((flags) >> UDK_SPI_XFER_FLAG_SKIP_READ_SHIFT & UDK_SPI_XFER_FLAG_SKIP_READ_MASK)
#define udk_spi_skip_read_to_xfer_flag(skip) \
	(((skip) & UDK_SPI_XFER_FLAG_SKIP_READ_MASK) << UDK_SPI_XFER_FLAG_SKIP_READ_SHIFT)

struct udk_spi_adapter;
struct udk_spi_client;

struct udk_spi_trans_para {
	unsigned int client_id;
	unsigned int flags;
	unsigned int speed;
	unsigned int mode;
};

struct udk_spi_ops {
	/* Initialize the SPI bus. Called on the first udk_spi_open_device for the SPI bus */
	int (*init)(struct udk_spi_adapter *adapter);
	/* De-initialize the SPI bus. Called on the last udk_spi_release_device for the SPI bus */
	int (*deinit)(struct udk_spi_adapter *adapter);
	/* Returns 0 if the mode is supported */
	int (*validate_mode)(const struct udk_spi_adapter *adapter, unsigned int mode);
	/* Returns the highest supported speed not exeeding "hz" (in Hz) */
	unsigned int (*validate_speed)(const struct udk_spi_adapter *adapter, unsigned int hz);
	int (*adapter_transfer)(const struct udk_spi_adapter *adapter, struct udk_spi_trans_para *para,
				    const unsigned char *out, unsigned char *in, unsigned long bit_len);
};

struct udk_spi_adapter {
	unsigned int id;
	unsigned int client_max;
	void *data;
	struct udk_spi_ops *ops;
};

int udk_spi_add_adapter(struct udk_spi_adapter *adapter);
int udk_spi_remove_adapter(const struct udk_spi_adapter *adapter);
int udk_spi_add_client(unsigned int adapter_id,
		       unsigned int mode, unsigned int speed, unsigned int client_id);
int udk_spi_remove_client(unsigned int adapter_id, unsigned int client_id);

struct udk_spi_client *udk_spi_open_client(unsigned int adapter_id, unsigned int client_id);
int udk_spi_close_client(struct udk_spi_client *client);

int udk_spi_write(const struct udk_spi_client *client,
		  unsigned char *wbuf, unsigned long bit_len);
int udk_spi_read(const struct udk_spi_client *client, unsigned char *wbuf,
		 unsigned char *rbuf, unsigned long bit_len, unsigned int flags);

#endif /* __UDK_SPI_H__ */
