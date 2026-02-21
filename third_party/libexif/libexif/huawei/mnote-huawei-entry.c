/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"
#include "exif-mnote-data-huawei.h"
#include "mnote-huawei-entry.h"
#include "mnote-huawei-tag.h"
#include "hilog/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include <libexif/exif-format.h>
#include <libexif/exif-utils.h>
#include <libexif/i18n.h>
#include <securec.h>

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002B60
#undef LOG_TAG
#define LOG_TAG "mnote-huawei-entry"

#define DATA_LENGTH 1024
#define BLANK_SIZE 1
#define NEGATIVE_SIGN_SIZE 1
#define PARSE_BASE_DECIMAL 10
#define TIMESTAMP_LENGTH 10
#define THREE_COMPONENTS 3
#define SHORT_INCREMENT 2

/* Get length of number for value in unsigned integer */
uint32_t get_unsigned_int_length(uint32_t value)
{
    if (value == 0) {
        return 1;
    }
    return (uint32_t)log10(value) + 1;
}

/* Get length of number for value in signed integer */
uint32_t get_signed_int_length(int32_t value)
{
    if (value == 0) {
        return 1;
    }

    /* If the number is negative, include the '-' sign in the length */
    return (uint32_t)log10(abs(value)) + (value < 0 ? 1 + NEGATIVE_SIGN_SIZE : 1);
}

/* Get components from exif */
char *
format_exif_components(MnoteHuaweiEntry *e, char *v, unsigned int maxlen, unsigned int write_pos)
{
	for (unsigned long i = 0; i < e->components && write_pos < maxlen; i++) {
		int returnSize;
		if (e->format == EXIF_FORMAT_UNDEFINED) {
            ExifLong data = 0;
			data = (e->data + i)[0];
			if ((get_unsigned_int_length(data) + BLANK_SIZE) > (maxlen - write_pos)) {
				return NULL;
			}
			returnSize = snprintf_s(v + write_pos, maxlen - write_pos,
				maxlen - write_pos, "%u ", data);
		} else if (e->format == EXIF_FORMAT_SLONG) {
            ExifSLong data = 0;
            data = exif_get_slong(e->data + i * sizeof(ExifSLong), e->order);
			if ((get_signed_int_length(data) + BLANK_SIZE) > (maxlen - write_pos)) {
				return NULL;
			}
			returnSize = snprintf_s(v + write_pos, maxlen - write_pos,
				maxlen - write_pos, "%d ", data);
		} else if (e->format == EXIF_FORMAT_LONG) {
            ExifLong data = 0;
			data = exif_get_long(e->data + i * sizeof(ExifLong), e->order);
			if ((get_unsigned_int_length(data) + BLANK_SIZE) > (maxlen - write_pos)) {
				return NULL;
			}
			returnSize = snprintf_s(v + write_pos, maxlen - write_pos,
				maxlen - write_pos, "%u ", data);
		} else if (e->format == EXIF_FORMAT_SSHORT) {
            ExifSShort data = 0;
			data = exif_get_sshort(e->data + i * sizeof(ExifSShort), e->order);
			if ((get_signed_int_length(data) + BLANK_SIZE) > (maxlen - write_pos)) {
				return NULL;
			}
			returnSize = snprintf_s(v + write_pos, maxlen - write_pos,
				maxlen - write_pos, "%d ", data);
		} else {
			snprintf_s(v, maxlen, maxlen, _("unsupported data types: %d"), e->format);
			return NULL;
		}

		if (returnSize < 0) {
			HILOG_INFO(LOG_CORE, "Do %{public}s snprintf_s fail", __func__);
			return NULL;
		}
		write_pos += (unsigned int)returnSize;
	}

	*(v + write_pos - 1) = 0;
	return v;
}

