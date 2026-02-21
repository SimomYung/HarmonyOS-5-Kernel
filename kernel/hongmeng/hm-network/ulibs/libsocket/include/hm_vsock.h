/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header file for public vsock interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 6 11:18:01 2021
 */

#ifndef __HM_VSOCK_H_
#define __HM_VSOCK_H_

#include <stdlib.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SO_VM_SOCKETS_BUFFER_SIZE 0
#define SO_VM_SOCKETS_BUFFER_MIN_SIZE 1
#define SO_VM_SOCKETS_BUFFER_MAX_SIZE 2
#define SO_VM_SOCKETS_PEER_HOST_VM_ID 3
#define SO_VM_SOCKETS_TRUSTED 5
#define SO_VM_SOCKETS_CONNECT_TIMEOUT 6
#define SO_VM_SOCKETS_NONBLOCK_TXRX 7

#define VMADDR_CID_ANY (0xffffffffU)
#define VMADDR_PORT_ANY (0xffffffffU)
#define VMADDR_CID_HYPERVISOR 0
#define VMADDR_CID_LOCAL 1
#define VMADDR_CID_HOST 2

#define PF_VSOCK_HOST 1
#define PF_VSOCK_DEFAULT 0
#define SO_VSOCK_REGISTER_NETDEV 1
#define SO_VSOCK_UNREGISTER_NETDEV 2
#define HM_GUEST_CID_LENGTH 4

struct sockaddr_vm {
	sa_family_t svm_family;
	unsigned short svm_reserved1;
	unsigned int svm_port;
	unsigned int svm_cid;
	unsigned char svm_zero[(((sizeof(struct sockaddr) -
			       sizeof(sa_family_t)) -
			       sizeof(unsigned short)) -
			       sizeof(unsigned int)) - sizeof(unsigned int)];
};

#define IOCTL_VM_SOCKETS_GET_LOCAL_CID		_IO(7, 0xb9)

int hm_vsock_register_vm(unsigned int cid, unsigned int dev_group_id);
int hm_vsock_unregister_vm(unsigned int cid);

int hm_vsock_vm_tx(unsigned int cid, void *iov, size_t nr_iov);
int hm_vsock_vq_ready(unsigned int cid);

#ifdef __cplusplus
}
#endif

#endif
