/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for devhost internal rpc sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <libsysif/devhost/server.h>

#include <lib/dlist.h>
#include <devhost/log.h>
#include <devhost/plugin.h>
#include <libalgo/idr.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_mutex.h>
#include <libhmucap/ucap.h>
#include <libhwsecurec/securec.h>

#include "devhost.h"
#include "devhost_api.h"

struct device_rpc_client {
	uint32_t cnode_idx;
	uref_t uref;
	struct dlist_node next;
};

struct device_rpc_entry {
	int devid;
	struct raw_mutex lock;
	struct devhost_rpc_ops *ops;
	struct dlist_node clients;
};

static DEFINE_IDR(rpc_devices);

PUBLIC_SYMBOL
int devhost_register_rpc(int devid, struct devhost_rpc_ops *ops)
{
	struct device_rpc_entry *entry = NULL;

	if (ops == NULL || ops->invoke == NULL) {
		return -EINVAL;
	}

	entry = idr_find(&rpc_devices, (unsigned)devid);
	if (entry == NULL) {
		return -ENOENT;
	}
	raw_mutex_lock(&entry->lock);

	if (entry->ops != NULL) {
		raw_mutex_unlock(&entry->lock);
		return -EEXIST;
	}
	entry->ops = ops;

	raw_mutex_unlock(&entry->lock);

	return 0;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_rpc_export, dh_rpc_export_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, devid,
			 unsigned int, cnode_idx)
{
	struct __actvret_drvcall_devhost_rpc_export *pret =
		actvhdlr_drvhandler_devhost_rpc_export_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	struct device_rpc_entry *entry = NULL;
	struct device_rpc_client *client = NULL;
	int ret = E_HM_OK;

	/* alloc client entry */
	client = (struct device_rpc_client *)malloc(sizeof(struct device_rpc_client));
	if (client == NULL) {
		ret = E_HM_NOMEM;
		goto err_alloc_client;
	}
	client->cnode_idx = (uint32_t)cnode_idx;
	dlist_init(&client->next);

	/* export ap */
	client->uref = libdh_ap_export(cnode_idx);
	if (IS_REF_ERR(client->uref)) {
		ret = REF_TO_ERR(client->uref);
		goto err_export_ap;
	}

	/* alloc rpc entry for device */
	entry = (struct device_rpc_entry *)malloc(sizeof(struct device_rpc_entry));
	if (entry == NULL) {
		ret = E_HM_NOMEM;
		goto err_alloc_entry;
	}
	entry->devid = devid;
	entry->ops = NULL;
	dlist_init(&entry->clients);
	raw_mutex_init(&entry->lock);

	/* add client to rpc entry */
	dlist_insert_tail(&entry->clients, &client->next);

	/* add rpc entry */
	ret = idr_set(&rpc_devices, entry, (unsigned)devid);
	if (ret < 0) {
		goto err_idr_set;
	}

	pret->uref = client->uref;

	return E_HM_OK;

err_idr_set:
	free(entry);
err_alloc_entry:
	(void)hm_ucap_revoke(0, client->uref);
err_export_ap:
	free(client);
err_alloc_client:
	return ret;
}

struct devhost_rpc_actv_buf_resp {
	struct devhost_rpc_resp resp;
	struct hm_actv_buf buf;
	bool done;
};

struct devhost_rpc_actv_buf_req {
	struct devhost_rpc_req req;
	struct hm_actv_buf buf;
	bool done;
};

static void devhost_rpc_acquire_resp_buf(struct devhost_rpc_resp *resp,
				  uint64_t *resp_buf, uint64_t *resp_size)
{
	struct devhost_rpc_actv_buf_resp *actv_buf_resp = NULL;
	if (resp == NULL || resp_buf == NULL || resp_size == NULL) {
		return;
	}

	actv_buf_resp = (typeof(actv_buf_resp))container_of(resp, typeof(*actv_buf_resp), resp);

	*resp_buf = actv_buf_resp->buf.buf;
	*resp_size = actv_buf_resp->buf.size;
	return;
}

