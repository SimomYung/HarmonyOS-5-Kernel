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

#ifndef LIBEXIF_EXIF_MNOTE_DATA_HUAWEI_H
#define LIBEXIF_EXIF_MNOTE_DATA_HUAWEI_H

#include <libexif/exif-byte-order.h>
#include <libexif/exif-mnote-data.h>
#include <libexif/exif-mnote-data-priv.h>
#include <libexif/exif-mem.h>
#include <libexif/exif-data.h>

#include <libexif/huawei/mnote-huawei-entry.h>
#include <libexif/huawei/mnote-huawei-data-type.h>

extern const char HUAWEI_HEADER[];
typedef struct _ExifMnoteDataHuawei ExifMnoteDataHuawei;
typedef struct _MnoteHuaweiEntryCount MnoteHuaweiEntryCount;

struct _ExifMnoteDataHuawei {
	ExifMnoteData parent;

	MnoteHuaweiEntry *entries;
	unsigned int count;

	ExifByteOrder order;
	unsigned int offset;
	unsigned int ifd_tag;
	unsigned int ifd_size;
	unsigned int is_loaded;
};

struct _MnoteHuaweiEntryCount {
	MnoteHuaweiEntry** entries;
	unsigned int idx;
	unsigned int size;
	ExifMem *mem;
};

/*! Detect if MakerNote is recognized as one handled by the Huawei module.
 * 
 * \param[in] ed image #ExifData to identify as as a Huawei type
 * \param[in] e #ExifEntry for EXIF_TAG_MAKER_NOTE, from within ed but
 *   duplicated here for convenience
 * \return 0 if not recognized, nonzero if recognized. The specific nonzero 
 *   value returned may identify a subtype unique within this module.
 */

int exif_mnote_data_huawei_identify (const ExifData *ed, const ExifEntry *e);


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int is_huawei_md(ExifMnoteData* md);
void print_huawei_md(const ExifMnoteDataHuawei* n);
ExifByteOrder exif_mnote_data_huawei_get_byte_order(ExifMnoteData *ne);
void mnote_huawei_free_entry_count(MnoteHuaweiEntryCount* entry_count);
void mnote_huawei_get_entry_count(const ExifMnoteDataHuawei* n, MnoteHuaweiEntryCount** entry_count);
void exif_mnote_data_huawei_clear (ExifMnoteDataHuawei *n);
int exif_mnote_data_add_entry (ExifMnoteData *ne, MnoteHuaweiEntry *e);
void exif_mnote_data_remove_entry (ExifMnoteData *ne, MnoteHuaweiEntry *e);
MnoteHuaweiEntry* exif_mnote_data_huawei_get_entry_by_tag (ExifMnoteDataHuawei *n, const MnoteHuaweiTag tag);
MnoteHuaweiEntry* exif_mnote_data_huawei_get_entry_by_index (ExifMnoteDataHuawei *n, const int dest_idx);
ExifMnoteData *exif_mnote_data_huawei_new (ExifMem *mem);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !defined(LIBEXIF_EXIF_MNOTE_DATA_HUAWEI_H) */
