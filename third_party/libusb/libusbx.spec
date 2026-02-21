#Resolve name conflict with libusb-compat-0.1
%global source  libusb

Name:           libusbx
Version:        1.0.26
Release:        1
Summary:        Library for accessing USB devices
License:        LGPLv2+
URL:            http://libusb.info
Source0:        https://github.com/libusb/libusb/releases/download/v1.0.26/libusb-1.0.26.tar.bz2

BuildRequires:  systemd-devel doxygen libtool

%description
libusb is a library for USB device access from Linux, macOS, Windows, OpenBSD/NetBSD and Haiku userspace. It is written in C (Haiku backend in C++) and licensed under the GNU Lesser General Public License version 2.1 or, at your option, any later version (see COPYING).

libusb is abstracted internally in such a way that it can hopefully be ported to other operating systems. Please see the PORTING file for more information.

%package        devel
Summary:        %{name} libs and docs files for development
Requires:       %{name}%{?_isa} = %{version}-%{release}
Provides:       libusb1-devel = %{version}-%{release}
Provides:       libusb1-devel-doc = %{version}-%{release}
Provides:       libusbx-devel-doc
Obsoletes:      libusb1-devel <= 1.0.9
Obsoletes:      libusb1-devel-doc <= 1.0.22-1
Obsoletes:      libusbx-devel-doc

%description    devel
The %{name}-devel package contains libraries and header files.

%prep
%autosetup -n %{source}-%{version} -p1

%build
mkdir -p m4
autoreconf -ivf
%configure --disable-static --enable-examples-build
%make_build
make docs -C doc

%check
make check

%install
%make_install

%ldconfig_scriptlets

%files
%doc AUTHORS README ChangeLog
%license COPYING
%{_libdir}/*.so.*
%exclude %{_libdir}/*.la

%files devel
%doc examples/*.c
%{_includedir}/libusb-1.0
%{_libdir}/*.so
%{_libdir}/pkgconfig/libusb-1.0.pc

%changelog
* Fri Oct 28 2022 Wenchao Hao <haowenchao@hauwei.com> - 1.0.26-1
- Update to libusb-1.0.26

* Tue Oct 11 2022 huangduirong <huangduirong@huawei.com> - 1.0.24-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:Move autoreconf to build

* Wed Nov 17 2021 Wenchao Hao <haowenchao@hauwei.com> - 1.0.24-1
- Update to libusb-1.0.24

* Fri Jul 23 2021 zhouwenpei <zhouwenpei1@hauwei.com> - 1.0.23-4
- remove unnecessary build require.

* Wed Nov 4 2020 lixiaokeng <lixiaokeng@hauwei.com> - 1.0.23-3
- add make check

* Mon Jul 27 2020 Zhiqiang Liu <lzhq28@mail.ustc.edu.cn> - 1.0.23-2
- backport upstream patches.

* Thu Jul 16 2020 zhangyouming <zhangyouming4@huawei.com> - 1.0.23-1
- upgrade form 1.0.22 to 1.0.23

* Wed Mar 25 2020 hy-euler <eulerstoragemt@huawei.com> - 1.0.22-3
- delete -Sgit in %autosetup

* Sat Aug 31 2019 suweifeng <suweifeng1@huawei.com> - 1.0.22-2
- Package init
