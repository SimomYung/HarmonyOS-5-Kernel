# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Unittest testcases of bi-directional ast parsing (expressions)
Author: Huawei OS Kernel Lab
Create: Tue 29 Dec 2020 8:30:00 AM CST
"""

from unittest import TestCase

from hmdsl.common.error import ParsingError
from hmdsl.common.c_biast.biast import CBinaryExpression, CBinaryOperator, CCallExpression, \
    CCastExpression, CCharConstant, CCompoundExpression, CConditionalExpression, \
    CConstructorExpression, CElementExpression, CExpression, CFloatConstant, \
    CIdentifierExpression, CIntegerBase, CIntegerConstant, CMacroExpression, CMemberExpression, \
    CPostUnaryOperator, CSequenceExpression, CStatement, CStringLiteral, CType, CUnaryExpression, \
    CUnaryOperator


class TestParsingConstants(TestCase):

    def test_integer_base_0(self):
        self.assertEqual(CExpression.of("0").base,
                         CIntegerBase.DEC)

    def test_integer_constant_0(self):
        self.assertEqual(CExpression.of("0"), CIntegerConstant(0))

    def test_integer_constant_1(self):
        self.assertEqual(CExpression.of("1"), CIntegerConstant(1))
        self.assertEqual(CExpression.of("+10"), CUnaryExpression(
            CUnaryOperator.POSITIVE,
            CIntegerConstant(10)
        ))
        self.assertEqual(CExpression.of("-234"), CUnaryExpression(
            CUnaryOperator.NEGATE,
            CIntegerConstant(234)
        ))

    def test_bin_integer_constant_1(self):
        self.assertEqual(CExpression.of("0b1"), CIntegerConstant(1))

    def test_oct_integer_constant_1(self):
        self.assertEqual(CExpression.of("001"), CIntegerConstant(1))

    def test_hex_integer_constant_1(self):
        self.assertEqual(CExpression.of("0x1"), CIntegerConstant(1))

    def test_integer_suffix_1(self):
        self.assertEqual(CExpression.of("0x1uLL"), CIntegerConstant(1, suffixes={
            CIntegerConstant.Suffix.UNSIGNED,
            CIntegerConstant.Suffix.LONGLONG,
        }))

    def test_integer_suffix_2(self):
        self.assertEqual(CExpression.of("0x1uL"), CIntegerConstant(1, suffixes={
            CIntegerConstant.Suffix.UNSIGNED,
            CIntegerConstant.Suffix.LONG,
        }))

    def test_integer_constant_bool_1(self):
        self.assertFalse(CExpression.of("0"))

    def test_float_constant_1(self):
        self.assertEqual(CExpression.of("3.42"), CFloatConstant("3.42"))

    def test_float_constant_2(self):
        self.assertEqual(CExpression.of("3.42e2"), CFloatConstant("3.42e2"))

    def test_float_constant_3(self):
        self.assertEqual(CExpression.of("3.e2"), CFloatConstant("3.e2"))

    def test_float_constant_4(self):
        self.assertEqual(CExpression.of(".234e2"), CFloatConstant(".234e2"))

    def test_float_constant_suffix_1(self):
        self.assertEqual(CExpression.of("3.e2f"),
                         CFloatConstant("3.e2", {CFloatConstant.Suffix.FLOAT}))

    def test_float_constant_suffix_2(self):
        self.assertEqual(CExpression.of(".234e2l"),
                         CFloatConstant(".234e2", {CFloatConstant.Suffix.LONGDOUBLE}))

    def test_float_constant_bool_1(self):
        self.assertFalse(CExpression.of("0."))

    def test_char_constant_1(self):
        self.assertEqual(CExpression.of("\'s\'"), CCharConstant("s"))

    def test_char_constant_2(self):
        self.assertEqual(CExpression.of("\'\\n\'"), CCharConstant("\\n"))

    def test_char_constant_bool_1(self):
        with self.assertRaises(ParsingError):
            CExpression.of("\'\\0\'")


class TestParsingAssignmentExpressions(TestCase):

    def test_unary_assign_expression_1(self):
        self.assertEqual(CExpression.of("++i"),
                         CUnaryExpression(CUnaryOperator.SELFINC, CIdentifierExpression("i")))

    def test_unary_assign_expression_2(self):
        self.assertEqual(CExpression.of("--i"),
                         CUnaryExpression(CUnaryOperator.SELFDEC, CIdentifierExpression("i")))

    def test_compound_expression_1(self):
        compound = "{int _a = (a), _b = (b); _a > _b ? _a : _b; }"
        self.assertEqual(CExpression.of(f"({compound})"),
                         CCompoundExpression(CStatement.of(compound)))

    def test_sequence_expression_1(self):
        self.assertEqual(CExpression.of("i++, j++, k--"),
                         CSequenceExpression([
                             CUnaryExpression(
                                 CPostUnaryOperator.SELFINC, CIdentifierExpression("i")),
                             CUnaryExpression(
                                 CPostUnaryOperator.SELFINC, CIdentifierExpression("j")),
                             CUnaryExpression(
                                 CPostUnaryOperator.SELFDEC, CIdentifierExpression("k"))]))

    def test_conditional_expression_1(self):
        self.assertEqual(CExpression.of("j + 2 > i ? k++ : k * 2 - 1"),
                         CConditionalExpression(CExpression.of("j + 2 > i"),
                                                CExpression.of("k++"),
                                                CExpression.of("k * 2 - 1")))

    def test_primary_expression_string_literal_1(self):
        self.assertEqual(CExpression.of('\"hello world! %d %f \\n\"'),
                         CStringLiteral(r"hello world! %d %f \n"))

    def test_primary_expression_string_literal_2(self):
        self.assertEqual(CExpression.of('\"hello\"\" world!\"\" %d %f \\n\"'),
                         CStringLiteral(r"hello world! %d %f \n"))

    def test_cast_expresion_1(self):
        self.assertEqual(CExpression.of("(AA *) (& c)"),
                         CCastExpression(CType.of("AA *"), CExpression.of("& c")))

    def test_cast_expresion_2(self):
        self.assertEqual(CExpression.of("(uintptr_t)(&c)"),
                         CCastExpression(CType.of("uintptr_t"), CExpression.of("&c")))

    def test_compound_literal_1(self):
        self.assertEqual(CExpression.of("(struct {int a; float b;}) {.a = 0, .b = 3.14,}"),
                         CCastExpression(CType.of("struct {int a; float b;}"),
                                         CConstructorExpression.of("{.a = 0, .b = 3.14}")))

    def test_function_call_1(self):
        self.assertEqual(CExpression.of("foo(a, b, c)"),
                         CCallExpression(CIdentifierExpression("foo"), [
                             CIdentifierExpression("a"),
                             CIdentifierExpression("b"),
                             CIdentifierExpression("c")
                         ]))

    def test_array_reference_1(self):
        self.assertEqual(CExpression.of("arr[i + 3]"),
                         CElementExpression(
                             CIdentifierExpression("arr"),
                             CBinaryExpression(CIdentifierExpression("i"),
                                               CBinaryOperator.ADD,
                                               CIntegerConstant(3))
                         ))

    def test_pointer_object_member_1(self):
        self.assertEqual(CExpression.of("AAA->mem"),
                         CMemberExpression(
                             CUnaryExpression(CUnaryOperator.DEREFERENCE,
                                              CIdentifierExpression("AAA")),
                             "mem")
                         )

    def test_pointer_object_member_2(self):
        self.assertEqual(CExpression.of("AAA.mem"),
                         CMemberExpression(
                             CIdentifierExpression("AAA"),
                             "mem")
                         )

    def test_builtin_va_arg_1(self):
        expr = CExpression.of("v = __builtin_va_arg(ap, unsigned int)")
        self.assertEqual(expr.rhs,
                         CMacroExpression(
                             "__builtin_va_arg",
                             [CIdentifierExpression("ap"), CType.of("unsigned int")]
                         ))

    def test_builtin_offsetof_1(self):
        expr = CExpression.of("v = __builtin_offsetof(struct AAA, id)")
        self.assertEqual(expr.rhs,
                         CMacroExpression(
                             "__builtin_offsetof",
                             [CType.of("struct AAA"), CIdentifierExpression("id")]
                         ))
