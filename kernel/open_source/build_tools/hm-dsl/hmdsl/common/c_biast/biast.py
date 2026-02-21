#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
Description: Bidirectional AST Elements Of C Language
Author: Huawei OS Kernel Lab
Create: Mon 28 Dec 2020 16:30:00 PM CST
"""

from __future__ import annotations, absolute_import

from typing import Dict, Iterable, List, Optional, Set, Tuple, Type as SubType, Union, Any
from dataclasses import dataclass, field
from collections import Counter
from copy import deepcopy
from enum import Enum

from antlr4 import InputStream, CommonTokenStream
from hmdsl.common.error import DSLErrorListener
from hmdsl.common.c_biast.BiastGrammarLexer import BiastGrammarLexer
from hmdsl.common.c_biast.BiastGrammarParser import BiastGrammarParser


class MacroExpandingError(Exception):
    """ class of macro expanding errors  """


class CBiAst:
    """ Base class of bi-directional C-Ast elements.  """

    @classmethod
    def of(cls, content: str):
        """ A function to parse a block of C codes to C BiAst """
        raise NotImplementedError(f"unimplemented method 'of' in {cls}")

    @classmethod
    def constant_of(cls, content: str):
        """ A function to parse a block of C codes to a C BiAst constant """
        raise NotImplementedError(f"unimplemented method 'constant_of' in {cls}")

    def rsvd_ast(self) -> CBiAst:
        """ reserve a public interface """
        return self


class CMacro:
    """ A BiAst element represents C macro """
    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ Base function to get base class """
        raise NotImplementedError

    def rsvd_macro(self) -> CMacro:
        """ reserve a public interface """
        return self


class CExpression(CBiAst):
    """ A BiAst element represents C Expression """
    @property
    def precedence(self) -> int:
        """ If not explicitly specified, the precedence of an expression is
        0, i.e. the highest, by default.
        """
        return 1

    @classmethod
    def of(cls, content: str):
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="expression")

    @classmethod
    def constant_of(cls, content: str):
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="expression", is_constant=True)

    @classmethod
    def null(cls) -> CExpression:
        """ To create a `NULL` expression """
        return CIdentifierExpression('NULL')

    def set_assoc(self, is_assoc: bool) -> CExpression:
        """ This attribute will be set
        when a sub expression with higher associative relation encountered.
        """
        setattr(self, "_assoc", is_assoc)
        return self

    def get_assoc(self) -> bool:
        """ To get the associative relation attribute """
        if hasattr(self, "_assoc"):
            return getattr(self, "_assoc")

        return False


@dataclass
class CTranslationUnit(CBiAst):
    """ A single pre-processed C file is called a translation
    unit.
    """

    content: List[Union[CDeclaration, CFunction]]

    @classmethod
    def of(cls, content: str) -> CTranslationUnit:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="translation_unit")

    @classmethod
    def constant_of(cls, content: str) -> CTranslationUnit:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="translation_unit", is_constant=True)


class CAttribute(CBiAst):
    """ A BiAst element represents C attribute """
    @classmethod
    def of(cls, content: str) -> CAttribute:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="attribute_specifier")

    @classmethod
    def constant_of(cls, content: str) -> CAttribute:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="attribute_specifier", is_constant=True)

    # A set of short-cut attribute constructors

    @classmethod
    def alias(cls, alias_name: str) -> CAttribute:
        """ An alias attribute tells the compiler that the function is
        an alias of an existing one, hence we have no need to implement this
        function, e.g.::

            void f(...) {
                ...
            }

            void __attribute__((alias("f"))) g;

        """

        return CClassicalAttribute("alias", [CStringLiteral(alias_name)])

    @classmethod
    def always_inline(cls) -> CAttribute:
        """ Create an `always_inline` attribute """
        return CClassicalAttribute("always_inline")


@dataclass
class CClassicalAttribute(CAttribute):
    """ A BiAst element represents C classical attribute """
    name: str
    arguments: Optional[List[CExpression]] = None


@dataclass
class CMacroAttribute(CAttribute, CMacro):
    """ A BiAst element represents C macro attribute """
    macro_name: str
    arguments: List[CBiAst] = field(default_factory=list)

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CAttribute


