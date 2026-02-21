/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Captype channel
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 4 07:56:13 2018
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(Channel, 4, CAPALLGRANT(Channel)
		    CAPNOGRANTMOVE(Channel)
		    CAPNODEFAULTCREF(Channel)
		    CAPRAWCREFRLOOKUP(Channel)
		    CAPDEFAULTSIZE(Channel),
	CAPMETHOD(Channel, MsgReceive)
	CAPMETHOD(Channel, MsgNotify)
	)

#ifdef CAP_EXPORT_API

#include <hmasm/types.h>

enum {
	CRIGHT_CHANNEL_MSG_RECEIVE_BIT,
	CRIGHT_CHANNEL_MSG_NOTIFY_BIT,
	CRIGHT_CHANNEL_MAX
};

/* sysarg_channel_rights */
#define CRIGHT_CHANNEL_MSG_RECEIVE	(1U << CRIGHT_CHANNEL_MSG_RECEIVE_BIT)
#define CRIGHT_CHANNEL_MSG_NOTIFY	(1U << CRIGHT_CHANNEL_MSG_NOTIFY_BIT)

struct sysarg_msginfo {
	__u64 src_tcb_cref;
	__u32 src_cnode_idx;
	__u32 msg_size;
};

#endif
