/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: client for HIUSB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "core.h"
#include "wrapper.h"

#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/slab.h>

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_CLIENT]%s: " fmt, __func__

#define to_hiusb_client(ptr) container_of((ptr), struct hiusb_client, dev)

bool hiusb_client_is_dp_mode(struct hiusb_client *client)
{
	if (!client || !client->port)
		return TCPC_NC;

	return hiusb_port_is_dp_mode(&client->port->last_event_state);
}

bool hiusb_client_is_usb3_phy(struct hiusb_client *client)
{
	if (!client || !client->port)
		return false;

	return (client->port->usb3_phy == NULL) ? false : true;
}

int hiusb_client_bind_port(struct hiusb_client *client, struct hiusb_port *port)
{
	int ret = 0;
	pr_info("+\n");

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->bind) {
		ret = client->ops->bind(client);
		if (ret) {
			dev_err(&client->dev, "failed %d\n", ret);
			return ret;
		}

		pr_info("port 0x%pK client 0x%pK\n", port, client);
		client->port = port;
	}
	pr_info("-\n");
	return ret;
}

int hiusb_client_unbind_port(struct hiusb_client *client)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->unbind) {
		ret = client->ops->unbind(client);
		if (ret) {
			dev_err(&client->dev, "failed %d\n", ret);
			return ret;
		}
		client->port = NULL;
	}

	return ret;
}

int hiusb_client_assign_port(struct hiusb_client *client, enum hiusb_port_usb_mode mode, struct device *parent)
{
	int ret = 0;
	pr_info("+\n");

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->assign) {
		ret = client->ops->assign(client, mode, parent);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}
	pr_info("-\n");

	return ret;
}

int hiusb_client_deassign_port(struct hiusb_client *client)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->deassign) {
		ret = client->ops->deassign(client);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}

	return ret;
}

int hiusb_client_suspend_port(struct hiusb_client *client, bool remote_wakeup)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->suspend) {
		ret = client->ops->suspend(client, remote_wakeup);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}

	return ret;
}

int hiusb_client_resume_port(struct hiusb_client *client, bool remote_wakeup)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->resume) {
		ret = client->ops->resume(client, remote_wakeup);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}

	return ret;
}

int hiusb_client_init_hw_port(struct hiusb_client *client)
{
	int ret = 0;
	pr_info("+\n");

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->init_hw) {
		ret = client->ops->init_hw(client);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}
	pr_info("-\n");

	return ret;
}

int hiusb_client_deinit_hw_port(struct hiusb_client *client, unsigned int keep_power)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->deinit_hw) {
		ret = client->ops->deinit_hw(client, keep_power);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}

	return ret;
}

int hiusb_client_hibernate_port(struct hiusb_client *client)
{
	int ret = 0;
	pr_info("+\n");

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->hibernate) {
		ret = client->ops->hibernate(client);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}
	pr_info("-\n");

	return ret;
}

int hiusb_client_wakeup_port(struct hiusb_client *client)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->wakeup) {
		ret = client->ops->wakeup(client);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}

	return ret;
}

int hiusb_client_port_ioctrl(struct hiusb_client *client, enum hiusb_port_client_operation operation_code, void* data)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->ioctl) {
		ret = client->ops->ioctl(client, operation_code, data);
		if (ret)
			dev_err(&client->dev, "failed %d\n", ret);
	}

	return ret;
}

void hiusb_client_lock_port(struct hiusb_client *client)
{
	if (!IS_ERR_OR_NULL(client))
		hiusb_port_lock(client->port);
}

void hiusb_client_unlock_port(struct hiusb_client *client)
{
	if (!IS_ERR_OR_NULL(client))
		hiusb_port_unlock(client->port);
}

void hiusb_client_set_drvdata(struct hiusb_client *client, void *data)
{
	if (!IS_ERR_OR_NULL(client))
		dev_set_drvdata(&client->dev, data);
}

void *hiusb_client_get_drvdata(struct hiusb_client *client)
{
	if (!IS_ERR_OR_NULL(client))
		return dev_get_drvdata(&client->dev);

	return NULL;
}

int hiusb_client_init_usb2(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->port->cur_client != client)
		return -EBUSY;

	return hiusb_port_usb2_init(client->port);
}

void hiusb_client_set_port_power_flag(struct hiusb_client *client, int val)
{
	if (IS_ERR_OR_NULL(client))
		return;

	if (client->port->cur_client != client)
		return;

	return hiusb_set_port_power_flag(client->port, val);
}

int hiusb_client_detect_charger_type(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->port->cur_client != client)
		return -EBUSY;

	client->port->charger_type = hiusb_port_detect_charger_type(client->port);
	if ((client->port->charger_type != CHARGER_TYPE_DCP) ||
		((client->port->charger_type == CHARGER_TYPE_DCP) && (client->port->bc_dcp_again_flag == 0)))
		hiusb_port_notify_charger_type(client->port);

	if(hiusb_port_bc_again_allowed(client->port))
		hiusb_port_schedule_bc_again(client->port);

	return 0;
}

void hiusb_client_handle_no_charger(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return;

	if (client->port->cur_client != client)
		return;

	hiusb_port_cancel_bc_again(client->port, 0);
	client->port->charger_type = CHARGER_TYPE_NONE;
	hiusb_port_notify_charger_type(client->port);
}

bool hiusb_client_enumerate_allowed(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return false;

	if (client->port->cur_client != client)
		return false;

	return hiusb_port_enumerate_allowed(client->port);
}

int hiusb_client_is_device_mode(struct hiusb_client *client)
{
	pr_info("+\n");
	if (IS_ERR_OR_NULL(client))
		return false;

	if (client->port->cur_client != client)
		return false;

	return hiusb_port_is_device_mode(client->port);
}