static int devhost_rpc_actv_buf_write(struct devhost_rpc_resp *resp, const char *data, size_t size)
{
	struct devhost_rpc_actv_buf_resp *actv_buf_resp = NULL;
	int ret = E_HM_OK;

	actv_buf_resp = (typeof(actv_buf_resp))container_of(resp, typeof(*actv_buf_resp), resp);
	if (actv_buf_resp->done) {
		dh_error("try to write on closed response\n");
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		if (actv_buf_resp->buf.size < size) {
			dh_error("failed to write response, buf_size=%llu, data_size=%zu\n",
				 (unsigned long long)actv_buf_resp->buf.size, size);
			ret = E_HM_FBIG;
		}
	}
	if (ret == E_HM_OK) {
		ret = hm_actv_write_xact_vm(u64_to_ptr(actv_buf_resp->buf.buf, void),
					    data, size);
	}
	if (ret == E_HM_OK) {
		actv_buf_resp->done = true;
		actv_buf_resp->resp.resp_size = size;
	}
	return -hmerrno2posix(ret);
}

static void devhost_rpc_acquire_req_buf(struct devhost_rpc_req *req,
					uint64_t *req_buf, uint64_t *req_size)
{
	struct devhost_rpc_actv_buf_req *actv_buf_req = NULL;
	if (req == NULL || req_buf == NULL || req_size == NULL) {
		return;
	}

	actv_buf_req = (typeof(actv_buf_req))container_of(req, typeof(*actv_buf_req), req);

	*req_buf = actv_buf_req->buf.buf;
	*req_size = actv_buf_req->buf.size;
	return;
}

static int devhost_rpc_actv_buf_read(struct devhost_rpc_req *req, const char *data, size_t size)
{
	struct devhost_rpc_actv_buf_req *actv_buf_req = NULL;
	int ret = E_HM_OK;

	actv_buf_req = (typeof(actv_buf_req))container_of(req, typeof(*actv_buf_req), req);
	if (actv_buf_req->done) {
		dh_error("try to read on closed req\n");
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		if (actv_buf_req->buf.size < size) {
			dh_error("failed to read require, buf_size=%llu, data_size=%zu\n",
				 (unsigned long long)actv_buf_req->buf.size, size);
			ret = E_HM_FBIG;
		}
	}
	if (ret == E_HM_OK) {
		ret = hm_actv_read_xact_vm(data, u64_to_ptr(actv_buf_req->buf.buf, void),
					   size);
	}
	if (ret == E_HM_OK) {
		actv_buf_req->done = true;
		actv_buf_req->req.req_size = size;
	}
	return -hmerrno2posix(ret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_rpc_invoke, dh_rpc_invoke_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, devid, int, cmd,
			 unsigned long, req_ubuf, unsigned long, req_size,
			 unsigned long, resp_ubuf, unsigned long, resp_size)
{
	struct __actvret_drvcall_devhost_rpc_invoke *pret =
		actvhdlr_drvhandler_devhost_rpc_invoke_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	struct device_rpc_entry *entry = NULL;
	int ret = E_HM_OK;
	struct devhost_rpc_actv_buf_resp resp;
	struct devhost_rpc_actv_buf_req req;

	entry = idr_find(&rpc_devices, (unsigned)devid);
	if (entry == NULL) {
		return E_HM_POSIX_NOENT;
	}

	mem_zero_s(req);
	req.req.read = devhost_rpc_actv_buf_read;
	req.req.acquire_buf = devhost_rpc_acquire_req_buf;
	req.buf.buf = req_ubuf;
	req.buf.size = req_size;

	mem_zero_s(resp);
	resp.resp.write = devhost_rpc_actv_buf_write;
	resp.resp.acquire_buf = devhost_rpc_acquire_resp_buf;
	resp.buf.buf = resp_ubuf;
	resp.buf.size = resp_size;

	if (entry->ops == NULL ||
	    entry->ops->invoke == NULL) {
		ret = E_HM_NOSYS;
		goto out;
	}

	/* req will be supported in the future */
	ret = entry->ops->invoke(devid, cmd, &req.req, &resp.resp);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		dh_error("devhost_rpc_invoke failed, ret=%s\n", hmstrerror(ret));
		goto out;
	}

	pret->resp_size = (unsigned long long)resp.resp.resp_size;

out:
	return ret;
}

/*
 * rpc sysif module, used for communication between multiple devhosts
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_rpc_export`
 *  - target sysif-ep: `drvhandler_devhost_rpc_invoke`
 *
 * export symbol:
 *  - func: `devhost_register_rpc`
 *  - func: `devhost_rpc_invoke`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(rpc, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_rpc_export, dh_rpc_export_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_rpc_invoke, dh_rpc_invoke_ext)
)