/* To process video timestamp in mnote_huawei_entry_get_value function */
char *handle_video_timestamp(MnoteHuaweiEntry *e, char *v, unsigned int maxlen, unsigned int *write_pos)
{
	for (unsigned long i = 0; i < e->components && *write_pos < maxlen; i++) {
		unsigned long offset = i * exif_format_get_size(e->format);
		ExifRational r = exif_get_rational(e->data + offset, e->order);
		int returnSize;
		if (r.denominator == 0) {
			returnSize = snprintf_s(v + *write_pos, maxlen - *write_pos,
				maxlen - *write_pos, "%u ", r.numerator);
		} else {
			returnSize = snprintf_s(v + *write_pos, maxlen - *write_pos,
				maxlen - *write_pos, "%u%u ", r.numerator, r.denominator);
		}
		if (returnSize < 0) {
			return NULL;
		}
		*write_pos += (unsigned long)returnSize;
	}
	*(v + *write_pos - 1) = 0;
	return v;
}

/* To process xtstyle in mnote_huawei_entry_get_value function */
char *handle_xtstyle(MnoteHuaweiEntry *e, char *v, unsigned int maxlen, unsigned int *write_pos)
{
    for (unsigned long i = 0; i < e->components && *write_pos < maxlen; i++) {
		unsigned long offset = i * exif_format_get_size(e->format);
		ExifRational r = exif_get_rational(e->data + offset, e->order);
		int returnSize;
		if (r.denominator != 0) {
			returnSize = snprintf_s(v + *write_pos, maxlen - *write_pos,
				maxlen - *write_pos, "%.3f", (double) r.numerator / (double) r.denominator);
		} else {
			returnSize = snprintf_s(v + *write_pos, maxlen - *write_pos,
				maxlen - *write_pos, "%u ", r.numerator);
		}
		if (returnSize < 0) {
			return NULL;
		}
		*write_pos += (unsigned long)returnSize;
	}
	*(v + *write_pos - 1) = 0;
	return v;
}

/* To process stars information in mnote_huawei_entry_get_value function */
char *handle_stars_info(MnoteHuaweiEntry *e, char *v, unsigned int maxlen, unsigned int *write_pos)
{
    for (unsigned long i = 0; i < e->components && *write_pos < maxlen; i++) {
		unsigned long offset = i * exif_format_get_size(e->format);
		ExifSLong slong_data = exif_get_slong(e->data + offset, e->order);
		int returnSize;
		returnSize = snprintf_s(v + *write_pos, maxlen - *write_pos,
			maxlen - *write_pos, "%d ", slong_data);
		if (returnSize < 0) {
			return NULL;
		}
		*write_pos += (unsigned long)returnSize;
	}
	*(v + *write_pos - 1) = 0;
	return v;
}

/* To judge if it is xtstyle */
bool is_xtstyle(MnoteHuaweiEntry *e)
{
	if ((e->tag == MNOTE_HUAWEI_XTSTYLE_CUSTOM_LIGHT_SHADOW ||
		e->tag == MNOTE_HUAWEI_XTSTYLE_CUSTOM_SATURATION ||
		e->tag == MNOTE_HUAWEI_XTSTYLE_CUSTOM_HUE) &&
		e->format == EXIF_FORMAT_RATIONAL) {
			return true;
		}
	return false;
}

char *
mnote_huawei_entry_get_value(MnoteHuaweiEntry *e, char *v, unsigned int maxlen)
{
	if (!v)
		return NULL;

	memset(v, 0, maxlen);
    unsigned int write_pos = 0;
    if (e == NULL || e->data == NULL) {
        return NULL;
    }

	if (e->format == EXIF_FORMAT_ASCII) {
		if (snprintf_s(v, maxlen, maxlen, _("%.*s"), e->size, e->data) < 0) {
			return NULL;
		}
		return v;
	}

	if (e->tag == MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US) {
		return handle_video_timestamp(e, v, maxlen, &write_pos);
	}

	if (is_xtstyle(e)) {
		return handle_xtstyle(e, v, maxlen, &write_pos);
	}

	if (e->tag == MNOTE_HUAWEI_STARS_INFO) {
		return handle_stars_info(e, v, maxlen, &write_pos);
	}

	return format_exif_components(e, v, maxlen, write_pos);
}

