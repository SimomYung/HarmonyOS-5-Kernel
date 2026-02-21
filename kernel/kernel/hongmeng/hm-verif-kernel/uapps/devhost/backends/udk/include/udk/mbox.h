/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Mailbox udk framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 18 10:41:26 2021
 */
#ifndef __UDK_MBOX_H__
#define __UDK_MBOX_H__

#include <udk/errno.h>
#include <libstrict/strict.h>

struct udk_mbox_controller;
struct udk_mbox_channel;

struct udk_mbox_ops {
	int (*request_channel)(struct udk_mbox_controller *mbox);
	int (*start_channel)(struct udk_mbox_channel *channel);
	int (*stop_channel)(struct udk_mbox_channel *channel);
	int (*send_message)(struct udk_mbox_channel *channel, void *message);
	bool (*is_sent)(struct udk_mbox_channel *channel);
};

struct udk_mbox_controller {
	unsigned int id;
	unsigned int channel_num;
	unsigned int tx_retry;
	unsigned int tx_delay_us;
	struct udk_mbox_channel *channels;
	struct udk_mbox_ops ops;
};

struct udk_mbox_channel {
	bool in_use;
	struct udk_mbox_controller *mbox;
	struct udk_mbox_client *client;
};

struct udk_mbox_client {
	unsigned int mbox_id;
	struct udk_mbox_channel *channel;
	void (*message_received)(struct udk_mbox_client *client, void *message);
};

#ifdef CONFIG_UDK_MAILBOX

int udk_mbox_register(struct udk_mbox_controller *mbox);
void udk_mbox_unregister(unsigned int id);
int udk_mbox_request_channel(struct udk_mbox_client *client);
int udk_mbox_free_channel(struct udk_mbox_channel *channel);
int udk_mbox_send_message(struct udk_mbox_channel *channel, void *message);
void udk_mbox_receive_message(struct udk_mbox_channel *channel, void *message);

#else

static inline int udk_mbox_register(struct udk_mbox_controller *mbox)
{
	UNUSED(mbox);
	return -ENOSYS;
}

static inline void udk_mbox_unregister(unsigned int id)
{
	UNUSED(id);
}

static inline int udk_mbox_request_channel(struct udk_mbox_client *client)
{
	UNUSED(client);
	return -ENOSYS;
}

static inline int udk_mbox_free_channel(struct udk_mbox_channel *channel)
{
	UNUSED(channel);
	return -ENOSYS;
}

static inline int
udk_mbox_send_message(struct udk_mbox_channel *channel, void *message)
{
	UNUSED(channel, message);
	return -ENOSYS;
}

static inline void
udk_mbox_receive_message(struct udk_mbox_channel *channel, void *message)
{
	UNUSED(channel, message);
}

#endif

#endif /* __UDK_MBOX_H__ */
