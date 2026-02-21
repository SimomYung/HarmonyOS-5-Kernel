/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: interface for network seharmony
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 6 10:34:09 2023
 */

#include <libsec/chk.h>
#include <libsec/server.h>
#include <libsec/component.h>

#include <libhmseharmony/avc.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/permissions.h>
#include <libhmseharmony/seharmony.h>
#include <libhmseharmony/services.h>
#include <libhmseharmony/network.h>

#include <libhmactv/actv.h>
#include <hongmeng/errno.h>
#include <netinet/in.h>
#include <bits/ioctl.h>
#include <generated/hmsd/hook/net/seharmony.h>
#include "security/seharmony.h"
#include "ux_socket.h"
#include "hm_netlink.h"
#include "security/sel_node.h"
#include "security/sel_port.h"
#include "net_pac.h"

#define SIN6_LEN_RFC2133	24
#define offsetofend(TYPE, FIELD) \
	(offsetof(TYPE, FIELD) + sizeof(((TYPE *)0)->FIELD))

struct sk_security_struct {
	__sec_data(net, cntx) hmsel_sid_t sid; /* SID of this object */
	__sec_data(net, cntx) hmsel_sid_t initial_sid; /* SID of create sock */
	__sec_data(net, cntx) hmsel_sid_t peer_sid; /* SID of peer */
	__sec_data(net, cntx) hmsel_class_t sclass; /* sock security class */
};

struct sel_net_log_data {
	int cmd;
};

struct sel_net_log_arg {
	struct sel_net_log_data data;
	const struct sec_chk_cred *cred;
};

static int sel_sk_alloc_security(ux_sock_t *sk)
{
	struct sk_security_struct *sksec;
	int rc = E_HM_OK;

	sksec = calloc(1, sizeof(*sksec));
	if (!sksec) {
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		sksec->peer_sid = HM_SECINITSID_UNLABELED;
		sksec->sid = HM_SECINITSID_UNLABELED;
		sksec->sclass = HM_SECCLASS_SOCKET;
		sk->sk_security = sksec;
	}

	return rc;
}

static int sel_sk_free_security(ux_sock_t *sk)
{
	struct sk_security_struct *sksec = sk->sk_security;

	sk->sk_security = NULL;
	free(sksec);

	return 0;
}

static int socket_create_sid(const struct sec_cred_seharmony *seharmony_cred,
			     struct hmsel_sid_context ccon, hmsel_sid_t *sid)
{
	int rc = E_HM_OK;
	if (seharmony_cred->sockcreate_sid > HM_SECSID_NULL) {
		*sid = seharmony_cred->sockcreate_sid;
	} else {
		rc = hmsel_transition_sid(ccon, NULL, sid);
	}
	return rc;
}

static int sel_socket_create(const struct sec_chk_cred *cred, int domain, int type, int protocol)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct hmsel_sid_context ccon = {0};
	uint32_t newsid;
	int rc = E_HM_OK;

	ccon.tclass = socket_type_to_sclass(domain, type, protocol);
	ccon.ssid = seharmony_cred->sid;
	ccon.tsid = ccon.ssid;

	rc = socket_create_sid(seharmony_cred, ccon, &newsid);
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "failed to create sock sid, ret:%s\n", hmstrerror(rc));
	}

	if (rc == E_HM_OK) {
		ccon.tsid = newsid;
		rc = hmsel_avc_check(ccon, HM_SOCKET__CREATE);
	}

	return rc;
}

static int sel_socket_post_create(const struct sec_chk_cred *cred,
				  ux_socket_t *sock, int domain, int type, int protocol)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct sk_security_struct *sksec = NULL;
	struct hmsel_sid_context ccon = {0};
	uint32_t sid = HM_SECINITSID_KERNEL;
	int rc = E_HM_OK;

	ccon.tclass = socket_type_to_sclass(domain, type, protocol);
	ccon.ssid = seharmony_cred->sid;
	ccon.tsid = ccon.ssid;

	rc = socket_create_sid(seharmony_cred, ccon, &sid);
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "failed to create sock sid, ret:%s\n", hmstrerror(rc));
	}

	if (rc == E_HM_OK && sock->sk) {
		sksec = sock->sk->sk_security;
		sksec->sclass = ccon.tclass;
		sksec->sid = sid;
		sksec->initial_sid = sid;
	}

	return rc;
}

