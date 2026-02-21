/* vi: set sw=4 ts=4: */
/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022.
 * Description: Implementation of restorecon
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 22 14:48:13 2022
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/xattr.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "hmmac/restorecon.h"
#include "libbb.h"
#include "securec.h"

#define FILE_CONTEXT_ENTRY_GROW_NUM 16
#define FILE_PATH_RESERVE_LEN 2              // for terminator and slash
#define PCRE2_ERROR_MESSAGE_LEN 256
#define PCRE2_PATTERN_RESERVE_LEN 3          // for terminator, ^ and $
#define FILE_MODE_LEN  2

struct file_context_entry {
	char *ctx;
	char *regex;
	char *type;
	mode_t mode;
	pcre2_code *regex_code;
	pcre2_match_data *match_data;
};

struct file_context_data {
	struct file_context_entry *context;
	unsigned int count;
	unsigned int index;
};

struct type_to_mode {
	char type;
	mode_t mode;
};

static struct file_context_data g_context_data;

void set_matchpathcon_flags(unsigned int flags)
{
	(void)flags;
}

void set_matchpathcon_canoncon(int (*f) (const char *p, unsigned l, char **c))
{
	(void)f;
}

static int grow_context_array(void)
{
	struct file_context_entry *new_list = NULL;
	unsigned int new_num;
	size_t entry_size;
	if (g_context_data.index < g_context_data.count) {
		return 0;
	}
	new_num = g_context_data.index + FILE_CONTEXT_ENTRY_GROW_NUM;
	entry_size = new_num * sizeof(struct file_context_entry);
	new_list = malloc(entry_size);
	if (new_list == NULL) {
		bb_error_msg("grow list failed\n");
		return -1;
	}
	if (memset_s(new_list, entry_size, 0, entry_size) != 0) {
		bb_error_msg("memset entry failed\n");
		free(new_list);
		return -1;
	}
	if ((g_context_data.count > 0) &&
		(memcpy_s(new_list, entry_size, g_context_data.context,
			     g_context_data.count * sizeof(struct file_context_entry)) != 0)) {
		bb_error_msg("memcpy entry failed\n");
		free(new_list);
		return -1;
	}
	free(g_context_data.context);
	g_context_data.context = new_list;
	g_context_data.count = new_num;
	return 0;
}

static int parse_entry(char **entry, char **buf, size_t *len)
{
	char *tmp_buf = NULL;
	size_t tmp_len = 0UL;
	int rc = 0;

	while (isspace(**buf) != 0 && **buf != '\0') {
		(*buf)++;
	}

	tmp_buf = *buf;

	while ((isspace(**buf) == 0) && (**buf != '\0')) {
		if (isascii((int)(**buf)) == 0) {
			bb_error_msg("parse fc entry failed: not ascii\n");
			rc = -1;
		}
		(*buf)++;
		tmp_len++;
	}

	if ((rc == 0) && (tmp_len > 0UL)) {
		*entry = strndup(tmp_buf, tmp_len);
		if (!*entry) {
			bb_error_msg("parse fc entry failed: dup entry failed\n");
			rc = -1;
		}
	}
	*len = tmp_len;

	return rc;
}

static int compile_regex(struct file_context_entry *entry, const char *path, unsigned int line_num)
{
	size_t size = strlen(entry->regex);
	char *tmp_reg = NULL;
	char *pattern = NULL;
	int error_code;
	PCRE2_SIZE error_offset;

	pattern = malloc(size + PCRE2_PATTERN_RESERVE_LEN);
	if (pattern == NULL) {
		bb_error_msg("compile regex failed: malloc failed\n");
		return -1;
	}
	tmp_reg = pattern;
	*tmp_reg++ = '^';
	if (memcpy_s(tmp_reg, size, entry->regex, size) != 0) {
		bb_error_msg("compile regex failed: memcpy failed\n");
		free(pattern);
		return -1;
	}
	tmp_reg += size;
	*tmp_reg++ = '$';
	*tmp_reg = '\0';

	entry->regex_code = pcre2_compile((PCRE2_SPTR)pattern, PCRE2_ZERO_TERMINATED, PCRE2_DOTALL,
				   &error_code, &error_offset, NULL);
	if (entry->regex_code == NULL) {
		char buffer[PCRE2_ERROR_MESSAGE_LEN];
		if (error_code == 0) {
			bb_error_msg("%s:line %u has invalid regex %s:REGEX back-end error: no error code At offset %zu:\n",
				     path, line_num, entry->regex, error_offset);
		} else {
			(void)pcre2_get_error_message(error_code,
						      (PCRE2_UCHAR *)buffer,
						      PCRE2_ERROR_MESSAGE_LEN);
			buffer[PCRE2_ERROR_MESSAGE_LEN - 1] = '\0';
			bb_error_msg("%s:line %u has invalid regex %s:REGEX back-end error: At offset %zu: %s\n",
				     path, line_num, entry->regex, error_offset, buffer);
		}
		free(pattern);
		return -1;
	}
	entry->match_data = pcre2_match_data_create_from_pattern(entry->regex_code, NULL);
	if (entry->match_data == NULL) {
		bb_error_msg("%s:  line %u create match data failed\n",
			     path, line_num);
		pcre2_code_free(entry->regex_code);
		free(pattern);
		return -1;
	}

	free(pattern);
	return 0;
}

