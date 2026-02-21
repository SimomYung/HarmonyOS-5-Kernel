Name:           glib2
Version:        2.72.2
Release:        14
Summary:        The core library that forms the basis for projects such as GTK+ and GNOME
License:        LGPLv2+
URL:            http://www.gtk.org
Source0:        https://download.gnome.org/sources/glib/2.71/glib-%{version}.tar.xz

Patch6000:      backport-add-version-macros-for-GLib-2.74.patch
Patch6001:      backport-gtype-Add-G_TYPE_FLAG_NONE.patch
Patch6002:      backport-gioenums-Add-G_TLS_CERTIFICATE_FLAGS_NONE.patch
Patch6003:      backport-gtestutils-Add-G_TEST_SUBPROCESS_DEFAULT.patch
Patch6004:      backport-gsignal-Add-G_CONNECT_DEFAULT.patch
Patch6005:      backport-giomodule-test-Dont-pass-a-magic-number-to-g_test_trap_subprocess.patch
Patch6006:      backport-giochannel-Add-G_IO_FLAG_NONE.patch
Patch6007:      backport-gmarkup-Add-G_MARKUP_PARSE_FLAGS_NONE.patch
Patch6008:      backport-gregex-Add-G_REGEX_DEFAULT-G_REGEX_MATCH_DEFAULT.patch
Patch6009:      backport-replace-pcre1-with-pcre2.patch
Patch6010:      backport-gregex-format-specifier-for-localized-error-message.patch
Patch6011:      backport-gregex-ensure-we-translate-the-errcode.patch
Patch6012:      backport-gregex-Free-match-info-if-offset-matching-recalc-failed.patch
Patch6013:      backport-gregex-use-G_REGEX_OPTIMIZE-flag-to-enable-JIT-compilation.patch
Patch6014:      backport-gregex-use-g_debug-instead-of-g_warning-in-case-JIT-is-not-available.patch
Patch6015:      backport-gregex-do-not-set-match-and-recursion-limits-on-match-context.patch
Patch6016:      backport-gregex-add-original-test-case.patch
Patch6017:      backport-gregex-use-correct-size-for-pcre2_pattern_info.patch
Patch6018:      backport-regex-Add-debug-strings-for-compile-and-match-option-flags.patch
Patch6019:      backport-regex-Actually-check-for-match-options-changes.patch
Patch6020:      backport-regex-Do-not-mix-PCRE2-Compile-Match-Newline-and-BSR-flags.patch
Patch6021:      backport-regex-Add-test-for-gtksourceview-regression.patch
Patch6022:      backport-gregex-Mark-g_match_info_get_regex-as-transfer-none.patch
Patch6023:      backport-gregex-Do-not-try-access-the-undefined-match-offsets.patch
Patch6024:      backport-gregex-Fix-a-potential-PCRE2-code-leak-on-reallocation-failures.patch
Patch6025:      backport-regex-Use-size-types-more-in-line-with-PCRE2-returned-values.patch
Patch6026:      backport-gregex-Handle-the-case-we-need-to-re-allocate-the-match-data.patch
Patch6027:      backport-gregex-Avoid-re-allocating-if-we-have-no-size-change.patch
Patch6028:      backport-regex-Compute-the-offsets-size-based-on-match-results.patch
Patch6029:      backport-regex-Avoid-allocating-offsets-until-we-ve-a-match.patch
Patch6030:      backport-regex-Handle-JIT-errors-more-explicitly.patch
Patch6031:      backport-regex-Make-possible-to-test-replacements-with-options.patch
Patch6032:      backport-regex-Do-not-use-JIT-when-using-unsupported-match-options.patch
Patch6033:      backport-regex-Perform-more-tests-both-with-and-without-optimizations.patch
Patch6034:      backport-gsocketclient-Fix-still-reachable-references-to-cancellables.patch
Patch6035:      backport-Add-lock-in-_g_get_unix_mount_points-around-fsent-functions.patch
Patch6036:      backport-g_get_unix_mount_points-reduce-syscalls-inside-loop.patch
Patch6037:      backport-xdgmime-fix-double-free.patch
Patch6038:      backport-Implement-GFileIface.set_display_name-for-resource-files.patch
Patch6039:      backport-tests-dbus-appinfo-Add-test-case-for-flatpak-opening-an-invalid-file.patch
Patch6040:      backport-documentportal-Fix-small-leak-in-add_documents-with-empty-URI-list.patch
Patch6041:      backport-gio-tests-gdbus-proxy-threads-Unref-GVariant-s-that-we-own.patch
Patch6042:      backport-gio-tests-gdbus-peer-Unref-cached-property-GVariant-value.patch
Patch6043:      backport-gdesktopappinfo-Unref-the-GDBus-call-results.patch
Patch6044:      backport-Handling-collision-between-standard-i-o-file-descriptors-and-newly-created-ones.patch
Patch6045:      backport-glocalfileoutputstream-Do-not-double-close-an-fd-on-unlink-error.patch
Patch6046:      backport-gregex-Use-pcre2-error-messages-if-we-dont-provide-a-specific-one.patch
Patch6047:      backport-regex-Use-critical-messages-if-an-unexpected-NULL-parameter-is-provided.patch
Patch6048:      backport-gregex-Allow-G_REGEX_JAVASCRIPT_COMPAT-in-compile-mask-for-g_regex_new.patch
Patch6049:      backport-gregex-Drop-explanation-G_REGEX_JAVASCRIPT_COMPAT.patch
Patch6050:      backport-gregex-Remove-an-unreachable-return-statement.patch
Patch6051:      backport-gmessages-Add-missing-trailing-newline-in-fallback-log-hander.patch
Patch6052:      backport-Revert-Handling-collision-between-standard-i-o-filedescriptors-and-newly-created-ones.patch
patch6053:      backport-gdbusinterfaceskeleton-Fix-a-use-after-free-of-a-GDBusMethodInvocation.patch
patch6054:      backport-CVE-2023-24593_CVE-2023-25180-1.patch
patch6055:      backport-CVE-2023-24593_CVE-2023-25180-2.patch
patch6056:      backport-gdbusconnection-Fix-double-unref-on-timeout-cancel-sending-a-message.patch
patch6057:      backport-add-g_free_sized-and-g_aligned_free_sized.patch
patch6058:      backport-gkeyfile-Fix-group-comment-management.patch
patch6059:      backport-gkeyfile-Ensure-we-don-t-add-extra-blank-line-above-new-group.patch
patch6060:      backport-gkeyfile-Skip-group-comment-when-adding-a-new-key-to-a-group.patch
patch6061:      backport-glocalfilemonitor-Avoid-file-monitor-destruction-from-event-thread.patch
patch6062:      backport-glocalfilemonitor-Skip-event-handling-if-the-source-has-been-destroyed.patch
patch6063:      backport-tests-Add-a-test-for-GFileMonitor-deadlocks.patch

