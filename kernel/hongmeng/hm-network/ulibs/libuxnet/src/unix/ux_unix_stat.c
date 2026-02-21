/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Get netstats for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 19 10:21:00 2020
 */
#include "ux_unix_util.h"
#include "ux_unix_stat.h"

static struct raw_rwlock ux_unix_lock = RAW_RDLOCK_INITIALIZER;
static DLIST_HEAD(ux_unix_list);
static unsigned int usk_cnt = 0;

void delete_unixlist(ux_unix_sock_t *usk)
{
	(void)raw_rwlock_wrlock(&ux_unix_lock);
	dlist_delete(&usk->head);
	usk_cnt--;
	(void)raw_rwlock_unlock(&ux_unix_lock);
}

/* the socket max number limited by hm share mem num CONFIG_SYSMGR_SHM_NODE_MAX_NUM */
void insert_unixlist(ux_unix_sock_t *usk)
{
	(void)raw_rwlock_wrlock(&ux_unix_lock);
	dlist_insert(&ux_unix_list, &usk->head);
	usk_cnt++;
	(void)raw_rwlock_unlock(&ux_unix_lock);
}

static int do_get_netstat(ux_unix_sock_t *usk, char *string, int offset, int mlen)
{
	int ret;

	dlist_for_each_entry(usk, &ux_unix_list, ux_unix_sock_t, head) {
		ux_sock_t *sk = &usk->sk;
		ux_socket_t *sock = ux_epoll_ctx_get_socket(&usk->sk.epoll_ctx);

		if (mlen < offset) {
			return E_HM_INVAL;
		}

		ret = sprintf_s(string + offset, (size_t)(unsigned int)(mlen - offset),
				"0: %08X 0 %X %X %X %llu ",
				(unsigned int)raw_refcnt_nr_counter(&sk->refcnt),
				(unsigned)(usk->state == SS_LISTENING ?
				__SO_ACCEPTCON : 0), (unsigned)usk->type,
				(unsigned)(usk->state == SS_LISTENING ?
				SS_UNCONNECTED : usk->state),
				(sock != NULL) ? ux_socket_ino(sock) : 0);
		if (ret < 0) {
			return E_HM_INVAL;
		}
		offset += ret;

		if (usk->pathlen != 0U) {
			if (mlen <= offset) {
				return E_HM_INVAL;
			}
			if (if_unix_sock_bind_abstract(usk)) {
				ret = snprintf_s(string + offset, (size_t)(unsigned int)(mlen - offset),
						usk->pathlen, "@%s", usk->path + 1);
			} else {
				ret = snprintf_s(string + offset, (size_t)(unsigned int)(mlen - offset),
						usk->pathlen, "%s", usk->path);
			}
			if (ret < 0) {
				return E_HM_INVAL;
			}
			offset += ret;
		}

		if (mlen <= offset) {
			return E_HM_INVAL;
		}
		ret = sprintf_s(string + offset, (size_t)(unsigned int)(mlen - offset), "\n");
		if (ret < 0) {
			return E_HM_INVAL;
		}
		offset += ret;
	}
	return offset;
}

int get_unixlist_netstat(ux_unix_sock_t *usk, char *string, int *offset, int mlen)
{
	int ret;

	(void)raw_rwlock_rdlock(&ux_unix_lock);
	if (dlist_empty(&ux_unix_list) || usk_cnt == 0) {
		(void)raw_rwlock_unlock(&ux_unix_lock);
		return 0;
	}

	ret = do_get_netstat(usk, string, *offset, mlen - 1);
	if (ret < 0) {
		(void)raw_rwlock_unlock(&ux_unix_lock);
		return ret;
	}
	*offset = ret;
	(void)raw_rwlock_unlock(&ux_unix_lock);
	return 0;
}

int get_netstat_unix(char **pstring, int *len, int type)
{
	int ret;
	char *string = NULL;
	uint64_t mlen;
	int offset = 0;
	ux_unix_sock_t *usk = NULL;
	const char *headinfo = "Num RefCount Proto  Flags Type State Inode Path\n";

	UNUSED(type);

	/* length of "0: %08X 0 %d %d %d 0 %14s", %d length is 2-bits */
	const unsigned int formatlen = 36;

	if (pstring == NULL || len == NULL) {
		return E_HM_INVAL;
	}

	mlen = (uint64_t)strlen(headinfo) \
		+ (uint64_t)((USK_PATH_USER_MAX + formatlen) * usk_cnt) + 1;
	if (mlen > INT_MAX) {
		return E_HM_NOMEM;
	}

	string = calloc(1, (size_t)mlen);
	if (string == NULL) {
		return E_HM_NOMEM;
	}

	if ((strcpy_s(string, (size_t)mlen, headinfo)) != 0) {
		free(string);
		return E_HM_POSIX_FAULT;
	}
	offset += (int)strlen(headinfo);

	ret = get_unixlist_netstat(usk, string, &offset, (int)mlen);
	if (ret < 0) {
		free(string);
		return ret;
	}
	*len = offset;
	*pstring = string;
	return 0;
}

void release_netstat_unix(char *msg, int type)
{
	UNUSED(type);

	if (msg != NULL) {
		free(msg);
	}
}
