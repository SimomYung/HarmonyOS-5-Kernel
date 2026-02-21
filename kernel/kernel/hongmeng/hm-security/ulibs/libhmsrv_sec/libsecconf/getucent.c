/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of getting user_conf entry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 29 12:45:05 2020
 */
// libc
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// hongmeng
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#include "libsecconf/getucent.h"

static FILE *g_file;
static char *g_line;
static struct sec_mac_user_conf g_userconf;
static size_t g_size;

/*
 * Read a userconf entry from file specified.
 * User need to open file outside function. 'f' is added in function name.
 * */
static int fread_ucent(FILE *f, struct sec_mac_user_conf *uc, char **line,
		       size_t *size, struct sec_mac_user_conf **res)
{
	ssize_t str_len;
	char *uc_str = NULL;
	char *context = NULL;
	int rv = E_HM_OK;
	int cs;
	struct sec_mac_user_conf *tmp_uc = uc;

	(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	str_len = getline(line, size, f);
	if (str_len < 0) {
		free(*line);
		*line = NULL;
		tmp_uc = NULL;
		hm_debug("read userconf failed\n");
		rv = E_HM_POSIX_NOENT;
		goto out;
	}

	uc_str = line[0];
	uc_str[str_len - 1] = '\0';
	tmp_uc->name = strtok_s(uc_str, ":", &context);
	tmp_uc->label = strtok_s(NULL, ":", &context);
	if (tmp_uc->name == NULL || tmp_uc->label == NULL) {
		hm_error("parse userconf failed\n");
		rv = E_HM_NOMEM;
	}
out:
	(void)pthread_setcancelstate(cs, NULL);
	*res = tmp_uc;
	return rv;
}

/*
 * Read a userconf entry from /etc/hmmac/userconf.
 * Different from previous function, this func will open file.
 * */
static int read_ucent(const char *name,
		      struct sec_mac_user_conf *uc,
		      char **buf,
		      size_t *size,
		      struct sec_mac_user_conf **res)
{
	FILE *f = NULL;
	int cs;
	int rv = E_HM_OK;
	*res = NULL;
	struct sec_mac_user_conf *tmp_uc = uc;

	(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	f = fopen(SEC_MAC_USER_CONF_PATH, "rbe");
	if (f == NULL) {
		hm_error("open userconf failed\n");
		rv = E_HM_POSIX_NOENT;
		goto out;
	}

	rv = fread_ucent(f, tmp_uc, buf, size, res);
	/*
	 * Loop termination condition:
	 * Following loop terminate when rv is not E_HM_OK or res is NULL, Since rv and res keep changing
	 * in loop body, it has a bound.
	 * Loop condition is constant and reasonable: read a userconf entry from /etc/hmmac/userconf,
	 * file size is limited, so the loop condition is reasonable.
	 */
	while (rv == E_HM_OK && *res != NULL) {
		if (name != NULL && (strcmp(name, (*res)->name) == 0)) {
			break;
		}
		rv = fread_ucent(f, tmp_uc, buf, size, res);
	}
	if (rv != E_HM_OK) {
		tmp_uc = NULL;
	}
	(void)fclose(f);
	*res = tmp_uc;
out:
	(void)pthread_setcancelstate(cs, NULL);
	return rv;
}

/*
 * Use this function to visit userconf file, when update userfile file.
 * if fail, res is NULL; if success, res point to local uc
 * */
struct sec_mac_user_conf *sec_mac_fget_userconf_entry(FILE *f)
{
	static char *read_line = NULL;
	static struct sec_mac_user_conf read_uc;
	size_t size = 0;
	struct sec_mac_user_conf *res = NULL;
	if (fread_ucent(f, &read_uc, &read_line, &size, &res) != E_HM_OK) {
		res = NULL;
	}
	return res;
}

// call before getucent
void sec_mac_set_userconf_entry(void)
{
	if (g_file != NULL) {
		(void)fclose(g_file);
	}
	g_file = NULL;
}

// call enducent after getucent finally
void sec_mac_end_userconf_entry(void)
{
	if (g_file != NULL) {
		(void)fclose(g_file);
	}
	g_file = NULL;
}

/*
 * Read each userconf entry sequentialy.
 * if fail, res is NULL; if success, res point to global g_userconf
 * */
struct sec_mac_user_conf *sec_mac_get_userconf_entry(void)
{
	struct sec_mac_user_conf *res = NULL;
	if (g_file == NULL) {
		g_file = fopen(SEC_MAC_USER_CONF_PATH, "rbe");
	}
	if (g_file != NULL) {
		if (fread_ucent(g_file, &g_userconf, &g_line, &g_size, &res) != E_HM_OK) {
			res = NULL;
		}
	}
	return res;
}

/*
 * get userconf entry by name, users need to supply mem space.
 * And the reuslt will store in the mem_space pointed by paramters.
 *
 * if fail, res is NULL; if success, res point to uc.
 * */
int sec_mac_get_userconf_by_name(const char *name,
				 struct sec_mac_user_conf *uc,
				 char *buf,
				 size_t size,
				 struct sec_mac_user_conf **res)
{
	char *line = NULL;
	size_t len = 0;
	int rv;
	int cs;

	if (name == NULL || uc == NULL || buf == NULL || res == NULL) {
		hm_error("param is invalid\n");
		rv = E_HM_INVAL;
	} else {
		(void)pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

		rv = read_ucent(name, uc, &line, &len, res);
		if (*res != NULL && size < len) {
			*res = NULL;
			hm_error("buf is too small\n");
			rv = E_HM_NOMEM;
			goto out;
		}
		if (*res != NULL) {
			int rc = memcpy_s(buf, size, line, len);
			if (rc != E_HM_OK) {
				hm_error("copy to buf failed\n");
				rv = E_HM_RANGE;
				goto out;
			}
			uc->name = ulong_to_ptr((ptr_to_ulong(uc->name) -
						ptr_to_ulong(line)) +
						ptr_to_ulong(buf), char);
			uc->label = ulong_to_ptr((ptr_to_ulong(uc->label) -
						 ptr_to_ulong(line)) +
						 ptr_to_ulong(buf), char);
		}
out:
		free(line);
		(void)pthread_setcancelstate(cs, NULL);
	}
	return rv;
}