static int sel_socket_socketpair(ux_socket_t *socka, ux_socket_t *sockb)
{
	struct sk_security_struct *sksec_a = socka->sk->sk_security;
	struct sk_security_struct *sksec_b = sockb->sk->sk_security;

	sksec_a->peer_sid = sksec_b->sid;
	sksec_b->peer_sid = sksec_a->sid;

	return 0;
}

static int sksec_perm(const struct sec_chk_cred *cred, struct sk_security_struct *sksec, hmsel_av_t perms)
{
	struct hmsel_sid_context ccon = {0};
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);

	if(hmsel_sid_context_is_equal(sksec->sid, HM_SECINITSID_KERNEL)) {
		return 0;
	}

	ccon.tclass = sksec->sclass;
	ccon.tsid = sksec->sid;
	ccon.ssid = seharmony_cred->sid;

	return hmsel_avc_check(ccon, perms);
}

static int err_sctp(struct sk_security_struct *sksec)
{
	int rc;
	/* Note that SCTP services expect -EINVAL, whereas
	 * others expect -EAFNOSUPPORT.
	 */
	if (sksec->sclass == HM_SECCLASS_SCTP_SOCKET) {
		rc = E_HM_INVAL;
	} else {
		rc = E_HM_AFNOSUPPORT;
	}
	return rc;
}

static int unspec_state(unsigned short family_sa, struct sockaddr_in *addr4, struct sk_security_struct *sksec)
{
	int rc = E_HM_OK;
	if (family_sa == AF_UNSPEC) {
		/* see __inet_bind(), we only want to allow
		* AF_UNSPEC if the address is INADDR_ANY
		*/
		if (addr4->sin_addr.s_addr != htonl(INADDR_ANY)) {
			rc = err_sctp(sksec);
		}
	}
	return rc;
}

static int get_inet_port_addr(const struct sockaddr *address, socklen_t addrlen,
			      struct sk_security_struct *sksec, unsigned short *snum, char **addr)
{
	int rc = E_HM_OK;
	struct sockaddr_in *addr4 = NULL;
	struct sockaddr_in6 *addr6 = NULL;
	unsigned short family_sa;

	/*
	 * sctp_bindx(3) calls via seharmony_sctp_bind_connect()
	 * that validates multiple binding addresses. Because of this
	 * need to check address->sa_family as it is possible to have
	 * sk->sk_family = PF_INET6 with addr->sa_family = AF_INET.
	 */
	if (addrlen < offsetofend(struct sockaddr, sa_family)) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		family_sa = address->sa_family;
		switch (family_sa) {
		case AF_UNSPEC:
		case AF_INET:
			if (addrlen < sizeof(struct sockaddr_in)) {
				rc = E_HM_INVAL;
			}
			if (rc == E_HM_OK) {
				addr4 = (struct sockaddr_in *)address;
				rc = unspec_state(family_sa, addr4, sksec);
			}
			if (rc == E_HM_OK) {
				*snum = ntohs(addr4->sin_port);
				*addr = (char *)&addr4->sin_addr.s_addr;
			}
			break;
		case AF_INET6:
			if (addrlen < SIN6_LEN_RFC2133) {
				rc = E_HM_INVAL;
			}
			if (rc == E_HM_OK) {
				addr6 = (struct sockaddr_in6 *)address;
				*snum = ntohs(addr6->sin6_port);
				*addr = (char *)&addr6->sin6_addr.s6_addr;
			}
			break;
		default:
			rc = err_sctp(sksec);
		}
	}
	return rc;
}

static int inet_name_bind(ux_sock_t *sk, unsigned short port)
{
	struct sk_security_struct *sksec = sk->sk_security;
	hmsel_sid_t sid;
	int rc = E_HM_OK;
	struct hmsel_sid_context ccon = {0};

	if (port) {
		if (port < IPPORT_RESERVED) {
			rc = sel_netport_sid((uint8_t)(sk->socket_protocol), port, &sid);
			if (rc == E_HM_OK) {
				ccon.tclass = sksec->sclass;
				ccon.tsid = sid;
				ccon.ssid = sksec->sid;
				rc = hmsel_avc_check(ccon, HM_SOCKET__NAME_BIND);
			}
			return rc;
		}
	}
	return rc;
}