Patch6064:      backport-gregex-set-default-max-stack-size-for-PCRE2-JIT-compiler-to-512KiB.patch
Patch6065:      backport-gregex-if-JIT-stack-limit-is-reached-fall-back-to-interpretive-matching.patch
Patch6066:      backport-Make-sure-the-GTask-is-freed-on-a-graceful-disconnect.patch
Patch6067:      backport-gmessages-fix-dropping-irrelevant-log-domains.patch
Patch6068:      backport-gutils-Fix-an-unlikely-minor-leak-in-g_build_user_data_dir.patch
Patch6069:      backport-openharmony-adapt.patch
Patch6070:      backport-openharmony-dummy.patch


BuildRequires:  chrpath gcc gcc-c++ gettext perl-interpreter
BUildRequires:  glibc-devel libattr-devel libselinux-devel meson
BuildRequires:  systemtap-sdt-devel pkgconfig(libelf) pkgconfig(libffi)
BuildRequires:  pkgconfig(libpcre2-8) pkgconfig(mount) pkgconfig(zlib)
BuildRequires:  python3-devel
%ifnarch i686
BuildRequires:  desktop-file-utils shared-mime-info gtk-doc 
%endif

Provides:  %{name}-fam = %{version}-%{release}
Obsoletes: %{name}-fam < %{version}-%{release}

