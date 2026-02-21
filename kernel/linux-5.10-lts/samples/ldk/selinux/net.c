// SPDX-License-Identifier: GPL-2.0
#include <net/netlink.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <security.h>
#include <objsec.h>

#include "sec.h"
#include "av_permissions.h"

int __override security_sk_alloc(struct sock *sk, int family, gfp_t priority)
{
	struct sk_security_struct *sksec;

	sksec = kzalloc(sizeof(*sksec), priority);
	if (!sksec)
		return -ENOMEM;

	sksec->peer_sid = SECINITSID_UNLABELED;
	sksec->sid = SECINITSID_UNLABELED;
	sksec->sclass = SECCLASS_SOCKET;

	sk->sk_security = sksec;

	return 0;
}

void __override security_sk_free(struct sock *sk)
{
	struct sk_security_struct *sksec = sk->sk_security;

	sk->sk_security = NULL;
	kfree(sksec);
}

void __override security_sk_clone(const struct sock *sk, struct sock *newsk)
{
	struct sk_security_struct *sksec = sk->sk_security;
	struct sk_security_struct *newsksec = newsk->sk_security;

	newsksec->sid = sksec->sid;
	newsksec->peer_sid = sksec->peer_sid;
	newsksec->sclass = sksec->sclass;
}

static int sock_has_perm(struct sock *sk, u32 perms)
{
	struct sk_security_struct *sksec = sk->sk_security;

	if (sksec->sid == SECINITSID_KERNEL)
		return 0;

	return sec_avc_perm(sksec->sid, sksec->sclass, perms);
}

int __override security_socket_post_create(struct socket *sock, int family, int type, int protocol, int kern)
{
	struct sk_security_struct *sksec;
	u32 sid = SECINITSID_KERNEL;
	int err = 0;
	unsigned int tsid;
	unsigned short tclass;
	err = sec_current_task_sid(&tsid);
	if (err == 0) {
		tclass = socket_type_to_secclass(family, type, protocol);

		if (!kern) {
			err = socket_sockcreate_sid(tsid, tclass, &sid);
		}
	}

	if (err == 0 && sock->sk) {
		sksec = sock->sk->sk_security;
		sksec->sclass = tclass;
		sksec->sid = sid;
	}

	return err;
}

int __override security_tun_dev_alloc_security(void **security)
{
	struct tun_security_struct *tunsec;
	int rc = 0;

	tunsec = kzalloc(sizeof(*tunsec), GFP_KERNEL);
	if (!tunsec) {
		rc = -ENOMEM;
	}
	if (rc == 0) {
		rc = sec_current_task_sid(&tunsec->sid);
	}
	if (rc == 0) {
		*security = tunsec;
	} else {
		if (tunsec != NULL) {
			kfree(tunsec);
		}
	}
	return rc;
}

void __override security_tun_dev_free_security(void *security)
{
	kfree(security);
}

int __override security_tun_dev_create(void)
{
	u32 tsid;
	int ret = sec_current_task_sid(&tsid);
	if (ret == 0)
	{
		ret = sec_avc_perm(tsid, SECCLASS_TUN_SOCKET, TUN_SOCKET__CREATE);
	}
	return ret;
}

int __override security_tun_dev_attach_queue(void *security)
{
	struct tun_security_struct *tunsec = security;
	return sec_avc_perm(tunsec->sid, SECCLASS_TUN_SOCKET, TUN_SOCKET__ATTACH_QUEUE);
}

int __override security_tun_dev_attach(struct sock *sk, void *security)
{
	struct tun_security_struct *tunsec = security;
	struct sk_security_struct *sksec = sk->sk_security;

	sksec->sid = tunsec->sid;
	sksec->sclass = SECCLASS_TUN_SOCKET;

	return 0;
}

int __override security_tun_dev_open(void *security)
{
	struct tun_security_struct *tunsec = security;
	int rc;
	int sid;

	rc = sec_avc_perm(tunsec->sid, SECCLASS_TUN_SOCKET, TUN_SOCKET__RELABELFROM);
	if (rc == 0) {
		rc = sec_current_task_sid(&sid);
	}
	if (rc == 0) {
		rc = sec_avc_perm(sid, SECCLASS_TUN_SOCKET, TUN_SOCKET__RELABELTO);
	}
	if (rc == 0) {
		rc = sec_current_task_sid(&sid);
	}
	if (rc == 0) {
		tunsec->sid = sid;
	}

	return rc;
}

int __override security_netlink_send(struct sock *sk, struct sk_buff *skb)
{
	int rc = 0;
	unsigned int msg_len;
	unsigned int data_len = skb->len;
	unsigned char *data = skb->data;
	struct nlmsghdr *nlh;
	struct sk_security_struct *sksec = sk->sk_security;
	u16 sclass = sksec->sclass;
	u32 perm;

	while (data_len >= nlmsg_total_size(0)) {
		nlh = (struct nlmsghdr *)data;
		if (nlh->nlmsg_len < NLMSG_HDRLEN || nlh->nlmsg_len > data_len)
			return 0;

		rc = selinux_nlmsg_lookup(sclass, nlh->nlmsg_type, &perm);
		if (rc == 0) {
#ifdef CONFIG_NETLINK_ROUTE_NLMSG_READPRIV
			struct net *net = sock_net(sk);
			if (net->core.sysctl_getlink_disable && sclass == SECCLASS_NETLINK_ROUTE_SOCKET
			    && nlh->nlmsg_type == RTM_GETLINK)
				perm = NETLINK_ROUTE_SOCKET__NLMSG_READPRIV;
#endif
			rc = sock_has_perm(sk, perm);
			if (rc)
				return rc;
		} else if (rc == -EINVAL) {
			/* -EINVAL is a missing msg/perm mapping */
			pr_warn_ratelimited("SELinux: unrecognized netlink\n");
			if (enforcing_enabled(&selinux_state) &&
			    !security_get_allow_unknown(&selinux_state))
				return rc;
			rc = 0;
		} else if (rc == -ENOENT) {
			/* -ENOENT is a missing socket/class mapping, ignore */
			rc = 0;
		} else {
			return rc;
		}

		/* move to the next message after applying netlink padding */
		msg_len = NLMSG_ALIGN(nlh->nlmsg_len);
		if (msg_len >= data_len)
			return 0;
		data_len -= msg_len;
		data += msg_len;
	}

	return rc;
}