static int inet_node_bind(struct sk_security_struct *sksec, unsigned short family_sa, char *addr, socklen_t addrlen)
{
	hmsel_av_t node_perm;
	hmsel_sid_t sid;
	struct hmsel_sid_context ccon = {0};
	int rc = E_HM_OK;

	switch (sksec->sclass) {
	case HM_SECCLASS_TCP_SOCKET:
		node_perm = HM_TCP_SOCKET__NODE_BIND;
		break;

	case HM_SECCLASS_UDP_SOCKET:
		node_perm = HM_UDP_SOCKET__NODE_BIND;
		break;

	case HM_SECCLASS_DCCP_SOCKET:
		node_perm = HM_DCCP_SOCKET__NODE_BIND;
		break;

	case HM_SECCLASS_SCTP_SOCKET:
		node_perm = HM_SCTP_SOCKET__NODE_BIND;
		break;

	default:
		node_perm = HM_RAWIP_SOCKET__NODE_BIND;
		break;
	}

	rc = sel_netnode_sid(family_sa, addr, addrlen, &sid);
	if (rc == E_HM_OK) {
		ccon.tclass = sksec->sclass;
		ccon.tsid = sid;
		ccon.ssid = sksec->sid;
		rc = hmsel_avc_check(ccon, node_perm);
	}
	return rc;
}

static int sel_socket_bind(const struct sec_chk_cred *cred,
			   ux_socket_t *sock, const struct sockaddr *address, socklen_t addrlen)
{
	ux_sock_t *sk = sock->sk;
	struct sk_security_struct *sksec = sk->sk_security;
	int domain;
	int rc;

	domain = sk->socket_domain;
	rc = sksec_perm(cred, sksec, HM_SOCKET__BIND);
	if (rc == E_HM_OK && (domain == PF_INET || domain == PF_INET6)) {
		/* If PF_INET or PF_INET6, check name_bind permission for the port. */
		unsigned short family_sa;
		unsigned short snum = 0;
		char *addr = NULL;

		rc = get_inet_port_addr(address, addrlen, sksec, &snum, &addr);
		if (rc == E_HM_OK) {
			rc = inet_name_bind(sk, snum);
		}
		if (rc == E_HM_OK) {
			family_sa = address->sa_family;
			if (family_sa == AF_UNSPEC) {
				family_sa = AF_INET;
			}
			rc = inet_node_bind(sksec, family_sa, addr, addrlen);
		}
	}
	return rc;
}

static int get_connect_port(struct sk_security_struct *sksec,
			    const struct sockaddr *address, socklen_t addrlen, unsigned short *snum)
{
	struct sockaddr_in *addr4 = NULL;
	struct sockaddr_in6 *addr6 = NULL;
	int rc = E_HM_OK;

	/* sctp_connectx(3) calls via seharmony_sctp_bind_connect()
	 * that validates multiple connect addresses. Because of this
	 * need to check address->sa_family as it is possible to have
	 * sk->sk_family = PF_INET6 with addr->sa_family = AF_INET.
	 */
	switch (address->sa_family) {
	case AF_INET:
		addr4 = (struct sockaddr_in *)address;
		if (addrlen < sizeof(struct sockaddr_in)) {
			rc = E_HM_INVAL;
		}
		if (rc == E_HM_OK) {
			*snum = ntohs(addr4->sin_port);
		}
		break;
	case AF_INET6:
		addr6 = (struct sockaddr_in6 *)address;
		if (addrlen < SIN6_LEN_RFC2133) {
			rc = E_HM_INVAL;
		}
		if (rc == E_HM_OK) {
			*snum = ntohs(addr6->sin6_port);
		}
		break;
	default:
		rc = err_sctp(sksec);
	}
	return rc;
}

static int inet_name_connect(ux_sock_t *sk, const struct sockaddr *address, socklen_t addrlen)
{
	struct sk_security_struct *sksec = sk->sk_security;
	struct hmsel_sid_context ccon = {0};
	unsigned short snum = 0;
	hmsel_av_t perm;
	hmsel_sid_t sid;
	int rc = E_HM_OK;

	rc = get_connect_port(sksec, address, addrlen, &snum);
	if (rc == E_HM_OK) {
		rc = sel_netport_sid((uint8_t)(sk->socket_protocol), snum, &sid);
	}
	if (rc == E_HM_OK) {
		switch (sksec->sclass) {
		case HM_SECCLASS_TCP_SOCKET:
			perm = HM_TCP_SOCKET__NAME_CONNECT;
			break;
		case HM_SECCLASS_DCCP_SOCKET:
			perm = HM_DCCP_SOCKET__NAME_CONNECT;
			break;
		case HM_SECCLASS_SCTP_SOCKET:
			perm = HM_SCTP_SOCKET__NAME_CONNECT;
			break;
		default:
			break;
		}

		ccon.tclass = sksec->sclass;
		ccon.tsid = sid;
		ccon.ssid = sksec->sid;
		rc = hmsel_avc_check(ccon, perm);
	}
	return rc;
}

