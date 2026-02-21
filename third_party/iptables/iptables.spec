%global script_path %{_libexecdir}/iptables
%global legacy_actions %{_libexecdir}/initscripts/legacy-actions
Name:		  iptables
Version:	  1.8.7
Release:	  14
Summary:	  IP packet filter administration utilities
License:	  GPLv2 and Artistic Licence 2.0 and ISC
URL:		  https://www.netfilter.org/
Source0:  	  https://www.netfilter.org/projects/iptables/files/iptables-%{version}.tar.bz2
Source1:          iptables.init
Source2:          iptables-config
Source3:          iptables.service
Source4:          sysconfig_iptables
Source5:          sysconfig_ip6tables

Patch0:		  bugfix-add-check-fw-in-entry.patch
Patch1:           tests-extensions-add-some-testcases.patch
Patch2:           backport-xshared-Fix-response-to-unprivileged-users.patch
Patch3:           backport-Improve-error-messages-for-unsupported-extensions.patch
Patch4:           backport-nft-Fix-EPERM-handling-for-extensions-without-rev-0.patch
Patch5:           backport-libxtables-Register-only-the-highest-revision-extension.patch
Patch6:           backport-nft-Expand-extended-error-reporting-to-nft_cmd-too.patch
Patch7:           backport-xtables-restore-Extend-failure-error-message.patch
Patch8:           enabled-makecheck-in-extensions.patch

Patch9:           backport-extensions-among-Fix-for-use-with-ebtables-restore.patch
Patch10:          backport-extensions-libebt_redirect-Fix-xlate-return-code.patch
Patch11:          backport-extensions-libipt_ttl-Sanitize-xlate-callback.patch
Patch12:          backport-iptables-restore-Free-handle-with-test-also.patch
Patch13:          backport-nft-Plug-memleak-in-nft_rule_zero_counters.patch
Patch14:          backport-iptables-Plug-memleaks-in-print_firewall.patch
Patch15:          backport-ebtables-translate-Print-flush-command-after-parsing-is-finished.patch
Patch16:          backport-xtables-eb-fix-crash-when-opts-isn-t-reallocated.patch
Patch17:          backport-iptables-Fix-handling-of-non-existent-chains.patch

BuildRequires:    bison flex gcc kernel-headers libpcap-devel libselinux-devel systemd
BuildRequires:    libmnl-devel libnetfilter_conntrack-devel libnfnetlink-devel libnftnl-devel
BuildRequires:    autogen autoconf automake libtool

Requires:         %{name}-libs = %{version}-%{release}
Conflicts:	  setup < 2.10.4-1

Requires(post):   %{_sbindir}/update-alternatives
Requires(postun): %{_sbindir}/update-alternatives
%{?systemd_requires}

Provides:         iptables-utils iptables-services
Obsoletes:        iptables-utils iptables-services 

%description
Netfilter is a set of hooks inside the Linux kernel that allows kernel 
modules to register callback functions with the network stack. A 
registered callback function is then called back for every packet that 
traverses the respective hook within the network stack.

Iptables is a generic table structure for the definition of rulesets. 
Each rule within an IP table consists of a number of classifiers 
(iptables matches) and one connected action (iptables target).

Netfilter, ip_tables, connection tracking (ip_conntrack, nf_conntrack) 
and the NAT subsystem together build the major parts of the framework.

%package          libs
Summary:          iptables libraries

%description      libs
iptables libraries.

%package          devel
Summary:          header files for iproute
Requires:         %{name} = %{version}-%{release} pkgconfig

%description      devel
Header files for iproute.

%package          nft
Summary:          nft package for iproute
Requires:         %{name} = %{version}-%{release}
Obsoletes:        iptables-compat < 1.6.2-4

%description      nft
Nft package for iproute.

%package_help

%prep
%autosetup -n %{name}-%{version} -p1

%build
./autogen.sh
%configure --enable-devel --enable-bpf-compiler --with-kernel=/usr --with-kbuild=/usr --with-ksource=/usr

%disable_rpath

rm -f include/linux/types.h

%make_build

%check
make check

%install
%make_install 

%delete_la

install -m 0755 -d %{buildroot}%{_includedir}/iptables
install -m 0644 include/ip*tables.h %{buildroot}%{_includedir}
install -m 0644 include/iptables/internal.h %{buildroot}%{_includedir}/iptables

