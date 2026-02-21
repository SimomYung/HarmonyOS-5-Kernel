/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Captypes
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 12 16:29:59 2018
 */

#ifndef UAPI_HMKERNEL_CAPABILITY_CAPTYPES_H
#define UAPI_HMKERNEL_CAPABILITY_CAPTYPES_H

/* Also change kernel/include/hmkernel/capability/captypes.h */
#include <hmkernel/capability/captype_cnode.h>
#include <hmkernel/capability/captype_thread.h>
#include <hmkernel/capability/captype_pmem.h>
#include <hmkernel/capability/captype_channel.h>
#include <hmkernel/capability/captype_activation.h>
#include <hmkernel/capability/captype_activation_pool.h>
#include <hmkernel/capability/captype_vspace.h>
#include <hmkernel/capability/captype_sysctrl.h>
#include <hmkernel/capability/captype_irqctrl.h>
#include <hmkernel/capability/captype_cpucontext.h>
#include <hmkernel/capability/captype_scheduler.h>
#include <hmkernel/capability/captype_vm.h>
#include <hmkernel/capability/captype_timer.h>
#include <hmkernel/capability/captype_module.h>
#include <hmkernel/capability/captype_permission.h>
#include <hmkernel/capability/captype_livepatch.h>
#include <hmkernel/capability/captype_ec_ep.h>
#include <hmkernel/capability/captype_ec_chn.h>
#include <hmkernel/capability/captype_dsucontext.h>
#include <hmkernel/capability/captype_fdtable.h>
#include <hmkernel/capability/captype_epoll.h>

/* survive the codechecker */
#undef UAPI_HMKERNEL_CAPABILITY_CAPTYPES_H

#endif