static int sel_socket_connect(const struct sec_chk_cred *cred, ux_socket_t *sock,
			      const struct sockaddr *address, socklen_t addrlen)
{
	ux_sock_t *sk = sock->sk;
	struct sk_security_struct *sksec = sk->sk_security;
	int rc;

	rc = sksec_perm(cred, sksec, HM_SOCKET__CONNECT);
	if (rc == E_HM_OK) {
		if (addrlen < offsetofend(struct sockaddr, sa_family)) {
			rc = E_HM_INVAL;
		}
	}

	/* connect(AF_UNSPEC) has special handling, as it is a documented
	 * way to disconnect the socket
	 */
	if ((rc == E_HM_OK) && (address->sa_family != AF_UNSPEC)) {
		/*
		* If a TCP, DCCP or SCTP socket, check name_connect permission
		* for the port.
		*/
		if (sksec->sclass == HM_SECCLASS_TCP_SOCKET ||
			sksec->sclass == HM_SECCLASS_DCCP_SOCKET ||
			sksec->sclass == HM_SECCLASS_SCTP_SOCKET) {
			rc = inet_name_connect(sk, address, addrlen);
		}
	}

	return rc;
}

static int sel_socket_listen(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__LISTEN);
}

static int sel_socket_accept(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__ACCEPT);
}

static int sel_socket_sendmsg(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__WRITE);
}

static int sel_socket_recvmsg(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__READ);
}

static int sel_socket_getsockname(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__GETATTR);
}

static int sel_socket_getpeername(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__GETATTR);
}

static int sel_socket_setsockopt(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__SETOPT);
}

static int sel_socket_getsockopt(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__GETOPT);
}

static int sel_socket_shutdown(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__SHUTDOWN);
}

static int sel_socket_unix_stream_connect(ux_sock_t *sock,
					  ux_sock_t *other, ux_sock_t *newsk)
{
	struct sk_security_struct *sksec_sock = sock->sk_security;
	struct sk_security_struct *sksec_other = other->sk_security;
	struct sk_security_struct *sksec_new = newsk->sk_security;
	struct hmsel_sid_context ccon = {0};
	int rc;

	ccon.tclass = sksec_other->sclass;
	ccon.tsid = sksec_other->sid;
	ccon.ssid = sksec_sock->sid;

	rc = hmsel_avc_check(ccon, HM_UNIX_STREAM_SOCKET__CONNECTTO);

	/* server child socket */
	sksec_new->peer_sid = sksec_sock->sid;

	sksec_new->sid = sksec_other->sid;

	/* connecting socket */
	sksec_sock->peer_sid = sksec_new->sid;

	return rc;
}

static int sel_socket_unix_may_send(ux_sock_t *sock, ux_sock_t *other)
{
	struct sk_security_struct *ssec = sock->sk_security;
	struct sk_security_struct *osec = other->sk_security;
	struct hmsel_sid_context ccon = {0};

	ccon.tclass = osec->sclass;
	ccon.tsid = osec->sid;
	ccon.ssid = ssec->sid;

	return hmsel_avc_check(ccon, HM_SOCKET__SENDTO);
}

static void sec_net_sel_audit(const void *c, const void *args, struct hmsel_audit_info *ai)
{
	UNUSED(c);
	const struct sel_net_log_data *data = (const struct sel_net_log_data *)args;
	int ret;

	size_t len = ai->len;
	ret = snprintf_s(ai->buf, len, len - 1, " ioctlcmd=0x%hx ", data->cmd);
	ai_update(ret, ai);
}

static int _sel_avc_perm(struct hmsel_sid_context scon,
			 hmsel_av_t perm, const struct sel_net_log_arg *arg)
{
	struct sec_sel_audit ad = {
		.func = sec_net_sel_audit,
		.cred = arg->cred,
		.data = &arg->data,
	};

	return hmsel_avc_query(scon, perm, &ad);
}

static int sel_avc_perm(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
			hmsel_av_t perm, const struct sel_net_log_arg *arg)
{
	struct hmsel_sid_context scon = {
		.ssid = ssid,
		.tsid = tsid,
		.tclass = tclass,
	};

	return _sel_avc_perm(scon, perm, arg);
}

