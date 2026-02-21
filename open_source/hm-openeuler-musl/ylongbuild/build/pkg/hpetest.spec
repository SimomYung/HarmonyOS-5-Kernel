Name: %{_name}
Version: %{_version}
Release: %{_release}
Source0: %{name}-%{version}.tar.gz
Source1: hpetest.spec
Prefix: /

Summary: High Performance env Test
Group: System Environment/Libraries
License: GPL

ExclusiveArch: x86_64 aarch64

Requires: hsecurec,hsecurec-devel,hpe-hlibc-qemu,hpe-hlibc-devel,hdophi-devel,hdophi-xdophi-libs
BuildRequires: hsecurec,hsecurec-devel,hpe-hlibc-qemu,hpe-hlibc-devel,hdophi-devel,hdophi-xdophi-libs

%description
HPK:High Performance Exe TestSuite
<description>
    <measurelist>
        <type>all</type>
    </measurelist>
</description>

%package cttc
Summary: N/A
License: N/A
Group: Applications/hpetest

%description cttc
uni-cmd-agent hpetest files.

%package dtest
Summary: N/A
License: N/A
Group: Applications/dtest

%description dtest
uni-cmd-agent dtest files.

%prep
%setup -q

%build
%{_builddir}/%{name}-%{version}/build/build_world.sh %{_arch} test %{_compile_mode}

%define __debug_install_post   \
%{_rpmconfigdir}/find-debuginfo.sh %{?_find_debuginfo_opts} "%{_builddir}/%{?buildsubdir}";\
rm -rf "${RPM_BUILD_ROOT}/usr/src/debug"; \
mkdir -p "${RPM_BUILD_ROOT}/usr/src/debug/%{name}-%{version}"; \
%{nil} 

%install
rm -rf %{buildroot}
mkdir -pv %{buildroot}/{bin/hlibc,bin/hm,conf,dtest-ysx}
source /opt/RTOS/setenv.sh %{_arch} 
%{_arch}-linux-gnu-strip -g -S -d --strip-unneeded -R .comment %{_builddir}/%{name}-%{version}/output/%{_arch}/bin/*
cp %{_builddir}/%{name}-%{version}/output/%{_arch}/bin/* %{buildroot}/bin/hlibc
cp -rf %{_builddir}/../../test_frame/*.tar.gz %{buildroot}/bin/hm
cp -rf %{_builddir}/%{name}-%{version}/test/dtest-ysx/* %{buildroot}/dtest-ysx/

chmod 755 %{buildroot}/bin/*

%clean 
rm -rf %{buildroot}

%files cttc
%defattr(755,root,root,755)
%attr(550,swm,hpp_d) /bin/hlibc/*
%attr(550,swm,hpp_d) /bin/hm/*

%files dtest
%defattr(755,root,root,755)
/dtest-ysx/*
