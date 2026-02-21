#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Model for sysif server
Author: Huawei OS Kernel Lab
Create: Mon 07 Dec 2020 08:30:42 PM CST
"""

from __future__ import annotations, absolute_import

from dataclasses import dataclass, field
from typing import FrozenSet, List, Optional, Set, Tuple, Dict, Union, Callable
from itertools import chain
from enum import Enum

from hmdsl.common.pyext import EOptional
from hmdsl.common.c_biast.biast import CTranslationUnit


# Should be refined in the future. For now we use simple string to represent a type in C.
CType = str


def general_call_macro_placeholder() -> str:
    """ The sysif methods defined without specified `ARCH` can use this as a key in a map. """
    return " "


def valid_arch_macros() -> Set[str]:
    """ Be caution to add macros here to control sysif methods.
    This may make a mess on exposing the sysif methods.
    """
    return set(["__aarch64__", "__arm__"])


@dataclass
class Element:
    """
    Represent #ifdef and #ifndef C directives
    """

    # The element is dumped only if the list of macros is defined
    ifdef_conditions: Set[str] = field(default_factory=set, init=False)

    # The element is dumped only if the list of macros is undefined
    ifndef_conditions: Set[str] = field(default_factory=set, init=False)

    def get_category(self) -> Tuple[FrozenSet[str], FrozenSet[str]]:
        """
        To Categoraize directives into #ifdefs and #ifndefs
        """
        return (frozenset(self.ifdef_conditions), frozenset(self.ifndef_conditions))


@dataclass
class CCode(Element):
    """
    A string block which is C code
    """

    content: str


@dataclass
class Server(Element):
    """ Description of A SYSIF Server. """

    # Name of the server. It should be specified in the SYSIF DSL through the
    # ``USE_SERVER_NAME(...)`` primitive.
    name: str = ''

    # C Header files required by the set of SYSIF apis
    headers: List[CCode] = field(default_factory=list)

    # Datatype declarations required by the set of SYSIF apis
    datatypes: List[CCode] = field(default_factory=list)

    # Prefix used in generating the sysif callers
    call_prefix: str = ''

    # Prefix used in generating the sysif handlers
    handler_prefix: str = ''

    # for __internal_rpccall
    group: Optional[str] = None

    # Whether we need to generate weak handlers.
    generate_weak_handler: bool = False

    managers: List[Manager] = field(default_factory=list)

    methods_by_arch: Optional[Dict[str, List[Tuple[Manager, List[Union[SyscallMethod, LsyscallMethod]]]]]] = None

    ro_after_init_postponed: bool = False

    def is_general(self) -> bool:
        """
        Determine whether the server has architecture related syscalls
        """
        for manager in self.managers:
            if manager.aarch64_lsyscalls or manager.arm_lsyscalls:
                return False

        return True

    def sort_methods_by_arch(self) -> Dict[str, List[Tuple[Manager, List[Union[SyscallMethod, LsyscallMethod]]]]]:
        """
        Categorize methods by arch macro
        """
        if self.methods_by_arch is not None:
            return self.methods_by_arch

        category: Dict[str, List[Tuple[Manager, List[Union[SyscallMethod, LsyscallMethod]]]]] = dict()
        all_general_methods: List[Tuple[Manager, List[Union[SyscallMethod, LsyscallMethod]]]] = []
        for manager in self.managers:
            general_methods = manager.syscalls + manager.lsyscalls.get(general_call_macro_placeholder(), [])
            for macro, calls in manager.lsyscalls.items():
                if macro != general_call_macro_placeholder() and macro not in category:
                    category[macro] = []
                    category[macro].append((manager, list(calls)))
                elif macro != general_call_macro_placeholder():
                    category[macro].append((manager, list(calls)))
            all_general_methods.append((manager, general_methods))

        category[general_call_macro_placeholder()] = all_general_methods
        macros_set = valid_arch_macros()
        for macro, mgr_calls in category.items():
            if macro != general_call_macro_placeholder():
                category[macro] = all_general_methods + mgr_calls
        for m_name in macros_set:
            if m_name not in category:
                category[m_name] = all_general_methods

        self.methods_by_arch = category
        return category

    def print_methods_in_arch_macros(self, call: Callable[..., List[Union[str, CTranslationUnit]]]) -> \
            List[Union[str, CTranslationUnit]]:
        """
        print lsyscalls and inside arch macros
        """
        ret: List[Union[str, CTranslationUnit]] = []
        category = self.sort_methods_by_arch()
        if (valid_arch_macros().intersection(set(category.keys()))) == 0:
            ret += call(category[general_call_macro_placeholder()])
        else:
            for macro, methods in sorted(category.items()):
                if macro != general_call_macro_placeholder():
                    if len(ret) == 0:
                        ret.append(f"#ifdef {macro}\n")
                    else:
                        ret.append(f"#elif defined({macro})\n")
                    ret += call(methods)
            ret.append("#else\n")
            ret += call(category[general_call_macro_placeholder()])
            ret.append(f"#endif\n")
        return ret

    def update(self, svr: Server):
        """ Update the current server by integrating all contents from another
        server ``svr``.
        """

        for header in self.headers:
            header.ifdef_conditions |= self.ifdef_conditions
            header.ifndef_conditions |= self.ifndef_conditions
        for datatype in self.datatypes:
            datatype.ifdef_conditions |= self.ifdef_conditions
            datatype.ifndef_conditions |= self.ifndef_conditions
        for manager in self.managers:
            manager.ifdef_conditions |= self.ifdef_conditions
            manager.ifndef_conditions |= self.ifndef_conditions
        self.ifdef_conditions.clear()
        self.ifndef_conditions.clear()

        for header in svr.headers:
            header.ifdef_conditions |= svr.ifdef_conditions
            header.ifndef_conditions |= svr.ifndef_conditions
        self.headers += svr.headers
        for datatype in svr.datatypes:
            datatype.ifdef_conditions |= svr.ifdef_conditions
            datatype.ifndef_conditions |= svr.ifndef_conditions
        self.datatypes += svr.datatypes
        for mgr in svr.managers:
            mgr.ifdef_conditions |= svr.ifdef_conditions
            mgr.ifndef_conditions |= svr.ifndef_conditions
            mgr.server = self
        self.managers += svr.managers
        svr.ifdef_conditions.clear()
        svr.ifndef_conditions.clear()

        self.name = self.name if self.name != '' else svr.name
        self.call_prefix = self.call_prefix if self.call_prefix != '' else svr.call_prefix
        if self.handler_prefix == '':
            self.handler_prefix = svr.handler_prefix
        self.group = EOptional.merge(self.group, svr.group)
        self.generate_weak_handler = self.generate_weak_handler or svr.generate_weak_handler
        self.ro_after_init_postponed = self.ro_after_init_postponed or svr.ro_after_init_postponed


@dataclass
class Manager(Element):
    """
    A sysif server is constructed by multiple managers
    """

    # Name of the manager.
    name: str

    server: Optional[Server]

    # Capacity of a manager indicates how many methods can be mounted to it.
    # For example, if the capacity of a manager is 5, declaring 6 methods inside
    # this manager will lead to an error.
    capacity: int

    syscalls: List[SyscallMethod] = field(default_factory=list)

    lsyscalls: Dict[str, List[LsyscallMethod]] = field(default_factory=dict)

    @property
    def methods(self) -> List[Union[SyscallMethod, LsyscallMethod]]:
        """
        To retrive all defined methods
        """
        return self.syscalls + list(chain(*self.lsyscalls.values()))

    @property
    def general_lsyscalls(self) -> List[LsyscallMethod]:
        """
        To retrive non architecture related lsyscalls
        """
        return list(filter(lambda x: x.arch_macro is None, list(chain(*self.lsyscalls.values()))))

    @property
    def aarch64_lsyscalls(self) -> List[LsyscallMethod]:
        """
        To retrive all aarch64 lsyscalls
        """
        return list(filter(lambda x: x.arch_macro == "__aarch64__", list(chain(*self.lsyscalls.values()))))

    @property
    def arm_lsyscalls(self) -> List[LsyscallMethod]:
        """
        To retrive all arm lsyscalls
        """
        return list(filter(lambda x: x.arch_macro == "__arm__", list(chain(*self.lsyscalls.values()))))

    def lsyscall_methods_list(self, with_mgr_name: bool, arch: Optional[str] = None) -> List[str]:
        """
        To get a full name list of defined lsyscall methods
        """
        lsyscalls = list(chain(*self.lsyscalls.values()))
        general_lsyscalls = list(filter(lambda x: x.arch_macro is None, lsyscalls))
        aarch64_lsyscalls = list(filter(lambda x: x.arch_macro == "__aarch64__", lsyscalls))
        arm_lsyscalls = list(filter(lambda x: x.arch_macro == "__arm__", lsyscalls))
        methods = []

        if arch is None:
            if with_mgr_name:
                methods = [f"{call.manager_name}_{call.name}"
                           for call in general_lsyscalls + aarch64_lsyscalls
                           ]
            else:
                methods = [call.name for call in general_lsyscalls + aarch64_lsyscalls]

            for call in arm_lsyscalls:
                if not with_mgr_name and call.name in methods:
                    continue

                if with_mgr_name and '_'.join([call.manager_name, call.name]) in methods:
                    continue

                if with_mgr_name:
                    methods.append(f"{call.manager_name}_{call.name}")
                else:
                    methods.append(call.name)
        elif arch == "__aarch64__":
            if with_mgr_name:
                methods = [f"{call.manager_name}_{call.name}"
                           for call in general_lsyscalls + aarch64_lsyscalls
                           ]
            else:
                methods = [call.name for call in general_lsyscalls + aarch64_lsyscalls]
        elif arch == "__arm__":
            if with_mgr_name:
                methods = [f"{call.manager_name}_{call.name}"
                           for call in general_lsyscalls + arm_lsyscalls
                           ]
            else:
                methods = [call.name for call in general_lsyscalls + arm_lsyscalls]

        return methods

    def actvcall_methods_list(self, with_mgr_name: bool, with_rsvd: bool) -> List[str]:
        """
        To get a full name list of defined actvcall methods
        """
        methods = []
        if with_mgr_name:
            methods = [f"{call.manager_name}_{call.name}" for call in self.syscalls]
        else:
            methods = [call.name for call in self.syscalls]

        if with_rsvd:
            methods += self.rsvd_methods_list(with_mgr_name)
        return methods

    def rsvd_methods_list(self, with_mgr_name: bool) -> List[str]:
        """
        To retrive reserved methods list
        """
        lsyscalls = list(chain(*self.lsyscalls.values()))
        general_lsyscalls = list(filter(lambda x: x.arch_macro is None, lsyscalls))
        aarch64_lsyscalls = list(filter(lambda x: x.arch_macro == "__aarch64__", lsyscalls))
        arm_lsyscalls = list(filter(lambda x: x.arch_macro == "__arm__", lsyscalls))
        max_len = len(self.syscalls) + len(general_lsyscalls) + min(len(aarch64_lsyscalls), len(arm_lsyscalls))
        rsvd_len = self.capacity - max_len
        methods = []
        if with_mgr_name:
            methods += list(f"{self.name}_rsvd_{num}" for num in range(rsvd_len, 0, -1))
        else:
            methods += list(f"rsvd_{num}" for num in range(rsvd_len, 0, -1))
        return methods

    def print_lsyscalls_in_arch_macros(self, call: Callable[..., Union[List[str], List[CTranslationUnit]]]) -> \
            List[Union[str, CTranslationUnit]]:
        """
        print lsyscalls inside arch macros
        """
        ret: List[Union[str, CTranslationUnit]] = []
        for macro, lsyscalls in self.lsyscalls.items():
            if macro == general_call_macro_placeholder():
                ret += call(lsyscalls)
            else:
                ret.append(f"#ifdef {macro}\n")
                ret += call(lsyscalls)
                ret.append(f"#endif\n")
        return ret


@dataclass
class Parameter(Element):
    """
    Parameter object to present parameters of sysif methods
    """

    name: str
    parameter_type: CType

    def __str__(self) -> str:
        """ format print of a parameter """
        return f"""{self.parameter_type} {self.name};"""


class MethodType(Enum):
    """
    Enum of different types of sysif apis
    """
    NOBUF = 0
    RETBUF = 1
    BUF = 2
    FALLBACK = 3


@dataclass
class Method(Element):
    """
    A manager is constructed with multiple sysif methods
    """

    # The name of the method.
    name: str

    # The manager of the method.
    manager: Optional[Manager]

    # List of the parameters of the method.
    parameters: List[Parameter]

    direct_return_type: CType

    # Whether the method supports self-call generation. If the option is
    # set to True, the code generator will use direct-call instead of rpc-call
    # on this function, when its caller is in the same module.
    support_self_call: bool

    @property
    def manager_name(self) -> str:
        """
        A manager name is used to construct a name for apis
        """
        return EOptional.get(self.manager).name


@dataclass
class SyscallMethod(Method):
    """
    Sysif methods can be normal syscall
    """

    # There are some inconsistencies with the name of the manager in the kernel.
    # This is just to compat with the inconsistencies.
    # To be removed in the future...
    fake_mgr_name: Optional[str]

    # If it is not None, the code generator package all these parameters in
    # a struct, and return it through the last method parameter. For example, ::
    #     int XXX(..., struct YYY * ret);
    # where ``YYY``'s definition includes all parameters in ``indirect_returned_values``.
    indirect_returned_values: Optional[List[Parameter]]

    slow_data: bool

    stk_check: bool = True

    @property
    def manager_name(self) -> str:
        """
        To forward compat with existing sysif DSL files which defines manager name wrongly
        """
        return self.fake_mgr_name or EOptional.get(self.manager).name

    @property
    def server_name(self) -> str:
        """
        Get the server name whom the method belongs to
        """
        return EOptional.get(EOptional.get(EOptional.get(self.manager).server).name)

    @property
    def method_type(self) -> MethodType:
        """
        Get the type of the sysif method
        """
        if self.indirect_returned_values is None and not self.slow_data:
            return MethodType.NOBUF

        if self.indirect_returned_values is None and self.slow_data:
            return MethodType.BUF

        if self.indirect_returned_values is not None and not self.slow_data:
            return MethodType.RETBUF

        return MethodType.FALLBACK


class LsyscallType(Enum):
    """
    Enum of different types of lsyscall
    """
    NONATIVE = 0
    NATIVENOSELFCALL = 1
    NATIVEWITHSELFCALL = 2


@dataclass
class LsyscallMethod(Method):
    """
    Sysif methods can be exgate syscall
    """

    syscall_num: str
    # whether the lsyscall has client apis
    is_native: bool
    arch_macro: Optional[str] = None

    stk_check: bool = True
    map_none: bool = False

    @property
    def manager_name(self) -> str:
        """
        A manager name is used to construct a name for apis
        """
        return EOptional.get(self.manager).name

    @property
    def indirect_returned_values(self) -> Optional[List[Parameter]]:
        """ A Lsyscall has no RET """
        return None

    @property
    def slow_data(self) -> bool:
        """ A Lsyscall has no slow data """
        return False

    @property
    def method_type(self) -> MethodType:
        """
        Lsyscall has no indirect_returned_values and no slowdata, thus a lsyscall is always NOBUF
        """
        return MethodType.NOBUF

    @property
    def server_name(self) -> str:
        """
        Get the server name whom the method belongs to
        """
        return EOptional.get(EOptional.get(EOptional.get(self.manager).server).name)

    @property
    def lsyscall_type(self) -> LsyscallType:
        """
        Get the type of the sysif method
        """
        if not self.is_native:
            return LsyscallType.NONATIVE

        if self.is_native and not self.support_self_call:
            return LsyscallType.NATIVENOSELFCALL

        return LsyscallType.NATIVEWITHSELFCALL