static void sel_log_arg_init(struct sel_net_log_arg *arg,
			     const struct sec_chk_cred *c, int cmd)
{
	arg->cred = c;
	arg->data.cmd = cmd;
}

static int ioctl_fd_has_perm(const struct sec_chk_cred *cred, ux_socket_t *sock, int cmd)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct sk_security_struct *sksec = sock->sk->sk_security;
	struct sel_net_log_arg arg;
	int rc = E_HM_OK;

	if(!hmsel_sid_context_is_equal(seharmony_cred->sid, sksec->initial_sid)) {
		sel_log_arg_init(&arg, cred, cmd);
		rc = sel_avc_perm(seharmony_cred->sid, sksec->sid, HM_SECCLASS_FD, HM_FD__USE, &arg);
	}
	return rc;
}

static int ioctl_sock_has_perm(const struct sec_chk_cred *cred, ux_socket_t *sock, int cmd, hmsel_av_t perms)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct sk_security_struct *sksec = sock->sk->sk_security;
	struct sel_net_log_arg arg;
	int rc;

	rc = ioctl_fd_has_perm(cred, sock, cmd);
	if (rc == E_HM_OK) {
		sel_log_arg_init(&arg, cred, cmd);
		rc = sel_avc_perm(seharmony_cred->sid, sksec->sid, sksec->sclass, perms, &arg);
	}
	return rc;
}

static int sel_xperms_check(const struct sec_chk_cred *cred, ux_socket_t *sock, hmsel_av_t perms, uint16_t cmd)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct sk_security_struct *sksec = sock->sk->sk_security;
	struct sel_net_log_arg arg;

	sel_log_arg_init(&arg, cred, cmd);
	uint8_t driver = cmd >> 8;
	uint8_t xperms = cmd & 0xff;
	struct hmsel_sid_context scon = {
		.ssid = seharmony_cred->sid,
		.tsid = sksec->sid,
		.tclass = sksec->sclass,
	};
	struct sec_sel_audit ad = {
		.func = sec_net_sel_audit,
		.cred = arg.cred,
		.data = &arg.data,
	};
	return hmsel_avc_xperms_query(scon, perms, driver, xperms, &ad);
}

static int _sel_check_ioctl(const struct sec_chk_cred *cred, ux_socket_t *sock, int cmd, hmsel_av_t perms)
{
	int rc;

	rc = ioctl_fd_has_perm(cred, sock, cmd);
	if (rc == E_HM_OK) {
		rc = sel_xperms_check(cred, sock, perms, (unsigned short) cmd);
	}

	return rc;
}

static int sel_socket_ioctl(const struct sec_chk_cred *cred, ux_socket_t *sock, int cmd)
{
	int ret = E_HM_OK;

	switch (cmd) {
	case FIONREAD:
		ret = ioctl_sock_has_perm(cred, sock, cmd, HM_FILE__GETATTR);
		break;

	case FIONBIO:
	case FIOASYNC:
		ret = ioctl_fd_has_perm(cred, sock, cmd);
		break;

	case FIOCLEX:
	case FIONCLEX:
		if (!hmsel_policycap_ioctl_skip_cloexec())
			ret = _sel_check_ioctl(cred, sock, cmd, HM_FILE__IOCTL);
		break;

	default:
		ret = _sel_check_ioctl(cred, sock, cmd, HM_FILE__IOCTL);
	}
	return ret;
}

static int sel_socket_fchown(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct sk_security_struct *sksec = sock->sk->sk_security;
	struct sel_net_log_arg arg;

	sel_log_arg_init(&arg, cred, 0);
	return sel_avc_perm(seharmony_cred->sid, sksec->sid, sksec->sclass, HM_FILE__SETATTR, &arg);
}

static int sel_socket_fcntl(const struct sec_chk_cred *cred, ux_socket_t *sock, int cmd)
{
	int ret = E_HM_OK;

	switch (cmd) {
	case F_GETFL:
	case F_SETFL:
		ret = ioctl_fd_has_perm(cred, sock, cmd);
		break;

	default:
		break;
	}

	return ret;
}

static int sel_socket_fstat(const struct sec_chk_cred *cred, ux_socket_t *sock)
{
	struct sec_cred_seharmony *seharmony_cred = sel_cred_of_sec_cred(cred);
	struct sk_security_struct *sksec = sock->sk->sk_security;
	struct sel_net_log_arg arg;

	sel_log_arg_init(&arg, cred, 0);
	return sel_avc_perm(seharmony_cred->sid, sksec->sid, sksec->sclass, HM_FILE__GETATTR, &arg);
}

