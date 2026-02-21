#!/bin/bash
# Copyright Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
set -e

function init_env()
{
    if [ "$2" == "test" ];then
        NAME=hpe-hlibc-test
    elif [ "$CPU_TYPE" == 'qemu' ]; then
        NAME=hpe-hlibc-qemu
    else
        NAME=hpe-hlibc
    fi
    VERSION=0.0.1
    RELEASE=1
    COMPILE_MODE=$3
    CUR_DIR=$(cd "$(dirname ""$0"")";pwd)
    ROOT_DIR="$CUR_DIR"/..
    OUTPUT_DIR="$ROOT_DIR"/output
    TARGET="$1"
    export MODULE="$2"
}

function rpm_build()
{
    #let's go
    if [ "test" == "$2" ];then
        download_gtest_framework
        download_hmtest_framework
        rpmbuild -ba --nodeps --define="_name ${NAME}" --define="_version ${VERSION}"  --define="_release ${RELEASE}" --target="$1" \
        --define="_compile_mode ${COMPILE_MODE}" "$OUTPUT_DIR"/rpmbuild/SOURCES/hpetest.spec | tee -a "$OUTPUT_DIR"/rpmbuild/BUILD/compile_log.txt 2>&1
    elif [ "bin" == "$2" ] || [ "" == "$2" ];then
        rpmbuild -ba --nodeps --define="_name ${NAME}" --define="_version ${VERSION}"  --define="_release ${RELEASE}" --target="$1" \
        --define="_compile_mode ${COMPILE_MODE}" "$OUTPUT_DIR"/rpmbuild/SOURCES/libhlibc.spec | tee -a "$OUTPUT_DIR"/rpmbuild/BUILD/compile_log.txt 2>&1
    elif [ "patch" == "$2" ];then
        $CUR_DIR/build_world.sh aarch64 patch "${COMPILE_MODE}"
    else
        echo "Unknow compile option!"
    fi
}
function prepare_simpo()
{
    source "${CUR_DIR}"/common_functions.sh
   # prepare_simpo_tools
  #  genereate_cros_code
}

function tailor_musl_123_update_code()
{
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/arm/crt_arch.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/arm/pthread_arch.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/arm/reloc.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/arm/syscall_arch.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/bits/alltypes.h.in
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/bits/fcntl.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/bits/stat.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/bits/syscall.h.in
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/or1k/bits/stat.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/include/sys/cdefs.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/aio/aio.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catanl.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_r.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/internal/syslog_impl.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/getpass.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/lutimes.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/ulimit.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/utmpx.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sqrtl.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/gethostid.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getpriority.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/nftw.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/openpty.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/pty.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/setrlimit.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/syslog.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_open.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/fexecve.c
    #rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/popen.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/cfsetospeed.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcdrain.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcflow.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcflush.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcgetattr.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcgetsid.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcsendbreak.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcsetattr.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/thread/arm/__set_thread_area.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/time/timer_impl.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/ctermid.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/faccessat.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/nice.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/readlinkat.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/tcgetpgrp.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/tcsetpgrp.c
    echo "tailor_musl_123_update_code"
}

