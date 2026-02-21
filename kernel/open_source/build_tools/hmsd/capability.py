#!/usr/bin/env python3
# coding=utf-8
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: Provide capability class
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 23 15:22:42 2022

from enum import IntEnum


class Capability(IntEnum):
    """A Capability object stands for a kind of POSIX capability."""

    CHOWN = 0
    DAC_OVERRIDE = 1
    DAC_READ_SEARCH = 2
    FOWNER = 3
    FSETID = 4
    KILL = 5
    SETGID = 6
    SETUID = 7
    SETPCAP = 8
    LINUX_IMMUTABLE = 9
    NET_BIND_SERVICE = 10
    NET_BROADCAST = 11
    NET_ADMIN = 12
    NET_RAW = 13
    IPC_LOCK = 14
    IPC_OWNER = 15
    SYS_MODULE = 16
    SYS_RAWIO = 17
    SYS_CHROOT = 18
    SYS_PTRACE = 19
    SYS_PACCT = 20
    SYS_ADMIN = 21
    SYS_BOOT = 22
    SYS_NICE = 23
    SYS_RESOURCE = 24
    SYS_TIME = 25
    SYS_TTY_CONFIG = 26
    MKNOD = 27
    LEASE = 28
    AUDIT_WRITE = 29
    AUDIT_CONTROL = 30
    SETFCAP = 31
    MAC_OVERRIDE = 32
    MAC_ADMIN = 33
    SYSLOG = 34
    WAKE_ALARM = 35
    BLOCK_SUSPEND = 36
    AUDIT_READ = 37
    PERFMON = 38
    BPF = 39
    CHECKPOINT_RESTORE = 40
    DUMMY = 41 # for sec_chk not check capability

    @classmethod
    def capbit(cls, cap):
        """
        Convert a capability to a shifted bit.
        """
        return 1 << (cap if isinstance(cap, Capability) else Capability[cap])


    @classmethod
    def capbitset(cls, caps):
        """
        Convert capabilities to a bitset.
        """
        return sum(Capability.capbit(c) for c in caps)


    @classmethod
    def contain(cls, name):
        """
        Check whether a name is a valid capability name.
        """
        return name in Capability.__members__


    @classmethod
    def names(cls):
        """
        Get names of all capabilities.
        """
        return Capability.__members__.keys()


# vim: ts=4:sw=4:expandtab