@dataclass
class CFunction(CBiAst):
    """ A BiAst element represents C function """
    name: str
    parameters: List[CParameterDeclaration]
    return_type: CType
    body: Optional[CCompoundStatement]
    inline: bool = False
    variable_length_argument: bool = False
    storage_qualifiers: Set[CStorageQualifier] = field(default_factory=set)
    attributes: List[CAttribute] = field(default_factory=list)

    @classmethod
    def of(cls, content: str) -> CFunction:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="function_definition")

    @classmethod
    def constant_of(cls, content: str) -> CFunction:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="function_definition", is_constant=True)


@dataclass
class CDeclaration(CBiAst):
    """ A BiAst element represents C declaration """
    name: Optional[str]
    type: CType
    attributes: List[CAttribute] = field(default_factory=list, init=False)

    @classmethod
    def of(cls, content: str):
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="declaration")

    @classmethod
    def constant_of(cls, content: str):
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="declaration", is_constant=True)

    def get_ident(self) -> CIdentifierExpression:
        """ To get identifier of a declaration """
        if self.name is None:
            raise NameError(f"cannot get identifier of an empty declaration {self}")

        return CIdentifierExpression(self.name)


@dataclass
class CMacroDeclaration(CDeclaration, CMacro):
    """ A BiAst element represents C macro declaration """
    macro_name: str
    arguments: List[CBiAst]

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CDeclaration


@dataclass
class CTypeDeclaration(CDeclaration):
    """ A BiAst element represents C type declaration """


@dataclass
class CVariableDeclaration(CDeclaration):
    """ A BiAst element represents C variable declaration """
    initializer: Optional[CExpression] = None
    storage_qualifiers: Set[CStorageQualifier] = field(default_factory=set)


@dataclass
class CParameterDeclaration(CDeclaration):
    """ A BiAst element represents C parameter declaration """
    @classmethod
    def of(cls, content: str) -> CParameterDeclaration:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="parameter_declaration")

    @classmethod
    def constant_of(cls, content: str) -> CParameterDeclaration:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="parameter_declaration", is_constant=True)


@dataclass
class CMacroParameterDeclaration(CParameterDeclaration, CMacro):
    """ A BiAst element represents C macro parameter declaration """
    macro_name: str
    arguments: List[CBiAst]

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CParameterDeclaration


@dataclass
class CFieldDeclaration(CDeclaration):
    """ A BiAst element represents C field declaration """
    bitwidth: Optional[int] = None


@dataclass
class CMacroFieldDeclaration(CDeclaration, CMacro):
    """ A BiAst element represents C macro field declaration """
    macro_name: str
    arguments: List[CBiAst]
    bitwidth: Optional[int] = None

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CFieldDeclaration


class CStorageQualifier(Enum):
    """ Enum of C storage qualifiers """
    AUTO = "auto"
    REGISTER = "register"
    STATIC = "static"
    EXTERN = "extern"
    TYPEDEF = "typedef"


class CTypeQualifier(Enum):
    """ Enum of C type qualifiers """
    CONST = "const"
    VOLATILE = "volatile"


class CFunctionSpecifier(Enum):
    """ Enum of C function specifier """
    INLINE = "inline"


@dataclass
class CType(CBiAst):
    """ A BiAst element represents C type """
    qualifiers: Set[CTypeQualifier] = field(default_factory=set, init=False)

    @classmethod
    def of(cls, content: str) -> CType:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="type_name")

    @classmethod
    def constant_of(cls, content: str) -> CType:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="type_name", is_constant=True)

    def add_qualifier(self, qualifier: CTypeQualifier) -> CType:
        """ To add a qualifier """
        self.qualifiers.add(qualifier)
        return self

    def add_qualifiers(self, qualifiers: Iterable[CTypeQualifier]) -> CType:
        """ To add qualifiers """
        self.qualifiers.update(qualifiers)
        return self


@dataclass
class CTypeOf(CType):
    """ The class :class:`CTypeOf` corresponds to the GNU *typeof* extension. """
    operand: Union[CType, CExpression]


@dataclass
class CMacroType(CType, CMacro):
    """ A BiAst element represents C macro type """
    macro_name: str
    arguments: List[CBiAst]

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CType


@dataclass
class CAliasType(CType):
    """ A BiAst element represents C alias type """
    name: str


@dataclass
class CVoidType(CType):
    """ A BiAst element represents C void type """


@dataclass
class CBoolType(CType):
    """ A BiAst element represents C bool type """


class CSignDescriptor(Enum):
    """ Enum of C sign descriptors """
    SIGNED = 'signed'
    UNSIGNED = 'unsigned'