function remove_hm_code()
{
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/generic/bits/ptrace.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/linux/ptrace.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/setjmp/aarch64/*_shadow_stack.S
    rm -rf "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/thread/{x86_64,x32,sh,s390x,riscv64,powerpc64,powerpc,or1k,mipsn32,mips64,mips,microblaze,m68k,i386,arm}
    rm -rf "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/signal/{x86_64,x32,sh,s390x,riscv64,powerpc64,powerpc,or1k,mipsn32,mips64,mips,microblaze,m68k,i386,arm}
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/thread/aarch64/__set_thread_area.s
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/string/rawmemchr.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/linux/cap.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/internal/hm_internal.h
    rm -rf "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/internal/{arm,aarch64}
    rm -rf "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/include/{time,nss}
}
function remove_malloc_code()
{
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/calloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/free.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/libc_calloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/lite_malloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/memalign.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/posix_memalign.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/realloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/reallocarray.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/replaced.c

    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/aligned_alloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/donate.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/free.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/glue.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/malloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/malloc_usable_size.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/meta.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/mallocng/realloc.c

    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/oldmalloc/aligned_alloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/oldmalloc/malloc.c
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/oldmalloc/malloc_impl.h
    rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/malloc/oldmalloc/malloc_usable_size.c
}

function tailor_locale()
{
echo "Start tailor src/locale."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/dirent/alphasort.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/__mo_lookup.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/big5.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/bind_textdomain_codeset.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/catclose.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/catgets.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/catopen.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/codepages.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/dcngettext.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/duplocale.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/freelocale.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/gb18030.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/hkscs.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/iconv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/iconv_close.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/jis0208.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/ksc.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/legacychars.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/locale_map.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/newlocale.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/pleval.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/pleval.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/revjis.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/setlocale.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/strcoll.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/strfmon.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/strxfrm.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/textdomain.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/uselocale.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/wcscoll.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/locale/wcsxfrm.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gai_strerror.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/herror.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/hstrerror.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/signal/psiginfo.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/signal/psignal.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/fgetws.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/fputwc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/fputws.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/fwprintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/fwscanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/getwc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/getwchar.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/open_wmemstream.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/putwc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/putwchar.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/swprintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/swscanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/vfwprintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/vfwscanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/vswprintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/vswscanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/vwprintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/vwscanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/wprintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/wscanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/string/strsignal.c
}
function tailor_passwd()
{
echo "Start tailor src/passwd."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/cuserid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/initgroups.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/fgetgrent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/fgetpwent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/fgetspent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getgr_a.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getgr_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getgrent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getgrent_a.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getgrouplist.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getpw_a.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getpw_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getpwent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getpwent_a.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getspent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getspnam.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/getspnam_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/lckpwdf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/nscd.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/nscd_query.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/putgrent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/putpwent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/putspent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/passwd/pwf.h
}
function tailor_crpyt()
{
echo "Start tailor src/crpyt."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_blowfish.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_des.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_des.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_md5.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_sha256.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/crypt_sha512.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/crypt/encrypt.c
}
function tailor_aio()
{
echo "Start tailor src/aio."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/aio/aio.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/aio/aio_suspend.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/aio/lio_listio.c
}
function tailor_select()
{
echo "Start tailor src/select."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getaddrinfo.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gethostbyaddr.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gethostbyaddr_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gethostbyname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gethostbyname2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gethostbyname2_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/gethostbyname_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getnameinfo.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getservbyport.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getservbyport_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/lookup_name.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_msend.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_query.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_querydomain.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_send.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/select/poll.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/select/pselect.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/select/select.c
}
function tailor_prng()
{
echo "Start tailor src/prng."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/__rand48_step.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/__seed48.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/drand48.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/lcong48.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/lrand48.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/mrand48.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/rand.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/rand48.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/rand_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/seed48.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/prng/srand48.c
}
function tailor_regex()
{
echo "Start tailor src/regex."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/fnmatch.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/glob.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/regcomp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/regerror.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/regexec.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/tre-mem.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/regex/tre.h
}
function tailor_mq()
{
echo "Start tailor src/mq."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_close.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_getattr.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_notify.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_open.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_receive.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_send.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_setattr.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_timedreceive.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_timedsend.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/mq/mq_unlink.c
}
function tailor_complex()
{
echo "Start tailor src/complex."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/__cexp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/__cexpf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cabsl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cacos.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cacosf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cacosh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cacoshf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cacoshl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cacosl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/carg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cargf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cargl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/casin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/casinf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/casinh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/casinhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/casinhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/casinl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catan.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catanh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catanhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catanhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/catanl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ccos.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ccosf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ccosh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ccoshf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ccoshl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ccosl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cexp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cexpf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cexpl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cimag.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cimagf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cimagl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/clog.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/clogf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/clogl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/conj.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/conjf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/conjl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cpow.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cpowf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cpowl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cproj.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cprojf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/cprojl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/creal.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/crealf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/creall.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csinf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csinh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csinhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csinhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csinl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/csqrtl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ctan.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ctanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ctanh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ctanhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ctanhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/complex/ctanl.c
}
function tailor_termios()
{
echo "Start tailor src/termios."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/getpass.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/forkpty.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/openpty.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/cfgetospeed.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/cfmakeraw.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/cfsetospeed.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcdrain.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcflow.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcflush.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcgetattr.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcgetsid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcsendbreak.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/termios/tcsetattr.c
}
function tailor_math()
{
echo "Start tailor src/math."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__cos.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__cosdf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__cosl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__expo2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__expo2f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__fpclassify.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__fpclassifyf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__fpclassifyl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__invtrigl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__invtrigl.h
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_divzero.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_divzerof.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_invalid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_invalidf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_oflow.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_oflowf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_uflow.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_uflowf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_xflow.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__math_xflowf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__polevll.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__rem_pio2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__rem_pio2_large.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__rem_pio2f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__rem_pio2l.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__signbit.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__signbitf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__signbitl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__sin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__sindf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__sinl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__tan.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__tandf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/__tanl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/ceil.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/ceilf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/floor.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/floorf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fmax.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fmaxf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fmin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/fminf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/llrint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/llrintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/llround.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/llroundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/lrint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/lrintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/lround.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/lroundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/nearbyint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/nearbyintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/rint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/rintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/round.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/roundf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/trunc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/aarch64/truncf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/acos.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/acosf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/acosh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/acoshf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/acoshl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/acosl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/arm/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/arm/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/arm/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/arm/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/arm/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/arm/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/asin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/asinf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/asinh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/asinhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/asinhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/asinl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atan.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atan2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atan2f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atan2l.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atanh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atanhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atanhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/atanl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cbrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cbrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cbrtl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ceil.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ceilf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ceill.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/copysign.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/copysignf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/copysignl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cos.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cosf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cosh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/coshf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/coshl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/cosl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/erf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/erff.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/erfl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp10.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp10f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp10l.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp2f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp2f_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp2f_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp2l.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/exp_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/expf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/expl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/expm1.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/expm1f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/expm1l.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fabsf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fabsl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fdim.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fdimf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fdiml.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/finite.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/finitef.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/floor.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/floorf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/floorl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmal.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmax.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmaxf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmaxl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fminf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fminl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmod.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmodf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/fmodl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/frexp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/frexpf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/frexpl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/hypot.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/hypotf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/hypotl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/__invtrigl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/acos.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/acosf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/acosl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/asin.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/asinf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/asinl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/atan.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/atan2.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/atan2f.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/atan2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/atanf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/atanl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/ceil.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/ceilf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/ceill.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/exp2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/exp_ld.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/expl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/expm1l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/fabs.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/fabsf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/fabsl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/floor.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/floorf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/floorl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/fmod.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/fmodf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/fmodl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/hypot.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/hypotf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/ldexp.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/ldexpf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/ldexpl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/llrint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/llrintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/llrintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log10.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log10f.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log10l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log1p.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log1pf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log1pl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log2.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log2f.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/log2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/logf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/logl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/lrint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/lrintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/lrintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/remainder.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/remainderf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/remainderl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/remquo.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/remquof.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/remquol.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/rint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/rintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/rintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/scalbln.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/scalblnf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/scalblnl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/scalbn.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/scalbnf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/scalbnl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/sqrt.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/sqrtf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/sqrtl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/trunc.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/truncf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/i386/truncl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ilogb.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ilogbf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ilogbl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/j0.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/j0f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/j1.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/j1f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/jn.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/jnf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ldexp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ldexpf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/ldexpl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lgamma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lgamma_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lgammaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lgammaf_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lgammal.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/llrint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/llrintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/llrintl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/llround.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/llroundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/llroundl.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log10.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log10f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log10l.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log1p.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log1pf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log1pl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2f.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2f_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2f_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log2l.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log_data.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/log_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logb.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logbf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logbl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logf_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logf_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/logl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lrint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lrintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lrintl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lround.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lroundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/lroundl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/mips/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/mips/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/mips/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/mips/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/modf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/modff.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/modfl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nan.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nanl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nearbyint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nearbyintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nearbyintl.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nextafter.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nextafterf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nextafterl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nexttoward.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nexttowardf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/nexttowardl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/pow.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/pow_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/pow_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/ceil.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/ceilf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/floor.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/floorf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fmax.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fmaxf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fmin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/fminf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/lrint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/lrintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/lround.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/lroundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/round.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/roundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/trunc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powerpc64/truncf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powf_data.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powf_data.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/powl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/remainder.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/remainderf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/remainderl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/remquo.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/remquof.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/remquol.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/rint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/rintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/rintl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/copysign.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/copysignf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fmax.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fmaxf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fmin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/fminf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/riscv64/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/round.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/roundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/roundl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/ceil.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/ceilf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/ceill.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/fabs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/fabsf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/fabsl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/floor.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/floorf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/floorl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/nearbyint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/nearbyintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/nearbyintl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/rint.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/rintf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/rintl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/round.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/roundf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/roundl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/sqrtl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/trunc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/truncf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/s390x/truncl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalb.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalbf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalbln.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalblnf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalblnl.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalbn.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalbnf.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/scalbnl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/signgam.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/significand.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/significandf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sincos.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sincosf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sincosl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sinf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sinh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sinhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sinhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sinl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sqrt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sqrtf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/sqrtl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tan.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tanf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tanh.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tanhf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tanhl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tanl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tgamma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tgammaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/tgammal.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/trunc.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/truncf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/truncl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/__invtrigl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/acosl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/asinl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/atan2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/atanl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/ceill.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/exp2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/expl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/expm1l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/fabs.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/fabsf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/fabsl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/floorl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/fmodl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/llrint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/llrintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/llrintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/log10l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/log1pl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/log2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/logl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/lrint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/lrintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/lrintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/remainderl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/rintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/sqrt.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/sqrtf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/sqrtl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x32/truncl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/__invtrigl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/acosl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/asinl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/atan2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/atanl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/ceill.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/exp2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/expl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/expm1l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/fabs.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/fabsf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/fabsl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/floorl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/fma.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/fmaf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/fmodl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/llrint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/llrintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/llrintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/log10l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/log1pl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/log2l.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/logl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/lrint.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/lrintf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/lrintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/remainderl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/rintl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/sqrt.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/sqrtf.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/sqrtl.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/math/x86_64/truncl.s
}



function tailor_fenv(){
	echo "Start tailor src/fenv."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/__flt_rounds.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/aarch64/fenv.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/arm/fenv-hf.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/arm/fenv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/fegetexceptflag.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/feholdexcept.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/fenv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/fesetexceptflag.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/fesetround.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/feupdateenv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/i386/fenv.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/m68k/fenv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/mips/fenv-sf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/mips/fenv.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/mips64/fenv-sf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/mips64/fenv.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/mipsn32/fenv-sf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/mipsn32/fenv.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/powerpc/fenv-sf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/powerpc/fenv.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/powerpc64/fenv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/riscv64/fenv-sf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/riscv64/fenv.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/s390x/fenv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/sh/fenv-nofpu.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/sh/fenv.S
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/x32/fenv.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/fenv/x86_64/fenv.s
}
function tailor_ipc()
{
    echo "Start tailor src/ipc."
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/ipc/ftok.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/ipc/msgctl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/ipc/msgget.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/ipc/msgrcv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/ipc/msgsnd.c
}
function tailor_network()
{
echo "Start tailor src/network."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/accept4.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/dn_comp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/dn_expand.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/dn_skipname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/dns_parse.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/ent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/ether.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/freeaddrinfo.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getpeername.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getservbyname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getservbyname_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getsockname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/getsockopt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/h_errno.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/if_freenameindex.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/if_indextoname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/if_nameindex.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/if_nametoindex.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/in6addr_any.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/in6addr_loopback.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/inet_legacy.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/lookup_ipliteral.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/lookup_serv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/netname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/ns_parse.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/proto.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/recvmmsg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/recvmsg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_init.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_mkquery.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/res_state.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/resolvconf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/sendmmsg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/sendmsg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/serv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/shutdown.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/sockatmark.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/network/socketpair.c
}
function tailor_process()
{
echo "Start tailor src/process."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/daemon.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/wordexp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/arm/vfork.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/execle.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/execlp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/execvp.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/fdop.h
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/fexecve.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/i386/vfork.s
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_addchdir.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_addclose.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_adddup2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_addfchdir.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_addopen.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_destroy.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawn_file_actions_init.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_destroy.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_getflags.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_getpgroup.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_getsigdefault.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_getsigmask.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_init.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_sched.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_setflags.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_setpgroup.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_setsigdefault.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnattr_setsigmask.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/posix_spawnp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/s390x/vfork.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/sh/vfork.s
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/system.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/wait.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/x32/vfork.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/process/x86_64/vfork.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/stdio/popen.c
}
function tailor_legacy()
{
echo "Start tailor src/legacy."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/err.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/euidaccess.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/ftw.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/futimes.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/getdtablesize.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/getloadavg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/getusershell.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/isastream.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/lutimes.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/ulimit.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/utmpx.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/legacy/valloc.c
}
function tailor_misc()
{
echo "Start tailor src/misc."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/a64l.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/ffs.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/ffsl.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/ffsll.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/fmtmsg.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/get_current_dir_name.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getauxval.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getdomainname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/gethostid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getpriority.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getresgid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getresuid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getrusage.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/getsubopt.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/issetugid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/lockf.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/login_tty.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/mntent.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/nftw.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/ptsname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/pty.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/setdomainname.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/setrlimit.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/syslog.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/misc/uname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/gethostname.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/nice.c
}
function tailor_unistd()
{
echo "Start tailor src/unistd."
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/acct.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/alarm.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/chdir.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/chown.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/ctermid.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/dup.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/dup2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/faccessat.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/fchdir.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/fchown.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/fchownat.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/fdatasync.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/fsync.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getcwd.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getegid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getgid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getgroups.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getlogin.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getlogin_r.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getpgid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getpgrp.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getppid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/getsid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/linkat.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/mips/pipe.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/mips64/pipe.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/mipsn32/lseek.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/mipsn32/pipe.s
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/pause.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/pipe2.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/posix_close.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/preadv.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/pwrite.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/pwritev.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/readlinkat.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/renameat.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setegid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/seteuid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setgid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setpgid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setpgrp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setregid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setresgid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setresuid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setreuid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setsid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setuid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/setxid.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/sh/pipe.s
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/symlink.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/symlinkat.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/sync.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/tcgetpgrp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/tcsetpgrp.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/truncate.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/ualarm.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/unlinkat.c
#rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/writev.c
rm -f "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/unistd/x32/lseek.c
}

function tailor_ylongc()
{
        echo "Start the ylongc slimming cutout."
    #	tailor_locale
    #	tailor_passwd
    #	tailor_network
    #	tailor_select
    #	tailor_prng
    #	tailor_regex
        tailor_unistd
        tailor_misc
        tailor_legacy
        tailor_ipc
        tailor_process
        tailor_fenv
        tailor_math
        tailor_crpyt
        tailor_aio
        tailor_mq
        tailor_complex
        tailor_termios
	echo "End the ylongc slimming cutout."

        cp -rv "$ROOT_DIR"/../ylong/{src,include,tools,ldso,crt,arch,Makefile,configure} "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/ > /dev/null
}

function build_code_prepare()
{
    mkdir -pv "$ROOT_DIR"/"$NAME"-"$VERSION"/{.git,.git/refs/heads,src/base/crt,src/base/arch} > /dev/null
    cp -rv "$ROOT_DIR"/{build,build.sh,config,README.md} "$ROOT_DIR"/"$NAME"-"$VERSION" > /dev/null
    cp -rv "$ROOT_DIR"/../{src,include,tools,dist,WHATSNEW,VERSION,README,INSTALL,dynamic.list,COPYRIGHT} "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/ > /dev/null
    cp -rv "$ROOT_DIR"/../crt/{aarch64,Scrt1.c,crtn.c,crti.c} "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/crt/ > /dev/null
    cp -rv "$ROOT_DIR"/../arch/{generic,aarch64} "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/arch/ > /dev/null
    mv "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/setjmp/aarch64/setjmp.S "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/setjmp/aarch64/setjmp.s
    mv "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/setjmp/aarch64/longjmp.S "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/src/setjmp/aarch64/longjmp.s

    remove_hm_code
    remove_malloc_code
    cp -rv "$ROOT_DIR"/../ylong/{src,include,tools,ldso,crt,arch,Makefile,configure} "$ROOT_DIR"/"$NAME"-"$VERSION"/src/base/ > /dev/null

    if [ "$MODULE" == "test" ];then
        cp -rv "$ROOT_DIR"/../ylongbuild/{test,CMakeLists.txt} "$ROOT_DIR"/"$NAME"-"$VERSION" > /dev/null
    fi

   tailor_ylongc

   tailor_musl_123_update_code
}

function env_ready()
{
    #清除上一次rpmbuild结果
    rm -rf "$OUTPUT_DIR"/*

    #准备源码
    build_code_prepare
    #此次进入ROOT_DIR目录 是为了给tar一个相对路径进行打包，这样不会产生"removing leading /' from member names"
    cd "$ROOT_DIR"
    tar -zcf "$NAME"-"$VERSION".tar.gz "$NAME"-"$VERSION" > /dev/null
    cd ~-

    rm -rf "$ROOT_DIR"/"$NAME"-"$VERSION"

    #更改rpmbuild根
    echo "%_topdir $OUTPUT_DIR/rpmbuild" > ~/.rpmmacros
    mkdir -pv "$OUTPUT_DIR"/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS} > /dev/null

    #debuginfo 临时注释，后续放开
    echo '%debug_package %{nil}' >> ~/.rpmmacros

    #源码归档到SOURCES,spec文件归档到SPECS
    mv "$ROOT_DIR"/"$NAME"-"$VERSION".tar.gz "$OUTPUT_DIR"/rpmbuild/SOURCES/

    if [ "$MODULE" == "test" ];then
        cp "$ROOT_DIR"/build/pkg/hpetest.spec "$OUTPUT_DIR"/rpmbuild/SOURCES/hpetest.spec > /dev/null
    fi

    case "$CPU_TYPE" in
        hi5651)
            cp "$ROOT_DIR"/build/pkg/libhlibc_ysx.spec "$OUTPUT_DIR"/rpmbuild/SOURCES/libhlibc.spec > /dev/null;;
        hi1213)
            cp "$ROOT_DIR"/build/pkg/libhlibc_ysl.spec "$OUTPUT_DIR"/rpmbuild/SOURCES/libhlibc.spec > /dev/null;;
        qemu)
            cp "$ROOT_DIR"/build/pkg/libhlibc_qemu.spec "$OUTPUT_DIR"/rpmbuild/SOURCES/libhlibc.spec > /dev/null;;
        *)
            echo "Unsupported cputype: [ hi5615 | qemu ]"
            exit -1;;
    esac
}

function main()
{
    init_env "$1" "$2" "$3"
    prepare_simpo
    env_ready

    case "$TARGET" in
        aarch64)
            rpm_build aarch64 "$MODULE" "$COMPILE_MODE";;
        x86_64)
            rpm_build x86_64 "$MODULE";;
        all)
            rpm_build x86_64 "$MODULE"
            rpm_build aarch64 "$MODULE";;
        help)
            echo "$0 [ x86_64 | aarch64 | all ]"
            exit -1;;
        *)
            echo "Unsupported architecture: [ x86_64 | aarch64 ]"
            exit -1;;
    esac
}

main "$1" "$2" "$3"