int hiusb_client_entry_device_mode(struct hiusb_client *client)
{
	pr_info("+\n");
	if (IS_ERR_OR_NULL(client))
		return false;

	if (client->port->cur_client != client)
		return false;

	return hiusb_entry_device_mode(client->port->portid);
}

void hiusb_client_exit_device_mode(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return;

	if (client->port->cur_client != client)
		return;

	return hiusb_exit_device_mode(client->port->portid);
}

int hiusb_client_deinit_usb2(struct hiusb_client *client, unsigned int keep_power)
{
	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->port->cur_client != client)
		return -EBUSY;

	return hiusb_port_usb2_exit(client->port, keep_power);
}

int hiusb_client_init_usb3(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->port->cur_client != client)
		return -EBUSY;

	return hiusb_port_usb3_init(client->port);
}

int hiusb_client_deinit_usb3(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->port->cur_client != client)
		return -EBUSY;

	return hiusb_port_usb3_exit(client->port);
}

int hiusb_client_get_protid(struct hiusb_client *client)
{
	int portid = -ENODEV;
	pr_info("+\n");
	if (IS_ERR_OR_NULL(client))
		return -EINVAL;

	if (client->ops->get_portid)
		portid = client->ops->get_portid(client);

	pr_info("-\n");
	return portid;
}

/**
 * fwnode_hiusb_client_get() - lookup and obtain a reference to a hiusb_client
 *
 * @fwnode: fwnode_handle for which to get the hiusb_client
 * @index: the index of the hiusb_client
 *
 * Returns the hiusb_client associated with the given fwnode value.
 */
struct hiusb_client *fwnode_hiusb_client_get(struct fwnode_handle *fwnode)
{
	struct device *dev = NULL;
	struct hiusb_client *client = NULL;
	struct class *hiusb_client_class = hiusb_get_client_class();

	if (!hiusb_client_class)
		return ERR_PTR(-ENODEV);
	if (!fwnode)
		return ERR_PTR(-EINVAL);

	dev = class_find_device_by_fwnode(hiusb_client_class, fwnode);
	if (!dev)
		return ERR_PTR(-EPROBE_DEFER);

	client = to_hiusb_client(dev);
	if (!try_module_get(client->desc->owner)) {
			put_device(dev);
			return ERR_PTR(-EPROBE_DEFER);
		}

	return client;
}

/**
 * hiusb_client_get() - lookup and obtain a reference to a hiusb_client
 * using a fwnode_handle.
 *
 * @parent: port for which the hiusb_client associated.
 * @con_id: name of the hiusb_client from device's point of view
 *
 * Returns the hiusb_client; or -ENODEV if there is no such phy.
 */
struct hiusb_client *hiusb_client_get_by_index(struct fwnode_handle *port, int index)
{
	struct fwnode_reference_args ref;
	struct hiusb_client *client = NULL;
	int ret;

	ret = fwnode_property_get_reference_args(port, "clients", NULL,
						 0, (unsigned int)index, &ref);
	if (ret) {
		pr_err("get client fwnode failed\n");
		return ERR_PTR(ret);
	}

	client = fwnode_hiusb_client_get(ref.fwnode);

	fwnode_handle_put(ref.fwnode);

	return client;
}
EXPORT_SYMBOL_GPL(hiusb_client_get_by_index);

/**
 * hiusb_client_put() - release the hiusb_client
 * @client: the hiusb_client returned by hiusb_client_get()
 *
 * Releases a refcount the caller received from hiusb_client_get().
 */
void hiusb_client_put(struct hiusb_client *client)
{
	if (IS_ERR_OR_NULL(client))
		return;

	module_put(client->desc->owner);
	put_device(&client->dev);
}
EXPORT_SYMBOL_GPL(hiusb_client_put);

static void hiusb_client_dev_release(struct device *dev)
{
	struct hiusb_client *client = to_hiusb_client(dev);

	kfree(client);
}

static const struct device_type g_hiusb_client_dev_type = {
	.name = "hiusb-client-dev",
	.release = hiusb_client_dev_release,
};

struct hiusb_client *hiusb_client_create(struct device *parent,
					 const struct hiusb_client_desc *desc,
					 const struct hiusb_client_ops *ops)
{
	int ret;
	struct hiusb_client *client = NULL;
	struct class *hiusb_client_class = hiusb_get_client_class();
	pr_info("+");

	if (!hiusb_client_class)
		return ERR_PTR(-ENODEV);
	if (!parent || !desc || !desc->name || !ops)
		return ERR_PTR(-EINVAL);
	if (!dev_fwnode(parent))
		return ERR_PTR(-EINVAL);

	client = kzalloc(sizeof(*client), GFP_KERNEL);
	if (!client)
		return ERR_PTR(-ENOMEM);

	INIT_LIST_HEAD(&client->list);
	client->desc = desc;
	client->ops = ops;

	client->dev.parent = parent;
	client->dev.fwnode = dev_fwnode(parent);
	client->dev.class = hiusb_client_class;
	client->dev.type = &g_hiusb_client_dev_type;
	dev_set_name(&client->dev, "%s-%s-hiusb-client",dev_name(parent), desc->name);

	pr_info("%s do device register\n", dev_name(&client->dev));
	ret = device_register(&client->dev);
	if (ret) {
		put_device(&client->dev);
		kfree(client);
		return ERR_PTR(ret);
	}
	pr_info("-");
	return client;
}
EXPORT_SYMBOL_GPL(hiusb_client_create);

void hiusb_client_destroy(struct hiusb_client *client)
{
	if (!IS_ERR_OR_NULL(client))
		device_unregister(&client->dev);
}
EXPORT_SYMBOL_GPL(hiusb_client_destroy);