Recommends:     shared-mime-info
Conflicts:      gcr < 3.28.1

Provides: bundled(gnulib)
Provides: bundled(gvdb)
Provides: bundled(libcharset)
Provides: bundled(xdgmime)

%description
GLib is a bundle of three (formerly five) low-level system libraries
written in C and developed mainly by GNOME. GLib's code was separated
from GTK, so it can be used by software other than GNOME and has been
developed in parallel ever since.

%package        devel
Summary:        Development for the GLib library
Requires:       %{name} = %{version}-%{release}
Requires:       gdb-headless

%description    devel
Development for the GLib library.

%package        static
Summary:        glib static
Requires:       pcre2-static
Requires:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-static = %{version}-%{release}
Obsoletes:      %{name}-static < %{version}-%{release}

%description    static
The %{name}-static subpackage contains libraries for %{name}.

%package        tests
Summary:        Tests for the glib2 package
Requires:       %{name}-devel = %{version}-%{release}
Provides:       %{name}-tests = %{version}-%{release}
Obsoletes:      %{name}-tests < %{version}-%{release}

%description    tests
The glib2-tests package contains tests that can be used to verify
the functionality of the installed package.

%ifnarch i686
%package        help
Summary:        help document for the glib2 package
Buildarch:      noarch
Provides:       %{name}-doc = %{version}-%{release}
Obsoletes:      %{name}-doc < %{version}-%{release}

%description    help
help document for the glib2 package.
%endif

%prep
%autosetup -n glib-%{version} -p1

%build

%meson --default-library=both  -Ddtrace=true  \
%ifnarch i686
    -Dman=true -Dgtk_doc=true \
%else
    -Dman=false -Dgtk_doc=false \
%endif
    -Dsystemtap=true -Dinstalled_tests=true \
    -Dglib_debug=disabled -Dsysprof=disabled 

%meson_build
find . -name *.dtrace-temp.c -exec rm -f {} \;

%check
%meson_test

