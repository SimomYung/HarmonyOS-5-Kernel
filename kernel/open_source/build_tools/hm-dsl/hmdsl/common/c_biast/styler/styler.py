#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Abstract class of C BiAst Stylers
Author: Huawei OS Kernel Lab
Create: Thu 14 Jan 2021 14:30:00 PM CST
"""

from __future__ import annotations, absolute_import

from typing import Dict, Type, Union, List, Optional
from dataclasses import dataclass, field

from hmdsl.common.c_biast.styler.config import CStylerConfig, CStylerPresets, BraceStyle
from hmdsl.common.c_biast.styler.stream import CStylerStream
from hmdsl.common.c_biast.biast import CExpression, CTranslationUnit, CClassicalAttribute, \
    CMacroAttribute, CFunction, CDeclaration, CTypeDeclaration, CVariableDeclaration, \
    CParameterDeclaration, CMacroParameterDeclaration, CFieldDeclaration, CType, CTypeOf, \
    CMacroType, CAliasType, CVoidType, CSignDescriptor, CIntegerType, CCharType, CShortType, \
    CIntType, CLongType, CLonglongType, CFloatType, CDoubleType, CPointerType, CArrayType, \
    CStructType, CUnionType, CEnumType, CFunctionType, CMacroExpression, CUnaryOperator, \
    CUnaryExpression, CBinaryExpression, CIdentifierExpression, CStringLiteral, \
    CAssignmentExpression, CConditionalExpression, CCastExpression, CElementExpression, \
    CMemberExpression, CCallExpression, CIndexDesignator, CRangeDesignator, CMemberDesignator, \
    CDesignation, CConstructorExpression, CEnumItem, CIntegerBase, CIntegerConstant, \
    CFloatConstant, CCharConstant, CCompoundStatement, CExpressionStatement, CNullStatement, \
    CMacroStatement, CIfStatement, CForStatement, CWhileStatement, CDoWhileStatement, CBreakStatement, \
    CContinueStatement, CReturnStatement, CBiAst


def unfold_type(ctype: Union[CArrayType, CPointerType]) -> List[CType]:
    """ pointers and arrays have base types to indicate the types
    of the elements they pointing to or containing of.
    For example: ``int a[4];`` is an array which contains 4 integers,
    thus the base type of this array is `int`;
    `int * a;` is a pointer to integer, thus the base type of this pointer is also `int`.
    This function is to unfold such types in the order of the nesting level.
    For example: `int* (*p)[5](void);` is an array of pointers pointing to functions.
    Thus after unfolding, we get a list of types CArrayType -> CPointerType -> CFunctionType.
    This is a helper function to render nesting types.
    """

    unfold_types: List[CType] = [ctype]

    base_type = ctype.base_type
    while isinstance(base_type, (CArrayType, CPointerType)):
        unfold_types.append(base_type)
        base_type = base_type.base_type

    unfold_types.append(base_type)

    return unfold_types


@dataclass
class CStyler:
    """
    A C code styler to print C codes with certain config and stream type
    """

    config: CStylerConfig = field(default_factory=lambda: CStylerPresets.export_hongmeng_config())
    stream: CStylerStream = field(default_factory=CStylerStream)

    def __post_init__(self):
        self.stream.width = self.config.width

    def render(self, ast: CBiAst) -> str:
        """
        To render a C BiAst to corresponding C codes

        Args:
            ast (CBiAst): A given C BiAst

        Returns:
            str: The corresponding C codes
        """
        self.stream.clear()
        self._render(ast)
        return self.stream.getvalue(self.config.indent)

    def _render(self, ast: CBiAst, parent: Optional[CBiAst] = None):
        """ here we trust the type of operators in ``opr`` is always
        correct ;)
        """

        ast_type = type(ast)
        for typ in (ast_type, *ast_type.__bases__):
            typname = typ.__name__.lower()
            if hasattr(self, f"_render_{typname}"):
                func = getattr(self, f"_render_{typname}")
                break
        else:
            raise NotImplementedError(f"rendering of {type(ast)} is not supported yet")

        # we put parentheses around expression when its parent expression has lower precedence,
        # or it has higher associative relation.
        # We also put parentheses around binary expression
        # with lower precedence than its parent binary expression
        # to silence warnings triggered by gcc command -Wparentheses (or -Wall)
        def parentheses_subexpr(ast: CExpression, parent: CExpression) -> bool:
            return ast.precedence > parent.precedence or \
                ast.precedence == parent.precedence and ast.get_assoc()

        parentheses_expr = isinstance(ast, CExpression) and isinstance(parent, CExpression) and \
            not isinstance(parent, CCallExpression) and parentheses_subexpr(ast, parent)

        if parentheses_expr or self.config.parentheses_style.wparentheses_nested_expression(ast, parent):
            self.stream.write('(')
            func(ast, parent)
            self.stream.write(')')
        else:
            func(ast, parent)

    def _render_enum_item(self, item: CEnumItem, _: Optional[CBiAst]):
        if item.name is not None:
            self.stream.write(item.name)

        if item.value is not None:
            self.stream.write("=")
            self._render(item.value, item)

        self.stream.write(",")
        self.stream.endl()

    def _render_ctranslationunit(self, unit: CTranslationUnit, _: Optional[CBiAst]):
        for elem in unit.content:
            self._render(elem, unit)
            self.stream.endl()
            self.stream.endl()

    def _render_cclassicalattribute(self, attr: CClassicalAttribute, _: Optional[CBiAst]):
        self.stream.write('__attribute__((')
        self.stream.write(attr.name)
        if attr.arguments is not None:
            self.stream.write('(')
            for i, arg in enumerate(attr.arguments):
                if i > 0:
                    self.stream.write(',')
                    self.stream.space()

                self._render(arg, attr)

            self.stream.write(')')

        self.stream.write('))')

    def _render_cmacroattribute(self, attr: CMacroAttribute, _: Optional[CBiAst]):
        self.stream.write(attr.macro_name)

        if len(attr.arguments) > 0:
            self.stream.write('(')
            for i, arg in enumerate(attr.arguments):
                if i > 0:
                    self.stream.write(',')
                    self.stream.space_or_endl()

                self._render(arg, attr)

            self.stream.write(')')

    def _render_ctypedeclaration(self, type_decl: CTypeDeclaration, _: Optional[CBiAst]):
        self.stream.write('typedef')
        self.stream.space()

        self._render(type_decl.type, type_decl)
        if not isinstance(type_decl.type, (CArrayType, CPointerType)):
            if type_decl.name is not None:
                self.stream.space()
                self.stream.write(type_decl.name)

        self.stream.write(";")

    def _render_cfunctiondeclaration(self, var_decl: CVariableDeclaration, _: Optional[CBiAst]):
        if not isinstance(var_decl.type, CFunctionType) or var_decl.name is None:
            raise AttributeError(f"Unexpected declaration {var_decl} to render as a function")
        func_type: CFunctionType = var_decl.type

        self._render(func_type.return_type, func_type)
        for attr in var_decl.attributes:
            self.stream.space()
            self._render(attr, var_decl)

        self.stream.space_or_endl()
        self.stream.write(var_decl.name)
        self.stream.write('(')

        for i, param in enumerate(func_type.parameter_decls):
            if i > 0:
                self.stream.write(',')
                self.stream.space()

            self._render(param, var_decl)

        if func_type.variable_length_argument and len(func_type.parameter_decls) > 0:
            self.stream.write(", ...")
        elif not func_type.variable_length_argument and len(func_type.parameter_decls) == 0:
            self.stream.write("void")

        self.stream.write(')')
        self.stream.write(';')
        self.stream.endl()

    def _render_cvariabledeclaration(self, var_decl: CVariableDeclaration,
                                     parent: Optional[CBiAst]):
        for qualifier in var_decl.storage_qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        if isinstance(var_decl.type, CFunctionType):
            self._render_cfunctiondeclaration(var_decl, parent)
            return

        self._render(var_decl.type, var_decl)
        if not isinstance(var_decl.type, (CArrayType, CPointerType)):
            for attribute in var_decl.attributes:
                self.stream.space()
                self._render(attribute, var_decl)

            if var_decl.name is not None:
                self.stream.space()
                self.stream.write(var_decl.name)

        if var_decl.initializer is not None:
            self.stream.space()
            self.stream.write('=')
            self.stream.space()
            self._render(var_decl.initializer, var_decl)

        self.stream.write(';')

    def _render_cparameterdeclaration(self, param_decl: CParameterDeclaration, _: Optional[CBiAst]):
        self._render(param_decl.type, param_decl)
        if not isinstance(param_decl.type, (CArrayType, CPointerType)):
            for attribute in param_decl.attributes:
                self.stream.space()
                self._render(attribute, param_decl)

            if param_decl.name is not None:
                self.stream.space()
                self.stream.write(param_decl.name)

    def _render_cfielddeclaration(self, decl: CFieldDeclaration, _: Optional[CBiAst]):
        self._render(decl.type, decl)
        if not isinstance(decl.type, (CArrayType, CPointerType)):
            if decl.name is not None:
                self.stream.space()
                self.stream.write(decl.name)

        if decl.bitwidth is not None:
            self.stream.write(":")
            self.stream.space()
            self.stream.write(str(decl.bitwidth))

        if not isinstance(decl.type, (CArrayType, CPointerType)):
            for attribute in decl.attributes:
                self.stream.space()
                self._render(attribute, decl)

        self.stream.write(";")
        self.stream.endl()

    def _render_cfunction(self, func: CFunction, _: Optional[CBiAst]):
        for qualifier in func.storage_qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        if func.inline:
            self.stream.write('inline')
            self.stream.space()

        self._render(func.return_type)
        for attr in func.attributes:
            self.stream.space()
            self._render(attr, func)

        self.stream.space_or_endl()
        self.stream.write(func.name)
        self.stream.write('(')

        for i, param in enumerate(func.parameters):
            if i > 0:
                self.stream.write(',')
                self.stream.space()

            self._render(param, func)

        if func.variable_length_argument and len(func.parameters) > 0:
            self.stream.write(", ...")
        elif not func.variable_length_argument and len(func.parameters) == 0:
            self.stream.write("void")

        self.stream.write(')')

        if func.body is None:
            self.stream.writeln(';')
            return

        self._render_ccompoundstatement(func.body, func)

    def _render_cintegertype(self, inttype: CIntegerType, _: Optional[CBiAst]):
        for qualifier in inttype.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        integer_type_names: Dict[Type[CIntegerType], str] = {
            CIntType: 'int',
            CLongType: 'long',
            CLonglongType: 'long long',
            CCharType: 'char',
            CShortType: 'short',
        }

        if inttype.sign_desc is not None:
            if isinstance(inttype, CCharType) or inttype.sign_desc is CSignDescriptor.UNSIGNED:
                # By default all types are signed (except for chars).
                self.stream.write(inttype.sign_desc.value)
                self.stream.space()

        name = integer_type_names.get(type(inttype))
        if name is None:
            raise TypeError(f"{type(inttype)} is not a valid integer type")

        self.stream.write(name)

    def _render_cfloattype(self, typ: CFloatType, _: Optional[CBiAst]):
        for qualifier in typ.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write('float')

    def _render_cdoubletype(self, typ: CDoubleType, _: Optional[CBiAst]):
        for qualifier in typ.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write('double')

    def _render_clongdoubletype(self, typ: CDoubleType, _: Optional[CBiAst]):
        for qualifier in typ.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write('long')
        self.stream.space()
        self.stream.write('double')

    def _render_cvoidtype(self, void_type: CVoidType, _: Optional[CBiAst]):
        for qualifier in void_type.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write('void')

    def _render_cpointertype(self, ptr_type: CPointerType, parent: Optional[CBiAst]):
        if isinstance(parent, CDeclaration):
            for attribute in parent.attributes:
                self._render(attribute, parent)
                self.stream.space()

            unfold_types = unfold_type(ptr_type)
            self._render_listtypes(unfold_types, parent, 0)
        else:
            self._render(ptr_type.base_type, ptr_type)
            self.stream.write('*')

    def _render_caliastype(self, alias_type: CAliasType, _: Optional[CBiAst]):
        for qualifier in alias_type.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write(alias_type.name)

    def _render_cstructtype(self, struct_type: CStructType, _: Optional[CBiAst]):
        for i, attr in enumerate(struct_type.attributes):
            if i > 0:
                self.stream.space()

            self._render(attr, struct_type)

        for qualifier in struct_type.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write("struct")
        if struct_type.name is not None:
            self.stream.space()
            self.stream.write(struct_type.name)

        if struct_type.field_decls is not None:
            self.stream.space()
            self.stream.write("{")
            self.stream.endl()
            self.stream.indent_level += 1

            for struct_field in struct_type.field_decls:
                self._render(struct_field, struct_type)

            self.stream.indent_level -= 1
            self.stream.write("}")

    def _render_cuniontype(self, union_type: CUnionType, _: Optional[CBiAst]):
        for attribute in union_type.attributes:
            self._render(attribute, union_type)
            self.stream.space()

        for qualifier in union_type.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write("union")
        if union_type.name is not None:
            self.stream.space()
            self.stream.write(union_type.name)

        if union_type.field_decls is not None:
            self.stream.space()
            self.stream.write("{")
            self.stream.endl()
            self.stream.indent_level += 1

            for union_field in union_type.field_decls:
                self._render(union_field, union_type)

            self.stream.indent_level -= 1
            self.stream.write("}")

    def _render_cenumtype(self, enum_type: CEnumType, _: Optional[CBiAst]):
        for attribute in enum_type.attributes:
            self._render(attribute, enum_type)
            self.stream.space()

        for qualifier in enum_type.qualifiers:
            self.stream.write(qualifier.value)
            self.stream.space()

        self.stream.write("enum")
        if enum_type.name is not None:
            self.stream.space()
            self.stream.write(enum_type.name)

        if enum_type.enum_items is not None:
            self.stream.space()
            self.stream.write("{")
            self.stream.endl()
            self.stream.indent_level += 1

            for item in enum_type.enum_items or []:
                self._render_enum_item(item, enum_type)

            self.stream.indent_level -= 1
            self.stream.write("}")

    def _render_listtypes(self, listtypes: List[CType], decl: CDeclaration, ptr_flag: int):
        if not listtypes:
            if decl.name is not None:
                self.stream.write(decl.name)
            return

        typ = listtypes.pop()
        if isinstance(typ, CPointerType):
            ptr_flag = self.__render_listtypes_pointertype(listtypes, decl, ptr_flag)
        elif isinstance(typ, CArrayType):
            ptr_flag = self.__render_listtypes_arraytype(typ, listtypes, decl, ptr_flag)
        elif isinstance(typ, CFunctionType):
            ptr_flag = self.__render_listtypes_functiontype(typ, listtypes, decl, ptr_flag)
        else:
            self._render(typ)
            if not isinstance(decl, CParameterDeclaration) or decl.name is not None:
                self.stream.space()
            self._render_listtypes(listtypes, decl, ptr_flag)

    def __render_listtypes_pointertype(self, listtypes: List[CType],
                                       decl: CDeclaration, ptr_flag: int) -> int:
        if ptr_flag == 1:
            self.stream.write('(')
        self.stream.write('*')
        self._render_listtypes(listtypes, decl, 0)
        if ptr_flag == 1:
            self.stream.write(')')
        ptr_flag = 0
        return ptr_flag

    def __render_listtypes_arraytype(self, typ: CArrayType, listtypes: List[CType],
                                     decl: CDeclaration, ptr_flag: int) -> int:
        ptr_flag = 1
        self._render_listtypes(listtypes, decl, ptr_flag)
        self.stream.write('[')
        if typ.capacity is not None:
            self._render(typ.capacity, typ)
        self.stream.write(']')
        return ptr_flag

    def __render_listtypes_functiontype(self, typ: CFunctionType, listtypes: List[CType],
                                        decl: CDeclaration, ptr_flag: int) -> int:
        ptr_flag = 1
        self._render(typ.return_type)
        self.stream.space()
        self._render_listtypes(listtypes, decl, ptr_flag)
        self.stream.write('(')
        for i, param in enumerate(typ.parameter_decls):
            if i > 0:
                self.stream.write(',')
                self.stream.space()
            self._render(param, decl)
        if typ.variable_length_argument and len(typ.parameter_decls) > 0:
            self.stream.write(", ...")
        elif not typ.variable_length_argument and len(typ.parameter_decls) == 0:
            self.stream.write("void")
        self.stream.write(')')
        return ptr_flag

    def _render_carraytype(self, array_type: CArrayType, parent: Optional[CBiAst]):
        if isinstance(parent, CDeclaration):
            for attribute in parent.attributes:
                self._render(attribute, parent)
                self.stream.space()

            unfold_types = unfold_type(array_type)
            self._render_listtypes(unfold_types, parent, 0)
        else:
            for qualifier in array_type.qualifiers:
                self.stream.write(qualifier.value)
                self.stream.space()

            self.stream.write('__typeof__')
            self.stream.write('(')
            self._render(array_type.base_type, array_type)
            self.stream.write("[")

            if array_type.capacity is not None:
                self._render(array_type.capacity, array_type)

            self.stream.write("]")
            self.stream.write(')')

    def _render_cfunctiontype(self, func_type: CFunctionType, _: Optional[CBiAst]):
        self.stream.write('__typeof__')
        self.stream.write('(')

        # render return type """
        self._render(func_type.return_type, func_type)
        self.stream.space()

        self.stream.write('(')
        for i, param_decl in enumerate(func_type.parameter_decls):
            if i > 0:
                self.stream.write(',')
                self.stream.space()

            if isinstance(param_decl, CMacroParameterDeclaration):
                self._render(param_decl)
            else:
                self._render(param_decl.type, param_decl)

        if not func_type.variable_length_argument and len(func_type.parameter_decls) == 0:
            self.stream.write('void')
        elif func_type.variable_length_argument and len(func_type.parameter_decls) > 0:
            self.stream.write(',')
            self.stream.space()
            self.stream.write('...')

        self.stream.write(')')
        self.stream.write(')')

    def _render_ctypeof(self, typeof: CTypeOf, _: Optional[CBiAst]):
        self.stream.write('__typeof__')
        self.stream.write('(')
        self._render(typeof.operand, typeof)
        self.stream.write(')')

    def _render_cmacrotype(self, macro_type: CMacroType, _: Optional[CBiAst]):
        self.stream.write(macro_type.macro_name)

        if macro_type.arguments:
            self.stream.write('(')
            for i, arg in enumerate(macro_type.arguments):
                if i > 0:
                    self.stream.write(',')
                    self.stream.space()

                self._render(arg, macro_type)

            self.stream.write(')')

    def _render_cexpressionstatement(self, expr_stmt: CExpressionStatement, _: Optional[CBiAst]):
        self._render(expr_stmt.expression, expr_stmt)
        self.stream.write(";")

    def _render_cnullstatement(self, __null_stmt: CNullStatement, _: Optional[CBiAst]):
        self.stream.write(";")

    def _render_ccompoundstatement(self, compound: CCompoundStatement, parent: Optional[CBiAst]):
        if parent is None or isinstance(parent, CCompoundStatement):
            pass
        elif isinstance(parent, CFunction) and self.config.brace_style is not BraceStyle.ATTACHED:
            self.stream.endl()
        elif self.config.brace_style is BraceStyle.BROKEN:
            self.stream.endl()
        elif not isinstance(parent, CCompoundStatement):
            self.stream.space()

        self.stream.write("{")
        self.stream.endl()
        self.stream.indent_level += 1

        for i, item in enumerate(compound.body):
            if i > 0:
                self.stream.endl()

            self._render(item, compound)

        self.stream.indent_level -= 1
        self.stream.endl()
        self.stream.write("}")

    def _render_cifstatement(self, ifstmt: CIfStatement, _: Optional[CBiAst]):
        self.stream.write("if")
        self.stream.space()
        self.stream.write("(")
        self._render(ifstmt.condition, ifstmt)
        self.stream.write(")")
        self._render(ifstmt.then_statement, ifstmt)

        if ifstmt.else_statement is not None:
            self.stream.write(" ", "else", " ")
            self._render(ifstmt.else_statement, ifstmt)

    def _render_cforstatement(self, for_stmt: CForStatement, _: Optional[CBiAst]):
        self.stream.write('for')
        self.stream.write('(')
        if for_stmt.initializer is not None:
            self._render(for_stmt.initializer, for_stmt)
            if isinstance(for_stmt.initializer, CExpression):
                self.stream.write('; ')
            else:
                self.stream.write(' ')
        if for_stmt.condition is not None:
            self._render(for_stmt.condition, for_stmt)
        self.stream.write('; ')
        if for_stmt.increment is not None:
            self._render(for_stmt.increment, for_stmt)
        self.stream.write(')')
        self.stream.space()
        self._render(for_stmt.body, for_stmt)

    def _render_cwhilestatement(self, while_stmt: CWhileStatement, _: Optional[CBiAst]):
        self.stream.write('while')
        self.stream.write('(')
        self._render(while_stmt.condition, while_stmt)
        self.stream.write(')')
        self.stream.space()
        self._render(while_stmt.body, while_stmt)

    def _render_cdowhilestatement(self, dowhile_stmt: CDoWhileStatement, _: Optional[CBiAst]):
        self.stream.write('do')
        self.stream.space()
        self._render(dowhile_stmt.body)
        self.stream.space()
        self.stream.write('while')
        self.stream.space()
        self.stream.write('(')
        self._render(dowhile_stmt.condition, dowhile_stmt)
        self.stream.write(')')
        self.stream.write(';')

    def _render_creturnstatement(self, retstmt: CReturnStatement, _: Optional[CBiAst]):
        self.stream.write("return")
        if retstmt.value is not None:
            self.stream.space()
            self._render(retstmt.value, retstmt)

        self.stream.write(";")

    def _render_cmacrostatement(self, stmt: CMacroStatement, _: Optional[CBiAst]):
        self.stream.write(stmt.macro_name)

        if stmt.arguments:
            self.stream.write('(')
            for i, arg in enumerate(stmt.arguments):
                if i > 0:
                    self.stream.write(',')
                    self.stream.space()

                self._render(arg, stmt)

            self.stream.write(')')

    def _render_ccontinuestatement(self, __stmt: CContinueStatement, _: Optional[CBiAst]):
        self.stream.write("continue")
        self.stream.write(";")

    def _render_cbreakstatement(self, __stmt: CBreakStatement, _: Optional[CBiAst]):
        self.stream.write("break")
        self.stream.write(";")

    def _render_cintegerconstant(self, value: CIntegerConstant, _: Optional[CBiAst]):
        if value.base is CIntegerBase.DEC:
            self.stream.write(f"{value.value}")
        elif value.base is CIntegerBase.BIN:
            self.stream.write(f"{bin(value.value)}")
        elif value.base is CIntegerBase.HEX:
            self.stream.write(f"{hex(value.value)}")
        elif value.base is CIntegerBase.OCT:
            val = f'{value.value:o}'
            if val[0] == '-':
                self.stream.write(f"-0{val[1:]}")
            else:
                self.stream.write(f"0{val}")
        else:
            raise NotImplementedError(f"unknown integer base {value.base}")

        suffix = ''.join(sorted((s.value for s in value.suffixes), reverse=True))
        self.stream.write(suffix)

    def _render_cfloatconstant(self, value: CFloatConstant, _: Optional[CBiAst]):
        self.stream.write(value.value)
        suffix = ''.join(sorted((s.value for s in value.suffixes), reverse=True))
        self.stream.write(suffix)

    def _render_ccharconstant(self, value: CCharConstant, _: Optional[CBiAst]):
        self.stream.write(f"'{value.value}'")

    def _render_cstringliteral(self, string: CStringLiteral, _: Optional[CBiAst]):
        self.stream.write("\"")
        self.stream.write(string.content)
        self.stream.write("\"")

    def _render_cassignmentexpression(self, expr: CAssignmentExpression, _: Optional[CBiAst]):
        self._render(expr.lhs, expr)
        self.stream.space()
        self.stream.write(expr.operator.value)
        self.stream.space()
        self._render(expr.rhs, expr)

    def _render_cidentifierexpression(self, ident: CIdentifierExpression, _: Optional[CBiAst]):
        self.stream.write(ident.name)

    def _render_cbinaryexpression(self, expr: CBinaryExpression, _: Optional[CBiAst]):
        self._render(expr.left, expr)
        self.stream.space()
        self.stream.write(expr.operator.value)
        self.stream.space()
        self._render(expr.right.set_assoc(True), expr)

    def _render_cunaryexpression(self, expr: CUnaryExpression, _: Optional[CBiAst]):
        if isinstance(expr.operator, CUnaryOperator):
            self.stream.write(expr.operator.value)
            if expr.operator.value == 'sizeof':
                self.stream.write('(')
            self._render(expr.operand, expr)
            if expr.operator.value == 'sizeof':
                self.stream.write(')')
        else:
            self._render(expr.operand, expr)
            self.stream.write(expr.operator.value)

    def _render_ccastexpression(self, cast: CCastExpression, _: Optional[CBiAst]):
        self.stream.write('(')
        self._render(cast.cast_type, cast)
        self.stream.write(')')
        self._render(cast.cast_content, cast)

    def _render_cdesignation(self, designation: CDesignation, _: Optional[CBiAst]):
        if len(designation.designators) == 0:
            return

        for designator in designation.designators:
            if isinstance(designator, CMemberDesignator):
                self.stream.write(f".{designator.field_name}")
            elif isinstance(designator, CIndexDesignator):
                self.stream.write("[")
                self._render(designator.index, designator)
                self.stream.write("]")
            elif isinstance(designator, CRangeDesignator):
                self.stream.write("[")
                self._render(designator.left, designator)
                self.stream.space()
                self.stream.write('...')
                self.stream.space()
                self._render(designator.right, designator)
                self.stream.write("]")
            else:
                raise NotImplementedError(f"unknown designator: {designator}")

        self.stream.space()
        self.stream.write('=')
        self.stream.space()

    def _render_cconstructorexpression(self, constructor: CConstructorExpression,
                                       _: Optional[CBiAst]):
        self.stream.write("{")
        self.stream.indent_level += 1
        self.stream.endl()

        for i, (designation, expression) in enumerate(constructor.items):
            if i > 0:
                self.stream.write(',')
                self.stream.endl()

            self._render(designation, constructor)
            self._render(expression, constructor)

        self.stream.indent_level -= 1
        self.stream.endl()
        self.stream.write("}")

    def _render_ccallexpression(self, call_expr: CCallExpression, _: Optional[CBiAst]):
        self._render(call_expr.callee, call_expr)
        self.stream.write('(')
        for i, arg in enumerate(call_expr.arguments):
            if i > 0:
                self.stream.write(',')
                self.stream.space_or_endl()

            self._render(arg, call_expr)

        self.stream.write(')')

    def _render_cmacroparameterdeclaration(self, param_decl: CMacroParameterDeclaration,
                                           _: Optional[CBiAst]):
        self.stream.write(param_decl.macro_name)

        if len(param_decl.arguments) > 0:
            self.stream.write('(')
            for i, arg in enumerate(param_decl.arguments):
                if i > 0:
                    self.stream.write(',')
                    self.stream.space_or_endl()

                self._render(arg, param_decl)

            self.stream.write(')')

    def _render_cmacroexpression(self, call_expr: CMacroExpression, _: Optional[CBiAst]):
        self.stream.write(call_expr.macro_name)

        if len(call_expr.arguments) > 0:
            self.stream.write('(')
            for i, arg in enumerate(call_expr.arguments):
                if i > 0:
                    self.stream.write(',')
                    self.stream.space_or_endl()

                self._render(arg, call_expr)

            self.stream.write(')')

    def _render_cmemberexpression(self, member_expr: CMemberExpression, _: Optional[CBiAst]):
        if isinstance(member_expr.object, CUnaryExpression) and \
                member_expr.object.operator is CUnaryOperator.DEREFERENCE:
            self._render(member_expr.object.operand, member_expr)
            self.stream.write('->')
            self.stream.write(member_expr.member)
        else:
            self._render(member_expr.object, member_expr)
            self.stream.write('.')
            self.stream.write(member_expr.member)

    def _render_celementexpression(self, element_expr: CElementExpression, _: Optional[CBiAst]):
        self._render(element_expr.array_object, element_expr)
        self.stream.write('[')
        self._render(element_expr.index, element_expr)
        self.stream.write(']')

    def _render_cconditionalexpression(self, conditional_expr: CConditionalExpression,
                                       _: Optional[CBiAst]):
        self._render(conditional_expr.condition.set_assoc(True), conditional_expr)
        self.stream.space()
        self.stream.write('?')
        self.stream.space()
        self._render(conditional_expr.consequent, conditional_expr)
        self.stream.space()
        self.stream.write(':')
        self.stream.space()
        self._render(conditional_expr.alternative, conditional_expr)