static int parse_rational_timestamp(const char *input, char *buffer, size_t buffer_size, unsigned long *result)
{
    if (snprintf_s(buffer, buffer_size, buffer_size, "%s", input) < 0) {
        return -1;
    }
    errno = 0;
    unsigned long value = strtoul(buffer, NULL, PARSE_BASE_DECIMAL);
    if (errno == ERANGE && value == ULONG_MAX) {
        return -1;
    }
    *result = value;
    return 0;
}

int
mnote_huawei_entry_rational_timestamp_process(char *buff, const int buff_size, const char *v, int strlen,
									 		   int *components, ExifMem *mem, ExifByteOrder order)
{
	char *timestamp_char = NULL, *pv = NULL;
	int components_local = 0, components_size = 0, ret = 0;
	const int increment = 8;

	pv = exif_mem_alloc(mem, strlen + 1);
	if (!pv) {
		ret = -1;
		goto FINISH;
	}
	*(pv + strlen) = 0;
	memcpy(pv, v, strlen);

    timestamp_char = strtok(pv, " ");
	for (; timestamp_char && components_size < buff_size;) {
		int offset = increment * components_local;
		char timestamp_buff[11] = {0};
		unsigned long numerator = 0;
		if (parse_rational_timestamp(timestamp_char, timestamp_buff, sizeof(timestamp_buff), &numerator) < 0) {
			ret = -1;
			goto FINISH;
		}
		unsigned long denominator = 0;
		if (parse_rational_timestamp(timestamp_char + TIMESTAMP_LENGTH,
			timestamp_buff, sizeof(timestamp_buff), &denominator) < 0) {
			ret = -1;
			goto FINISH;
		}

		ExifRational r = {
			.numerator = numerator,
			.denominator = denominator,
		};

		exif_set_rational((unsigned char*)buff + offset, order, r);
		components_local++;
		components_size = components_local * increment;
        timestamp_char = strtok(NULL, " ");
	}

FINISH:
	*components = components_local;
	if (!components_local) {
		ret = -1;
	}
	if (pv)
		exif_mem_free(mem, pv);
	return ret;
}

int
mnote_huawei_entry_rational_process(char *buff, const int buff_size, const char *v, int strlen,
									 		   int *components, ExifMem *mem, ExifByteOrder order)
{
	char *timestamp_char = NULL, *pv = NULL;
	int components_local = 0, components_size = 0, ret = 0;
	const int increment = 8;

	pv = exif_mem_alloc(mem, strlen + 1);
	if (!pv) {
		ret = -1;
		goto FINISH;
	}
	*(pv + strlen) = 0;
	if (EOK != memcpy_s(pv, strlen + 1, v, strlen)) {
		HILOG_INFO(LOG_CORE,
			"memcpy failed at %{public}s, strlen:%{public}d, strlen:%{public}s", __func__, strlen, v);
		ret = -1;
		goto FINISH;
	}

    timestamp_char = strtok(pv, " ");
	for (; timestamp_char && components_size < buff_size;) {
		int offset = increment * components_local;
		char numerator_str[11] = {0};
		char denominator_str[11] = {0};
		char *slash_pos = strchr(timestamp_char, '/');
		unsigned long numerator = 0;
		unsigned long denominator = 0;
		if (slash_pos) {
			size_t numerator_len = slash_pos - timestamp_char;
			if (snprintf_s(numerator_str, sizeof(numerator_str),
				sizeof(numerator_str), "%.*s", numerator_len, timestamp_char) < 0) {
				ret = -1;
				goto FINISH;
			}
			numerator = strtoul(numerator_str, NULL, PARSE_BASE_DECIMAL);

            if (snprintf_s(denominator_str, sizeof(denominator_str),
				sizeof(denominator_str), "%s", slash_pos + 1) < 0) {
				ret = -1;
				goto FINISH;
			}
			denominator = strtoul(denominator_str, NULL, PARSE_BASE_DECIMAL);
		} else {
        	if (snprintf_s(numerator_str, sizeof(numerator_str),
				sizeof(numerator_str), "%s", timestamp_char) < 0) {
				ret = -1;
				goto FINISH;
			}
			numerator = strtoul(numerator_str, NULL, PARSE_BASE_DECIMAL);
        	denominator = 1;
		}

		if (denominator == 0) {
			ret = -1;
			goto FINISH;
		}
		ExifRational r = {
			.numerator = numerator,
			.denominator = denominator,
		};

		exif_set_rational((unsigned char*)buff + offset, order, r);
		components_local++;
		components_size = components_local * increment;
        timestamp_char = strtok(NULL, " ");
	}

FINISH:
	*components = components_local;
	if (!components_local) {
		ret = -1;
	}
	if (pv)
		exif_mem_free(mem, pv);
	return ret;
}