install -m 0755 -d %{buildroot}%{_includedir}/libipulog/
install -m 0644 include/libipulog/*.h %{buildroot}%{_includedir}/libipulog

install -m 0755 -d %{buildroot}/%{script_path}
install -m 0755 -c %{SOURCE1} %{buildroot}/%{script_path}/iptables.init
sed -e 's;iptables;ip6tables;g' -e 's;IPTABLES;IP6TABLES;g' < %{SOURCE1} > ip6tables.init
install -m 0755 ip6tables.init %{buildroot}/%{script_path}/ip6tables.init
install -m 0755 -d %{buildroot}%{_sysconfdir}/sysconfig
install -m 0600 -c %{SOURCE2} %{buildroot}%{_sysconfdir}/sysconfig/iptables-config
sed -e 's;iptables;ip6tables;g' -e 's;IPTABLES;IP6TABLES;g' < %{SOURCE2} > ip6tables-config
install -m 0600 -c ip6tables-config %{buildroot}%{_sysconfdir}/sysconfig/ip6tables-config
install -m 0600 -c %{SOURCE4} %{buildroot}%{_sysconfdir}/sysconfig/iptables
install -m 0600 -c %{SOURCE5} %{buildroot}%{_sysconfdir}/sysconfig/ip6tables

install -m 0755 -d %{buildroot}%{_unitdir}
install -m 0644 -c %{SOURCE3} %{buildroot}%{_unitdir}
sed -e 's;iptables;ip6tables;g' -e 's;IPv4;IPv6;g' -e 's;/usr/libexec/ip6tables;/usr/libexec/iptables;g' < %{SOURCE3} > ip6tables.service
install -m 0644 -c ip6tables.service %{buildroot}%{_unitdir}

install -m 0755 -d %{buildroot}/%{legacy_actions}/iptables
install -m 0755 -d %{buildroot}/%{legacy_actions}/ip6tables

pushd  %{buildroot}/%{legacy_actions}/iptables
cat << EOF > save
#!/bin/bash
exec %{script_path}/iptables.init save
EOF
chmod 0755 save
popd
sed -e 's;iptables.init;ip6tables.init;g' -e 's;IPTABLES;IP6TABLES;g' < %{buildroot}/%{legacy_actions}/iptables/save > ip6tabes.save-legacy
install -m 0755 -c ip6tabes.save-legacy %{buildroot}/%{legacy_actions}/ip6tables/save

pushd %{buildroot}/%{legacy_actions}/iptables
cat << EOF > panic
#!/bin/bash
exec %{script_path}/iptables.init panic
EOF
chmod 0755 panic
popd
sed -e 's;iptables.init;ip6tables.init;g' -e 's;IPTABLES;IP6TABLES;g' < %{buildroot}/%{legacy_actions}/iptables/panic > ip6tabes.panic-legacy
install -m 0755 -c ip6tabes.panic-legacy %{buildroot}/%{legacy_actions}/ip6tables/panic

install -m 0755 iptables/iptables-apply %{buildroot}%{_sbindir}
install -m 0755 iptables/iptables-apply.8 %{buildroot}%{_mandir}/man8

# Remove /etc/ethertypes (now part of setup)
rm -f %{buildroot}%{_sysconfdir}/ethertypes

touch %{buildroot}%{_libexecdir}/arptables-helper

touch %{buildroot}%{_mandir}/man8/arptables.8
touch %{buildroot}%{_mandir}/man8/arptables-save.8
touch %{buildroot}%{_mandir}/man8/arptables-restore.8
touch %{buildroot}%{_mandir}/man8/ebtables.8

cp -a %{_libdir}/libip*tc.so.0.* %{buildroot}%{_libdir}
%ldconfig_scriptlets

%post
pfx=%{_sbindir}/iptables
pfx6=%{_sbindir}/ip6tables
%{_sbindir}/update-alternatives --install \
	$pfx iptables $pfx-legacy 10 \
	--slave $pfx6 ip6tables $pfx6-legacy \
        --slave $pfx-restore iptables-restore $pfx-legacy-restore \
        --slave $pfx-save iptables-save $pfx-legacy-save \
        --slave $pfx6-restore ip6tables-restore $pfx6-legacy-restore \
        --slave $pfx6-save ip6tables-save $pfx6-legacy-save

%systemd_post iptables.service ip6tables.service

%preun
%systemd_preun iptables.service ip6tables.service

%postun
if [ $1 -eq 0 ]; then
	%{_sbindir}/update-alternatives --remove \
		iptables %{_sbindir}/iptables-legacy
fi
%?ldconfig
%systemd_postun iptables.service ip6tables.service

%post             nft
pfx=%{_sbindir}/iptables
pfx6=%{_sbindir}/ip6tables
%{_sbindir}/update-alternatives --install \
	$pfx iptables $pfx-nft 10 \
	--slave $pfx6 ip6tables $pfx6-nft \
	--slave $pfx-restore iptables-restore $pfx-nft-restore \
	--slave $pfx-save iptables-save $pfx-nft-save \
	--slave $pfx6-restore ip6tables-restore $pfx6-nft-restore \
	--slave $pfx6-save ip6tables-save $pfx6-nft-save

pfx=%{_sbindir}/ebtables
manpfx=%{_mandir}/man8/ebtables
for sfx in "" "-restore" "-save"; do
	if [ "$(readlink -e $pfx$sfx)" == $pfx$sfx ]; then
		rm -f $pfx$sfx
	fi
done
if [ "$(readlink -e $manpfx.8.gz)" == $manpfx.8.gz ]; then
	rm -f $manpfx.8.gz
fi
%{_sbindir}/update-alternatives --install \
	$pfx ebtables $pfx-nft 10 \
	--slave $pfx-save ebtables-save $pfx-nft-save \
	--slave $pfx-restore ebtables-restore $pfx-nft-restore \
	--slave $manpfx.8.gz ebtables-man $manpfx-nft.8.gz

pfx=%{_sbindir}/arptables
manpfx=%{_mandir}/man8/arptables
lepfx=%{_libexecdir}/arptables
for sfx in "" "-restore" "-save"; do
	if [ "$(readlink -e $pfx$sfx)" == $pfx$sfx ]; then
		rm -f $pfx$sfx
	fi
	if [ "$(readlink -e $manpfx$sfx.8.gz)" == $manpfx$sfx.8.gz ]; then
		rm -f $manpfx$sfx.8.gz
	fi
done
if [ "$(readlink -e $lepfx-helper)" == $lepfx-helper ]; then
	rm -f $lepfx-helper
fi
%{_sbindir}/update-alternatives --install \
	$pfx arptables $pfx-nft 10 \
	--slave $pfx-save arptables-save $pfx-nft-save \
	--slave $pfx-restore arptables-restore $pfx-nft-restore \
	--slave $manpfx.8.gz arptables-man $manpfx-nft.8.gz \
	--slave $manpfx-save.8.gz arptables-save-man $manpfx-nft-save.8.gz \
	--slave $manpfx-restore.8.gz arptables-restore-man $manpfx-nft-restore.8.gz \
	--slave $lepfx-helper arptables-helper $lepfx-nft-helper

if [ x`rpm -qa firewalld` != x ]; then
	firews=`systemctl status firewalld | grep Active | awk '{print $3}'`
	if [ "$firews" == "(running)" ]; then
		%systemd_postun_with_restart firewalld.service
	fi
fi

%postun           nft
if [ $1 -eq 0 ]; then
	for cmd in iptables ebtables arptables; do
		%{_sbindir}/update-alternatives --remove \
			$cmd %{_sbindir}/$cmd-nft
	done
fi

%files
%defattr(-,root,root)
%license COPYING
%{script_path}/ip*tables.init
%{_sysconfdir}/ethertypes
%config(noreplace) %{_sysconfdir}/sysconfig/*
%{_sbindir}/nfnl_osf
%{_sbindir}/nfbpf_*
%{_sbindir}/iptables-apply
%{_sbindir}/ip6tables-apply
%{_sbindir}/ip*tables-legacy*
%{_sbindir}/xtables-legacy-multi
%exclude %{_sbindir}/*-nft*
%exclude %{_sbindir}/*-translate
%exclude %{_sbindir}/xtables-monitor
%{_bindir}/iptables-xml
%{_unitdir}/*.service
%dir %{legacy_actions}
%{legacy_actions}/ip*
%{_datadir}/xtables/pf.os
%ghost %{_sbindir}/ip*tables
%ghost %{_sbindir}/ip*tables-restore
%ghost %{_sbindir}/ip*tables-save

%files            libs
%defattr(-,root,root)
%{_libdir}/libip*tc.so.*
%{_libdir}/libxtables.so.*
%{_libdir}/libxtables.so.12*
%dir %{_libdir}/xtables
%{_libdir}/xtables/libipt*
%{_libdir}/xtables/libip6t*
%{_libdir}/xtables/libxt*

%files            devel
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%files            nft
%defattr(-,root,root)
%{_sbindir}/iptables-nft*
%{_sbindir}/iptables-restore-translate
%{_sbindir}/iptables-translate
%{_sbindir}/ip6tables-nft*
%{_sbindir}/ip6tables-restore-translate
%{_sbindir}/ip6tables-translate
%{_sbindir}/ebtables-nft*
%{_sbindir}/arptables-nft*
%{_sbindir}/xtables-nft-multi
%{_sbindir}/xtables-monitor
%dir %{_libdir}/xtables
%{_libdir}/xtables/libarpt*
%{_libdir}/xtables/libebt*
%ghost %{_sbindir}/iptables
%ghost %{_sbindir}/iptables-restore
%ghost %{_sbindir}/iptables-save
%ghost %{_sbindir}/ip6tables
%ghost %{_sbindir}/ip6tables-restore
%ghost %{_sbindir}/ip6tables-save
%ghost %{_sbindir}/ebtables
%ghost %{_sbindir}/ebtables-save
%ghost %{_sbindir}/ebtables-restore
%ghost %{_sbindir}/arptables
%ghost %{_sbindir}/arptables-save
%ghost %{_sbindir}/arptables-restore
%ghost %{_libexecdir}/arptables-helper

%files            help
%defattr(-,root,root)
%doc INCOMPATIBILITIES
%ghost %{_mandir}/man8/arptables.8.gz
%ghost %{_mandir}/man8/arptables-save.8.gz
%ghost %{_mandir}/man8/arptables-restore.8.gz
%ghost %{_mandir}/man8/ebtables.8.gz
%{_mandir}/man8/xtables-monitor*
%{_mandir}/man8/xtables-translate*
%{_mandir}/man8/*-nft*
%{_mandir}/man8/nfnl_osf*
%{_mandir}/man8/nfbpf_compile*
%{_mandir}/man1/iptables-xml*
%{_mandir}/man8/iptables*
%{_mandir}/man8/ip6tables*
%{_mandir}/man8/xtables-legacy*

%changelog
* Mon Aug 14 2023 zhanghao <zhanghao383@huawei.com> - 1.8.7-14
- Type:bugfix
- CVE:NA
- SUG:NA
- DESC:iptables: Fix handling of non-existent chains

* Wed Apr 12 2023 zhanghao <zhanghao383@huawei.com> - 1.8.7-13
- Type:bugfix
- CVE:NA
- SUG:NA
- DESC:xtables-eb: fix crash when opts isn't reallocated

* Tue Mar 21 2023 zhanghao <zhanghao383@huawei.com> - 1.8.7-12
- Type:bugfix
- CVE:NA
- SUG:NA
- DESC:extensions among Fix for use with ebtables restore
extensions libebt redirect Fix xlate return code
extensions libipt ttl Sanitize xlate callback
iptables restore  Free handle with test also
nft Plug memleak in nft rule zero counters
iptables Plug memleaks in print firewall
ebtables translate Print flush command after parsing is finished

* Wed Nov 30 2022 huangyu <huangyu106@huawei.com> - 1.8.7-11
- Type:feature
- ID:NA
- SUG:NA
- DESC:enabled DT test

* Mon Nov 21 2022 huangyu <huangyu106@huawei.com> - 1.8.7-10
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:add some patches

* Thu Sep 29 2022 huangyu <huangyu106@huawei.com> - 1.8.7-9
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:add some patches

* Fri Jul 01 2022 xingwei <xingwei14@h-partners.com> - 1.8.7-8
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:/etc/ethertypes has been moved into the setup package

* Wed Apr 06 2022 chenzhen <vchanger123456@163.com> - 1.8.7-7
- Type:Enhancement
- ID:NA
- SUG:NA
- DESC:add some testcases of extensions

* Thu Mar 24 2022 yanglu <yanglu72@h-partners.com> - 1.8.7-6
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:delete useless so files

* Wed Mar 02 2022 duyiwei <duyiwei@kylinos.cn> - 1.8.7-5
- change %systemd_requires to %{?systemd_requires}

* Wed Feb 23 2022 gaihuiying <eaglegai@163.com> - 1.8.7-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:fix failed message when stop iptables service

* Wed Feb 9 2022 xingwei <xingwei14@h-partners.com> - 1.8.7-3
- Type:bugfix
- ID:NA
- SUG:restart
- DESC:add check fw in entry

* Mon Aug 02 2021 chenyanpanHW <chenyanpan@huawei.com> - 1.8.7-2
- DESC: delete -S git from %autosetup, and delete BuildRequires git

* Fri Jul 23 gaihuiying <gaihuiying11@huawei.com> - 1.8.7-1
- update to 1.8.7

* Sat Jul 25 hanzhijun <hanzhijun1@huawei.com> - 1.8.5-1
- update to 1.8.5

* Thu Apr 16 2020 chenzhen <chenzhen44@huawei.com> - 1.8.1-5
- Type:cves
- ID:CVE-2019-11360
- SUG:restart
- DESC:fix CVE-2019-11360

* Sat Jan 18 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.8.1-4
- add executable permissions to iptables.init

* Wed Jan 15 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.8.1-3
- optimization the patch

* Sun Jan 12 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.8.1-2
- optimization the patch

* Fri Jan 10 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.8.1-1
- Package update

* Thu Nov 7 2019 openEuler Buildteam <buildteam@openeuler.org> - 1.8.0-6
- Type:bugfix
- Id:NA
- SUG:NA
- DESC:add iptables-libs package

* Fri Sep 20 2019 openEuler Buildteam <buildteam@openeuler.org> - 1.8.0-5
- Package init
