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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <libexif/exif-format.h>
#include <libexif/exif-utils.h>
#include <libexif/i18n.h>

#define DATA_LENGTH 1024

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
		snprintf(v, maxlen, _("%s"), e->data);
		return v;
	}

	if (e->tag == MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US) {
		for (unsigned long i = 0; i < e->components && write_pos < maxlen; i++) {
			unsigned long offset = i * exif_format_get_size(e->format);
			ExifRational r = exif_get_rational(e->data + offset, e->order);
			if (r.denominator == 0) {
				write_pos += (unsigned int)snprintf(v + write_pos, maxlen - write_pos, "%u ",
									  r.numerator);
			} else {
				write_pos += (unsigned int)snprintf(v + write_pos, maxlen - write_pos, "%u%u ",
									  r.numerator, r.denominator);
			}
		}
		*(v + write_pos - 1) = 0;
		return v;
	}

	for (unsigned long i = 0; i < e->components && write_pos < maxlen; i++) {
		if (e->format == EXIF_FORMAT_UNDEFINED) {
            ExifLong data = 0;
			data = (e->data + i)[0];
            write_pos += (unsigned int)snprintf(v + write_pos, maxlen - write_pos, "%u ", data);
		} else if (e->format == EXIF_FORMAT_SLONG) {
            ExifSLong data = 0;
			data = exif_get_slong(e->data + i * sizeof(ExifSLong), e->order);
            write_pos += (unsigned int)snprintf(v + write_pos, maxlen - write_pos, "%d ", data);
		} else if (e->format == EXIF_FORMAT_LONG) {
            ExifLong data = 0;
            data = exif_get_long(e->data + i * sizeof(ExifLong), e->order);
            write_pos += (unsigned int)snprintf(v + write_pos, maxlen - write_pos, "%u ", data);
		} else {
			snprintf(v, maxlen, _("unsupported data types: %d"), e->format);
			return NULL;
		}
	}

	*(v + write_pos - 1) = 0;
	return v;
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
		snprintf(timestamp_buff, sizeof(timestamp_buff), "%s", timestamp_char);
		unsigned long numerator = strtoul(timestamp_buff, NULL, 10);

		snprintf(timestamp_buff, 8, "%s", timestamp_char+10);
		unsigned long denominator = strtoul(timestamp_buff, NULL, 10);

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
	if (!components_local)
		ret = -1;
	if (pv)
		exif_mem_free(mem, pv);
	return ret;
}

int
mnote_huawei_entry_string_process(char *buff, const int buff_size, const char *v, int strlen,
									  int *components)
{
	int ret = 0;
	*components = snprintf(buff, buff_size, _("%s"), v);
	if (!(*components))
		ret = -1;
	else {
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
	memcpy(pv, v, strlen);

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
		} else {
			exif_set_slong((unsigned char*)(buff + offset), order, value);
		}
		components_local++;
		components_size = components_local * increment;
		token = strtok(NULL, " ");
	}

FINISH:
	*components = components_local;
	if (!components_local)
		ret = -1;
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

