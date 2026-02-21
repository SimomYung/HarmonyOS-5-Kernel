/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Mailbox udk framework
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 17 17:23:25 2021
 */
#include <string.h>

#include <udk/log.h>
#include <udk/mbox.h>
#include <udk/sync.h>
#include <udk/delay.h>
#include <udk/iolib.h>
#include <udk/securec.h>
#include <udk/lib/idr.h>
#include <internal/init.h>
#include <hongmeng/errno.h>

#define MBOX_CHANNELS_NUM_MAX		64
#define MBOX_TX_RETRY_MAX		1000
#define MBOX_TX_DELAY_US_MAX		1000

struct udk_mbox_manager {
	struct idr mboxes;
	struct udk_mutex mutex;
};

static struct udk_mbox_manager mbox_manager;

static inline void mbox_manager_lock(void)
{
	udk_mutex_lock(&mbox_manager.mutex);
}

static inline void mbox_manager_unlock(void)
{
	udk_mutex_unlock(&mbox_manager.mutex);
}

static struct udk_mbox_channel *mbox_channels_create(unsigned int num)
{
	struct udk_mbox_channel *channels = NULL;

	channels = (struct udk_mbox_channel *)udk_malloc(num * sizeof(*channels));
	if (channels == NULL) {
		udk_error("malloc mbox channel failed\n");
		return NULL;
	}
	mem_zero_s(*channels);

	return channels;
}

static void mbox_channels_destory(struct udk_mbox_controller *mbox)
{
	udk_free(mbox->channels);
	mbox->channels = NULL;
}

static int mbox_controller_init(struct udk_mbox_controller *mbox)
{
	unsigned int i;
	struct udk_mbox_channel *channels = NULL;

	if (mbox->channel_num == 0 ||
	    mbox->channel_num > MBOX_CHANNELS_NUM_MAX) {
		udk_error("invalid channels number %d\n",  mbox->channel_num);
		return -EINVAL;
	}
	if (mbox->tx_retry == 0 || mbox->tx_retry > MBOX_TX_RETRY_MAX) {
		udk_warn("invalid tx retry, using default value: %d\n",
			  MBOX_TX_RETRY_MAX);
		mbox->tx_retry = MBOX_TX_RETRY_MAX;
	}
	if (mbox->tx_delay_us == 0 ||
	    mbox->tx_delay_us > MBOX_TX_DELAY_US_MAX) {
		udk_warn("invalid tx delay us, using default value: %d\n",
			  MBOX_TX_DELAY_US_MAX);
		mbox->tx_delay_us = MBOX_TX_DELAY_US_MAX;
	}
	channels = mbox_channels_create(mbox->channel_num);
	if (channels == NULL) {
		udk_error("channles create failed\n");
		return -ENOMEM;
	}
	for (i = 0; i < mbox->channel_num; i++) {
		channels[i].in_use = false;
		channels[i].mbox = mbox;
	}
	mbox->channels = channels;

	return 0;
}

int udk_mbox_register(struct udk_mbox_controller *mbox)
{
	int err;

	if (mbox == NULL) {
		udk_error("invalid mbox controller\n");
		return -EINVAL;
	}
	mbox_manager_lock();
	err = mbox_controller_init(mbox);
	if (err < 0) {
		udk_error("mbox controller init failed: %s\n", strerror(-err));
		mbox_manager_unlock();
		return err;
	}
	err = idr_set(&mbox_manager.mboxes, (void *)mbox, mbox->id);
	if (err < 0) {
		udk_error("mbox idr set failed: %s\n", strerror(-err));
		mbox_channels_destory(mbox);
		mbox_manager_unlock();
		return -hmerrno2posix(err);
	}
	mbox_manager_unlock();

	return 0;
}

void udk_mbox_unregister(unsigned int id)
{
	struct udk_mbox_controller *mbox = NULL;

	mbox_manager_lock();
	mbox = (struct udk_mbox_controller *)idr_remove(&mbox_manager.mboxes, id);
	if (mbox == NULL) {
		udk_error("mbox %d not found\n", id);
		mbox_manager_unlock();
		return;
	}
	mbox_channels_destory(mbox);
	mbox_manager_unlock();
}

static int mbox_channel_setup(struct udk_mbox_client *client,
			      struct udk_mbox_channel *channel)
{
	int err;
	struct udk_mbox_controller *mbox = channel->mbox;

	if (channel->in_use) {
		udk_error("mbox channel is busy\n");
		return -EBUSY;
	}
	if (mbox->ops.start_channel == NULL) {
		udk_error("start channel not installed\n");
		return -ENOENT;
	}
	err = mbox->ops.start_channel(channel);
	if (err < 0) {
		udk_error("start channel failed: %s\n", strerror(-err));
		return err;
	}
	channel->client = client;
	channel->in_use = true;
	client->channel = channel;

