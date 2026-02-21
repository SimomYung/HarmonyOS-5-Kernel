# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Unittest testcases of bi-directional ast parsing (declarations)
Author: Huawei OS Kernel Lab
Create: Tue 31 Dec 2020 16:30:00 AM CST
"""

from unittest import TestCase

from hmdsl.common.c_biast.biast import CArrayType, CCharType, CClassicalAttribute, \
    CConstructorExpression, CDeclaration, CDesignation, CFieldDeclaration, CFloatConstant, \
    CFloatType, CFunctionType, CIdentifierExpression, CIndexDesignator, CIntType, \
    CIntegerConstant, CLongType, CMemberDesignator, CParameterDeclaration, CPointerType, \
    CRangeDesignator, CStructType, CType, CTypeDeclaration, CTypeQualifier, CUnionType, \
    CVariableDeclaration


class TestParsingVariableDeclarations(TestCase):

    def test_variable_declaration_1(self):
        declarator_type = CIntType()
        declarator_type.add_qualifier(CTypeQualifier.CONST)
        self.assertEqual(
            CDeclaration.of("const int a;"),
            [CVariableDeclaration("a", declarator_type, None)]
        )

    def test_initialized_variable_declaration_1(self):
        self.assertEqual(
            CDeclaration.of("int a = 3;"),
            [CVariableDeclaration("a", CIntType(), initializer=CIntegerConstant(3))]
        )

    def test_initialized_variable_declaration_attribute_1(self):
        attributes = [CClassicalAttribute("deprecated", None),
                      CClassicalAttribute("aligned", [CIntegerConstant(8)])]
        var = CVariableDeclaration("a", CIntType(), initializer=CIntegerConstant(3))
        var.attributes = attributes
        self.assertEqual(
            CDeclaration.of("int a = 3 __attribute__((deprecated)) __attribute__((aligned(8)));"),
            [var]
        )

    def test_initialized_variable_declaration_attribute_2(self):
        attributes = [CClassicalAttribute("deprecated", None),
                      CClassicalAttribute("aligned", [CIntegerConstant(8)])]
        var = CVariableDeclaration("a", CIntType(), initializer=CIntegerConstant(3))
        var.attributes = attributes
        self.assertEqual(
            CDeclaration.of("int __attribute__((deprecated)) __attribute__((aligned(8))) a = 3;"),
            [var]
        )

    def test_function_variable_declaration_1(self):
        parameter_type = CFunctionType(parameter_decls=[CParameterDeclaration(None, CLongType())],
                                       return_type=CPointerType(CIntType()),
                                       variable_length_argument=False)
        function_type = CFunctionType(
            parameter_decls=[CParameterDeclaration(None, parameter_type)],
            return_type=CIntType(),
            variable_length_argument=False)
        self.assertEqual(CDeclaration.of("int f (int *(long));"),
                         [CVariableDeclaration("f", function_type, None)])

    def test_function_variable_declaration_2(self):
        parameter_type = CPointerType(CFunctionType(
            parameter_decls=[CParameterDeclaration(None, CLongType())],
            return_type=CIntType(),
            variable_length_argument=False))
        function_type = CFunctionType(
            parameter_decls=[CParameterDeclaration(None, parameter_type)],
            return_type=CIntType(),
            variable_length_argument=False)

        self.assertEqual(CDeclaration.of("int f (int (*)(long));"),
                         [CVariableDeclaration("f", function_type, None)])

    def test_array_variable_declaration_1(self):
        parameter_type = CArrayType(CPointerType(CIntType()), CIdentifierExpression("MAX"))
        function_type = CFunctionType(
            parameter_decls=[CParameterDeclaration(None, parameter_type)],
            return_type=CIntType(),
            variable_length_argument=False)
        self.assertEqual(CDeclaration.of("int f (int * [MAX]);"),
                         [CVariableDeclaration("f", function_type, None)])

    def test_array_variable_declaration_2(self):
        parameter_type = CPointerType(CArrayType(CIntType(), CIdentifierExpression("MAX")))
        function_type = CFunctionType(
            parameter_decls=[CParameterDeclaration(None, parameter_type)],
            return_type=CIntType(),
            variable_length_argument=False)
        self.assertEqual(CDeclaration.of("int f (int (*)[MAX]);"),
                         [CVariableDeclaration("f", function_type, None)])

    def test_multi_variable_declaration_1(self):
        self.assertEqual(
            CDeclaration.of("int a = 3, b, c = 2, *foo(void);"),
            [CVariableDeclaration("a", CIntType(), CIntegerConstant(3)),
             CVariableDeclaration("b", CIntType(), None),
             CVariableDeclaration("c", CIntType(), CIntegerConstant(2)),
             CVariableDeclaration("foo", CFunctionType(
                 parameter_decls=[],
                 return_type=CPointerType(CIntType()),
                 variable_length_argument=False),
                 initializer=None)]
        )

    def test_struct_variable_declaration_1(self):
        struct_type = CStructType("A", [CFieldDeclaration("a", CIntType()),
                                        CFieldDeclaration("b", CFloatType())])
        self.assertEqual(
            CDeclaration.of("struct A{int a; float b;} mystruct;"),
            [CVariableDeclaration("mystruct", struct_type, None)]
        )

    def test_initialized_variable_declaration_2(self):
        union_type = CUnionType("A", [CFieldDeclaration("a", CIntType()),
                                      CFieldDeclaration("b", CFloatType())])
        initializer = CConstructorExpression([
            (CDesignation([CMemberDesignator("a")]), CIntegerConstant(2)),
            (CDesignation([CMemberDesignator("b")]), CFloatConstant("3.2"))]
        )

        self.assertEqual(
            CDeclaration.of("union A{int a; float b;} myunion = {.a = 2, .b = 3.2,};"),
            [CVariableDeclaration("myunion", union_type, initializer=initializer)]
        )

    def test_array_declaration_1(self):
        array_type = CArrayType(CPointerType(CCharType()), CIntegerConstant(5))
        self.assertEqual(CDeclaration.of("char * A[5];"),
                         [CVariableDeclaration("A", array_type, None)])

    def test_array_declaration_2(self):
        array_type = CArrayType(CIntType(), None)
        initializer = CConstructorExpression([
            (CDesignation(), CIntegerConstant(2)),
            (CDesignation(), CIntegerConstant(3)),
            (CDesignation([CIndexDesignator(CIntegerConstant(2))]), CIntegerConstant(4))
        ])

        self.assertEqual(CDeclaration.of("int A[] = {2, 3, [2] = 4};"),
                         [CVariableDeclaration("A", array_type, initializer)])

    def test_array_declaration_3(self):
        array_type = CArrayType(CStructType(None, [
            CFieldDeclaration("a", CArrayType(CIntType(), CIntegerConstant(3))),
            CFieldDeclaration("b", CIntType())
        ]), None)

        initializer = CConstructorExpression.create(
            CConstructorExpression.create(CIntegerConstant(1)),
            CIntegerConstant(2)
        )

        self.assertEqual(CDeclaration.of("struct {int a[3], b;} w[] = {{1}, 2};"),
                         [CVariableDeclaration("w", array_type, initializer)])

    def test_range_array_declaration_1(self):
        widths_decl = CDeclaration.of("int widths[] = { [0 ... 9] = 1, [10 ... 99] = 2 };")[0]

        initializer = CConstructorExpression([
            (CDesignation([CRangeDesignator(CIntegerConstant(0), CIntegerConstant(9))]),
             CIntegerConstant(1)),
            (CDesignation([CRangeDesignator(CIntegerConstant(10), CIntegerConstant(99))]),
             CIntegerConstant(2))
        ])

        self.assertEqual(widths_decl.initializer, initializer)


class TestParsingTypeDeclarations(TestCase):

    def test_simple_typedef_1(self):
        self.assertEqual(CDeclaration.of("typedef int T;"),
                         [CTypeDeclaration(name='T', type=CIntType())])

    def test_multi_typedef_1(self):
        self.assertEqual(
            CDeclaration.of("typedef int T1, T2;"),
            [
                CTypeDeclaration(name='T1', type=CIntType()),
                CTypeDeclaration(name='T2', type=CIntType()),
            ]
        )

    def test_multi_typedef_2(self):
        self.assertEqual(
            CDeclaration.of("typedef int T1, *T2;"),
            [
                CTypeDeclaration(name='T1', type=CIntType()),
                CTypeDeclaration(name='T2', type=CPointerType(CIntType())),
            ]
        )


class TestParsingAnonymousVariableDeclarations(TestCase):

    def test_anonymous_variable_declaration_1(self):
        self.assertEqual(
            CDeclaration.of("union cc{int a;};"),
            [CVariableDeclaration(None, CType.of("union cc{int a;}"))]
        )

    def test_anonymous_type_declaration_1(self):
        with self.assertRaises(NotImplementedError):
            CDeclaration.of("typedef int;")

    def test_invalid_type_failure_1(self):
        with self.assertRaises(TypeError):
            CDeclaration.of("int float;")
