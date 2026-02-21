Name:          libnl3
Version:       3.7.0
Release:       4
Summary:       Providing APIs to netlink protocol based Linux kernel interfaces
License:       LGPLv2
URL:           http://www.infradead.org/~tgr/libnl/
Source:        https://github.com/thom311/libnl/releases/download/libnl3_7_0/libnl-3.7.0.tar.gz

Patch6000:     backport-prevent-segfault-in-af_request_type.patch
Patch6001:     backport-fix-bridge-info-parsing.patch

Patch9000:     solve-redefinition-of-struct-ipv6_mreq.patch

patch6002:      backport-add-some-tests-about-addr-class-rule-neigh-qdisc.patch
patch6003:      backport-clear-XFRM_SP_ATTR_TMPL-when-removing-the-last-template.patch
patch6004:      backport-fix-reference-counters-of-sa-selector-addresses.patch
patch6005:      backport-do-not-use-static-array-indices-for-buffer.patch
patch6006:      backport-fix-leak-in-error-handling-of-rtnl_flower_append_action.patch
patch6007:      backport-fix-signed-overflow-warning-in-nl_object_diff.patch
patch6008:      backport-workaround-coverity-warning-about-time_t-handling.patch
patch6009:      backport-fix-leaking-usertemplate-in-xfrmnl_sp_parse.patch
patch6010:      backport-avoid-integer-overflow-in-rtnl_tc_calc_cell_log.patch
patch6011:      backport-fix-crashes-in-case-of-ENOMEM.patch
patch6012:      backport-accept-NULL-argument-in-nla_nest_cancel-for-robustness.patch
patch6013:      backport-fix-error-handling-in-nl_str2ip_protos.patch
patch6014:      backport-handle-negative-and-zero-size-in-nla_memcpy.patch
patch6015:      backport-use-thread-safe-gmtime_r-instead-of-gmtime.patch

BuildRequires: flex bison libtool autoconf automake swig
Requires:      %{name} = %{version}-%{release}

Provides:      %{name}-cli
Obsoletes:     %{name}-cli

%description
This package contains a collection of libraries providing
APIs to netlink based Linux kernel sockets interfaces.

%package devel
Summary: Libraries and headers for libnl3
Requires: %{name} = %{version}-%{release} kernel-headers

%description devel
This package provides various libraries and headers for using libnl3

%package help
Summary: Document for libnl3
Requires:      %{name} = %{version}-%{release}

Provides:      %{name}-doc = %{version}-%{release}
Obsoletes:     %{name}-doc < %{version}-%{release}

%description help
This package contains libnl3 related documentations

%package -n python3-libnl3
Summary: Python3 binding for libnl3
BuildRequires: python3-devel
Requires: %{name} = %{version}-%{release}

%description -n python3-libnl3
Python3 bindings for libnl3

%prep
%autosetup -n libnl-%{version} -p1

%build
autoreconf -vif
%configure --disable-static
%make_build

cd python
CFLAGS="$RPM_OPT_FLAGS" %py3_build
CFLAGS="$RPM_OPT_FLAGS" %py3_build

%install
%make_install

find $RPM_BUILD_ROOT -name *.la |xargs rm -f

cd python
%py3_install

%check
make check

cd python
%{__python3} setup.py check

%ldconfig_scriptlets

%files
%doc COPYING
%{_libdir}/libnl-*.so.*
%config(noreplace) %{_sysconfdir}/*
%{_libdir}/libnl/
%{_bindir}/*

%files devel
%{_includedir}/libnl3/netlink/
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%files help
%{_mandir}/man8/*

%files -n python3-libnl3
%{python3_sitearch}/netlink
%{python3_sitearch}/netlink-*.egg-info

%changelog
* Thu Apr 25 2024 sunhai <sunhai10@huawei.com> - 3.7.0-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:sync some pathes from upstream

* Fri Jan 12 2024 chengyechun <chengyechun1@huawei.com> - 3.7.0-3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:Use the user-mode IPv6 header file

* Mon Aug 14 2023 chengyechun <chengyechun1@huawei.com> - 3.7.0-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:prevent segfault in af_request_type
       fix bridge info parsing

* Wed Nov 9 2022 chengyechun <chengyechun1@huawei.com> - 3.7.0-1
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:update to libnl-3.7.0

* Sat Dec 18 2021 chengyechun <chengyechun1@huawei.com> - 3.5.0-6
- Type:bugfix
- ID:NA
- SUG:NA
-DES:add seome tests about add and delele addr,
     add and delete neigh,
     add and delete qdisc,
     add link and rule,
     delete route;

* Thu Dec 16 2021 chengyechun <chengyechun1@huawei.com> - 3.5.0-5
- Type:bugfix
- ID:NA
- SUG:NA
- DES:add a test:test add route

* Wed Mar 10 2021 zengwefeng <zwfeng@huawei.com> - 3.5.0-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:add missing check for NULL return from allocate_rfd
       add include netlink private nl-auto-h header
       use proper int type for id attributes
       add RTNL_LINK_REASM_OVERLAPS stat
       check for null pointer in macvlan
       fix leaking in link msg parser
       fix NLE_NOMEM handling in parse multipath

* Thu Oct 29 2020 gaihuiying <gaihuiying1@huawei.com> - 3.5.0-3
- Type:requirement
- ID:NA
- SUG:NA
- DESC:remove python2

* Thu Sep 10 2020 lunankun <lunankun@huawei.com> - 3.5.0-2
- Type: bugfix
- ID: NA
- SUG: NA
- DESC: fix Source0 url

* Sun Jan 12 2020 openEuler Buildteam <buildteam@openeuler.org> - 3.5.0-1
- update software to 3.5.0

* Wed Dec 25 2019 openEuler Buildteam <buildteam@openeuler.org> - 3.4.0-8
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:provides libnl3-doc

* Sat Sep 7 2019 liyongqiang<liyongqiang10@huawei.com> - 3.4.0-7
- Package init