static mode_t string_to_mode(char *mode)
{
	size_t len;
	static struct type_to_mode type[] = {
		{'b', S_IFBLK},
		{'c', S_IFCHR},
		{'d', S_IFDIR},
		{'p', S_IFIFO},
		{'l', S_IFLNK},
		{'s', S_IFSOCK},
		{'-', S_IFREG},
	};

	if (!mode) {
		return 0;
	}
	len = strlen(mode);
	if ((mode[0] != '-') || (len != FILE_MODE_LEN)) {
		return 0;
	}
	for (unsigned int i = 0U; i < ARRAY_SIZE(type); i++) {
		if (mode[1] == type[i].type) {
			return type[i].mode;
		}
	}
	return 0;
}

static int parse_line(char *buf, const char *file_path, unsigned int line_num)
{
	char *path = NULL;
	char *type = NULL;
	char *context = NULL;
	int rc;
	size_t len = 0;
	unsigned int index;

	rc = parse_entry(&path, &buf, &len);
	if ((rc != 0) || (len == 0UL)) {
		bb_error_msg("%s: line %u parse path failed\n", file_path, line_num);
		return -1;
	}
	rc = parse_entry(&type, &buf, &len);
	if ((rc != 0) || (len == 0UL)) {
		bb_error_msg("%s: line %u parse type failed\n", file_path, line_num);
		return -1;
	}
	rc = parse_entry(&context, &buf, &len);
	if (len == 0UL) {
		context = type;
		type = NULL;
	}
	if (grow_context_array() != 0) {
		bb_error_msg("parse fc line failed: grow failed\n");
		return -1;
	}
	index = g_context_data.index;
	g_context_data.context[index].regex = path;

	g_context_data.context[index].type = type;
	g_context_data.context[index].mode = 0;
	if (type != NULL) {
		mode_t mode = string_to_mode(type);
		g_context_data.context[index].mode = mode;
	}

	g_context_data.context[index].ctx = context;

	rc = compile_regex(&g_context_data.context[index], file_path, line_num);
	if (rc != 0) {
		return -1;
	}
	g_context_data.index++;

	return 0;
}

static int parse_file(const char *path)
{
	int rc = 0;
	char *line_buf = NULL;
	size_t line_len;
	FILE *fp = NULL;
	unsigned int line_num = 0;

	fp = fopen(path, "r");
	if (fp == NULL) {
		bb_error_msg("parse file failed: open %s failed, errno=%s\n", path, strerror(errno));
		return -1;
	}
	while (getline(&line_buf, &line_len, fp) > 0 && rc == 0) {
		line_num++;
		rc = parse_line(line_buf, path, line_num);
	}
	free(line_buf);
	(void)fclose(fp);
	return rc;
}

#define FILE_CONTEXTS  "/etc/hmmac/contexts/files/file_contexts"
/* Parse the file_contexts and load entries with regular expressions. */
int matchpathcon_init_prefix(const char *path, const char *subset)
{
	(void)path;
	(void)subset;
	return parse_file(FILE_CONTEXTS);
}

int matchpathcon_init(const char *path)
{
	return matchpathcon_init_prefix(path, NULL);
}

void matchpathcon_fini_prefix(void)
{
	if (g_context_data.context != NULL) {
		for (unsigned int i = 0; i < g_context_data.index; i++) {
			struct file_context_entry *entry = &g_context_data.context[i];
			pcre2_code_free(entry->regex_code);
			pcre2_match_data_free(entry->match_data);
			free(entry->type);
			free(entry->ctx);
			free(entry->regex);
		}
		free(g_context_data.context);
	}
}

void matchpathcon_checkmatches(char *str __attribute__((unused)))
{
}

void set_matchpathcon_printf(void (*f) (const char *fmt, ...))
{
	(void)f;
}

void matchpathcon_filespec_eval(void)
{
}

void matchpathcon_filespec_destroy(void)
{
}

static char *match_context(const char *name, mode_t mode)
{
	for (unsigned int i = g_context_data.index; i >= 1U; i--) {
		struct file_context_entry *entry = &g_context_data.context[i - 1];
		int rc;
		if ((mode == 0) || (entry->mode == 0) || ((mode & S_IFMT) == entry->mode)) {
			rc = pcre2_match(entry->regex_code, (PCRE2_SPTR)name, PCRE2_ZERO_TERMINATED,
					 0, 0, entry->match_data, NULL);
			if (rc > 0) {
				return entry->ctx;
			}
		}
	}
	return NULL;
}