@dataclass
class CIntegerType(CType):
    """ A BiAst element represents C integer type """
    sign_desc: Optional[CSignDescriptor] = CSignDescriptor.SIGNED


@dataclass
class CCharType(CIntegerType):
    """ A BiAst element represents C char type """
    sign_desc: Optional[CSignDescriptor] = None


@dataclass
class CShortType(CIntegerType):
    """ A BiAst element represents C short type """


@dataclass
class CIntType(CIntegerType):
    """ A BiAst element represents C int type """


@dataclass
class CLongType(CIntegerType):
    """ A BiAst element represents C long type """


@dataclass
class CLonglongType(CIntegerType):
    """ A BiAst element represents C long long type """


@dataclass
class CFloatType(CType):
    """ A BiAst element represents C float type """


@dataclass
class CDoubleType(CType):
    """ A BiAst element represents C double type """


@dataclass
class CLongDoubleType(CType):
    """ A BiAst element represents C long double type """


@dataclass
class CPointerType(CType):
    """ A BiAst element represents C pointer type """
    base_type: CType

    @classmethod
    def create(cls, base_type: CType, level: int) -> CType:
        """ To create a pointer type according to its nested level """
        if level < 0:
            raise ValueError(f"cannot create negative-level pointer types")
        for _ in range(level):
            base_type = CPointerType(base_type)

        return base_type


@dataclass
class CArrayType(CType):
    """ A BiAst element represents C array type """
    base_type: CType
    capacity: Optional[CExpression]


@dataclass
class CStructType(CType):
    """ A BiAst element represents C struct type """
    name: Optional[str] = None
    field_decls: Optional[List[CFieldDeclaration]] = None
    attributes: List[CAttribute] = field(default_factory=list, init=False)


@dataclass
class CUnionType(CType):
    """ A BiAst element represents C union type """
    name: Optional[str] = None
    field_decls: Optional[List[CFieldDeclaration]] = None
    attributes: List[CAttribute] = field(default_factory=list, init=False)


@dataclass
class CEnumType(CType):
    """ A BiAst element represents C enum type """
    name: Optional[str] = None
    enum_items: Optional[List[CEnumItem]] = None
    attributes: List[CAttribute] = field(default_factory=list, init=False)


@dataclass
class CFunctionType(CType):
    """ A BiAst element represents C function type """
    parameter_decls: List[CParameterDeclaration]
    return_type: CType
    variable_length_argument: bool


@dataclass
class CMacroExpression(CExpression, CMacro):
    """ A BiAst element represents C macro expression """
    macro_name: str
    arguments: List[CBiAst] = field(default_factory=list)

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CExpression


class CUnaryOperator(Enum):
    """ Enum of C unary operators """
    POSITIVE = "+"
    NEGATE = '-'
    SELFINC = '++'
    SELFDEC = '--'
    BNOT = '~'
    LNOT = '!'
    SIZEOF = "sizeof"
    ADDRESS = '&'
    DEREFERENCE = '*'

    @property
    def precedence(self) -> int:
        """ Precedence of unary operators """
        return 2


class CPostUnaryOperator(Enum):
    """ Enum of C post unary operators """
    SELFINC = '++'
    SELFDEC = '--'

    @property
    def precedence(self) -> int:
        """ Precedence of post unary operators """
        return 1


class CBinaryOperator(Enum):
    """ Enum of C binary operators """
    ADD = '+'
    SUB = '-'
    MUL = '*'
    DIV = '/'
    MOD = '%'
    LSHIFT = '<<'
    RSHIFT = '>>'
    LE = '<='
    LT = '<'
    GE = '>='
    GT = '>'
    EQ = '=='
    NE = '!='
    BAND = '&'
    BOR = '|'
    XOR = '^'
    LAND = '&&'
    LOR = '||'

    @property
    def precedence(self) -> int:
        """ Precedence of binary operators """
        mapping: Dict[CBinaryOperator, int] = {
            CBinaryOperator.LOR: 12,
            CBinaryOperator.LAND: 11,
            CBinaryOperator.BOR: 10,
            CBinaryOperator.XOR: 9,
            CBinaryOperator.BAND: 8,
            CBinaryOperator.EQ: 7, CBinaryOperator.NE: 7,
            CBinaryOperator.LE: 6, CBinaryOperator.GE: 6,
            CBinaryOperator.LT: 6, CBinaryOperator.GT: 6,
            CBinaryOperator.LSHIFT: 5, CBinaryOperator.RSHIFT: 5,
            CBinaryOperator.ADD: 4, CBinaryOperator.SUB: 4,
            CBinaryOperator.MUL: 3, CBinaryOperator.DIV: 3, CBinaryOperator.MOD: 3,
        }

        precedence_rs = mapping.get(self)
        if precedence_rs is None:
            raise ValueError(f"{self} is an invalid C binary operator")
        return precedence_rs


