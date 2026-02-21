#!/bin/bash

# Copyright (c) 2021-2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
set -e

PASRC_PATH=$1
PASRC_OUT_PATH=$2

function clean_pa_src() {
    echo "Clean local generated files"
    rm -rf ABOUT-NLS Makefile Makefile.in PulseAudioConfig.cmake PulseAudioConfigVersion.cmake aclocal.m4
    rm -rf autom4te.cache/ build-aux/ config.h config.h.in config.h.in~ config.log config.status configure
    rm -rf libpulse-mainloop-glib.pc libpulse-simple.pc libpulse.pc libtool stamp-h1
}

clean_pa_src

sed -i 's/\.\/git-version-gen .tarball-version//g' configure.ac
sed -i 's/\[m4_esyscmd()\],//g' configure.ac
sed -i 's/src doxygen man po/src doxygen man #po/g' Makefile.am

PA_CONFIG_OPTIONS="
    --without-caps
    --disable-alsa
    --disable-x11
    --disable-oss-output
    --disable-coreaudio-output
    --disable-alsa
    --disable-esound
    --disable-gsettings
    --disable-dbus
    --disable-udev
    --disable-ipv6
    --disable-openssl
    --disable-avahi
    --disable-jack
"
# We check for this here, because if pkg-config is not found in the
# system, it's likely that the pkg.m4 macro file is also not present,
# which will make PKG_PROG_PKG_CONFIG be undefined and the generated
# configure file faulty.
if ! pkg-config --version &>/dev/null; then
    echo "pkg-config is required to bootstrap this program"

fi
# Other necessary programs
if ! autopoint --version &>/dev/null ; then
    echo "autopoint is required to bootstrap this program"

fi

autoreconf --force --install --verbose

if test "x$NOCONFIGURE" = "x"; then
    CFLAGS="$CFLAGS -g -O0" $PASRC_PATH/configure --enable-force-preopen ${PA_CONFIG_OPTIONS} && \
        make clean
fi

sed -i 's/#define ENABLE_NLS 1//g' config.h
sed -i 's/#define HAVE_SHM_OPEN 1//g' config.h
sed -i 's/#define HAVE_RUNNING_FROM_BUILD_TREE 1//g' config.h
sed -i 's/#define HAVE_CPUID_H 1//g' config.h
sed -i 's/#define HAVE_EXECINFO_H 1//g' config.h
sed -i 's/#define HAVE_MEMFD 1//g' config.h
echo "#define PACKAGE_NAME \"pulseaudio\"" >> config.h
