/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

/* This file stubs the functionality of
 * libtool library used by pulseaudio.
 */

#ifndef LTDL_H
#define LTDL_H

typedef void  *lt_dlhandle;

const char *lt_dlerror(void);
const char *lt_dlgetsearchpath(void);
int lt_dlclose(lt_dlhandle handle);
lt_dlhandle lt_dlopenext(const char *filename);
void *lt_dlsym(lt_dlhandle handle, const char *symbol);

#endif // LTDL_H
