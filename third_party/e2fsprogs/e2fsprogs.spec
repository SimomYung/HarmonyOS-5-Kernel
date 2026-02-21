Name:           e2fsprogs
Version:        1.46.4
Release:        24
Summary:        Second extended file system management tools
License:        GPLv2+ and LGPLv2 and MIT
URL:            http://e2fsprogs.sourceforge.net/
Source0:        https://www.kernel.org/pub/linux/kernel/people/tytso/%{name}/v%{version}/%{name}-%{version}.tar.xz
Source1:        ext2_types-wrapper.h

Patch1:		0001-e2fsprogs-set-hugefile-from-4T-to-1T-in-hugefile-tes.patch
Patch2:		0002-libss-add-newer-libreadline.so.8-to-dlopen-path.patch
Patch3:		0003-tests-update-expect-files-for-f_mmp_garbage.patch
Patch4:		0004-tests-update-expect-files-for-f_large_dir-and-f_larg.patch
Patch5:		0005-resize2fs-resize2fs-disk-hardlinks-will-be-error.patch
Patch6:		0006-e2fsck-exit-journal-recovery-when-find-EIO-ENOMEM-er.patch
Patch7:		0007-e2fsck-exit-journal-recovery-when-jounral-superblock.patch
Patch8:		0008-e2fsck-add-env-param-E2FS_UNRELIABLE_IO-to-fi.patch
Patch9:		0009-e2mmpstatus.8.in-detele-filesystem-can-be-UUID-or-LA.patch		
Patch10:	0010-tests-update-expect-file-for-u_direct_io.patch
Patch11:	0011-libext2fs-don-t-old-the-CACHE_MTX-while-doing-I-O.patch
Patch12:	0012-tests-fix-ACL-printing-tests.patch
Patch13:	0013-e2fsck-do-not-clean-up-file-acl-if-the-inode-is-trun.patch
Patch14:	0014-e2fsck-handle-level-is-overflow-in-ext2fs_extent_get.patch
Patch15:	0015-libext2fs-add-sanity-check-to-extent-manipulation.patch
Patch16:        0016-e2fsprogs-add-sw64.patch
Patch17:	0017-tune2fs-do-not-change-j_tail_sequence-in-journal-sup.patch
Patch18:	0018-debugfs-teach-logdump-the-n-num_trans-option.patch
Patch19:	0019-tune2fs-fix-tune2fs-segfault-when-ext2fs_run_ext3_jo.patch
Patch20:	0020-tune2fs-tune2fs_main-should-return-rc-when-some-erro.patch
Patch21:	0021-tune2fs-exit-directly-when-fs-freed-in-ext2fs_run_ext3_journal.patch
Patch22:	0022-unix_io.c-fix-deadlock-problem-in-unix_write_blk64.patch
Patch23:	0023-debugfs-fix-repeated-output-problem-with-logdump-O-n.patch
Patch24:        0024-tune2fs-check-return-value-of-ext2fs_mmp_update2-in-.patch
Patch25:        0025-mmp-fix-wrong-comparison-in-ext2fs_mmp_stop.patch
Patch26:        0026-misc-fsck.c-Processes-may-kill-other-processes.patch
Patch27:        0027-dumpe2fs-resize2fs-avoid-memory-leak-on-error-path.patch
Patch28:        0028-libext2fs-fix-memory-leak-in-error-path-while-openin.patch
Patch29:        0029-e2fsck-avoid-theoretical-null-dereference-in-end_pro.patch
Patch30:        0030-e2fsck-fix-potential-out-of-bounds-read-in-inc_ea_in.patch
Patch31:        0031-e2fsck-avoid-out-of-bounds-write-for-very-deep-exten.patch
Patch32:        0032-e2fsck-fix-potential-fencepost-error-in-e2fsck_shoul.patch
Patch33:        0033-libext2fs-fix-potential-integer-overflow-in-bitmap-a.patch
Patch34:        0034-tune2fs-fix-an-error-message.patch
Patch35:        0035-e2fsck-don-t-allow-journal-inode-to-have-encrypt-fla.patch
Patch36:        0036-lib-ext2fs-fix-unbalanced-mutex-unlock-for-BOUNCE_MT.patch
Patch37:        0037-libext2fs-fix-ext2fs_compare_generic_bmap-logic.patch
Patch38:        0038-Quiet-unused-variable-warnings.patch
Patch39:        0039-ext2fs-Use-64bit-lseek-when-_FILE_OFFSET_BITS-is-64.patch
Patch40:        0040-e2fsck-fix-bad-htree-checksums-in-preen-mode.patch
Patch41:	0041-debugfs-Fix-infinite-loop-when-dump-log.patch