@dataclass
class CUnaryExpression(CExpression):
    """ A BiAst element represents C unary expression """
    operator: Union[CUnaryOperator, CPostUnaryOperator]
    operand: CExpression

    @property
    def precedence(self) -> int:
        """ Precedence of unary operator in an unary expression """
        return self.operator.precedence


@dataclass
class CBinaryExpression(CExpression):
    """ A BiAst element represents C binary expression """
    left: CExpression
    operator: CBinaryOperator
    right: CExpression

    @property
    def precedence(self) -> int:
        """ Precedence of binary operator in a binary expression """
        return self.operator.precedence


@dataclass
class CIdentifierExpression(CExpression):
    """ A BiAst element represents C identifier expression """
    name: str


@dataclass
class CStringLiteral(CExpression):
    """ A BiAst element represents C string literal """
    content: str


class CAssignmentOperator(Enum):
    """ Enum of C assignment operators """
    ASSIGN = '='
    ADDASSIGN = '+='
    SUBASSIGN = '-='
    MUTASSIGN = '*='
    DIVASSIGN = '/='
    MODASSIGN = '%='
    RSHIFTASSIGN = '>>='
    LSHIFTASSIGN = '<<='
    BANDASSIGN = '&='
    BORASSIGN = '|='
    XORASSIGN = '^='


@dataclass
class CAssignmentExpression(CExpression):
    """ A BiAst element represents C assignment expression """
    lhs: CExpression
    operator: CAssignmentOperator
    rhs: CExpression

    @property
    def precedence(self) -> int:
        """ Precedence of assignment expression """
        return 14


@dataclass
class CSequenceExpression(CExpression):
    """ A BiAst element represents C sequence expression """
    sequence: List[CExpression]

    @property
    def precedence(self) -> int:
        """ Precedence of sequence expression """
        return 15


@dataclass
class CConditionalExpression(CExpression):
    """ A BiAst element represents C conditional expression """
    condition: CExpression
    consequent: CExpression
    alternative: CExpression

    @property
    def precedence(self) -> int:
        """ Precedence of conditional expression """
        return 13


@dataclass
class CCastExpression(CExpression):
    """ A BiAst element represents C cast expression """
    cast_type: CType
    cast_content: CExpression

    @property
    def precedence(self) -> int:
        """ Precedence of cast expression """
        return 2


@dataclass
class CElementExpression(CExpression):
    """ A BiAst element represents C get array element expression """
    array_object: CExpression
    index: CExpression


@dataclass
class CMemberExpression(CExpression):
    """ A BiAst element represents C get memeber expression """
    object: CExpression
    member: str


@dataclass
class CCallExpression(CExpression):
    """ A BiAst element represents C function call expression """
    callee: CExpression
    arguments: List[CExpression]


@dataclass
class CCompoundExpression(CExpression):
    """ A BiAst element represents C compound expression """
    compound: CCompoundStatement


@dataclass
class CDesignator(CBiAst):
    """ A BiAst element represents C designator """

    @classmethod
    def of(cls, content: str) -> CBiAst:
        """ A function to parse a block of C codes to C BiAst """
        return super().of(content)


@dataclass
class CIndexDesignator(CDesignator):
    """ A BiAst element represents C index designator """
    index: CExpression

    @classmethod
    def of(cls, content: str) -> CBiAst:
        """ A function to parse a block of C codes to C BiAst """
        return super().of(content)


@dataclass
class CRangeDesignator(CDesignator):
    """ Specifically used to describe the range
    initializer, e.g. ``int a [] = { [0 ... 1] = 2, [2] = 3 };``.
    """

    left: CExpression
    right: CExpression

    @classmethod
    def of(cls, content: str) -> CBiAst:
        """ A function to parse a block of C codes to C BiAst """
        return super().of(content)