int
mnote_huawei_entry_string_process(char *buff, const int buff_size, const char *v, int strlen,
									  int *components)
{
	int ret = 0;
	*components = snprintf_s(buff, buff_size, strlen, _("%s"), v);
	if ((*components) <= 0) {
		ret = -1;
	} else {
		*components = *components + 1;
	}
	return ret;
}

int
mnote_huawei_entry_value_process(char *buff, const int buff_size, const char *v, int strlen,
									 const int increment, int *components, ExifMem *mem, ExifByteOrder order)
{
	char *token = NULL, *pv = NULL;
	int components_local = 0, components_size = 0, ret = 0;

	pv = exif_mem_alloc(mem, strlen + 1);
	if (!pv) {
		ret = -1;
		goto FINISH;
	}
	*(pv + strlen) = 0;
	if (EOK != memcpy_s(pv, strlen + 1, v, strlen)) {
		HILOG_INFO(LOG_CORE,
			"memcpy failed at %{public}s, strlen:%{public}d, strlen:%{public}s", __func__, strlen, v);
		ret = -1;
		goto FINISH;
	}

	token = strtok(pv, " ");
	for (; token && components_size < buff_size;) {
		int value = atoi(token);
		int offset = increment * components_local;
		if (increment == 1) {
			if (value > 0xff || value < 0) {
				ret = -1;
				goto FINISH;
			}
			*(buff + offset) = value;
		} else if (increment == SHORT_INCREMENT) {
			exif_set_sshort((unsigned char*)(buff + offset), order, value);
		} else {
			exif_set_slong((unsigned char*)(buff + offset), order, value);
		}
		components_local++;
		components_size = components_local * increment;
		token = strtok(NULL, " ");
	}

FINISH:
	*components = components_local;
	if (!components_local) {
		ret = -1;
	}
	if (pv)
		exif_mem_free(mem, pv);
	return ret;
}