BuildRequires:  gcc pkgconfig texinfo
BuildRequires:  fuse-devel libblkid-devel libuuid-devel
BuildRequires:  audit
Recommends:	%{name}-help = %{version}-%{release}

Provides:       e2fsprogs-libs%{?_isa} = %{version}-%{release} e2fsprogs-libs = %{version}-%{release}
Obsoletes:      e2fsprogs-libs < %{version}
Provides:       libcom_err%{?_isa} = %{version}-%{release} libcom_err = %{version}-%{release}
Obsoletes:      libcom_err < %{version}
Provides:       libss%{?_isa} = %{version}-%{release} libss = %{version}-%{release}
Obsoletes:      libss < %{version}

%description
The e2fsprogs package consists of a lot of tools for users to create,
check, modify, and correct any inconsistencies in second extended file
system.

%package devel
Summary: Second extended file system libraries and headers
License: GPLv2 and LGPLv2 and MIT
Requires: e2fsprogs = %{version}-%{release}
Requires: gawk
Requires: pkgconfig
Requires(post): info
Requires(preun): info
Provides: libcom_err-devel%{?_isa} = %{version}-%{release} libcom_err-devel = %{version}-%{release}
Obsoletes: libcom_err-devel < %{version}
Provides: libss-devel%{?_isa} = %{version}-%{release} libss-devel = %{version}-%{release}
Obsoletes: libss-devel < %{version}
Provides: e2fsprogs-static{?_isa} = %{version}-%{release} e2fsprogs-static = %{version}-%{release}
Obsoletes: e2fsprogs-static < %{version}

%description devel
This package provides libraries and header files to develop
second extended file system userspace programs.

%package help
Summary: man files for e2fsprogs
Requires: man
BuildArch: noarch

%description help
This packages includes man files for e2fsprogs.

%prep
%autosetup -n %{name}-%{version} -p1


%build
%configure CFLAGS="$RPM_OPT_FLAGS -fno-strict-aliasing" \
           --enable-elf-shlibs --enable-nls --disable-uuidd --disable-fsck \
           --disable-e2initrd-helper --disable-libblkid --disable-libuuid \
           --enable-quota --with-root-prefix=/usr
%make_build V=1

%install
make install install-libs DESTDIR=%{buildroot} INSTALL="%{__install} -p" \
    root_sbindir=%{_sbindir} root_libdir=%{_libdir}
