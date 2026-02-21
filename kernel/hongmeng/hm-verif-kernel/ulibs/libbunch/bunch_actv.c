/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2023. All rights reserved.
 * Description: Bunch actv model based on actv_write_caller_vm
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 26 11:21:31 2023
 */

#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <libbunch/bunch_actv.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

#define BUNCH_ACTV_PAYLOAD_EXPAND_FACTOR		2U

static struct bunch_actv_attr *actv_create_attr_buf(size_t size)
{
	int err = E_HM_OK;
	void *buf = NULL;
	struct bunch_actv_attr *attr = NULL;
	size_t real_size = size;

	if (real_size == 0U) {
		real_size = BUNCH_ACTV_DEFAULT_SIZE;
	}

	buf = malloc(real_size);
	if (buf == NULL) {
		err = E_HM_NOMEM;
	}

	if (err == E_HM_OK) {
		mem_zero_b(buf, real_size);
		attr = malloc(sizeof(struct bunch_actv_attr));
		if (attr == NULL) {
			err = E_HM_NOMEM;
			free(buf);
		}
	}

	if (err == E_HM_OK) {
		/* assign all members, no need to zero the attr */
		attr->buf = buf;
		attr->size = (uint32_t)real_size;
	}

	return attr;
}

static void actv_destroy_attr_buf(struct bunch_actv_attr *attr)
{
	if (attr != NULL) {
		if (attr->buf != NULL) {
			free(attr->buf);
		}
		free(attr);
	}
}

struct bunch *bunch_actv_create(size_t size)
{
	struct bunch *bunch = NULL;
	struct bunch_actv_attr *attr = NULL;

	attr = actv_create_attr_buf(size);
	if (attr == NULL) {
		hm_warn("malloc attr failed\n");
	} else {
		/* all fields will be assigned, no need to zero it */
		bunch = bunch_actv_open(attr);
		if (bunch == NULL) {
			hm_warn("open bunch failed\n");
			actv_destroy_attr_buf(attr);
		}
	}

	return bunch;
}

void bunch_actv_destroy(struct bunch *bunch)
{
	if (bunch != NULL) {
		void *priv = bunch_actv_close(bunch);
		(void)actv_destroy_attr_buf(ptr_from_void(priv, struct bunch_actv_attr));
	}
}

struct bunch *bunch_actv_open(struct bunch_actv_attr *attr)
{
	int err = E_HM_OK;
	struct bunch *bunch = NULL;

	if (attr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		bunch = malloc(sizeof(struct bunch));
		if (bunch == NULL) {
			hm_warn("actv malloc bunch failed\n");
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		(void)bunch_payload_install(bunch, attr->buf, attr->size);
		bunch_set_private_data(bunch, attr);
	}

	return bunch;
}

void *bunch_actv_close(struct bunch *bunch)
{
	void *priv_data = NULL;
	if (bunch != NULL) {
		priv_data = bunch_get_private_data(bunch);
		free(bunch);
	}

	return priv_data;
}

static int actv_request_expand_attr(struct bunch_actv_attr *attr)
{
	size_t old_size = attr->size;
	size_t new_size = old_size * BUNCH_ACTV_PAYLOAD_EXPAND_FACTOR;
	void *new_buf = NULL;
	int err = E_HM_OK;

	if (new_size / BUNCH_ACTV_PAYLOAD_EXPAND_FACTOR != old_size) {
		/* if overflow, set size to default size */
		err = E_HM_OVERFLOW;
	}

	if (err == E_HM_OK) {
		new_buf = malloc(new_size);
		if (new_buf == NULL) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		free(attr->buf);
		attr->buf = new_buf;
		attr->size = (uint32_t)new_size;
	}

	return err;
}

static int actv_request_fetch_retry(struct bunch_actv_attr *attr,
				    struct bunch_actv_reader_ctx *ctx)
{
	unsigned int i;
	int err = ctx->actvcall(attr, ctx->private_data);
	for (i = 0U; i < BUNCH_ACTV_FETCH_RETRY_MAX; i++) {
		if (err == E_HM_OK) {
			/* buffer is enough */
			break;
		} else if (err == E_HM_NOMEM) {
			/* retry with larger mem */
			err = actv_request_expand_attr(attr);
			if (err != E_HM_OK) {
				hm_warn("expand attr failed:%s\n", hmstrerror(err));
				break;
			}
			err = ctx->actvcall(attr, ctx->private_data);
		} else {
			/* unknown erros, return directly */
			break;
		}
	}

	return err;
}

int bunch_actv_request(struct bunch *bunch, struct bunch_actv_reader_ctx *ctx)
{
	int err = E_HM_OK;
	struct bunch_actv_attr *attr = NULL;

	if (bunch == NULL || ctx == NULL || ctx->actvcall == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		void *priv = bunch_get_private_data(bunch);
		attr = ptr_from_void(priv, struct bunch_actv_attr);
		if (attr == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		err = actv_request_fetch_retry(attr, ctx);
		/* even err is not OK we still need to update bunch
		 * as attr might be updated */
		bunch->payload = attr->buf;
		bunch->size = attr->size;
	}

	return err;
}

int bunch_actv_write_caller(struct bunch *bunch,
			    const struct bunch_actv_attr *caller_attr)
{
	int err = E_HM_OK;
	size_t src_size = 0U;

	if (bunch == NULL || caller_attr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		src_size = bunch_get_position(bunch);
		if (src_size > caller_attr->size) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		err = hm_actv_write_caller_vm(caller_attr->buf,
					      bunch_payload_of(bunch),
					      src_size);
		if (err < 0) {
			hm_warn("actv write to caller vm failed:%s\n",
				hmstrerror(err));
			/* we reset the error so that error won't be
			 * mis-handled (e.g. return E_HM_NOMEM) */
			err = E_HM_FAULT;
		}
	}

	return err;
}
