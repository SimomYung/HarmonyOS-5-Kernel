/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Parse config for devhost
 * Author: Huawei OS Kernel Lab
 * Create: Tues May 23 10:45:12 2022
 */
#include <devhost/cfg.h>

#include <sys/types.h>
#include <hongmeng/errno.h>
#include <api/devhost_api.h>
#include "devhost.h"
#include <devhost/log.h>
#include "devhost_backend.h"

struct devhost_pools {
	unsigned int pool_num;
	unsigned int total_watermark;
	struct pool_info pool[MAX_POOL_NUM];
};

struct devhost_pools pools = {0};

unsigned int devhost_get_pool_num(void)
{
	BUG_ON(pools.pool_num > MAX_POOL_NUM);
	return pools.pool_num;
}

struct pool_info *devhost_get_pool_info(unsigned int pool_idx)
{
	BUG_ON(pool_idx >= pools.pool_num);
	return &pools.pool[pool_idx];
}

enum devhost_pool_type devhost_get_pool_type(unsigned int pool_idx)
{
	BUG_ON(pool_idx >= pools.pool_num);
	return pools.pool[pool_idx].type;
}

static int devhost_parse_pool_type(char *str, unsigned int *type)
{
	int ret;
	char tmp_buf[POOL_NAME_LEN] = {0};

	if (!strstr(str, "type")) {
		return E_HM_INVAL;
	}
	ret = sscanf_s(str, "type=%s", tmp_buf, POOL_NAME_LEN - 1);
	if (ret != 1) {
		dh_error("devhost pool cfg parse type faild, str=[%s]\n", str);
		return E_HM_INVAL;
	}
	if (strcmp(tmp_buf, "mem") == 0) {
		*type = DEVHOST_POOL_TYPE_MEM;
	} else if (strcmp(tmp_buf, "cma") == 0) {
		*type = DEVHOST_POOL_TYPE_CMA;
	} else if (strcmp(tmp_buf, "discrete") == 0) {
		*type = DEVHOST_POOL_TYPE_DISCRETE;
	} else {
		dh_error("devhost pool cfg parse type error, tmp_buf=[%s]\n", tmp_buf);
		return E_HM_INVAL;
	}
	dh_info("devhost pool type=%s\n", tmp_buf);
	return 0;
}

static int devhost_parse_pool_rsv(char *str, unsigned int *rsv)
{
	int ret;
	char tmp_buf[POOL_NAME_LEN] = {0};

	if (!strstr(str, "rsv")) {
		return E_HM_INVAL;
	}
	ret = sscanf_s(str, "rsv=%s", tmp_buf, POOL_NAME_LEN - 1);
	if (ret != 1) {
		dh_error("devhost pool cfg parse reserve faild, str=[%s]\n", str);
		return E_HM_INVAL;
	}
	if (strcmp(tmp_buf, "st") == 0) {
		*rsv = DEVHOST_RSV_TYPE_ST;
	} else if (strcmp(tmp_buf, "dyn") == 0) {
		*rsv = DEVHOST_RSV_TYPE_DYN;
	} else {
		dh_error("devhost pool cfg parse rsv error, tmp_buf=[%s]\n", tmp_buf);
		return E_HM_INVAL;
	}
	dh_info("devhost pool rsv=%s\n", tmp_buf);
	return 0;
}

static int devhost_parse_pool_name(char *str, char *name)
{
	int ret;

	ret = sscanf_s(str, "name=%s", name, POOL_NAME_LEN - 1);
	if (ret != 1) {
		dh_error("devhost pool cfg parse name faild, str=[%s]\n", str);
		return E_HM_INVAL;
	}
	dh_info("devhost pool name=%s\n", name);

	return 0;
}