int
mnote_huawei_entry_set_value(MnoteHuaweiEntry *e, const char *v, int strlen)
{
	char data[DATA_LENGTH] = {0};
	int increment = 0, ret = 0, components = 0;
    unsigned int components_size = 0;

	if (!e || !v || e->md) {
		ret = -1;
		goto FINISH;
	}

	if (e->format == EXIF_FORMAT_UNDEFINED || e->format == EXIF_FORMAT_ASCII) {
		increment = 1;
	} else if (e->format == EXIF_FORMAT_SHORT || e->format == EXIF_FORMAT_SSHORT) {
		increment = SHORT_INCREMENT;
	} else if (e->format == EXIF_FORMAT_SLONG || e->format == EXIF_FORMAT_LONG) {
		increment = 4;
	} else if (e->format == EXIF_FORMAT_RATIONAL || e->format == EXIF_FORMAT_SRATIONAL) {
		increment = 8;
	} else {
		ret = -1;
		goto FINISH;
	}

	if (e->tag == MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US) {
		ret = mnote_huawei_entry_rational_timestamp_process(data, sizeof(data), v, strlen,
															&components, e->mem, e->order);
	} else if (e->format == EXIF_FORMAT_ASCII) {
		ret = mnote_huawei_entry_string_process(data, sizeof(data), v, strlen, &components);
	} else if (e->format == EXIF_FORMAT_RATIONAL) {
		ret = mnote_huawei_entry_rational_process(data, sizeof(data), v, strlen,
															&components, e->mem, e->order);
	} else {
		ret = mnote_huawei_entry_value_process(data, sizeof(data), v, strlen, increment,
											   &components, e->mem, e->order);
	}

	if (ret) {
		goto FINISH;
	}

	components_size = (unsigned int) (increment * components);
	if (e->size < components_size) {
		unsigned char *realloc = exif_mem_realloc(e->mem, e->data, components_size);
		if (!realloc) {
			ret = -1;
			goto FINISH;
		}
		e->data = realloc;
		e->size = components_size;
	}

	e->components = (unsigned long)components;
    if (e->size < components_size || components_size > DATA_LENGTH) {
        ret = -1;
        goto FINISH;
    }
	memcpy(e->data, data, components_size);

FINISH:
	return ret;
}

MnoteHuaweiEntry *
mnote_huawei_entry_new(ExifMnoteData *n)
{
	if (!is_huawei_md(n))
		return NULL;
	ExifMem *mem = n->mem;
	MnoteHuaweiEntry *e = exif_mem_alloc(mem, sizeof(MnoteHuaweiEntry));
	if (!e)
		return e;
	e->mem = mem;
	return e;
}

void
mnote_huawei_entry_replace_mem(MnoteHuaweiEntry *e, ExifMem *mem)
{
	if (!e || !mem)
		return;
	if (e->mem == mem)
		return;
	exif_mem_unref(e->mem);
	e->mem = mem;
	return;
}

void
mnote_huawei_entry_free_content(MnoteHuaweiEntry *e)
{
	if (!e)
		return;
	if (e->data)
		exif_mem_free(e->mem, e->data);
	if (e->md)
		exif_mnote_data_huawei_clear((ExifMnoteDataHuawei*) e->md);
}

void
mnote_huawei_entry_free_contour(MnoteHuaweiEntry *e)
{
	if (!e)
		return;
	exif_mem_free(e->mem, e);
}

void
mnote_huawei_entry_free(MnoteHuaweiEntry *e)
{
	if (!e)
		return;
	mnote_huawei_entry_free_content(e);
	exif_mem_free(e->mem, e);
}

static void *
mnote_huawei_entry_alloc (MnoteHuaweiEntry *e, unsigned int i)
{
	if (!e || !i) return NULL;
	void *p = exif_mem_alloc (e->mem, i);
	if (p) memset(p, 0, i);
	return p;
}

static void
clear_mnote_huawei_entry (MnoteHuaweiEntry *e)
{
	e->components = 0;
	e->size = 0;
}

static const unsigned int VIDEO_TIMESTAMP_COMPONENTS = 8;

