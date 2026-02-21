/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Aarch64 Livepatch instructions opreation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 18 16:15:23 2022
 */

#ifndef ARCH64_LIVEPATCH_INSN_H
#define ARCH64_LIVEPATCH_INSN_H

#include <lib/livepatch/livepatch.h>
#include <hmasm/klivepatch_arch_insn.h>

int arch_patch_kfunc(struct livepatch_kfunc_s *func);
int arch_unpatch_kfunc(struct livepatch_kfunc_s *func);
int arch_backup_klp_old_insns(struct livepatch_old_insns_s *old_insns, uptr_t addr);

#endif