@dataclass
class CMemberDesignator(CDesignator):
    """ A BiAst element represents C structure member designator """
    field_name: str

    @classmethod
    def of(cls, content: str) -> CBiAst:
        """ A function to parse a block of C codes to C BiAst """
        return super().of(content)


@dataclass
class CDesignation(CBiAst):
    """ Designation, including a list of designators (potentially empty),
    is a special way to initialize array elements or struct members. For
    example, ::

        int a[10] = { [1 .. 2] = 1, [5] = 2 };
                      ^^^^^^^^      ^^^
            a range designator      an index designator

    designator is also composable, e.g.::

        struct {
            int arr[10];
        } t = { .arr[0] = 1; };
                ^^^^^^^^
                [ member designator, index designator ]

    """
    designators: List[CDesignator] = field(default_factory=list)

    @classmethod
    def of(cls, content: str) -> CDesignation:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry='designation')

    @classmethod
    def constant_of(cls, content: str) -> CDesignation:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry='designation', is_constant=True)


@dataclass
class CConstructorExpression(CExpression):
    """ initializer is None means the declarator was not initialized,
    initializer is empty means the declarator was initialized with defaults
    """
    items: List[Tuple[CDesignation, CExpression]] = field(default_factory=list)

    @classmethod
    def create(cls, *elems: Union[CExpression, Tuple[CDesignation, CExpression]]) -> \
            CConstructorExpression:
        """ To create a constructor expression with given elements """
        ret = CConstructorExpression()
        for elem in elems:
            if isinstance(elem, CExpression):
                ret.items.append((CDesignation(), elem))
            else:
                ret.items.append(elem)

        return ret

    @classmethod
    def of(cls, content: str) -> CExpression:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="initializer")

    @classmethod
    def constant_of(cls, content: str) -> CExpression:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="initializer", is_constant=True)


@dataclass
class CEnumItem(CBiAst):
    """ A BiAst element represents C enum item """
    name: Optional[str]
    value: Optional[CExpression]

    @classmethod
    def of(cls, content: str) -> CEnumItem:
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="enumerator")

    @classmethod
    def constant_of(cls, content: str) -> CEnumItem:
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="enumerator", is_constant=True)


class CConstant(CExpression):
    """ A BiAst element represents C constant """

    def __bool__(self):
        raise NotImplementedError(f"__bool__ method not implemented for {type(self).__name__}")


class CIntegerBase(Enum):
    """ A BiAst element represents C integer constants base """
    BIN = 2
    OCT = 8
    DEC = 10
    HEX = 16


@dataclass
class CIntegerConstant(CConstant):
    """ A BiAst element represents C integer constant """

    class Suffix(Enum):
        """ suffix of integer constants """
        LONG = 'L'
        LONGLONG = 'LL'
        UNSIGNED = 'U'

    value: int
    base: CIntegerBase = field(default=CIntegerBase.DEC, compare=False)
    suffixes: Set[CIntegerConstant.Suffix] = field(default_factory=set)

    def __bool__(self):
        return self.value != 0


@dataclass
class CFloatConstant(CConstant):
    """ A BiAst element represents C float constant """

    class Suffix(Enum):
        """ Enum of float constant suffix """
        FLOAT = 'F'
        LONGDOUBLE = 'L'

    value: str
    # hexadecimal floating constant is not supported currently
    suffixes: Set[CFloatConstant.Suffix] = field(default_factory=set)

    def __bool__(self):
        return float(self.value) != 0


@dataclass
class CCharConstant(CConstant):
    """ A BiAst element represents C char constant """
    value: str

    def __bool__(self):
        return self.value != r"\0"


class CStatement(CBiAst):
    """ A BiAst element represents C statement """
    @classmethod
    def of(cls, content: str):
        """ A function to parse a block of C codes to C BiAst """
        return parse(content, entry="statement")

    @classmethod
    def constant_of(cls, content: str):
        """ A function to parse a block of C codes to a C BiAst constant """
        return parse(content, entry="statement", is_constant=True)

    def rsvd_ast(self) -> CStatement:
        """ reserve a public interface """
        return self


CStatementOrDeclaration = Union[CStatement, CVariableDeclaration, CTypeDeclaration]


@dataclass
class CCompoundStatement(CStatement):
    """ A BiAst element represents C compound statement """
    body: List[CStatementOrDeclaration]


@dataclass
class CExpressionStatement(CStatement):
    """ A BiAst element represents C expression statement """
    expression: CExpression


