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

#ifndef LIBEXIF_MNOTE_HUAWEI_ENTRY_H
#define LIBEXIF_MNOTE_HUAWEI_ENTRY_H

#include <libexif/exif-format.h>
#include <libexif/exif-byte-order.h>
#include <libexif/huawei/mnote-huawei-tag.h>

typedef struct _MnoteHuaweiEntry        MnoteHuaweiEntry;
typedef struct _ExifMnoteDataHuawei     ExifMnoteDataHuawei;

struct _MnoteHuaweiEntry {
	MnoteHuaweiTag tag;
	ExifFormat format;
	unsigned long components;

	unsigned char *data;
	unsigned int size;
	ExifByteOrder order;

	void *md;
	void *parent_md;
	ExifMem *mem;
};

/*
 * This struct is used to store MnoteHuaweiTag and their associated initialization information.
 * It defines the data format, component count, default values, and other initialization options for each tag.
 */
typedef struct {
    /*
     * Huawei custom Exif tag type, representing specific metadata items.
     */
    MnoteHuaweiTag tag;
    
    /*
     * Exif data format, defining the data type of the tag, such as long integer or rational number.
     */
    ExifFormat format;
    
    /*
     * Number of components in the data, indicating how many basic units the data consists of.
     * For example, MNOTE_HUAWEI_XTSTYLE_EXPOSUREPARAM_PARAM have three components.
     */
    unsigned int components;
    
    /*
     * Default ASCII string value for the tag, applicable only to ASCII format tags.
     * If NULL, it indicates no default value.
     */
    const char *default_ascii;
    
    /*
     * Indicates whether to zero-initialize the data during initialization.
	 * If true, the data area will be set to zero after memory allocation.
	 * false is 0, true is 1.
     */
    int zero_initialize;
} HuaweiTagInitInfo;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char *mnote_huawei_entry_get_value (MnoteHuaweiEntry *entry, char *v, unsigned int maxlen);
int mnote_huawei_entry_set_value(MnoteHuaweiEntry *entry, const char *v, int strlen);

MnoteHuaweiEntry *mnote_huawei_entry_new(ExifMnoteData *n);
void mnote_huawei_entry_replace_mem(MnoteHuaweiEntry *e, ExifMem *mem);
void mnote_huawei_entry_free_content(MnoteHuaweiEntry *e);
void mnote_huawei_entry_free_contour(MnoteHuaweiEntry *e);
void mnote_huawei_entry_free(MnoteHuaweiEntry *e);
void mnote_huawei_entry_initialize(MnoteHuaweiEntry *e, MnoteHuaweiTag tag, ExifByteOrder order);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !defined(LIBEXIF_MNOTE_HUAWEI_ENTRY_H) */