chmod +w %{buildroot}%{_libdir}/*.a

# Replace arch-dependent header file with arch-independent stub (when needed).
#%multilib_fix_c_header --file %{_includedir}/ext2fs/ext2_types.h
# ugly hack to allow parallel install of 32-bit and 64-bit -devel packages:
%define multilib_arches %{ix86} x86_64

%ifarch %{multilib_arches}
mv -f %{buildroot}%{_includedir}/ext2fs/ext2_types.h \
      %{buildroot}%{_includedir}/ext2fs/ext2_types-%{_arch}.h

install -p -m 644 %{SOURCE1} %{buildroot}%{_includedir}/ext2fs/ext2_types.h

%endif

%find_lang %{name}

rm -f %{buildroot}/etc/cron.d/e2scrub_all
rm -f %{buildroot}%{_libdir}/e2fsprogs/e2scrub_all_cron

%ifarch i686
rm -rf %{buildroot}%{_unitdir}/e2scrub*
%endif

%check
make fullcheck

%ldconfig_scriptlets

%post devel
if [ -f %{_infodir}/libext2fs.info.gz ]; then
   /sbin/install-info %{_infodir}/libext2fs.info.gz %{_infodir}/dir || :
fi

%preun devel
if [ $1 = 0 -a -f %{_infodir}/libext2fs.info.gz ]; then
   /sbin/install-info --delete %{_infodir}/libext2fs.info.gz %{_infodir}/dir || :
fi
exit 0

%files -f %{name}.lang
%doc README
%license NOTICE
%config(noreplace) /etc/mke2fs.conf
%config(noreplace) /etc/e2scrub.conf
%{_bindir}/chattr
%{_bindir}/fuse2fs
%{_bindir}/lsattr
%{_libdir}/e2fsprogs/e2scrub_fail
%{_libdir}/libe2p.so.*
%{_libdir}/libcom_err.so.*
%{_libdir}/libss.so.*
%{_sbindir}/*
%{_udevrulesdir}/*.rules
%ifnarch i686
%{_unitdir}/e2scrub*
%endif

%files devel
%{_bindir}/compile_et
%{_bindir}/mk_cmds
%{_datadir}/et
%{_datadir}/ss
%{_infodir}/libext2fs.info*
%{_includedir}/e2p
%{_includedir}/ext2fs
%{_includedir}/et
%{_includedir}/com_err.h
%{_includedir}/ss
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so
%{_libdir}/*.a

%files help
%{_mandir}/man1/*
%{_mandir}/man3/*
%{_mandir}/man5/*
%{_mandir}/man8/*

%changelog
* Tue Dec 19 2023 haowenchao <haowenchao2@huawei.com> - 1.46.4-24
- debugfs: Fix infinite loop when dump log

* Thu Nov 09 2023 Xinliang Liu <xinliang.liu@linaro.org> - 1.46.4-23
- Fix rpmlint Provides/Obsoletes unversioned warnings to fix dnf update

* Mon Oct 30 2023 volcanodragon <linfeilong@huawei.com> - 1.46.4-22
- e2fsck fix bad htree checksum in preen mode

* Sun Jun 25 2023 suweifeng <suweifeng1@huawei.com> - 1.46.4-21
- backport patches from upstream

* Fri Jun 9 2023 tangyuchen <tangyuchen5@huawei.com> - 1.46.4-20
- delete invalid link for ext4

* Thu Mar 30 2023 Zhiqiang Liu <liuzhiqiang26@huawei.com> - 1.46.4-19
- backport one patch to fix: processes may kill other processes in misc/fsck.c

* Thu Feb 9 2023 lihaoxiang <lihaoxiang9@huawei.com> - 1.46.4-18
- Upstream patches regress for debugfs, tune2fs and mmp.

* Thu Dec 1 2022 Zhiqiang Liu <liuzhiqiang26@huawei.com> - 1.46.4-17
- fix deadlock problem in unix_write_blk64

* Fri Oct 14 2022 Zhiqiang Liu <liuzhiqiang26@huawei.com> - 1.46.4-16
- tune2fs: fix segfault problem

* Fri Sep 23 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-15
- test: fix ACL-printing tests from community

* Sat Aug 20 2022 Zhiqiang Liu <liuzhiqiang26@huawei.com> - 1.46.4-14
- debugfs: teach logdump the -n <num_trans> option

* Fri Aug 12 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-13
- tune2fs: do not change j_tail_sequence in journal superblock

* Fri Jun 24 2022 wuzx<wuzx1226@qq.com> - 1.46.4-12
- add sw64 patch

* Tue Jun 21 2022 lihaoxiang <lihaoxiang9@huawei.com> - 1.46.4-11
- DESC:add wrapper header file for i686 and x86_64 then fix conflicts when intall i686 rpms.
 
* Sat May 28 2022 Zhiqiang Liu <liuzhiqiang26@huawei.com> - 1.46.4-10
- fix CVE-2022-1304

* Fri May 20 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-9
- e2fsck: handle->level is overflow in ext2fs_extent_get.

* Wed May 18 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-8
- e2fsck: do not clean up file acl if the inode is truncating type

* Thu Mar 17 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-7
- tests: skip m_rootdir_acl if selinux is not disabled

* Wed Mar 9 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-6
- libext2fs: don't old the CACHE_MTX while doing I/O

* Tue Mar 8 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-5
- tests: update expect file for u_direct_io

* Wed Mar 2 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-4
- e2mmpstatus.8.in: detele filesystem can be UUID or LABEL in manpage 

* Thu Feb 24 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-3
- adapt patchs from openEuler-20.03-LTS

* Thu Jan 27 2022 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-2
- replace License in spec

* Sat Nov 27 2021 zhanchengbin <zhanchengbin1@huawei.com> - 1.46.4-1
- update package to v1.46.4.

* Mon Nov 15 2021 zhanchengbin <zhanchengbin1@huawei.com> - 1.45.6-7
- DESC: integrate community patches.

* Mon Sep 13 2021 lixiaokeng <lixiaokeng@huawei.com> - 1.45.6-6
- DESC: add newer libreadline.so.8 to dlopen path

* Fri Aug 20 2021 chenyanpanHW <chenyanpan@huawei.com> - 1.45.6-5
- DESC: add necessary BuildRequires audit

* Fri Jul 30 2021 chenyanpanHW <chenyanpan@huawei.com> - 1.45.6-4
- DESC: delete -Sgit from %autosetup, and delete BuildRequires git

* Wed Dec 16 2020 yanglongkang <yanglongkang@huawei.com> - 1.45.6-3
- Set help package as install require

* Fri Oct 30 2020 Zhiqiang Liu <lzhq28@mail.ustc.edu.cn> - 1.45.6-2
- backport upstream patches-epoch2 to fix some problems

* Wed Jul 15 2020 Zhiqiang Liu <lzhq28@mail.ustc.edu.cn> - 1.45.6-1
- rebuild package

* Wed Jul 1 2020 Wu Bo <wubo009@163.com> - 1.45.3-5
- rebuild package

* Mon Feb 3 2020 luoshijie <luoshijie1@huawei.com> - 1.45.3-4
- Type:cves
- ID:CVE-2019-5094
- SUG:restart
- DESC:backport patch to fix CVE-2019-5094.

* Wed Jan 22 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.45.3-3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:change path to remove no used file.

* Wed Jan 22 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.45.3-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:fix local rpmbuild error.

* Tue Jan 14 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.45.3-1
- Type:cves
- ID:CVE-2019-5188
- SUG:restart
- DESC:backport patch to fix CVE-2019-5188.

* Tue Jan 14 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.45.3-0
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:update package from 1.44.3 to 1.45.3.

* Mon Jan 13 2020 openEuler Buildteam <buildteam@openeuler.org> - 1.44.3-8
- Type:enhancement
- ID:NA
- SUG:NA
- DESC:update spec.

* Wed Sep 18 2019 luoshijie <luoshijie1@huawei.com> - 1.44.3-7
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:modify spec file to follow spec rules.

* Fri Sep 6 2019 luoshijie <luoshijie1@huawei.com> - 1.44.3-6
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:openEuler Debranding

* Tue Aug 20 2019 luoshijie <luoshijie1@huawei.com> - 1.44.3-5
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:rename patch name

* Wed Jul 10 2019 zhangyujing <zhangyujing1@huawei.com> - 1.44.3-4
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:e2freefrag: fix memory leak in scan_online()
       create_inode: fix potential memory leak in path_append()
       mke2fs: fix check for absurdly large devices

* Fri Mar 15 2019 zhangyujing <zhangyujing1@huawei.com> - 1.44.3-3
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:blkid avoid FPE crash when probing a HFS superblock
       AOSP e2fsdroid Fix crash with invalid command line a
       e2fsck fix fd leak in reserve_stdio_fds
       libext2fs fix uninitialized length in rep_strdup
       tune2fs fix dereference of freed memory after journa
       libe2p avoid segfault when s_nr_users is too high
       e2freefrag fix free blocks count during live scan

* Wed Jan 23 2019 wangxiao <wangxiao65@huawei.com> - 1.44.3-2
- Type:bugfix
- ID:NA
- SUG:NA
- DESC:disable the metadata_csum creat by mke2fs -t ext4 by default
- Package init