@dataclass
class CLabelledStatement(CStatement):
    """ A BiAst element represents C labelled statement """
    label: str
    body: CStatement


@dataclass
class CNullStatement(CStatement):
    """ A BiAst element represents C null statement """


@dataclass
class CMacroStatement(CStatement, CMacro):
    """ A BiAst element represents C macro statement """
    macro_name: str
    arguments: List[CBiAst]

    @classmethod
    def get_base_class(cls) -> SubType[CBiAst]:
        """ To get base class """
        return CStatement


@dataclass
class CIfStatement(CStatement):
    """ A BiAst element represents C if else statement """
    condition: CExpression
    then_statement: CStatement
    else_statement: Optional[CStatement] = None


@dataclass
class CSwitchStatement(CStatement):
    """ A BiAst element represents C switch statement """
    controller: CExpression
    cases: List[Tuple[CExpression, List[CStatementOrDeclaration]]]
    default: Optional[List[CStatementOrDeclaration]]


@dataclass
class CForStatement(CStatement):
    """ A BiAst element represents C for statement """
    initializer: Optional[Union[CVariableDeclaration, CExpression]]
    condition: Optional[CExpression]
    increment: Optional[CExpression]
    body: CStatement


@dataclass
class CWhileStatement(CStatement):
    """ A BiAst element represents C while statement """
    condition: CExpression
    body: CStatement


@dataclass
class CDoWhileStatement(CStatement):
    """ A BiAst element represents C do while statement """
    body: CStatement
    condition: CExpression


@dataclass
class CAsmOperand(CBiAst):
    """ A BiAst element represents C asm operand """
    symbolic_name: Optional[str]
    constraint: str
    cexpr: CExpression

    @classmethod
    def of(cls, content: str) -> CBiAst:
        """ A function to parse a block of C codes to C BiAst """
        return super().of(content)


@dataclass
class CAsmStatement(CStatement):
    """ A BiAst element represents C asm statement """
    template: str
    output_operands: List[CAsmOperand]
    input_operands: List[CAsmOperand]
    clobbers: List[str]
    goto_labels: List[str]
    volatile: bool
    inline: bool


class CJumpStatement(CStatement):
    """ A BiAst element represents C jump statement """

    def rsvd_ast(self) -> CJumpStatement:
        """ reserve a public interface """
        return self


@dataclass
class CBreakStatement(CJumpStatement):
    """ A BiAst element represents C break statement """


@dataclass
class CContinueStatement(CJumpStatement):
    """ A BiAst element represents C continue statement """


@dataclass
class CGotoStatement(CJumpStatement):
    """ A BiAst element represents C goto statement """
    target: str


@dataclass
class CReturnStatement(CJumpStatement):
    """ A BiAst element represents C return statement """
    value: Optional[CExpression] = None


class CTypeSpecifier(Enum):
    """ Enum of C type specifiers """
    VOID = 'void'
    BOOL = '_Bool'
    CHAR = 'char'
    SHORT = 'short'
    INT = 'int'
    LONG = 'long'
    FLOAT = 'float'
    DOUBLE = 'double'
    SIGNED = 'signed'
    UNSIGNED = 'unsigned'


@dataclass
class CTypeDecorator:
    """ A BiAst element represents C type decorator """
    qualifiers: Set[CTypeQualifier]
    base_decorator: Optional[CTypeDecorator]

    def get_type(self, base_type: CType) -> CType:
        """ To get type of the decorator """
        if self.base_decorator is not None:
            return self.base_decorator.get_type(base_type)

        return base_type


@dataclass
class CPointerDecorator(CTypeDecorator):
    """ A BiAst element represents C pointer type decorator """
    def get_type(self, base_type: CType) -> CType:
        return super().get_type(
            CPointerType(base_type).add_qualifiers(self.qualifiers)
        )


@dataclass
class CArrayDecorator(CTypeDecorator):
    """ A BiAst element represents C array decorator """
    capacity: Optional[CExpression]

    def get_type(self, base_type: CType) -> CType:
        return super().get_type(
            CArrayType(base_type, self.capacity).add_qualifiers(self.qualifiers)
        )


@dataclass
class CFunctionDecorator(CTypeDecorator):
    """ A BiAst element represents C function decorator """
    parameters: List[CParameterDeclaration]
    variable_length_argument: bool
    return_ptr_level: int = 0

    def get_type(self, base_type: CType) -> CType:
        return super().get_type(CFunctionType(
            self.parameters,
            CPointerType.create(base_type, self.return_ptr_level),
            self.variable_length_argument
        ))