static char *absolute_path(const char *name, char *buf, size_t buf_len)
{
	char *tmp_name = NULL;
	char *tmp = NULL;
	char *path = NULL;
	size_t path_len;

	if (name[0] == '/') {
		if (strcpy_s(buf, buf_len, name) != 0) {
			bb_error_msg("strcpy failed\n");
			return NULL;
		}
		return buf;
	}
	tmp_name = strdup(name);
	if (tmp_name == NULL) {
		bb_error_msg("get absolute path failed:%s\n", name);
		return NULL;
	}
	tmp = strrchr(tmp_name, '/');
	if (tmp != NULL) {
		*tmp = '\0';
		tmp++;
		path = realpath(tmp_name, buf);
	} else {
		tmp = tmp_name;
		path = realpath("/", buf);
	}
	if (path == NULL) {
		bb_error_msg("get realpath failed:%s\n", name);
	} else {
		path_len = strlen(path);
		if (path_len + strlen(tmp) + FILE_PATH_RESERVE_LEN > PATH_MAX) {
			bb_error_msg("Filename too long:%s\n", name);
			path = NULL;
		} else {
			*(buf + path_len) = '/';
			if (strcpy_s((buf + path_len + 1U), (buf_len - path_len) - 1U, tmp) != 0) {
				bb_error_msg("strcpy failed\n");
				path = NULL;
			}
		}
	}
	free(tmp_name);
	return path;
}
/* Matches the specified pathname and pattern with the configuration in file_contexts and
 * set *con to refer to the resulting context. */
int matchpathcon_index(const char *name, mode_t mode, char **con)
{
	int rc = 0;
	char path[PATH_MAX + 1];
	char *ctx = NULL;
	char *p = NULL;
	const char *local_name = name;

	if (g_context_data.context == NULL) {
		rc = matchpathcon_init_prefix(NULL, NULL);
	}
	if (rc != 0) {
		bb_error_msg("match index failed: init failed\n");
		return -1;
	}
	if (S_ISLNK(mode)) {
		p = absolute_path(local_name, path, PATH_MAX + 1);
	} else {
		p = realpath(local_name, path);
	}
	if (p != NULL) {
		local_name = p;
	}
	ctx = match_context(local_name, mode);
	if (ctx != NULL) {
		*con = strdup(ctx);
		rc = 0;
	} else {
		rc = -1;
	}
	return rc;
}

int matchpathcon_filespec_add(ino_t ino, int specind, const char *file)
{
	(void)ino;
	(void)specind;
	(void)file;
	return 0;
}

int security_canonicalize_context_raw(const char *con, char **canoncon)
{
	(void)con;
	(void)canoncon;
	return 0;
}

void freecon(char *con)
{
	free(con);
}

#define XATTR_NAME_HMMAC	"security.hmmac"
#define XATTR_DEFAULT_LEN	256
/* get file context */
int lgetfilecon_raw(const char *path, char **context)
{
	char *buf = NULL;
	size_t size;
	ssize_t rc;

	size = XATTR_DEFAULT_LEN;
	buf = malloc(size);
	if (!buf) {
		return -1;
	}

	if (memset_s(buf, size, 0, size) != 0) {
		bb_error_msg("memset buf failed\n");
		free(buf);
		return -1;
	}

	rc = lgetxattr(path, XATTR_NAME_HMMAC, buf, size - 1);
	if ((rc < 0) && (errno == ERANGE)) {
		char *newbuf;
		rc = lgetxattr(path, XATTR_NAME_HMMAC, NULL, 0);
		if (rc < 0) {
			goto out;
		}
		size = (size_t)(rc + 1);
		newbuf = malloc(size);
		if (!newbuf) {
			rc = ENOMEM;
			goto out;
		}
		free(buf);
		buf = newbuf;
		if (memset_s(buf, size, 0, size) != 0) {
			rc = EFAULT;
			goto out;
		}
		rc = lgetxattr(path, XATTR_NAME_HMMAC, buf, size - 1);
	}
out:
	if (rc == 0) {
		errno = ENOTSUP;
		rc = -1;
	}
	if (rc < 0) {
		free(buf);
	} else {
		*context = buf;
	}

	return (int)rc;
}

/* set file context */
int lsetfilecon(const char *path, const char *context)
{
	int rc = lsetxattr(path, XATTR_NAME_HMMAC, context, strlen(context) + 1, 0);
	char *cur_context = NULL;
	if ((rc < 0) && (errno == ENOTSUP)) {
		if ((lgetfilecon_raw(path, &cur_context) >= 0) && (strcmp(context, cur_context) == 0)) {
			rc = 0;
		} else {
			errno = ENOTSUP;
		}
		free(cur_context);
	}
	return rc;
}

int is_context_customizable(const char *scontext)
{
	(void)scontext;
	return 0;
}
