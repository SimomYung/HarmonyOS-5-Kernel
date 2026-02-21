/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 2 15:11:34 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_MSG_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_MSG_H

#include <sys/msg.h>
#include "hm_ipc_comm.h"

int hm_ipc_msgget(key_t key, int msgflg);

int hm_ipc_msgsnd(int msqid, struct msgbuf *buf, size_t msgsz, int msgflg);

int hm_ipc_msgrcv(int msqid, struct msgbuf *buf, size_t msgsz, long msgtyp, int msgflg);

int hm_ipc_msgctl(int msqid, int cmd, struct msqid_ds *buf);

#endif