static int devhost_parse_pool_int(char *str, unsigned int pool_idx)
{
	int ret = 0;

	if (strstr(str, "sizeorder")) {
		ret = sscanf_s(str, "sizeorder=%u", &pools.pool[pool_idx].sizeorder);
	} else if (strstr(str, "reclaimorder")) {
		ret = sscanf_s(str, "reclaimorder=%u", &pools.pool[pool_idx].reclaimorder);
	} else if (strstr(str, "high_watermark")) {
		ret = sscanf_s(str, "high_watermark=%u", &pools.pool[pool_idx].high_watermark);
	} else if (strstr(str, "low_watermark")) {
		ret = sscanf_s(str, "low_watermark=%u", &pools.pool[pool_idx].low_watermark);
	} else {
		dh_warn("devhost pool cfg [%s] is not match\n", str);
		return 0;
	}

	if (ret != 1) {
		dh_error("devhost pool cfg parse faild, str=[%s]\n", str);
		return E_HM_INVAL;
	}

	dh_info("devhost pool cfg %s succeed\n", str);
	return 0;
}

static int devhost_parse_pool_cfg(char *buf, unsigned int pool_idx)
{
	int ret;
	char *str = NULL;
	char *next_token = NULL;

	pools.pool[pool_idx].low_watermark = 0;
	pools.pool[pool_idx].high_watermark = 0;
	str = strtok_s(buf, ",", &next_token);
	while(str != NULL) {
		if (strstr(str, "name")) {
			ret = devhost_parse_pool_name(str, pools.pool[pool_idx].name);
		} else if (strstr(str, "type")) {
			ret = devhost_parse_pool_type(str, &pools.pool[pool_idx].type);
		} else if (strstr(str, "rsv")) {
			ret = devhost_parse_pool_rsv(str, &pools.pool[pool_idx].rsv);
		} else {
			ret = devhost_parse_pool_int(str, pool_idx);
		}

		if (ret != 0) {
			return ret;
		}
		str = strtok_s(NULL, ",", &next_token);
	}

	return 0;
}

void devhost_parse_pools(char *buf)
{
	int ret;
	char *pools_buf = NULL;
	char *buf_next = NULL;
	char *pool_cfg = NULL;

	if (buf == NULL || *buf == '\0') {
		return;
	}

	pools.pool_num = 0;

	pools_buf = strdup(buf);
	if (pools_buf == NULL) {
		dh_error("devhost parse pools failed, buf=%s \n", buf);
		return;
	}

	buf_next = pools_buf;
	while ((pool_cfg = strsep(&buf_next, ":")) != NULL) {
		if (pools.pool_num >= MAX_POOL_NUM) {
			dh_warn("devhost parse pools exceeded max num, buf=%s\n", buf);
			break;
		}
		ret = devhost_parse_pool_cfg(pool_cfg, pools.pool_num);
		BUG_ON(ret < 0);
		pools.pool_num++;
	}

	free(pools_buf);
}

unsigned int devhost_get_poolcfg_total_watermark(void)
{
	return pools.total_watermark;
}

static int devhost_parse_poolcfg_buf(char *buf)
{
	int ret;
	char *str = NULL;
	char *next_token = NULL;

	str = strtok_s(buf, ",", &next_token);
	str = buf;
	while(str != NULL) {
		if (strstr(str, "total_watermark_mb")) {
			ret = sscanf_s(str, "total_watermark_mb=%u", &pools.total_watermark);
			if (ret != 1) {
				dh_error("devhost poolcfg parse total watermark faild, buf=[%s]\n", buf);
				return E_HM_INVAL;
			}
			dh_info("devhost total watermark=%u\n", pools.total_watermark);
		}
		str = strtok_s(NULL, ",", &next_token);
	}

	return 0;
}

void devhost_parse_poolcfg(char *buf)
{
	int ret;
	char *poolcfg_buf = NULL;

	if (buf == NULL || *buf == '\0') {
		return;
	}

	poolcfg_buf = strdup(buf);
	if (poolcfg_buf == NULL) {
		dh_error("devhost parse poolcfg failed, buf=%s \n", buf);
		return;
	}

	ret = devhost_parse_poolcfg_buf(poolcfg_buf);
	BUG_ON(ret < 0);

	free(poolcfg_buf);
}
