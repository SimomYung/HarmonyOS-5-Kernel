/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  UEV tracepoint collection
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 27 14:16:32 2021
 */

/* netdev */
#include <ux_netdev.h>
UEV_POINT(netd, netdev_notify,
	  UEV_ARG(3, char *, ifname, int, devno, int, ifevent),
	  UEV_TRACE_FIELD(2, char *, ifname, int, devno, int, ifevent),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "netdev_notify");
			   uev_data_write_string(&trace_data, ifname);
			   uev_data_write_int(&trace_data, devno);
			   uev_data_write_int(&trace_data, ifevent);
	  ),
	  UEV_TRACE_EFMT_FIELD(TRACE_STRING(STRING, char *, event)
			       TRACE_STRING(STRING, char *, ifname)
			       TRACE_FIELD(INT, int, devno)
			       TRACE_FIELD(INT, int, ifevent)
	  )
)

UEV_POINT(netd, netdev_xmit,
	  UEV_ARG(3, ux_netdev_t *, dev, int, len, int, flags),
	  UEV_TRACE_FIELD(3, ux_netdev_t *, dev, int, len, int, flags),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "netdev_xmit");
			   uev_data_write_string(&trace_data, dev->name);
			   uev_data_write_int(&trace_data, len);
			   uev_data_write_int(&trace_data, flags);
	  ),
	  UEV_TRACE_EFMT_FIELD(TRACE_STRING(STRING, char *, event)
			       TRACE_STRING(STRING, char *, dev->name)
			       TRACE_FIELD(INT, int, len)
			       TRACE_FIELD(INT, int, flags)
	  )
)

UEV_POINT(netd, netdev_recv,
	  UEV_ARG(2, ux_netdev_t *, dev, int, len),
	  UEV_TRACE_FIELD(2, ux_netdev_t *, dev, int, len),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "netdev_recv");
			   uev_data_write_string(&trace_data, dev->name);
			   uev_data_write_int(&trace_data, len);
	  ),
	  UEV_TRACE_EFMT_FIELD(TRACE_STRING(STRING, char *, event)
			       TRACE_STRING(STRING, char *, dev->name)
			       TRACE_FIELD(INT, int, len)
	  )
)

/* scm */
#include "ux_event.h" /* ux_scm_cookie_t */
UEV_POINT(netd, scm_send,
	  UEV_ARG(1, struct cmsghdr *, cmsg),
	  UEV_TRACE_FIELD(1, struct cmsghdr *, cmsg),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_send");
		  uev_data_write_int(&trace_data, cmsg->cmsg_len);
		  uev_data_write_int(&trace_data, cmsg->cmsg_level);
		  uev_data_write_int(&trace_data, cmsg->cmsg_type);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_NUM(unsigned int, cmsg_len)
		  OUTPUT_NUM(int, cmsg_level)
		  OUTPUT_NUM(int, cmsg_type)
	  )
)

UEV_POINT(netd, scm_send_log,
	  UEV_ARG(3, const char *, func, const char *, msg, int, rc),
	  UEV_TRACE_FIELD(3, const char *, func, const char *, msg, int, rc),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_send_log");
		  uev_data_write_string(&trace_data, func);
		  uev_data_write_string(&trace_data, msg);
		  uev_data_write_int(&trace_data, rc);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_STRING(function)
		  OUTPUT_STRING(message)
		  OUTPUT_NUM(int, rc)
	  )
)

UEV_POINT(netd, scm_send_cred,
	  UEV_ARG(1, ux_scm_creds_t *, cred),
	  UEV_TRACE_FIELD(1, ux_scm_creds_t *, cred),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_send_cred");
		  uev_data_write_int(&trace_data, cred->pid);
		  uev_data_write_int(&trace_data, cred->uid);
		  uev_data_write_int(&trace_data, cred->gid);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_NUM(int, pid)
		  OUTPUT_NUM(unsigned int, uid)
		  OUTPUT_NUM(unsigned int, gid)
	  )
)

UEV_POINT(netd, scm_send_fd,
	  UEV_ARG(2, struct cmsghdr *, cmsg, ux_scm_fd_t *, fdp),
	  UEV_TRACE_FIELD(2, struct cmsghdr *, cmsg, ux_scm_fd_t *, fdp),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_send_fd");
		  uev_data_write_int(&trace_data, cmsg->cmsg_len);
		  uev_data_write_int(&trace_data, cmsg->cmsg_level);
		  uev_data_write_int(&trace_data, cmsg->cmsg_type);
		  uev_data_write_int(&trace_data, fdp->count);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_NUM(unsigned int, cmsg_len)
		  OUTPUT_NUM(int, cmsg_level)
		  OUTPUT_NUM(int, cmsg_type)
		  OUTPUT_NUM(unsigned int, fdp_count)
	  )
)

UEV_POINT(netd, scm_recv_log,
	  UEV_ARG(3, const char *, func, const char *, msg, int, ret),
	  UEV_TRACE_FIELD(3, const char *, func, const char *, msg, int, ret),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_recv_log");
		  uev_data_write_string(&trace_data, func);
		  uev_data_write_string(&trace_data, msg);
		  uev_data_write_int(&trace_data, ret);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_STRING(function)
		  OUTPUT_STRING(message)
		  OUTPUT_NUM(int, ret)
	  )
)

UEV_POINT(netd, scm_detach_cred,
	  UEV_ARG(1, ux_scm_creds_t *, cred),
	  UEV_TRACE_FIELD(1, ux_scm_creds_t *, cred),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_detach_cred");
		  uev_data_write_int(&trace_data, cred->pid);
		  uev_data_write_int(&trace_data, cred->uid);
		  uev_data_write_int(&trace_data, cred->gid);
		  uev_data_write_int(&trace_data, cred->passcd);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_NUM(int, pid)
		  OUTPUT_NUM(unsigned int, uid)
		  OUTPUT_NUM(unsigned int, gid)
		  OUTPUT_NUM(unsigned int, passcd)
	  )
)

UEV_POINT(netd, scm_detach_fd,
	  UEV_ARG(3, struct msghdr *, msg, ux_scm_cookie_t *, scm, int, flags),
	  UEV_TRACE_FIELD(3, struct msghdr *, msg, ux_scm_cookie_t *, scm, int, flags),
	  UEV_TRACE_ASSIGN(
		  uev_data_write_string(&trace_data, "scm_detach_fd");
		  uev_data_write_int(&trace_data, msg->msg_controllen);
		  uev_data_write_int(&trace_data, scm->fds.count);
		  uev_data_write_int(&trace_data, scm->creds.passcd);
		  uev_data_write_int(&trace_data, flags);
	  ),
	  UEV_TRACE_EFMT_FIELD(
		  OUTPUT_EVTNAME(event)
		  OUTPUT_NUM(unsigned int, msg_controllen)
		  OUTPUT_NUM(unsigned int, scm_fdcount)
		  OUTPUT_NUM(unsigned int, scm_passcd)
		  OUTPUT_NUM(int, flags)
	  )
)
