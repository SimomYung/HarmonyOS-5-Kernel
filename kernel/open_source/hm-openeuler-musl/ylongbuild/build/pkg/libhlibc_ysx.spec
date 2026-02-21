Name: %{_name}
Version: %{_version}
Release: %{_release}
Source0: %{name}-%{version}.tar.gz
Source1: libhlibc.spec
Prefix: /

Summary: High Performance env C Libraries
Group: System Environment/Libraries
License: GPL

ExclusiveArch: x86_64 aarch64

%description
hlibc provide comm,mm,sched etc.

%package -n ylong_c-toolchain
Summary: ylong_c toolchain
%description -n ylong_c-toolchain

%package -n ylong_c
Summary: ylong_c base lib
%description -n ylong_c
libc.so

%prep
%setup -q

%build
%{_builddir}/%{name}-%{version}/build/build_world.sh %{_arch} bin %{_compile_mode}

%install
rm -rf %{buildroot}
mkdir -pv %{buildroot}/{opt/buildtools/ylongc,hpe}
%ifarch aarch64
cp -r %{_builddir}/%{name}-%{version}/output/hpe-toolchain/opt/buildtools/ylongc/hpe/aarch64/lib/libc.so %{buildroot}/hpe/
cp -r %{_builddir}/%{name}-%{version}/output/hpe-toolchain/opt/buildtools/ylongc/* %{buildroot}/opt/buildtools/ylongc/
%endif

%clean 
#rm -rf %{buildroot} 

%files -n ylong_c-toolchain
/opt/buildtools/ylongc/*

%files -n ylong_c
%attr(640,root,verona) /hpe/libc.so

