# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Unittest testcases of bi-directional ast parsing
Author: Huawei OS Kernel Lab
Create: Mon 28 Dec 2020 16:30:00 PM CST
"""

from unittest import TestCase
from itertools import permutations

from hmdsl.common.error import ParsingError
from hmdsl.common.c_biast.biast import CArrayType, CBoolType, CCharType, CDoubleType, CEnumItem, \
    CEnumType, CExpression, CFieldDeclaration, CFloatType, CFunctionType, CIntType, \
    CIntegerConstant, CLongDoubleType, CLongType, CLonglongType, CParameterDeclaration, \
    CPointerType, CShortType, CSignDescriptor, CStructType, CType, CTypeOf, CTypeQualifier, \
    CUnionType, CVoidType


class TestParsingPrimitiveTypes(TestCase):

    def test_empty(self):
        with self.assertRaises(ParsingError):
            CType.of("")

    def test_void(self):
        self.assertIsInstance(CType.of("void"), CVoidType)

    def test_bool(self):
        self.assertEqual(CType.of("_Bool"), CBoolType())

    def test_permutation_types(self):
        for tokens in permutations(["unsigned", "short", "int"]):
            self.assertEqual(CType.of(" ".join(tokens)),
                             CShortType(sign_desc=CSignDescriptor.UNSIGNED))

    def test_char(self):
        self.assertEqual(CType.of("char"), CCharType())
        self.assertEqual(CType.of("signed char"), CCharType(sign_desc=CSignDescriptor.SIGNED))
        self.assertEqual(CType.of("unsigned char"), CCharType(sign_desc=CSignDescriptor.UNSIGNED))

    def test_short(self):
        self.assertEqual(CType.of("short"), CShortType())
        self.assertEqual(CType.of("signed short"), CShortType(sign_desc=CSignDescriptor.SIGNED))
        self.assertEqual(CType.of("unsigned short"),
                         CShortType(sign_desc=CSignDescriptor.UNSIGNED))

        self.assertEqual(CType.of("short int"), CShortType())
        self.assertEqual(CType.of("signed short int"),
                         CShortType(sign_desc=CSignDescriptor.SIGNED))
        self.assertEqual(CType.of("unsigned short int"),
                         CShortType(sign_desc=CSignDescriptor.UNSIGNED))

    def test_int(self):
        self.assertEqual(CType.of("signed"), CIntType(sign_desc=CSignDescriptor.SIGNED))
        self.assertEqual(CType.of("unsigned"), CIntType(sign_desc=CSignDescriptor.UNSIGNED))

        self.assertEqual(CType.of("int"), CIntType())
        self.assertEqual(CType.of("signed int"), CIntType(sign_desc=CSignDescriptor.SIGNED))
        self.assertEqual(CType.of("unsigned int"), CIntType(sign_desc=CSignDescriptor.UNSIGNED))

        self.assertEqual(CType.of("long int"), CLongType())
        self.assertEqual(CType.of("unsigned long int"),
                         CLongType(sign_desc=CSignDescriptor.UNSIGNED))
        self.assertEqual(CType.of("signed long int"), CLongType(sign_desc=CSignDescriptor.SIGNED))

        self.assertEqual(CType.of("long long int"), CLonglongType())
        self.assertEqual(CType.of("unsigned long long int"),
                         CLonglongType(sign_desc=CSignDescriptor.UNSIGNED))
        self.assertEqual(CType.of("signed long long int"),
                         CLonglongType(sign_desc=CSignDescriptor.SIGNED))

    def test_floats(self):
        self.assertEqual(CType.of("float"), CFloatType())
        self.assertEqual(CType.of("double"), CDoubleType())
        self.assertEqual(CType.of("long double"), CLongDoubleType())


class TestParsingPointerType(TestCase):
    def test_pointer_void_type_1(self):
        self.assertEqual(CType.of("void *"), CPointerType(base_type=CVoidType()))

    def test_pointer_primitive_type_1(self):
        self.assertEqual(CType.of("int *"), CPointerType(base_type=CIntType()))

    def test_pointer_struct_type_1(self):
        base_type = CStructType(name="Point", field_decls=None)
        self.assertEqual(CType.of("struct Point *"), CPointerType(base_type=base_type))

    def test_pointer_union_type_1(self):
        base_type = CUnionType(name="myu", field_decls=None)
        self.assertEqual(CType.of("union myu *"), CPointerType(base_type=base_type))

    def test_pointer_of_pointer_1(self):
        base_type = CPointerType(base_type=CIntType())
        self.assertEqual(CType.of("int * *"), CPointerType(base_type=base_type))


class TestParsingStructType(TestCase):

    def test_blank(self):
        with self.assertRaises(ParsingError):
            CStructType.of("")

    def test_empty_struct(self):
        self.assertEqual(CStructType.of("struct {}"), CStructType(field_decls=[]))

    def test_named_struct_1(self):
        self.assertEqual(CStructType.of("struct mystruct"),
                         CStructType(name="mystruct", field_decls=None))

    def test_named_struct_2(self):
        self.assertEqual(CStructType.of("struct asdf {}"),
                         CStructType(name="asdf", field_decls=[]))

    def test_normal_struct_1(self):
        cstruct = CStructType.of("""
        struct Point {
            int x;
            int y;
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CIntType()),
                CFieldDeclaration("y", CIntType()),
            ]
        ))

    def test_field_multiple_declarator_1(self):
        cstruct = CStructType.of("""
        struct Point {
            int *x, y[];
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CPointerType(base_type=CIntType())),
                CFieldDeclaration("y", CArrayType(base_type=CIntType(), capacity=None)),
            ]
        ))

    def test_field_pointer_declarator_1(self):
        cstruct = CStructType.of("""
        struct Point {
            int *x;
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CPointerType(base_type=CIntType())),
            ]
        ))

    def test_field_pointer_declarator_2(self):
        cstruct = CStructType.of("""
        struct Point {
            int *const x;
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x",
                                  CPointerType(CIntType()).add_qualifier(CTypeQualifier.CONST)),
            ]
        ))

    def test_field_array_declarator_1(self):
        cstruct = CStructType.of("""
        struct Point {
            int x[10];
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(CIntType(), CIntegerConstant(10))),
            ]
        ))

    def test_field_array_declarator_2(self):
        cstruct = CStructType.of("""
        struct Point {
            int x[];
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(CIntType(), None)),
            ]
        ))

    def test_field_array_declarator_3(self):
        cstruct = CStructType.of("""
        struct Point {
            int x[10][2];
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(
                    CArrayType(CIntType(), CIntegerConstant(2)),
                    CIntegerConstant(10)
                )),
            ]
        ))

    def test_field_pointer_array_declarator_1(self):
        cstruct = CStructType.of("""
        struct Point {
            int *x[10][2];
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(
                    CArrayType(CPointerType(CIntType()), CIntegerConstant(2)),
                    CIntegerConstant(10)
                )),
            ]
        ))

    def test_field_pointer_function_pointer_1(self):
        cstruct = CStructType.of("""
        struct Point {
            int (*opr)(void);
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("opr", CPointerType(CFunctionType(
                    parameter_decls=[],
                    return_type=CIntType(),
                    variable_length_argument=False
                )))
            ]
        ))

    def test_field_pointer_function_pointer_2(self):
        cstruct = CStructType.of("""
        struct Point {
            int (*opr)(int, ...);
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="Point",
            field_decls=[
                CFieldDeclaration("opr", CPointerType(CFunctionType(
                    parameter_decls=[CParameterDeclaration(None, CIntType())],
                    return_type=CIntType(),
                    variable_length_argument=True
                )))
            ]
        ))

    def test_unnamed_bitfield_1(self):
        cstruct = CStructType.of("""
        struct tag {
            unsigned int :4;
            const signed int :5;
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="tag",
            field_decls=[CFieldDeclaration(
                None, CIntType(sign_desc=CSignDescriptor.UNSIGNED), CIntegerConstant(4)),
                         CFieldDeclaration(
                None, CIntType().add_qualifier(CTypeQualifier.CONST), CIntegerConstant(5)),
                         ]
        ))

    def test_bitfield_1(self):
        cstruct = CStructType.of("""
        struct Point {
            unsigned t: 4;
        }
        """)

        self.assertEqual(cstruct, CStructType('Point', [
            CFieldDeclaration('t', CType.of("unsigned"), CIntegerConstant(4))
        ]))

    def test_unnamed_field_1(self):
        cstruct = CStructType.of("""
        struct tag {
            unsigned int :4;
            struct {
                int a;
            };
        }
        """)

        self.assertEqual(cstruct, CStructType(
            name="tag",
            field_decls=[CFieldDeclaration(
                None, CIntType(sign_desc=CSignDescriptor.UNSIGNED), CIntegerConstant(4)),
                         CFieldDeclaration(
                None, CStructType(None, [CFieldDeclaration("a", CIntType())]), None),
                         ]))


