/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Help to process signal in actvation handler
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 13 17:44:07 2020
 */
#ifndef ULIBS_LIBHMPM_ACTV_HELPER_H
#define ULIBS_LIBHMPM_ACTV_HELPER_H

struct actv_helper_obj {
	int (*callback)(void *data);
	void *data;
	int policy;
	int prio;
};

int actv_helper_init(struct actv_helper_obj *helper);

#endif
