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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mnote-huawei-data-type.h"
#include "exif-mnote-data-huawei.h"

MnoteHuaweiEntryCount *
exif_huawei_entry_count_new (void) 
{
	ExifMem *mem = exif_mem_new_default ();
	MnoteHuaweiEntryCount *ec = exif_huawei_entry_count_new_mem (mem);

	exif_mem_unref (mem);

	return ec;
}

MnoteHuaweiEntryCount *
exif_huawei_entry_count_new_mem (ExifMem *mem) 
{
	MnoteHuaweiEntryCount *ec;

	if (!mem) 
		return NULL;
	
	ec = exif_mem_alloc (mem, sizeof (MnoteHuaweiEntryCount));
	if (!ec) 
		return NULL;
	
	memset(ec, 0, sizeof (MnoteHuaweiEntryCount));
	ec->mem = mem;
	exif_mem_ref (mem);

	return ec;
}

void
exif_huawei_entry_count_free (MnoteHuaweiEntryCount *ec) 
{
	ExifMem *mem;

	if (!ec)
		return;

	mem = ec->mem;
	if (ec->entries)
		exif_mem_free(mem, ec->entries);
	
	exif_mem_free (mem, ec);
	exif_mem_unref (mem);
}