@dataclass
class CDeclarator:
    """ A BiAst element represents C declarator """
    name: Optional[str]
    decorator: Optional[CTypeDecorator] = None
    attributes: List[CAttribute] = field(default_factory=list, init=False)

    def get_type(self, specifier_qualifiers: list) -> CType:
        """ To get the complete type of a declarator """
        specifiers = []
        qualifiers: Set[CTypeQualifier] = set()
        complex_specifier: Optional[CType] = None

        for spec_qlr in specifier_qualifiers:
            if isinstance(spec_qlr, CTypeSpecifier):
                specifiers.append(spec_qlr)
            elif isinstance(spec_qlr, (CMacroType, CTypeOf, CStructType,
                                       CUnionType, CEnumType, CAliasType)):
                complex_specifier = spec_qlr
            elif isinstance(spec_qlr, CTypeQualifier):
                qualifiers.add(spec_qlr)
            elif not isinstance(spec_qlr, (CStorageQualifier, CFunctionSpecifier, CAttribute)):
                # attributes, storage qualifiers and function specifier have nothing to do
                # with the type of a declaration, just pass
                raise TypeError(f"unknown type specifier or qualifier {spec_qlr}")

        specified_type: CType
        if complex_specifier is None:
            specifier_set = frozenset(Counter(specifiers).items())
            # The following valid type-specifier-set follows the specification in
            # *Section 6.7.2, Type specifiers* in ISO/C1999.
            spc_var = specifier_type_mapping.get(specifier_set)
            if spc_var is not None:
                specified_type = deepcopy(spc_var)
            else:
                raise TypeError(f"invalid set of type specifiers: {specifiers}")
        else:
            specified_type = complex_specifier

        ret_type = specified_type
        ret_type.add_qualifiers(qualifiers)

        if self.decorator is not None:
            ret_type = self.decorator.get_type(ret_type)

        return ret_type


@dataclass
class CAbstractDeclarator(CDeclarator):
    """ A BiAst element represents C abstract declarator """
    name: Optional[str] = None


@dataclass
class CInitDeclarator(CDeclarator):
    """ A BiAst element represents C init declarator """
    initializer: Optional[CExpression] = None


@dataclass
class CFieldDeclarator(CDeclarator):
    """ A BiAst element represents C structure field declarator """
    bitwidth: Optional[CExpression] = None


@dataclass
class CCaseStatement(CStatement):
    """ A BiAst element represents C case statement """
    value: CExpression
    stmt: CStatement


@dataclass
class CDefaultCaseStatement(CStatement):
    """ A BiAst element represents C default case statement """
    stmt: CStatement


class CAsmQualifier(Enum):
    """ Enum of asm qualifiers """
    VOLATILE = "volatile"
    INLINE = "inline"
    GOTO = "goto"