	return 0;
}

int udk_mbox_request_channel(struct udk_mbox_client *client)
{
	int err;
	unsigned int channel_id, mbox_id;
	struct udk_mbox_controller *mbox = NULL;

	if (client == NULL) {
		udk_error("invalid mbox client\n");
		return -EINVAL;
	}
	mbox_id = client->mbox_id;
	mbox_manager_lock();
	mbox = (struct udk_mbox_controller *)idr_find(&mbox_manager.mboxes, mbox_id);
	if (mbox == NULL) {
		udk_error("mbox %d not found\n", mbox_id);
		mbox_manager_unlock();
		return -ENOENT;
	}
	if (mbox->ops.request_channel == NULL) {
		udk_error("request channel not installed\n");
		mbox_manager_unlock();
		return -ENOENT;
	}
	err = mbox->ops.request_channel(mbox);
	if (err < 0) {
		udk_error("mbox request channel failed: %s\n", strerror(-err));
		mbox_manager_unlock();
		return err;
	}
	channel_id = (unsigned int)err;
	if (channel_id >= mbox->channel_num) {
		udk_error("invalid channel id: %d\n", channel_id);
		mbox_manager_unlock();
		return -EINVAL;
	}
	err = mbox_channel_setup(client, &mbox->channels[channel_id]);
	if (err < 0) {
		udk_error("channel setup failed: %s\n", strerror(-err));
		mbox_manager_unlock();
		return err;
	}
	mbox_manager_unlock();

	return 0;
}

int udk_mbox_free_channel(struct udk_mbox_channel *channel)
{
	int err;
	struct udk_mbox_controller *mbox = NULL;

	if (channel == NULL) {
		udk_error("invalid channel\n");
		return -EINVAL;
	}
	mbox = channel->mbox;
	mbox_manager_lock();
	if (!channel->in_use) {
		udk_warn("mbox channel is free\n");
		mbox_manager_unlock();
		return 0;
	}
	if (mbox->ops.stop_channel == NULL) {
		udk_error("stop channel not installed\n");
		mbox_manager_unlock();
		return -ENOENT;
	}
	err = mbox->ops.stop_channel(channel);
	if (err < 0) {
		udk_error("stop channel failed: %s\n", strerror(-err));
		mbox_manager_unlock();
		return err;
	}
	channel->in_use = false;
	mbox_manager_unlock();

	return 0;
}

static int wait_for_tx_completed(struct udk_mbox_channel *channel)
{
	bool done = false;
	unsigned int retry = 0;
	struct udk_mbox_controller *mbox = channel->mbox;
	unsigned int retry_max = mbox->tx_retry;
	unsigned int tx_delay_us = mbox->tx_delay_us;

	if (mbox->ops.is_sent == NULL) {
		udk_error("is sent not installed\n");
		return -ENOENT;
	}
	while (retry < retry_max) {
		done = mbox->ops.is_sent(channel);
		if (done) {
			break;
		}
		udk_udelay(tx_delay_us);
		retry++;
	}
	if (retry >= retry_max) {
		return -ETIMEDOUT;
	}

	return 0;
}

int udk_mbox_send_message(struct udk_mbox_channel *channel, void *message)
{
	int err;
	struct udk_mbox_controller *mbox = NULL;

	if (channel == NULL) {
		udk_error("invalid channel\n");
		return -EINVAL;
	}
	if (message == NULL) {
		udk_error("invalid message\n");
		return -EINVAL;
	}
	mbox = channel->mbox;
	mbox_manager_lock();
	if (mbox->ops.send_message == NULL) {
		udk_error("send message not installed\n");
		mbox_manager_unlock();
		return -ENOENT;
	}
	err = mbox->ops.send_message(channel, message);
	if (err < 0) {
		udk_error("mbox send message failed: %s\n", strerror(-err));
		mbox_manager_unlock();
		return err;
	}
	err = wait_for_tx_completed(channel);
	if (err < 0) {
		udk_error("wait tx failed: %s\n", strerror(-err));
		mbox_manager_unlock();
		return err;
	}
	mbox_manager_unlock();

	return 0;
}

void udk_mbox_receive_message(struct udk_mbox_channel *channel, void *message)
{
	struct udk_mbox_client *client = NULL;

	if (channel == NULL) {
		return;
	}
	client = channel->client;
	if (client == NULL || client->message_received == NULL) {
		return;
	}
	client->message_received(client, message);
}

static int udk_mbox_init(void)
{
	idr_init(&mbox_manager.mboxes);
	udk_mutex_init(&mbox_manager.mutex);

	return 0;
}
udk_init_call(udk_mbox_init);
