# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Unittest testcases of the C Bi-Ast styler
Author: Huawei OS Kernel Lab
Create: Thu 14 Jan 2021 12:30:00 AM CST
"""

from unittest import TestCase

from hmdsl.common.c_biast.styler.styler import CStyler
from hmdsl.common.c_biast.styler.config import CStylerConfig, BraceStyle
from hmdsl.common.c_biast.biast import CExpression, CType, CSignDescriptor, \
    CCharType, CIntType, CStatement, CFunction, CFloatConstant, CIntegerConstant, CIntegerBase, \
    CDeclaration, CForStatement, CCompoundStatement, CVariableDeclaration, CFieldDeclaration, \
    CIdentifierExpression, CAttribute, CParameterDeclaration


class TestBraceStyle(TestCase):

    linux_brace_styler = CStyler(CStylerConfig(brace_style=BraceStyle.LINUX))
    attached_brace_styler = CStyler(CStylerConfig(brace_style=BraceStyle.ATTACHED))
    broken_brace_styler = CStyler(CStylerConfig(brace_style=BraceStyle.BROKEN))

    def test_linux_brace_1(self):
        ast = CFunction.of("int f() { return 1; }")
        expected = "int f()\n{\n\treturn 1;\n}"

        self.assertEqual(self.linux_brace_styler.render(ast), expected)

    def test_linux_brace_2(self):
        ast = CStatement.of("{ return 1; }")
        expected = "{\n\treturn 1;\n}"

        self.assertEqual(self.linux_brace_styler.render(ast), expected)

    def test_attached_brace_1(self):
        ast = CFunction.of("int f() { return 1; }")
        expected = "int f() {\n\treturn 1;\n}"

        self.assertEqual(self.attached_brace_styler.render(ast), expected)

    def test_attached_brace_2(self):
        ast = CStatement.of("{ return 1; }")
        expected = "{\n\treturn 1;\n}"

        self.assertEqual(self.attached_brace_styler.render(ast), expected)

    def test_attached_brace_3(self):
        ast = CStatement.of("if (1) { return 1; }")
        expected = "if (1) {\n\treturn 1;\n}"

        self.assertEqual(self.attached_brace_styler.render(ast), expected)

    def test_broken_brace_1(self):
        ast = CFunction.of("int f() { return 1; }")
        expected = "int f()\n{\n\treturn 1;\n}"

        self.assertEqual(self.broken_brace_styler.render(ast), expected)

    def test_broken_brace_2(self):
        ast = CStatement.of("{ return 1; }")
        expected = "{\n\treturn 1;\n}"

        self.assertEqual(self.broken_brace_styler.render(ast), expected)

    def test_broken_brace_3(self):
        ast = CStatement.of("if (1) { return 1; }")
        expected = "if (1)\n{\n\treturn 1;\n}"

        self.assertEqual(self.broken_brace_styler.render(ast), expected)


class TestTypeStyling(TestCase):
    styler = CStyler()

    def test_struct_nobody_1(self):
        ast = CType.of("struct A")
        self.assertEqual(self.styler.render(ast), "struct A")

    def test_float_1(self):
        ast = CType.of("float")
        self.assertEqual(self.styler.render(ast), "float")

    def test_float_2(self):
        ast = CType.of("double")
        self.assertEqual(self.styler.render(ast), "double")

    def test_float_3(self):
        ast = CType.of("long double")
        self.assertEqual(self.styler.render(ast), "long double")


class TestExpression(TestCase):
    styler = CStyler()

    def test_precedence_1(self):
        ast = CExpression.of("1 + (2 * 3)")
        self.assertEqual(self.styler.render(ast),
                         "1 + 2 * 3")

    def test_precedence_2(self):
        ast = CExpression.of("1 * (2 + 3)")
        self.assertEqual(self.styler.render(ast),
                         "1 * (2 + 3)")

    def test_precedence_3(self):
        ast = CExpression.of("(uintptr_t)(&a)")
        self.assertEqual(self.styler.render(ast),
                         "(uintptr_t)&a")

    def test_precedence_4(self):
        ast = CExpression.of("!(a + b)")
        self.assertEqual(self.styler.render(ast), "!(a + b)")

    def test_precedence_5(self):
        ast = CExpression.of("1 - (2 - 3)")
        self.assertEqual(self.styler.render(ast), "1 - (2 - 3)")

    def test_precedence_6(self):
        ast = CExpression.of("1 + (2 - 3)")
        self.assertEqual(self.styler.render(ast), "1 + (2 - 3)")

    def test_precedence_7(self):
        ast = CExpression.of("1 ? 2 : (3 ? 4 : 5)")
        self.assertEqual(self.styler.render(ast), "1 ? 2 : 3 ? 4 : 5")

    def test_precedence_8(self):
        ast = CExpression.of("(1 ? 2 : 3) ? 4 : 5")
        self.assertEqual(self.styler.render(ast), "(1 ? 2 : 3) ? 4 : 5")

    def test_wparentheses_precedence_1(self):
        ast = CExpression.of(
           """ s8 == (signed char)u8 || s8 < (signed char)u8 && u8 <= (unsigned char)u16 """)
        self.assertEqual(
            self.styler.render(ast),
            "s8 == (signed char)u8 || (s8 < (signed char)u8 && u8 <= (unsigned char)u16)")

    def test_wparentheses_precedence_2(self):
        ast = CExpression.of("3 & 4 | 5")
        self.assertEqual(self.styler.render(ast), "(3 & 4) | 5")

    def test_wparentheses_precedence_3(self):
        ast = CExpression.of("a + b & c")
        self.assertEqual(self.styler.render(ast), "(a + b) & c")

    def test_wparentheses_precedence_4(self):
        ast = CExpression.of("3 != 4 >= 5")
        self.assertEqual(self.styler.render(ast), "3 != (4 >= 5)")

    def test_wparentheses_precedence_5(self):
        ast = CExpression.of("a + b >> c")
        self.assertEqual(self.styler.render(ast), "(a + b) >> c")

    def test_wparentheses_precedence_6(self):
        ast = CExpression.of("a | b >= c")
        self.assertEqual(self.styler.render(ast), "a | b >= c")
        self.styler.config.parentheses_style.append('|', '>=')
        self.assertEqual(self.styler.render(ast), "a | (b >= c)")

    def test_wparentheses_precedence_7(self):
        ast = CExpression.of("a | b >= c & c != d ^ b <= c")
        self.assertEqual(self.styler.render(ast), "a | ((b >= c & c != d) ^ b <= c)")
        self.styler.config.parentheses_style.update(('&', '>='), ('&', '!='), ('^', '<='))
        self.assertEqual(self.styler.render(ast), "a | (((b >= c) & (c != d)) ^ (b <= c))")

    def test_char_value_1(self):
        ast = CExpression.of("'a'")
        self.assertEqual(self.styler.render(ast), "'a'")

    def test_unary_expression_1(self):
        """ A function to test render unary expression  """
        ast = CExpression.of("++i")
        self.assertEqual(self.styler.render(ast), "++i")

    def test_unary_expression_2(self):
        """ A function to test render unary expression   """
        ast = CExpression.of("i++")
        self.assertEqual(self.styler.render(ast), "i++")

    def test_unary_expression_3(self):
        """ A function to test render unary expression   """
        ast = CExpression.of("sizeof(char)")
        self.assertEqual(self.styler.render(ast), "sizeof(char)")


class TestConstant(TestCase):
    styler = CStyler()

    def test_integer_constant(self):
        ast = CIntegerConstant(3, CIntegerBase.DEC, {CIntegerConstant.Suffix.LONG})
        self.assertEqual(self.styler.render(ast), "3L")

    def test_integer_constant_1(self):
        ast = CIntegerConstant(-3, CIntegerBase.DEC, {CIntegerConstant.Suffix.LONG})
        self.assertEqual(self.styler.render(ast), "-3L")

    def test_integer_constant_2(self):
        ast = CIntegerConstant(-17, CIntegerBase.HEX, {CIntegerConstant.Suffix.LONG})
        self.assertEqual(self.styler.render(ast), "-0x11L")

    def test_integer_constant_3(self):
        ast = CIntegerConstant(47, CIntegerBase.HEX, {})
        self.assertEqual(self.styler.render(ast), "0x2f")

    def test_integer_constant_4(self):
        ast = CIntegerConstant(23, CIntegerBase.OCT, {})
        self.assertEqual(self.styler.render(ast), "027")

    def test_integer_constant_5(self):
        ast = CIntegerConstant(-39, CIntegerBase.OCT, {})
        self.assertEqual(self.styler.render(ast), "-047")

    def test_float_constant_1(self):
        ast = CFloatConstant('3.67', {})
        self.assertEqual(self.styler.render(ast), "3.67")

    def test_float_constant_2(self):
        ast = CFloatConstant('0.123446677', {CFloatConstant.Suffix.LONGDOUBLE})
        self.assertEqual(self.styler.render(ast), "0.123446677L")

    def test_float_constant_3(self):
        ast = CFloatConstant('-4.5', {CFloatConstant.Suffix.FLOAT})
        self.assertEqual(self.styler.render(ast), "-4.5F")


class TestType(TestCase):
    styler = CStyler()

    def test_primitive_1(self):
        ast = CIntType(CSignDescriptor.UNSIGNED)
        self.assertEqual(self.styler.render(ast), "unsigned int")

    def test_primitive_2(self):
        ast = CCharType(sign_desc=None)
        self.assertEqual(self.styler.render(ast), "char")

    def test_primitive_3(self):
        ast = CCharType(sign_desc=CSignDescriptor.SIGNED)
        self.assertEqual(self.styler.render(ast), "signed char")

    def test_array_1(self):
        ast = CDeclaration.of("int a[10];")[0]
        self.assertEqual(self.styler.render(ast), "int a[10];")

    def test_pointer_to_array(self):
        ast = CDeclaration.of("int (** a) [MAX];")[0]
        self.assertEqual(self.styler.render(ast), "int (**a)[MAX];")

    def test_pointer_to_array_1(self):
        ast = CDeclaration.of("int * (*a) [MAX];")[0]
        self.assertEqual(self.styler.render(ast), "int *(*a)[MAX];")

    def test_pointer_to_function(self):
        ast = CDeclaration.of("void * (* p_foo) (void) = & my_foo;")[0]
        self.assertEqual(self.styler.render(ast), "void* (*p_foo)(void) = &my_foo;")

    def test_pointer_to_function_decl(self):
        ast = CDeclaration.of("int (*** p_foo) (int *, float);")[0]
        self.assertEqual(self.styler.render(ast), "int (***p_foo)(int*, float);")

    def test_function_decl(self):
        ast = CDeclaration.of("int myfoo(int *a, float b);")[0]
        self.assertEqual(self.styler.render(ast), "int myfoo(int *a, float b);\n")

    def test_pointer_to_array_of_function(self):
        ast = CDeclaration.of("int **(**a)[10](void);")[0]
        self.assertEqual(self.styler.render(ast), "int** (**a)[10](void);")

    def test_array_of_pointers_to_function(self):
        ast = CDeclaration.of("int** (**a[MAX])(void);")[0]
        self.assertEqual(self.styler.render(ast), "int** (**a[MAX])(void);")

    def test_pointer(self):
        ast = CDeclaration.of("int ***a;")[0]
        self.assertEqual(self.styler.render(ast), "int ***a;")

    def test_function_pointer_decl(self):
        ast = CDeclaration.of("int (*** p_foo) (int, float);")[0]
        self.assertEqual(self.styler.render(ast), "int (***p_foo)(int, float);")

    def test_function_decl_1(self):
        ast = CDeclaration.of("int myfoo(int a, float b);")[0]
        self.assertEqual(self.styler.render(ast), "int myfoo(int a, float b);\n")

    def test_fielddeclaration_attr_1(self):
        ast = CFieldDeclaration("y", CType.of("int *"), 8)
        ast.attributes = CAttribute.of("__attribute__((packed))")
        self.assertEqual(self.styler.render(ast), "__attribute__((packed)) int *y: 8;\n")

    def test_parameterdeclaration_attr_1(self):
        ast1 = CParameterDeclaration.of("int __attribute__((unused)) argc")
        ast2 = CParameterDeclaration.of("char ** __attribute__((unused)) argv")
        self.assertEqual(self.styler.render(ast1), "int __attribute__((unused)) argc")
        self.assertEqual(self.styler.render(ast2), "__attribute__((unused)) char **argv")


class TestStatement(TestCase):
    """ class of test statement  """
    styler = CStyler()

    def test_for_statement_1(self):
        """ A function to test render CForStatement """
        ast = CForStatement(
            CExpression.of("i = 0"),
            CExpression.of("i < 10"),
            CExpression.of("++i"),
            CCompoundStatement([CStatement.of("a += i;")])
        )
        expected = "for(i = 0; i < 10; ++i)  {\n\ta += i;\n}"

        self.assertEqual(self.styler.render(ast), expected)

    def test_for_statement_2(self):
        """ A function to test render CForStatement  """
        ast = CForStatement(
            CVariableDeclaration.of("int i = 0;")[0],
            CExpression.of("i < 10"),
            CExpression.of("++i"),
            CCompoundStatement([CStatement.of("a += i;")])
        )
        expected = "for(int i = 0; i < 10; ++i)  {\n\ta += i;\n}"

        self.assertEqual(self.styler.render(ast), expected)
