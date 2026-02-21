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

#include "config.h"
#include <dlfcn.h>
#include <pulsecore/core-util.h>

#include "ltdl.h"

#if (defined(__aarch64__) || defined(__x86_64__))
#define SYSTEM_LIB_PATH "/system/lib64/"
#else
#define SYSTEM_LIB_PATH "/system/lib/"
#endif

lt_dlhandle lt_dlopenext(const char *filename)
{
    pa_assert(filename);
    return (dlopen(filename, RTLD_NOW));
}

void *lt_dlsym(lt_dlhandle handle, const char *symbol)
{
    pa_assert(handle);
    pa_assert(symbol);

    return (dlsym(handle, symbol));
}

int lt_dlclose(lt_dlhandle handle)
{
    pa_assert(handle);
    return (dlclose(handle));
}

const char *lt_dlerror(void)
{
    return dlerror();
}

const char *lt_dlgetsearchpath(void)
{
    const char *path = SYSTEM_LIB_PATH;
    return path;
}
