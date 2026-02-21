#!/bin/bash
# Allen Riemann
set -e

ALSA_LIB_PATH=$1
modules=
alisp=
prefix=
libdir="/lib"
configdir="/etc/audio/alsa/share"
args="--disable-aload --prefix=$prefix --libdir=$libdir --with-configdir=$configdir"

export LC_ALL=C
cd ${ALSA_LIB_PATH}
touch ltconfig
libtoolize --force --copy --automake
aclocal $ACLOCAL_FLAGS
autoheader
automake --foreign --copy --add-missing
touch depcomp		# seems to be missing for old automake
autoconf
echo "./configure $args"
${ALSA_LIB_PATH}/configure $args || exit 1
cd -
cat <<!EOF
!EOF
