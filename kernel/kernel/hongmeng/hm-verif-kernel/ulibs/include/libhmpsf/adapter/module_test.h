/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declaration of xvm module test interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 20:44:04 2024
 */
#ifndef ULIBS_LIBHMPSF_ADAPTER_MODULE_TEST_H
#define ULIBS_LIBHMPSF_ADAPTER_MODULE_TEST_H

#include <lib/hmpsf/hmpsf_def.h>
#include <stdbool.h>

int xvm_module_test_attach(unsigned int id);
int xvm_module_test_detach(unsigned int id, bool force_invalidate);
int xvm_module_test_exec(struct hmpsf_test_config_s *config);

#endif