int sec_chk_sel_socket_perms(const struct sec_chk_cred *cred,
			     ux_socket_t *sock)
{
	return sksec_perm(cred, sock->sk->sk_security, HM_SOCKET__READ | HM_SOCKET__WRITE);
}

static int sel_socket_getpeersec_stream(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	if (optval == NULL || sock == NULL ||
	    sock->sk == NULL || sock->sk->sk_security == NULL) {
		return E_HM_INVAL;
	}
	int ret;
	char *scon = NULL;
	uint32_t scon_len = 0u;
	struct sk_security_struct *sksec = sock->sk->sk_security;
	hmsel_sid_t sid = HM_SECSID_NULL;
	hmsel_class_t sclass = sksec->sclass;
	if (sclass == HM_SECCLASS_UNIX_STREAM_SOCKET ||
	    sclass == HM_SECCLASS_TCP_SOCKET ||
	    sclass == HM_SECCLASS_SCTP_SOCKET) {
		sid = sksec->peer_sid;
	}
	if (sid == HM_SECSID_NULL) {
		return E_HM_AFNOSUPPORT;
	}
	ret = hmsel_sid_to_context(sid, &scon, &scon_len);
	if (ret != E_HM_OK) {
		return ret;
	}
	if (scon_len > optlen) {
		free(scon);
		return E_HM_INVAL;
	}

	if (hm_actv_write_xact_vm(optval, scon, scon_len) != E_HM_OK) {
		free(scon);
		return E_HM_FAULT;
	}

	free(scon);
	return E_HM_OK;
}

static int sel_socket_post_accept(ux_socket_t *sock, ux_socket_t *nsock)
{
	struct sk_security_struct *sksec = NULL;
	struct sk_security_struct *nsksec = NULL;

	if (sock != NULL && sock->sk != NULL && nsock != NULL && nsock->sk != NULL) {
		sksec = sock->sk->sk_security;
		nsksec = nsock->sk->sk_security;
		nsksec->sclass = sksec->sclass;
		nsksec->sid = sksec->sid;
		nsksec->initial_sid = sksec->sid;
		return E_HM_OK;
	} else {
		net_error(HMNET_COREM, "accept new socket security error\n");
		return E_HM_INVAL;
	}
}

SEC_DEFINE_COMPONENT(net, seharmony,
			 sec_define_hook_method(sk_alloc_security, sel_sk_alloc_security),
			 sec_define_hook_method(sk_free_security, sel_sk_free_security),
			 sec_define_chk_method(socket_create, sel_socket_create),
			 sec_define_chk_method(socket_post_create, sel_socket_post_create),
			 sec_define_hook_method(socket_socketpair, sel_socket_socketpair),
			 sec_define_chk_method(socket_bind, sel_socket_bind),
			 sec_define_chk_method(socket_connect, sel_socket_connect),
			 sec_define_chk_method(socket_listen, sel_socket_listen),
			 sec_define_chk_method(socket_accept, sel_socket_accept),
			 sec_define_hook_method(socket_post_accept, sel_socket_post_accept),
			 sec_define_chk_method(socket_sendmsg, sel_socket_sendmsg),
			 sec_define_chk_method(socket_recvmsg, sel_socket_recvmsg),
			 sec_define_chk_method(socket_getsockname, sel_socket_getsockname),
			 sec_define_chk_method(socket_getpeername, sel_socket_getpeername),
			 sec_define_chk_method(socket_setsockopt, sel_socket_setsockopt),
			 sec_define_chk_method(socket_getsockopt, sel_socket_getsockopt),
			 sec_define_chk_method(socket_shutdown, sel_socket_shutdown),
			 sec_define_hook_method(unix_stream_connect, sel_socket_unix_stream_connect),
			 sec_define_hook_method(unix_may_send, sel_socket_unix_may_send),
			 sec_define_chk_method(socket_ioctl, sel_socket_ioctl),
			 sec_define_chk_method(socket_fchown, sel_socket_fchown),
			 sec_define_chk_method(socket_fcntl, sel_socket_fcntl),
			 sec_define_chk_method(socket_fstat, sel_socket_fstat),
			 sec_define_hook_method(socket_getpeersec_stream, sel_socket_getpeersec_stream)
			);
#define ENABLE_SEC_MOD_SEHARMONY
#include <generated/hmsd/module/net/seharmony.h>
