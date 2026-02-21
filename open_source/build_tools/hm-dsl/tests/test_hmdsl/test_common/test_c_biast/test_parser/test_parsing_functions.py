# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Unittest testcases of bi-directional ast parsing (functions)
Author: Huawei OS Kernel Lab
Create: Tue 31 Dec 2020 17:50:00 AM CST
"""

from unittest import TestCase

from hmdsl.common.c_biast.biast import CClassicalAttribute, CCompoundStatement, CFunction, \
    CIntType, CIntegerConstant, CParameterDeclaration, CPointerType, CReturnStatement, \
    CStorageQualifier


class TestParsingFunctions(TestCase):

    def test_trivial_function_1(self):
        f = CFunction.of("""
        int f() {
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        self.assertEqual(f, CFunction('f', [], CIntType(), body, False, True))

    def test_trivial_function_2(self):
        f = CFunction.of("""
        int f(int a) {
            return 0;
        }
        """)

        parameters = [CParameterDeclaration("a", CIntType())]
        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        self.assertEqual(f, CFunction('f', parameters, CIntType(), body, False, False))

    def test_parameter_void_type_1(self):
        f = CFunction.of("""
        int f(void) {
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        parameters = []
        self.assertEqual(f, CFunction('f', parameters, CIntType(), body, False, False))

    def test_parameter_post_declaration_1(self):
        f = CFunction.of("""
        int f(a, b)
            int b;
            int a;
        {
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        parameters = [CParameterDeclaration('a', CIntType()),
                      CParameterDeclaration('b', CIntType())]

        self.assertEqual(f, CFunction('f', parameters, CIntType(), body, False, False))

    def test_parameter_post_declaration_failure_1(self):
        with self.assertRaises(KeyError):
            CFunction.of("""
            int f(a, b)
                int a;
            {
                return 0;
            }
            """)

    def test_parameter_post_declaration_failure_2(self):
        with self.assertRaises(KeyError):
            CFunction.of("""
            int f(a, b)
            {
                return 0;
            }
            """)

    def test_function_qualifier_1(self):
        f = CFunction.of("""
        static int f() {
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        self.assertEqual(f, CFunction('f', [], CIntType(),
                                      body, False, True, {CStorageQualifier.STATIC}))

    def test_function_specifier_1(self):
        f = CFunction.of("""
        inline int f(){
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        self.assertEqual(f, CFunction('f', [], CIntType(), body, True, True))

    def test_return_pointer_type_1(self):
        f = CFunction.of("""
        int * f(){
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        self.assertEqual(f, CFunction('f', [], CPointerType(base_type=CIntType()),
                                      body, False, True))

    def test_function_attribute_ignored_1(self):
        f = CFunction.of("""
        int __attribute__(()) * f(){
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        self.assertEqual(f, CFunction('f', [], CPointerType(base_type=CIntType()),
                                      body, False, True))

    def test_function_attribute_1(self):
        f = CFunction.of("""
        int __attribute__((always_inline)) * f(){
            return 0;
        }
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        func = CFunction('f', [], CPointerType(base_type=CIntType()), body,
                         inline=False, variable_length_argument=True)
        func.attributes = [CClassicalAttribute.always_inline()]
        self.assertEqual(f, func)

    def test_function_attribute_2(self):
        f = CFunction.of("""
        int * f(){
            return 0;
        } __attribute__((always_inline))
        """)

        body = CCompoundStatement([
            CReturnStatement(CIntegerConstant(0))
        ])
        func = CFunction('f', [], CPointerType(base_type=CIntType()), body, False, True)
        func.attributes = [CClassicalAttribute("always_inline", None)]
        self.assertEqual(f, func)

    def test_function_attribute_3(self):
        f = CFunction.of("""
        int @__used:attr f(){
            return 0;
        }
        """)

    def test_parameter_name_1(self):
        with self.assertRaises(AttributeError):
            CFunction.of("""
            int f(int, int)
            {
                return 0;
            }
            """)