# _Complex and _Imaginary left unimplemented
specifier_type_mapping = {
    # void
    frozenset(Counter([CTypeSpecifier.VOID]).items()):
        CVoidType(),

    # char
    frozenset(Counter([CTypeSpecifier.CHAR]).items()):
        CCharType(sign_desc=None),

    # signed char
    frozenset(Counter([CTypeSpecifier.SIGNED, CTypeSpecifier.CHAR]).items()):
        CCharType(sign_desc=CSignDescriptor.SIGNED),

    # unsigned char
    frozenset(Counter([CTypeSpecifier.UNSIGNED, CTypeSpecifier.CHAR]).items()):
        CCharType(sign_desc=CSignDescriptor.UNSIGNED),

    # short, signed short, short int, signed short int
    frozenset(Counter([CTypeSpecifier.SHORT]).items()):
        CShortType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SHORT, CTypeSpecifier.INT]).items()):
        CShortType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED, CTypeSpecifier.SHORT]).items()):
        CShortType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED,
                       CTypeSpecifier.SHORT, CTypeSpecifier.INT]).items()):
        CShortType(sign_desc=CSignDescriptor.SIGNED),

    # unsigned short, unsigned short int
    frozenset(Counter([CTypeSpecifier.UNSIGNED, CTypeSpecifier.SHORT]).items()):
        CShortType(sign_desc=CSignDescriptor.UNSIGNED),
    frozenset(Counter([CTypeSpecifier.UNSIGNED,
                       CTypeSpecifier.SHORT, CTypeSpecifier.INT]).items()):
        CShortType(sign_desc=CSignDescriptor.UNSIGNED),

    # int, signed, signed int
    frozenset(Counter([CTypeSpecifier.INT]).items()):
        CIntType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED]).items()):
        CIntType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED, CTypeSpecifier.INT]).items()):
        CIntType(sign_desc=CSignDescriptor.SIGNED),

    # unsigned, unsigned int
    frozenset(Counter([CTypeSpecifier.UNSIGNED]).items()):
        CIntType(sign_desc=CSignDescriptor.UNSIGNED),
    frozenset(Counter([CTypeSpecifier.UNSIGNED, CTypeSpecifier.INT]).items()):
        CIntType(sign_desc=CSignDescriptor.UNSIGNED),

    # long, signed long, long int, signed long int
    frozenset(Counter([CTypeSpecifier.LONG]).items()):
        CLongType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.LONG, CTypeSpecifier.INT]).items()):
        CLongType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED, CTypeSpecifier.LONG]).items()):
        CLongType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED,
                       CTypeSpecifier.LONG, CTypeSpecifier.INT]).items()):
        CLongType(sign_desc=CSignDescriptor.SIGNED),

    # unsigned long, unsigned long int
    frozenset(Counter([CTypeSpecifier.UNSIGNED, CTypeSpecifier.LONG]).items()):
        CLongType(sign_desc=CSignDescriptor.UNSIGNED),
    frozenset(Counter([CTypeSpecifier.UNSIGNED,
                       CTypeSpecifier.LONG, CTypeSpecifier.INT]).items()):
        CLongType(sign_desc=CSignDescriptor.UNSIGNED),

    # long long, signed long long, long long int, signed long long int
    frozenset(Counter([CTypeSpecifier.LONG, CTypeSpecifier.LONG]).items()):
        CLonglongType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.LONG, CTypeSpecifier.LONG, CTypeSpecifier.INT]).items()):
        CLonglongType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED,
                       CTypeSpecifier.LONG, CTypeSpecifier.LONG]).items()):
        CLonglongType(sign_desc=CSignDescriptor.SIGNED),
    frozenset(Counter([CTypeSpecifier.SIGNED,
                       CTypeSpecifier.LONG,
                       CTypeSpecifier.LONG,
                       CTypeSpecifier.INT]).items()):
        CLonglongType(sign_desc=CSignDescriptor.SIGNED),

    # unsigned long long, unsigned long long int
    frozenset(Counter([CTypeSpecifier.UNSIGNED,
                       CTypeSpecifier.LONG, CTypeSpecifier.LONG]).items()):
        CLonglongType(sign_desc=CSignDescriptor.UNSIGNED),
    frozenset(Counter([CTypeSpecifier.UNSIGNED,
                       CTypeSpecifier.LONG,
                       CTypeSpecifier.LONG,
                       CTypeSpecifier.INT]).items()):
        CLonglongType(sign_desc=CSignDescriptor.UNSIGNED),

    # float
    frozenset(Counter([CTypeSpecifier.FLOAT]).items()):
        CFloatType(),

    # double
    frozenset(Counter([CTypeSpecifier.DOUBLE]).items()):
        CDoubleType(),

    # long double
    frozenset(Counter([CTypeSpecifier.LONG, CTypeSpecifier.DOUBLE]).items()):
        CLongDoubleType(),

    # _Bool
    frozenset(Counter([CTypeSpecifier.BOOL]).items()):
        CBoolType(),
}


# A parser cache for performance
parser_cache: Dict[Any, Any] = {}


def parse(content: str, entry: str, is_constant: bool = False):
    """ To parse and transform text to CBiAst """
    from hmdsl.common.c_biast.biast_visitor import BiastVisitor

    key = (content, entry)
    if key in parser_cache:
        ret = parser_cache.get(key)
        if is_constant:
            return ret
    else:
        lexer = BiastGrammarLexer(InputStream(content))
        lexer.removeErrorListeners()
        lexer.addErrorListener(DSLErrorListener())
        parser = BiastGrammarParser(CommonTokenStream(lexer))
        parser.removeErrorListeners()
        parser.addErrorListener(DSLErrorListener())
        ret = BiastVisitor().visit(getattr(parser, entry)())
        parser_cache[key] = ret

    return deepcopy(ret)