%install
%meson_install
%global py_reproducible_pyc_path %{buildroot}%{_datadir}
touch -r gio/gdbus-2.0/codegen/config.py.in %{buildroot}%{_datadir}/glib-2.0/codegen/*.py
chrpath --delete %{buildroot}%{_libdir}/*.so

export PYTHONHASHSEED=0
%py_byte_compile %{__python3} %{buildroot}%{_datadir}

mv %{buildroot}%{_bindir}/gio-querymodules %{buildroot}%{_bindir}/gio-querymodules-%{__isa_bits}
sed -i -e "/^gio_querymodules=/s/gio-querymodules/gio-querymodules-%{__isa_bits}/" %{buildroot}%{_libdir}/pkgconfig/gio-2.0.pc
mkdir -p %{buildroot}%{_libdir}/gio/modules
touch %{buildroot}%{_libdir}/gio/modules/giomodule.cache

# remove pycache
rm -rf %{buildroot}/%{_datadir}/gdb/auto-load/%{_libdir}/__pycache__
rm -rf %{buildroot}/%{_datadir}/glib-2.0/codegen/__pycache__
rm -rf %{buildroot}/%{_datadir}/glib-2.0/gdb/__pycache__

# remove rpath
chrpath -d %{buildroot}%{_libexecdir}/installed-tests/glib/gdbus-peer

%find_lang glib20

%transfiletriggerin -- %{_libdir}/gio/modules
gio-querymodules-%{__isa_bits} %{_libdir}/gio/modules &> /dev/null || :

%transfiletriggerpostun -- %{_libdir}/gio/modules
gio-querymodules-%{__isa_bits} %{_libdir}/gio/modules &> /dev/null || :

%transfiletriggerin -- %{_datadir}/glib-2.0/schemas
glib-compile-schemas %{_datadir}/glib-2.0/schemas &> /dev/null || :

%transfiletriggerpostun -- %{_datadir}/glib-2.0/schemas
glib-compile-schemas %{_datadir}/glib-2.0/schemas &> /dev/null || :

%files -f glib20.lang
%defattr(-,root,root)
%doc AUTHORS NEWS README
%license COPYING
%{_libdir}/*.so.*
%dir %{_libdir}/gio
%ghost %{_libdir}/gio/modules/giomodule.cache

%dir %{_datadir}/glib-2.0
%dir %{_datadir}/bash-completion
%{_datadir}/bash-completion/completions/gapplication
%{_datadir}/bash-completion/completions/gdbus
%{_datadir}/bash-completion/completions/gio
%{_datadir}/bash-completion/completions/gsettings

%{_bindir}/gio
%{_bindir}/gio-querymodules*
%{_bindir}/glib-compile-schemas
%{_bindir}/gsettings
%{_bindir}/gdbus
%{_bindir}/gapplication

%files devel
%{_libdir}/lib*.so
%{_libdir}/glib-2.0
%{_libdir}/pkgconfig/*
%{_includedir}/*

%{_datadir}/aclocal/*
%{_datadir}/glib-2.0/*
%{_datadir}/bash-completion/completions/gresource
%{_datadir}/gdb/auto-load/%{_libdir}/*-gdb.py
%{_datadir}/gettext/
%{_datadir}/systemtap/

%{_bindir}/glib-genmarshal
%{_bindir}/glib-gettextize
%{_bindir}/glib-mkenums
%{_bindir}/gobject-query
%{_bindir}/gtester
%{_bindir}/gdbus-codegen
%{_bindir}/glib-compile-resources
%{_bindir}/gresource
%attr (0755, root, root) %{_bindir}/gtester-report

%files static
%{_libdir}/*.a

%files tests
%{_libexecdir}/installed-tests
%exclude %{_libexecdir}/installed-tests/glib/cert-tests
%exclude %{_libexecdir}/installed-tests/glib/tls-certificate

%{_datadir}/installed-tests
%exclude %{_datadir}/installed-tests/glib/tls-certificate.test

%ifnarch i686
%files help
%defattr(-,root,root)
%{_mandir}/man1/*
%doc %{_datadir}/gtk-doc/html/*
%endif

%changelog
* Mon Feb 19 2024 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-14
- disable sysprof

* Thu Jan 11 2024 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-13
- fix pcre2 error , memory leak and log domains error

* Mon Sep 25 2023 liningjie <liningjie@xfusion.com> - 2.72.2-12
- glocalfilemonitor: Avoid file monitor destruction from event thread

* Sat Aug 19 2023 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-11
- fix double unref and fix group comment management

* Sat Apr 1 2023 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-10
- fix CVE-2023-24593 and CVE-2023-25180

* Tue Mar 14 2023 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-9
- fix a use-after-free of GDBusMethodInvocation

* Thu Feb 16 2023 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-8
- backport some patches from community

* Tue Jan 10 2023 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-7
- adjust pcre2 requires

* Tue Dec 27 2022 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-6
- fix some pcre2 error

* Wed Nov 9 2022 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-5
- separate the test and static package from devel package

* Sat Oct 15 2022 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-4
- backport some patches from community

* Mon Sep 5 2022 hanhuihui <hanhuihui5@huawei.com> - 2.72.2-3
- replace pcre1 with pcre2

* Sat Jun 18 2022 zhujunhao <zhujunhao11@huawei.com> - 2.72.2-2
- remove gnutls require

* Mon Jun 6 2022 lin zhang <lin.zhang@turbolinux.com.cn> - 2.72.2-1
- Update to 2.72.2

* Thu Jun 2 2022 lin zhang <lin.zhang@turbolinux.com.cn> - 2.72.0-1
- Update to 2.72.0

* Thu Apr 28 2022 yanan <yanan@huawei.com> - 2.68.1-12
- Type:bugfix
- DESC:fix issues found by svace static code analyzer
       fix segfault,memory leak,EOF,arguments leak

* Wed Apr 6 2022 hanhui <hanhui15@h-partners.com> - 2.68.1-11
- DESC:fix gfileenumerator/gdbusobjectmanagerservice/gdbusauth of memory leak

* Fri Mar 11 2022 weijin deng <weijin.deng@turbolinux.com.cn> - 2.68.1-10
- Type:bugfix
- DESC:solve glib2 enable "glib2_debug" option causes gnome-calendar reopen
       coredumped in gtk3's _gtk_widget_get_toplevel()

* Wed Mar 9 2022 yangcheng<yangcheng87@h-partners.com> - 2.68.1-9
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:remove gdbus-peer rpath compile option

* Wed Mar 2 2022 hanhui<hanhui15@h-partners.com> - 2.68.1-8
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:custom installation depend on sysprof

* Sat Feb 19 2022 wangkerong<wangkerong@h-partners.com> - 2.68.1-7
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:add i686 options，fix build failure on i686

* Sun Nov 14 2021 fengtao<fengtao40@huawei.com> - 2.68.1-6
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:add require gdb-headless for devel


* Tue Sep 14 2021 yangcheng<yangcheng87@huawei.com> - 2.68.1-5
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:Drop dependebcy on gamin

* Tue Sep 7 2021 fengtao<fengtao40@huawei.com> - 2.68.1-4
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:enable all tests

* Sat Aug 14 2021 liuyumeng<liuyumeng5@huawei.com> - 2.68.1-3
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:fix a memory leak

* Tue Aug 10 2021 liuyumeng<liuyumeng5@huawei.com> - 2.68.1-2
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:fix the third parameter of clise-range

* Wed Jun 30 2021 weijin deng <weijin.deng@turbolinux.com.cn> - 2.68.1-1
- Upgrade to 2.68.1 because gnome-builder and more new gnome applications
  need function g_memdup2 which needs glib2 ≥2.67.3 to instead of g_memdup

* Wed May 19 2021 weijin deng <weijin.deng@turbolinux.com.cn> - 2.66.8-1
- Upgrade to 2.66.8
- Update Version, Release
- Delete patch files, delete gio-launch-desktop(not exist in 2.66.8)
- Correct date, make it match weekday

* Tue Apr 13 2021 hanhui<hanhui15@huawei.com> - 2.62.5-5
- Type:cve
- Id:CVE-2021-28153
- SUG:NA
- DESC:fix CVE-2021-28153

* Sat Mar 6 2021 hanhui<hanhui15@huawei.com> - 2.62.5-4
- Type:cve
- Id:CVE-2021-27219
- SUG:NA
- DESC:fix CVE-2021-27219

* Mon Mar 1 2021 jinzhimin<jinzhimin2@huawei.com> - 2.62.5-3
- Type:cve
- Id:CVE-2021-27218
- SUG:NA
- DESC:fix CVE-2021-27218

* Sat Feb 27 2021 zhujunhao<zhujunhao8@huawei.com> - 2.62.5-2
- Type:cve
- Id:CVE-2020-35457
- SUG:NA
- DESC:fix CVE-2020-35457

* Tue Jul 21 2020 hanhui<hanhui15@huawei.com> - 2.62.5-1
- Update to 2.62.5

* Mon Mar 2 2020 hexiujun<hexiujun1@huawei.com> - 2.62.1-4
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:fix accidentally delete temp file within dtrace

* Fri Feb 28 2020 zhangrui <zhangrui182@huawei.com> - 2.62.1-3
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:remove dist in spec

* Mon Feb 24 2020 openEuler Buildteam <buildteam@openeuler.org> - 2.62.1-2
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:exclude some unnecessary files

* Thu Jan 9 2020 openEuler Buildteam <buildteam@openeuler.org> - 2.62.1-1
- update to 2.62.1

* Tue Dec 24 2019 openEuler Buildteam <buildteam@openeuler.org> - 2.58.1-6
- change the path of files

* Sat Dec 21 2019 openEuler Buildteam <buildteam@openeuler.org> - 2.58.1-5
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:Fix a leaking GRemoteActionGroup member

* Sat Nov 23 2019 openEuler Buildteam <buildteam@openeuler.org> - 2.58.1-4
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:add the libxslt in buildrequires

* Wed Sep 25 2019 huzunhao<huzunhao2@huawei.com> - 2.58.1-3
- Type:cves
- ID:CVE-2019-12450 CVE-2019-13012
- SUG:restart
- DESC:fix CVE-2019-12450 CVE-2019-13012

* Thu Sep 19 2019 Lijin Yang <yanglijin@huawei.com> - 2.58.1-2
- Package init
