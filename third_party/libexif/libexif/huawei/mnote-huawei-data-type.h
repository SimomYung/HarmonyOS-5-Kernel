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

#ifndef LIBEXIF_MNOTE_HUAWEI_DATA_TYPE_H
#define LIBEXIF_MNOTE_HUAWEI_DATA_TYPE_H

typedef struct _MnoteHuaweiEntryCount MnoteHuaweiEntryCount;
typedef struct _ExifMem ExifMem;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

MnoteHuaweiEntryCount *
exif_huawei_entry_count_new (void);

MnoteHuaweiEntryCount *
exif_huawei_entry_count_new_mem (ExifMem *mem);

void
exif_huawei_entry_count_free (MnoteHuaweiEntryCount *ec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !defined(LIBEXIF_MNOTE_HUAWEI_DATA_TYPE_H) */