/* Base information about Huawei tags */
static const HuaweiTagInitInfo huawei_tag_init_table[] = {
    { MNOTE_HUAWEI_SCENE_INFO, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_INFO, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_CAPTURE_MODE, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_BURST_NUMBER, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_FRONT_CAMERA, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_PHYSICAL_APERTURE, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_MOVING_PHOTO_VERSION, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_VERSION, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_COUNT, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_VERSION, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_FOOD_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_STAGE_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_BLUESKY_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_GREENPLANT_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_BEACH_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_SNOW_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_SUNSET_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_FLOWERS_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_NIGHT_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_SCENE_TEXT_CONF, EXIF_FORMAT_LONG, 1, NULL, 0 },

    { MNOTE_HUAWEI_ROLL_ANGLE, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_PITCH_ANGLE, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_FOCUS_MODE, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_IS_XMAGE_SUPPORTED, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_XMAGE_MODE, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_XMAGE_LEFT, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_XMAGE_TOP, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_XMAGE_RIGHT, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_XMAGE_BOTTOM, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_CLOUD_ENHANCEMENT_MODE, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_WIND_SNAPSHOT_MODE, EXIF_FORMAT_SLONG, 1, NULL, 0 },
    { MNOTE_HUAWEI_AI_EDIT, EXIF_FORMAT_SLONG, 1, NULL, 0 },

    { MNOTE_MOVING_PHOTO_ID, EXIF_FORMAT_ASCII, sizeof("[None]"), "[None]", 0 },
    { MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US,
		EXIF_FORMAT_RATIONAL, VIDEO_TIMESTAMP_COMPONENTS, NULL, 1 },
    { MNOTE_HUAWEI_FACE_SMILE_SCORE, EXIF_FORMAT_UNDEFINED, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_RECT, EXIF_FORMAT_UNDEFINED, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_LEYE_CENTER, EXIF_FORMAT_UNDEFINED, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_REYE_CENTER, EXIF_FORMAT_UNDEFINED, 1, NULL, 0 },
    { MNOTE_HUAWEI_FACE_MOUTH_CENTER, EXIF_FORMAT_UNDEFINED, 1, NULL, 0 },
    { MNOTE_HUAWEI_XTSTYLE_TEMPLATE_NAME, EXIF_FORMAT_SSHORT, 1, NULL, 0 },
    { MNOTE_HUAWEI_XTSTYLE_CUSTOM_LIGHT_SHADOW, EXIF_FORMAT_RATIONAL, 1, NULL, 1 },
    { MNOTE_HUAWEI_XTSTYLE_CUSTOM_SATURATION, EXIF_FORMAT_RATIONAL, 1, NULL, 1 },
    { MNOTE_HUAWEI_XTSTYLE_CUSTOM_HUE, EXIF_FORMAT_RATIONAL, 1, NULL, 1 },
    { MNOTE_HUAWEI_XTSTYLE_EXPOSUREPARAM_PARAM, EXIF_FORMAT_SSHORT, THREE_COMPONENTS, NULL, 0 },
    { MNOTE_HUAWEI_STARS_INFO, EXIF_FORMAT_SLONG, 1, NULL, 0 }
};

void mnote_huawei_entry_initialize(MnoteHuaweiEntry *e, MnoteHuaweiTag tag, ExifByteOrder order)
{
    if (!e || e->data)
        return;

    e->tag = tag;
    e->order = order;

    const HuaweiTagInitInfo *info = NULL;
    for (size_t i = 0; i < sizeof(huawei_tag_init_table) / sizeof(huawei_tag_init_table[0]); ++i) {
        if (huawei_tag_init_table[i].tag == tag) {
            info = &huawei_tag_init_table[i];
            break;
        }
    }

    if (info) {
        e->components = info->components;
        e->format = info->format;
        e->size = e->components * exif_format_get_size(e->format);
        e->data = mnote_huawei_entry_alloc(e, e->size);
        if (!e->data) {
            clear_mnote_huawei_entry(e);
            return;
        }

        if (info->default_ascii) {
            if (snprintf_s((char *)e->data, e->size, e->components, "%s", info->default_ascii)) {
                clear_mnote_huawei_entry(e);
                return;
            }
        }

        if (info->zero_initialize) {
            memset(e->data, 0, e->size);
        }
    } else {
        e->components = 0;
        e->format = EXIF_FORMAT_UNDEFINED;
        e->size = 0;
        e->data = NULL;
    }
}