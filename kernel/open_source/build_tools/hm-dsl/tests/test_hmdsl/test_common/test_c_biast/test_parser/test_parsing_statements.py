# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
Description: Unittest testcases of bi-directional ast parsing (statements)
Author: Huawei OS Kernel Lab
Create: Tue 29 Dec 2020 8:30:00 AM CST
"""

from unittest import TestCase

from hmdsl.common.c_biast.biast import CAsmOperand, CAsmStatement, CAssignmentExpression, \
    CAssignmentOperator, CBreakStatement, CCompoundStatement, CContinueStatement, CDeclaration, \
    CDoWhileStatement, CExpression, CExpressionStatement, CForStatement, CGotoStatement, \
    CIdentifierExpression, CIfStatement, CIntegerConstant, CLabelledStatement, CNullStatement, \
    CReturnStatement, CStatement, CSwitchStatement, CWhileStatement


class TestParsingStatements(TestCase):

    def test_assign_nolhs_1(self):
        self.assertEqual(CStatement.of("1;"), CExpressionStatement(CIntegerConstant(1)))

    def test_assign_1(self):
        self.assertEqual(CStatement.of("a = 1;"),
                         CExpressionStatement(CAssignmentExpression(
                             CIdentifierExpression("a"),
                             CAssignmentOperator.ASSIGN,
                             CIntegerConstant(1)
                         )))

    def test_labelled_statement(self):
        self.assertEqual(CStatement.of("L1: a;"),
                         CLabelledStatement("L1",
                                            CExpressionStatement(CIdentifierExpression("a"))))

    def test_return_statement_1(self):
        self.assertEqual(CStatement.of("return 0;"),
                         CReturnStatement(CIntegerConstant(0)))

    def test_if_statement_1(self):
        stmt = CStatement.of("""
        if (a > 0) {
            return 1;
        }
        """)

        self.assertEqual(
            stmt,
            CIfStatement(CExpression.of("a > 0"), CStatement.of("{ return 1; }"))
        )

    def test_if_statement_2(self):
        stmt = CStatement.of("""
        if (a > 0) {
            return 1;
        } else {
            b += 1;
        }
        """)

        self.assertEqual(
            stmt,
            CIfStatement(CExpression.of("a > 0"), CStatement.of("{ return 1; }"),
                         CStatement.of("{ b += 1; }"))
        )

    def test_invalid_switch_1(self):
        with self.assertRaises(ValueError):
            CStatement.of("""
            switch (a + 1) {
            a *= 2;
            }
            """)

    def test_switch_statement_1(self):
        stmt = CStatement.of("""
        switch (a + 1) {
            case 1:
                return 0;
            case 2:
                return 1;
            default:
                return 2;
        }
        """)

        self.assertEqual(stmt, CSwitchStatement(
            CExpression.of("a + 1"),
            [
                (CExpression.of("1"), [CStatement.of("return 0;")]),
                (CExpression.of("2"), [CStatement.of("return 1;")]),
            ],
            [CStatement.of("return 2;")]
        ))

    def test_switch_statement_2(self):
        stmt = CStatement.of("""
        switch (a + 1) {
            case 1:
                return 0;
            case 2:
                return 1;
            default:
                3 + 2;
            return 2;
        }
        """)

        self.assertEqual(stmt, CSwitchStatement(
            CExpression.of("a + 1"),
            [
                (CExpression.of("1"), [CStatement.of("return 0;")]),
                (CExpression.of("2"), [CStatement.of("return 1;")]),
            ],
            [CStatement.of("3 + 2;"), CStatement.of("return 2;")]
        ))

    def test_switch_statement_3(self):
        stmt = CStatement.of("""
        switch (a + 1) {
            case 1:
                return 0;
            case 2:
            3 + 2;
            return 2;
        }
        """)

        self.assertEqual(stmt, CSwitchStatement(
            CExpression.of("a + 1"),
            [
                (CExpression.of("1"), [CStatement.of("return 0;")]),
                (CExpression.of("2"), [CStatement.of("3 + 2;"), CStatement.of("return 2;")]),
            ],
            None
        ))

    def test_empty_for_statement_1(self):
        stmt = CStatement.of("""
        for (;;);
        """)

        self.assertEqual(stmt, CForStatement(None, None, None, CNullStatement()))

    def test_for_statement_1(self):
        stmt = CStatement.of("""
        for (i = 0; i < 10; ++i)
            a += i;
        """)

        self.assertEqual(stmt, CForStatement(
            CExpression.of("i = 0"),
            CExpression.of("i < 10"),
            CExpression.of("++i"),
            CStatement.of("a += i;")
        ))

    def test_for_statement_decl_1(self):
        stmt = CStatement.of("""
        for (int i = 0, j = 0; i < 10 && j > 3; ++i, j--)
            a += i;
        """)

        self.assertEqual(stmt, CForStatement(
            CDeclaration.of("int i = 0, j = 0;"),
            CExpression.of("i < 10 && j > 3"),
            CExpression.of("++i, j--"),
            CStatement.of("a += i;")
        ))

    def test_for_statement_2(self):
        stmt = CStatement.of("""
        for (;;) {}
        """)

        self.assertEqual(stmt, CForStatement(
            None, None, None,
            CCompoundStatement([])
        ))

    def test_while_statement_1(self):
        stmt = CStatement.of("""
        while (1) {
            i += 1;
        }
        """)

        self.assertEqual(stmt, CWhileStatement(
            CIntegerConstant(1),
            CStatement.of("{ i += 1; }")
        ))

    def test_do_while_statement_1(self):
        stmt = CStatement.of("""
        do {
            i += 1;
        } while (i < 10);
        """)

        self.assertEqual(stmt, CDoWhileStatement(
            CStatement.of("{ i += 1; }"),
            CExpression.of("i < 10")
        ))

    def test_jump_statement_1(self):
        self.assertEqual(CStatement.of("continue;"),
                         CContinueStatement())

    def test_jump_statement_2(self):
        self.assertEqual(CStatement.of("break;"),
                         CBreakStatement())

    def test_jump_statement_3(self):
        self.assertEqual(CStatement.of("goto L1;"),
                         CGotoStatement("L1"))


class TestParsingAsmStatements(TestCase):
    def test_trivial_inst_1(self):
        stmt = CStatement.of('asm ("inst":::);')
        self.assertEqual(stmt, CAsmStatement(
            "inst", [], [], [], [], False, False
            ))

    def test_trivial_inst_2(self):
        stmt = CStatement.of('asm volatile ("inst;\\n");')
        self.assertEqual(stmt, CAsmStatement(
            "inst;\\n", [], [], [], [], True, False
            ))

    def test_trivial_inst_3(self):
        stmt = CStatement.of('asm ("push rax\\n\\t" "mov rax, 0\\n\\t" "pop rax");')
        self.assertEqual(stmt, CAsmStatement(
            "push rax\\n\\tmov rax, 0\\n\\tpop rax", [], [], [], [], False, False
            ))

    def test_qualifiers_1(self):
        stmt = CStatement.of('asm volatile ("inst":::);')
        self.assertEqual(stmt, CAsmStatement(
            "inst", [], [], [], [], True, False
            ))

    def test_qualifiers_2(self):
        stmt = CStatement.of('asm goto ("inst"::::error1, error2, error3);')
        self.assertEqual(stmt, CAsmStatement(
            "inst", [], [], [], ["error1", "error2", "error3"], False, False
            ))

    def test_qualifiers_3(self):
        stmt = CStatement.of('asm goto ("inst"::::);')
        self.assertEqual(stmt, CAsmStatement(
            "inst", [], [], [], [], False, False
            ))

    def test_qualifiers_4(self):
        stmt = CStatement.of('asm inline ("inst":::);')
        self.assertEqual(stmt, CAsmStatement(
            "inst", [], [], [], [], False, True
            ))

    def test_operands_1(self):
        stmt = CStatement.of('asm volatile ("dmb ish":"r"(a));')
        self.assertEqual(stmt.output_operands, [
            CAsmOperand(None, "r", CIdentifierExpression("a"))
        ])

    def test_operands_2(self):
        stmt = CStatement.of('asm volatile ("":[Value] "r"(a));')
        self.assertEqual(stmt.output_operands, [
            CAsmOperand("Value", "r", CIdentifierExpression("a"))
        ])

    def test_operands_3(self):
        stmt = CStatement.of('asm volatile ("dmb ish":"r"(a), "=t"(result):"0"(x), "u"(y));')
        self.assertEqual(stmt, CAsmStatement(
            "dmb ish",
            [CAsmOperand(None, "r", CIdentifierExpression("a")),
             CAsmOperand(None, "=t", CIdentifierExpression("result"))],
            [CAsmOperand(None, "0", CIdentifierExpression("x")),
             CAsmOperand(None, "u", CIdentifierExpression("y"))],
            [],
            [],
            True,
            False
        ))

    def test_operands_4(self):
        stmt = CStatement.of('asm volatile ("dmb ish":"r"(a):[Val]"w"(b):"some_clobbers");')
        self.assertEqual(stmt, CAsmStatement(
            "dmb ish",
            [CAsmOperand(None, "r", CIdentifierExpression("a"))],
            [CAsmOperand("Val", "w", CIdentifierExpression("b"))],
            ["some_clobbers"],
            [],
            True,
            False
        ))
