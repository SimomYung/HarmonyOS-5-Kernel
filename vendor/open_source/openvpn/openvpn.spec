Name: openvpn
Version: 2.6.9
Release: 1
Summary: A full-featured open source SSL VPN solution
License: GPL-2.0-or-later and OpenSSL and SSLeay
URL:     https://community.openvpn.net/openvpn
Source0: https://build.openvpn.net/downloads/releases/%{name}-%{version}.tar.gz
Patch0:  openvpn-2.4-change-tmpfiles-permissions.patch
BuildRequires: openssl-devel lz4-devel systemd-devel lzo-devel gcc
BuildRequires: iproute pam-devel pkcs11-helper-devel >= 1.11
BuildRequires: libselinux-devel
BuildRequires: libcap-ng-devel

Requires: iproute
Requires(pre): /usr/sbin/useradd

%description
OpenVPN is a full-featured open source SSL VPN solution that accommodates a wide range of configurations, 
including remote access, site-to-site VPNs, Wi-Fi security, and enterprise-scale remote access solutions with load balancing, 
failover, and fine-grained access-controls. Starting with the fundamental premise that complexity is the enemy of security, 
OpenVPN offers a cost-effective, lightweight alternative to other VPN technologies that is well-adapted for the SME and enterprise markets.

%package devel
Summary: Development headers and examples for OpenVPN plug-ins

%description devel
OpenVPN can be extended through the --plugin option, which provides possibilities to add specialized authentication, 
user accounting, packet filtering and related features. These plug-ins need to be written in C and 
provides a more low-level and information rich access to similar features as the various script-hooks.

%package help
Summary: Documents for %{name}
BuildArch: noarch

%description help
User guide and other related documents for %{name}.


%prep
%autosetup -n %{name}-%{version} -p1

%build
%configure --enable-x509-alt-username --enable-iproute2 --with-crypto-library=openssl --enable-pkcs11 --enable-selinux --enable-systemd SYSTEMD_UNIT_DIR=%{_unitdir} TMPFILES_DIR=%{_tmpfilesdir} IPROUTE=/sbin/ip
%make_build


%install
%make_install
%delete_la
mkdir -p -m 0750 $RPM_BUILD_ROOT%{_sysconfdir}/%{name}/client $RPM_BUILD_ROOT%{_sysconfdir}/%{name}/server
cp sample/sample-config-files/client.conf $RPM_BUILD_ROOT%{_sysconfdir}/%{name}/client
cp sample/sample-config-files/server.conf $RPM_BUILD_ROOT%{_sysconfdir}/%{name}/server

mkdir -m 0750 -p $RPM_BUILD_ROOT%{_rundir}/%{name}-{client,server}
mkdir -m 0770 -p $RPM_BUILD_ROOT%{_sharedstatedir}/%{name}

cp -a contrib sample $RPM_BUILD_ROOT%{_pkgdocdir}

%check
make check

%pre
getent group openvpn &>/dev/null || groupadd -r openvpn
getent passwd openvpn &>/dev/null || \
    /usr/sbin/useradd -r -g openvpn -s /sbin/nologin -c OpenVPN \
        -d /etc/openvpn openvpn

%post
if [ $1 -eq 1 ] ; then
        # Initial installation
        systemctl --no-reload preset openvpn-client@\*.service &>/dev/null || :
fi


if [ $1 -eq 1 ] ; then
        # Initial installation
        systemctl --no-reload preset openvpn-server@\*.service &>/dev/null || :
fi

%preun
if [ $1 -eq 0 ] ; then 
        # Package removal, not upgrade 
        systemctl --no-reload disable --now openvpn-client@\*.service &>/dev/null || : 
fi 


if [ $1 -eq 0 ] ; then 
        # Package removal, not upgrade 
        systemctl --no-reload disable --now openvpn-server@\*.service &>/dev/null || : 
fi

%postun
if [ $1 -ge 1 ] ; then 
        # Package upgrade, not uninstall 
        systemctl try-restart openvpn-client@\*.service &>/dev/null || : 
fi 


if [ $1 -ge 1 ] ; then 
        # Package upgrade, not uninstall 
        systemctl try-restart openvpn-server@\*.service &>/dev/null || : 
fi 

%files
%license AUTHORS COPYING COPYRIGHT.GPL
%config %{_sysconfdir}/%{name}/*/*
%{_sbindir}/%{name}
%{_libdir}/%{name}/
%{_unitdir}/%{name}-client@.service
%{_unitdir}/%{name}-server@.service
%{_tmpfilesdir}/%{name}.conf
%attr(0750,-,-) %{_rundir}/%{name}-client
%attr(0750,-,-) %{_rundir}/%{name}-server
%attr(0770,openvpn,openvpn) %{_sharedstatedir}/%{name}

%files devel
%{_pkgdocdir}/sample/sample-plugins
%{_includedir}/openvpn-plugin.h
%{_includedir}/openvpn-msg.h

%files help
%{_pkgdocdir}
%{_mandir}/man8/%{name}.8*
%{_mandir}/man5/openvpn-examples.5.gz

%changelog
* Thu Feb 22 2024 yaoxin <yao_xin001@hoperun.com> - 2.6.9-1
- Upgrade to 2.6.9

* Mon Jan 08 2024 Ge Wang <wang__ge@126.com> - 2.6.8-1
- Update to version 2.6.8

* Wed Nov 22 2023 liningjie <liningjie@xfusion.com> - 2.6.2-2
- Fix CVE-2023-46849 CVE-2023-46850

* Thu Apr 20 2023 xu_ping <707078654@qq.com> - 2.6.2-1
- Update to 2.6.2

* Thu Feb 02 2023 yaoxin <yaoxin30@h-partners.com> - 2.5.8-1
- Update to 2.5.8

* Wed Mar 30 2022 wangkai <wangkai385@huawei.com> - 2.5.5-2
- Fix CVE-2022-0547

* Wed Dec 29 2021 zhangjiapeng <zhangjiapeng9@huawei.com> - 2.5.5-1
- Update to 2.5.5

* Wed Jun 9 2021 zhaoyao <zhaoyao32@huawei.com> - 2.4.8-6
- fix faileds: /bin/sh: gcc: command not found.

* Tue May 25 2021 wangyue <wangyue92@huawei.com> - 2.4.8-5
- fix CVE-2020-15078

* Thu Feb 04 2021 wangyue <wangyue92@huawei.com> 2.4.8-4
- fix CVE-2020-11810

* Mon Mar 16 2020 daiqianwen <daiqianwen@huawei.com> 2.4.8-3
- modify systemd post preun postun

* Mon Nov 11 2019 guanyalong <guanyalong@huawei.com> 2.4.8-2
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:enable pkcs11 support

* Thu Nov 7 2019 openEuler Buildteam <buildteam@openeuler.org> - 2.4.8.1
- Package init
