/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: device/driver compatible string
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 18 10:24:09 2020
 */

#include <libdevhost/compatible.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int compatible_string_init(struct compatible_string *compat_str, const char *compat, size_t compat_len)
{
	int ret = E_HM_OK;
	if (compat_str == NULL) {
		ret = E_HM_INVAL;
		goto err_out;
	}
	if (compat == NULL || compat_len == 0U) {
		compat_str->value = NULL;
		compat_str->length = 0;
		goto err_out;
	}
	if (compat[compat_len - 1U] != '\0') {
		ret = E_HM_INVAL;
		goto err_out;
	}

	compat_str->value = malloc(compat_len);
	if (compat_str->value == NULL) {
		ret = E_HM_NOMEM;
		goto err_out;
	}
	ret = memcpy_s(compat_str->value, compat_len, compat, compat_len);
	if (ret != 0) {
		free(compat_str->value);
		compat_str->value = NULL;
		ret = E_HM_INVAL;
		goto err_out;
	}
	compat_str->length = compat_len;
err_out:
	return ret;
}

void compatible_string_reset(struct compatible_string *compat_str)
{
	if ((compat_str != NULL) && (compat_str->value != NULL)) {
		free(compat_str->value);
		compat_str->value = NULL;
		compat_str->length = 0;
	}
}

int compatible_string_concat(struct compatible_string *compat_str, const char *compat, size_t compat_len)
{
	int ret = E_HM_OK;
	char *buf = NULL;
	if (compat_str == NULL) {
		ret = E_HM_INVAL;
		goto err_out;
	}
	if (compat == NULL || compat_len == 0U) {
		goto err_out;
	}
	if (compat[compat_len - 1U] != '\0') {
		ret = E_HM_INVAL;
		goto err_out;
	}

	buf = malloc(compat_str->length + compat_len);
	if (buf == NULL) {
		ret = E_HM_NOMEM;
		goto err_out;
	}
	if (compat_str->length > 0U) {
		ret = memcpy_s(buf, compat_str->length + compat_len, compat_str->value, compat_str->length);
		if (ret != 0) {
			free(buf);
			ret = E_HM_INVAL;
			goto err_out;
		}
	}
	ret = memcpy_s(buf + compat_str->length, compat_len, compat, compat_len);
	if (ret != 0) {
		free(buf);
		ret = E_HM_INVAL;
		goto err_out;
	}

	if (compat_str->value != NULL) {
		free(compat_str->value);
	}
	compat_str->value = buf;
	compat_str->length += compat_len;
err_out:
	return ret;
}

const char *compatible_string_next(const struct compatible_string *compat, const char *cur)
{
	const char *next = NULL;
	if (compat != NULL) {
		if (cur == NULL) {
			next = compat->value;
		} else {
			next = cur + strlen(cur) + 1;
			if (next >= compat->value + compat->length) {
				next = NULL;
			}
		}
	}
	return next;
}

const char *compatible_string_compare(const struct compatible_string *a,
				      const struct compatible_string *b)
{
	const char *cp_a = NULL;
	const char *cp_b = NULL;
	const char *result = NULL;

	cp_a = compatible_string_next(a, NULL);
	while (cp_a != NULL) {
		cp_b = compatible_string_next(b, NULL);
		while (cp_b != NULL) {
			if (strcmp(cp_a, cp_b) == 0) {
				result = cp_a;
				break;
			}
			cp_b = compatible_string_next(b, cp_b);
		}

		if (result != NULL) {
			break;
		}
		cp_a = compatible_string_next(a, cp_a);
	}

	return result;
}

#define UDK_SUFFIX ".udk"
#define LDK_SUFFIX ".ldk"
#define SO_SUFFIX ".so"
#define KO_SUFFIX ".ko"
static size_t __libname_prefix_len(const char *arg)
{
	const char *suffixes[] = {UDK_SUFFIX, LDK_SUFFIX, SO_SUFFIX, KO_SUFFIX};
	const char *p_end = arg + strlen(arg);
	size_t arg_len = strlen(arg);
	size_t result = arg_len;
	for (unsigned int i = 0; i < sizeof(suffixes) / sizeof(char *); i++) {
		size_t suffixes_len = strlen(suffixes[i]);
		if ((arg_len > suffixes_len) &&
		    (strcmp(p_end - suffixes_len, suffixes[i]) == 0)) {
			result = arg_len - suffixes_len;
			break;
		}
	}
	return result;
}

int compatible_string_init_from_libname(struct compatible_string *compat_str,
					const char *libname)
{
	int ret = E_HM_OK;
	size_t prefix_len;
	char *tmp = NULL;
	if (compat_str == NULL) {
		ret = E_HM_INVAL;
	} else {
		if (libname == NULL) {
			compat_str->value = NULL;
			compat_str->length = 0;
		} else {
			tmp = strdup(libname);
			if (tmp == NULL) {
				ret = E_HM_NOMEM;
			} else {
				prefix_len = __libname_prefix_len(libname);
				tmp[prefix_len] = '\0';
				ret = compatible_string_init(compat_str, tmp, prefix_len + 1U);
				free(tmp);
			}
		}
	}
	return ret;
}
