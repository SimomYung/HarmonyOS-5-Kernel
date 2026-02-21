/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Sub-controllers of resmgr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 02 11:12:08 2023
 */

#ifdef CONFIG_RESMGR_FREEZER
RES_CONTROLLER(freezer)
#endif
#ifdef CONFIG_RESMGR_PIDS
RES_CONTROLLER(pids)
#endif
#ifdef CONFIG_RESMGR_CPUSET
RES_CONTROLLER(cpuset)
#endif
#ifdef CONFIG_RESMGR_MEMORY
RES_CONTROLLER(memory)
#endif
#ifdef CONFIG_RESMGR_CPU
RES_CONTROLLER(cpu)
#endif
#ifdef CONFIG_RESMGR_LOAD
RES_CONTROLLER(load)
#endif
#ifdef CONFIG_RESMGR_IOLIMIT
RES_CONTROLLER(iolimit)
#endif
#ifdef CONFIG_RESMGR_WORKINGSET
RES_CONTROLLER(workingset)
#endif
#ifdef CONFIG_RESMGR_DUMMY
RES_CONTROLLER(dummy)
#endif
