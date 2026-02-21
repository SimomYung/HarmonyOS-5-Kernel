#!/bin/bash
# Copyright(c) 2023 Huawei Device Co., Ltd.

set -e
cd $1
find . ! -path "*/\.*" ! -path "./patch*" ! \( -name glib-2.72.2.tar.xz\
    -o -name BUILD.gn\
    -o -name config.gni\
    -o -name install.sh\
    -o -name bundle.json\
    -o -name OAT.xml\
    -o -name README.OpenSource\
    -o -name glib2.spec\
    -o -name COPYING\
    -o -name backport-patch.log\
    -o -name "README*"\
    -o -name CONTRIBUTING.md\
    -o -name SECURITY.md\
    -o -name ".*" \)\
    -prune -print -exec rm -rf {} \;
tar -xvf glib-2.72.2.tar.xz
mv glib-2.72.2/* .
rm -rf glib-2.72.2
echo "reset working dir success"
file="backport-patch.log"
exec < $file
while read line
do
    line=${line:16}
    echo $line
    patch -p1 < patch/$line --fuzz=0 --no-backup-if-mismatch
done
echo "all file patch success!"
exit 0

