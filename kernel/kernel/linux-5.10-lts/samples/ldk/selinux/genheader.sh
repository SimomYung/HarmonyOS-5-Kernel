#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only
set -e

sdkroot=$3

perm_file=$(dirname $2)/perm_file.h

cp $sdkroot/usr/include/libhmseharmony/flask.h $1
cp $sdkroot/usr/include/libhmseharmony/permissions.h $2
cp $sdkroot/usr/include/libhmseharmony/perm_file.h $perm_file

sed -i "s/HM_SEC/SEC/" $1
sed -i "s/hmsel_class_t/u16/" $1
sed -i "s/hmsel_socket_class/security_is_socket_class/" $1
sed -i "/#define.*SECCLASS_NULL/d" $1
sed -i "/#define.*SECSID_NULL/d" $1
sed -i "s/#define HM_/#define /" $2
sed -i "s/#define HM_/#define /" $perm_file