void
mnote_huawei_entry_initialize(MnoteHuaweiEntry *e, MnoteHuaweiTag tag, ExifByteOrder order)
{
	if (!e || e->data)
		return;
	e->tag = tag;
	e->order = order;
	switch (tag) {
		case MNOTE_HUAWEI_SCENE_INFO:
		case MNOTE_HUAWEI_FACE_INFO:
		case MNOTE_HUAWEI_CAPTURE_MODE:
		case MNOTE_HUAWEI_BURST_NUMBER:
		case MNOTE_HUAWEI_FRONT_CAMERA:
		case MNOTE_HUAWEI_PHYSICAL_APERTURE:
		case MNOTE_MOVING_PHOTO_VERSION:
		case MNOTE_HUAWEI_FACE_VERSION:
		case MNOTE_HUAWEI_FACE_COUNT:
		case MNOTE_HUAWEI_FACE_CONF:
		case MNOTE_HUAWEI_SCENE_VERSION:
		case MNOTE_HUAWEI_SCENE_FOOD_CONF:
		case MNOTE_HUAWEI_SCENE_STAGE_CONF:
		case MNOTE_HUAWEI_SCENE_BLUESKY_CONF:
		case MNOTE_HUAWEI_SCENE_GREENPLANT_CONF:
		case MNOTE_HUAWEI_SCENE_BEACH_CONF:
		case MNOTE_HUAWEI_SCENE_SNOW_CONF:
		case MNOTE_HUAWEI_SCENE_SUNSET_CONF:
		case MNOTE_HUAWEI_SCENE_FLOWERS_CONF:
		case MNOTE_HUAWEI_SCENE_NIGHT_CONF:
		case MNOTE_HUAWEI_SCENE_TEXT_CONF:
			e->components = 1;
			e->format = EXIF_FORMAT_LONG;
			e->size = e->components * exif_format_get_size(e->format);
			e->data = mnote_huawei_entry_alloc(e, e->size);
			if (!e->data) { clear_mnote_huawei_entry(e); break; }
			break;
		case MNOTE_HUAWEI_ROLL_ANGLE:
		case MNOTE_HUAWEI_PITCH_ANGLE:
		case MNOTE_HUAWEI_FOCUS_MODE:
        case MNOTE_HUAWEI_IS_XMAGE_SUPPORTED:
        case MNOTE_HUAWEI_XMAGE_MODE:
        case MNOTE_HUAWEI_XMAGE_LEFT:
        case MNOTE_HUAWEI_XMAGE_TOP:
        case MNOTE_HUAWEI_XMAGE_RIGHT:
        case MNOTE_HUAWEI_XMAGE_BOTTOM:
        case MNOTE_HUAWEI_CLOUD_ENHANCEMENT_MODE:
        case MNOTE_HUAWEI_WIND_SNAPSHOT_MODE:
        case MNOTE_HUAWEI_AI_EDIT:
			e->components = 1;
			e->format = EXIF_FORMAT_SLONG;
			e->size = e->components * exif_format_get_size(e->format);
			e->data = mnote_huawei_entry_alloc(e, e->size);
			if (!e->data) { clear_mnote_huawei_entry(e); break; }
			break;

		case MNOTE_MOVING_PHOTO_ID:
			e->components = sizeof("[None]");
			e->format = EXIF_FORMAT_ASCII;
			e->size = e->components * exif_format_get_size(e->format);
			e->data = mnote_huawei_entry_alloc(e, e->size);
			if (!e->data) { clear_mnote_huawei_entry(e); break; }
			snprintf((char*)e->data, e->size, "%s", "[None]");
			break;

		case MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US:
			e->components = 8;
			e->format = EXIF_FORMAT_RATIONAL;
			e->size = e->components * exif_format_get_size(e->format);
			e->data = mnote_huawei_entry_alloc(e, e->size);
			if (!e->data) { clear_mnote_huawei_entry(e); break; }
			memset(e->data, 0, e->size);
			break;

		case MNOTE_HUAWEI_FACE_SMILE_SCORE:
		case MNOTE_HUAWEI_FACE_RECT:
		case MNOTE_HUAWEI_FACE_LEYE_CENTER:
		case MNOTE_HUAWEI_FACE_REYE_CENTER:
		case MNOTE_HUAWEI_FACE_MOUTH_CENTER:
			e->components = 1;
			e->format = EXIF_FORMAT_UNDEFINED;
			e->size = e->components * exif_format_get_size(e->format);
			e->data = mnote_huawei_entry_alloc(e, e->size);
			if (!e->data) { clear_mnote_huawei_entry(e); break; }
			break;

		default:
			e->components = 0;
			e->format = EXIF_FORMAT_UNDEFINED;
			e->size = 0;
			e->data = NULL;
			break;
	}
}