class TestParsingUnionType(TestCase):

    def test_blank(self):
        with self.assertRaises(ParsingError):
            CUnionType.of("")

    def test_empty_union(self):
        self.assertEqual(CUnionType.of("union {}"), CUnionType(field_decls=[]))

    def test_named_union(self):
        self.assertEqual(CUnionType.of("union asdf {}"), CUnionType(name="asdf", field_decls=[]))

    def test_normal_union_1(self):
        cunion = CUnionType.of("""
        union Point {
            int x;
            int y;
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CIntType()),
                CFieldDeclaration("y", CIntType()),
            ]
        ))

    def test_field_multiple_declarator_1(self):
        cunion = CUnionType.of("""
        union Point {
            int *x, y[];
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CPointerType(base_type=CIntType())),
                CFieldDeclaration("y", CArrayType(base_type=CIntType(), capacity=None)),
            ]
        ))

    def test_field_pointer_declarator_1(self):
        cunion = CUnionType.of("""
        union Point {
            int *x;
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CPointerType(base_type=CIntType())),
            ]
        ))

    def test_field_pointer_declarator_2(self):
        cunion = CUnionType.of("""
        union Point {
            int *const x;
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[CFieldDeclaration(
                "x", CPointerType(CIntType()).add_qualifier(CTypeQualifier.CONST)),
                         ]))

    def test_field_array_declarator_1(self):
        cunion = CUnionType.of("""
        union Point {
            int x[10];
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(CIntType(), CIntegerConstant(10))),
            ]
        ))

    def test_field_array_declarator_2(self):
        cunion = CUnionType.of("""
        union Point {
            int x[];
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(CIntType(), None)),
            ]
        ))

    def test_field_array_declarator_3(self):
        cunion = CUnionType.of("""
        union Point {
            int x[10][2];
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(
                    CArrayType(CIntType(), CIntegerConstant(2)),
                    CIntegerConstant(10)
                )),
            ]
        ))

    def test_field_pointer_array_declarator_1(self):
        cunion = CUnionType.of("""
        union Point {
            int *x[10][2];
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("x", CArrayType(
                    CArrayType(CPointerType(CIntType()), CIntegerConstant(2)),
                    CIntegerConstant(10)
                )),
            ]
        ))

    def test_field_pointer_function_pointer_1(self):
        cunion = CUnionType.of("""
        union Point {
            int (*opr)(void);
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("opr", CPointerType(CFunctionType(
                    parameter_decls=[],
                    return_type=CIntType(),
                    variable_length_argument=False
                )))
            ]
        ))

    def test_field_pointer_function_pointer_2(self):
        cunion = CUnionType.of("""
        union Point {
            int (*opr)(int, ...);
        }
        """)

        self.assertEqual(cunion, CUnionType(
            name="Point",
            field_decls=[
                CFieldDeclaration("opr", CPointerType(CFunctionType(
                    parameter_decls=[CParameterDeclaration(None, CIntType())],
                    return_type=CIntType(),
                    variable_length_argument=True
                )))
            ]
        ))


class TestParsingEnumType(TestCase):

    def test_normal_enum(self):
        self.assertEqual(CEnumType.of("enum {CONTENT}"),
                         CEnumType(enum_items=[CEnumItem("CONTENT", None)]))

    def test_named_union(self):
        enum_items = [CEnumItem("CONTENT", None)]
        self.assertEqual(CEnumType.of("enum asdf {CONTENT}"),
                         CEnumType(name="asdf", enum_items=enum_items))

    def test_enum_items_1(self):
        enum_items = [CEnumItem("AAA", None),
                      CEnumItem("BBB", CIntegerConstant(1)),
                      CEnumItem("CCC", None),
                      CEnumItem("DDD", CIntegerConstant(5))]
        self.assertEqual(CEnumType.of("enum myenum {AAA, BBB=1, CCC, DDD=5,}"),
                         CEnumType(name="myenum", enum_items=enum_items))


class TestParsingTypeOf(TestCase):

    def test_typeof_expr_1(self):
        typ1 = CType.of("const typeof(1l)")
        typ2 = CTypeOf(CIntegerConstant(1, suffixes={CIntegerConstant.Suffix.LONG}))
        typ2.add_qualifier(CTypeQualifier.CONST)
        self.assertEqual(typ1, typ2)

    def test_typeof_expr_2(self):
        typ1 = CType.of("__typeof__(a + b)")
        typ2 = CTypeOf(CExpression.of("a + b"))
        self.assertEqual(typ1, typ2)

    def test_typeof_type_1(self):
        typ1 = CType.of("typeof(int *)")
        typ2 = CTypeOf(CType.of("int *"))
        self.assertEqual(typ1, typ2)
