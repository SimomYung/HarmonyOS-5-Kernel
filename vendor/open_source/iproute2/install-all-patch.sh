# Copyright (c) 2020-2023 Huawei Device Co., Ltd.
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

if [ -d "${2}/iproute2-5.15.0" ];then
    rm -rf iproute2-5.15.0
fi

if [ ! -d $2 ];then
    mkdir -p $2
    echo "make \$2 dir, and \$2 is now $2"
fi

cp ${1}/*.patch $2
tar -xf ${1}/iproute2-5.15.0.tar.xz -C $2
cd $2
cd ./iproute2-5.15.0
patch -p1 < ../bugfix-iproute2-change-proc-to-ipnetnsproc-which-is-private.patch

patch -p1 < ../backport-devlink-fix-devlink-health-dump-command-without-arg.patch
patch -p1 < ../backport-ip-Fix-size_columns-for-very-large-values.patch
patch -p1 < ../backport-ip-Fix-size_columns-invocation-that-passes-a-32-bit-.patch
patch -p1 < ../backport-l2tp-fix-typo-in-AF_INET6-checksum-JSON-print.patch
patch -p1 < ../backport-libnetlink-fix-socket-leak-in-rtnl_open_byproto.patch
patch -p1 < ../backport-lnstat-fix-buffer-overflow-in-header-output.patch
patch -p1 < ../backport-lnstat-fix-strdup-leak-in-w-argument-parsing.patch
patch -p1 < ../backport-q_cake-allow-changing-to-diffserv3.patch
patch -p1 < ../backport-tc-em_u32-fix-offset-parsing.patch
patch -p1 < ../backport-tc-flower-Fix-buffer-overflow-on-large-labels.patch
patch -p1 < ../backport-tc_util-Fix-parsing-action-control-with-space-and-sl.patch
patch -p1 < ../backport-tipc-fix-keylen-check.patch

patch -p1 < ../backport-bridge-Fix-memory-leak-when-doing-fdb-get.patch
patch -p1 < ../backport-ip-address-Fix-memory-leak-when-specifying-device.patch
patch -p1 < ../backport-ip-neigh-Fix-memory-leak-when-doing-get.patch
patch -p1 < ../backport-mptcp-Fix-memory-leak-when-doing-endpoint-show.patch
patch -p1 < ../backport-mptcp-Fix-memory-leak-when-getting-limits.patch
patch -p1 < ../backport-tc_util-Fix-no-error-return-when-large-parent-id-used.patch
patch -p1 < ../backport-tc_util-Change-datatype-for-maj-to-avoid-overflow-issue.patch
patch -p1 < ../backport-tc-ct-Fix-invalid-pointer-dereference.patch
patch -p1 < ../backport-libnetlink-Fix-memory-leak-in-__rtnl_talk_iov.patch
patch -p1 < ../backport-xfrm-prepare-state-offload-logic-to-set-mode.patch

patch -p1 < ../feature-iproute-add-support-for-ipvlan-l2e-mode.patch
patch -p1 < ../huawei-feature-peer_notify_delay-renamed-to-peer_notif_delay.patch

cd ../
patch -p1 < ./backport-fix-musl-compatible.patch

echo "unzip iproute2-5.15.0 done."
exit 0