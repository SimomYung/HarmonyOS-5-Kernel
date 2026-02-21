/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: UDK i2c framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 29 17:34:36 2022
 */
#ifndef	__UDK_I2C_H__
#define	__UDK_I2C_H__

#include <udk/of.h>

struct udk_i2c_adapter;
struct udk_i2c_client;

enum udk_i2c_flags {
	UDK_I2C_FLAG_NONE = 0x0,
	UDK_I2C_FLAG_READ = 0x1,
};

struct udk_i2c_msg {
	unsigned int address;
	unsigned short flags;
	unsigned int len;
	unsigned char *buf;
};

struct udk_i2c_ops {
	int (*adapter_transfer)(const struct udk_i2c_adapter *adapter,
				struct udk_i2c_msg *msgs, int num);
};

struct udk_i2c_adapter {
	unsigned int id;
	void *data;
	struct udk_i2c_ops *ops;
};

static inline void *udk_i2c_get_data(const struct udk_i2c_adapter *adapter)
{
	return adapter->data;
}

int udk_i2c_add_adapter(struct udk_i2c_adapter *adapter);
int udk_i2c_remove_adapter(const struct udk_i2c_adapter *adapter);
struct udk_i2c_client *udk_i2c_add_client(unsigned int i2c_id,
					  unsigned int address);
int udk_i2c_remove_client(struct udk_i2c_client *client);
struct udk_i2c_client *udk_i2c_of_add_client(struct udk_dt_node *node);
int udk_i2c_write(const struct udk_i2c_client *client,
		  unsigned char *wbuf, unsigned int wlen);
int udk_i2c_read(const struct udk_i2c_client *client, unsigned char *wbuf,
		 unsigned int wlen, unsigned char *rbuf, unsigned int rlen);

#endif /* __UDK_I2C_H__ */
