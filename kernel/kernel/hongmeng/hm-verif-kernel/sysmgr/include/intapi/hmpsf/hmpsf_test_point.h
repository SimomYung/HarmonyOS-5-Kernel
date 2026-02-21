/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 21 21:26:58 2023
 */
#ifndef H_SYSMGR_INCLUDE_HMPSF_TEST_POINT_H
#define H_SYSMGR_INCLUDE_HMPSF_TEST_POINT_H

#include <lib/hmpsf/hmpsf_def.h>
#include <hmkit/types.h>

int hmpsf_module_test_point_attach(int module_id);
int hmpsf_module_test_point_detach(int module_id, bool force_invalidate);
int hmpsf_test_point_exec(struct hmpsf_test_config_s *config);

#endif
