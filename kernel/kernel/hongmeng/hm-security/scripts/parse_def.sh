#!/bin/bash

# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: generate and override the .config file
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 20 17:58:08 2021

#whenever error occurs, exit
set -e

DEFCONFIG_PATH=${1}

# parse Kconfig and defconfig file to .config
kbuild-conf --defconfig=${DEFCONFIG_PATH} Kconfig-sec

# override items on .config file according to environment variable
if test x"${CONFIG_XOM}" != x"" ; then
        echo CONFIG_XOM=${CONFIG_XOM};
        sed -i "s/.*CONFIG_XOM.*/CONFIG_XOM="${CONFIG_XOM}"/g" .config
fi

if test x"${CONFIG_COMPAT}" != x"" ; then
        echo CONFIG_COMPAT=${CONFIG_COMPAT};
        sed -i "s/.*CONFIG_COMPAT.*/CONFIG_COMPAT="${CONFIG_COMPAT}"/g" .config
fi

if test x"${CONFIG_AUDIT}" != x"" ; then
        echo CONFIG_AUDIT=${CONFIG_AUDIT};
        sed -i "s/.*CONFIG_AUDIT.*/CONFIG_AUDIT="${CONFIG_AUDIT}"/g" .config
fi

if test x"${CONFIG_SECCOMP}" != x"" ; then
        echo CONFIG_SECCOMP=${CONFIG_SECCOMP};
        sed -i "s/.*CONFIG_SECCOMP.*/CONFIG_SECCOMP="${CONFIG_SECCOMP}"/g" .config
fi

if test x"${CONFIG_ENABLE_KASAN}" != x"" ; then
        echo CONFIG_ENABLE_KASAN=${CONFIG_ENABLE_KASAN};
        sed -i "s/.*CONFIG_ENABLE_KASAN.*/CONFIG_ENABLE_KASAN="${CONFIG_ENABLE_KASAN}"/g" .config
fi

if test x"${CONFIG_KASAN_RUNTIME}" != x"" ; then
        echo CONFIG_KASAN_RUNTIME=${CONFIG_KASAN_RUNTIME};
        sed -i "s/.*CONFIG_KASAN_RUNTIME.*/CONFIG_KASAN_RUNTIME="${CONFIG_KASAN_RUNTIME}"/g" .config
fi

if test x"${CONFIG_SHADOW_STACK}" != x"" ; then
        echo CONFIG_SHADOW_STACK=${CONFIG_SHADOW_STACK};
        sed -i "s/.*CONFIG_SHADOW_STACK.*/CONFIG_SHADOW_STACK="${CONFIG_SHADOW_STACK}"/g" .config
fi

# reproduct .config file and header file
kbuild-conf --syncconfig Kconfig-sec

# delete AUTOCONF_TIMESTAMP on header file
sed -i "/AUTOCONF_TIMESTAMP/ d" ${KCONFIG_AUTOHEADER}
