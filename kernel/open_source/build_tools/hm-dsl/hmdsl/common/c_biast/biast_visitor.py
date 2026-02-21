#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
Description: A biast visitor to construct C translation unit from parser
Author: Huawei OS Kernel Lab
Create: Wed Apr 20 16:30:26 2022
"""

from __future__ import absolute_import

from typing import List, Union, Dict, Set, Tuple, Optional
from itertools import chain

from hmdsl.common.c_biast.BiastGrammarParser import BiastGrammarParser
from hmdsl.common.c_biast.BiastGrammarVisitor import BiastGrammarVisitor
from hmdsl.common.c_biast.biast import CAbstractDeclarator, CAsmQualifier, CAliasType, \
    CAsmOperand, CAsmStatement, CAttribute, CArrayDecorator, CAssignmentExpression, \
    CAssignmentOperator, CBreakStatement, CBinaryExpression, CBinaryOperator, CCaseStatement, \
    CCompoundStatement, CConstructorExpression, CContinueStatement, CCallExpression, \
    CCastExpression, CCharConstant, CClassicalAttribute, CCompoundExpression, \
    CConditionalExpression, CDeclaration, CDefaultCaseStatement, CDesignation, CDoWhileStatement, \
    CDeclarator, CExpression, CExpressionStatement, CElementExpression, CEnumItem, CEnumType, \
    CFloatConstant, CForStatement, CFunction, CFunctionDecorator, CFunctionSpecifier, \
    CFieldDeclaration, CFieldDeclarator, CGotoStatement, CIfStatement, CIndexDesignator, \
    CInitDeclarator, CIntegerConstant, CIntegerBase, CIdentifierExpression, CLabelledStatement, \
    CMacroDeclaration, CMemberDesignator, CMacroStatement, CMacroParameterDeclaration, \
    CMacroAttribute, CMacroExpression, CMacroFieldDeclaration, CMacroType, CMemberExpression, \
    CNullStatement, CParameterDeclaration, CPointerDecorator, CPostUnaryOperator, \
    CRangeDesignator, CReturnStatement, CStorageQualifier, CStatementOrDeclaration, CStructType, \
    CSwitchStatement, CSequenceExpression, CStringLiteral, CType, CTranslationUnit, \
    CTypeDeclaration, CTypeOf, CTypeQualifier, CTypeSpecifier, CUnaryExpression, CUnaryOperator, \
    CUnionType, CVariableDeclaration, CVoidType, CWhileStatement


class BiastVisitor(BiastGrammarVisitor):
    """ A Biast Visitor """

    @staticmethod
    def visitFunction_specifier(ctx: BiastGrammarParser.Function_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#function_specifier."""
        return CFunctionSpecifier(ctx.getText())

    @staticmethod
    def visitMember_designator(ctx: BiastGrammarParser.Member_designatorContext):
        """ Visit a parse tree produced by BiastGrammarParser#member_designator."""
        return CMemberDesignator(ctx.CNAME().getText())

    @staticmethod
    def visitNull_statement(__ctx: BiastGrammarParser.Null_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#null_statement."""
        return CNullStatement()

    @staticmethod
    def visitGoto_statement(ctx: BiastGrammarParser.Goto_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#goto_statement."""
        return CGotoStatement(ctx.CNAME().getText())

    @staticmethod
    def visitContinue_statement(__ctx: BiastGrammarParser.Continue_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#continue_statement."""
        return CContinueStatement()

    @staticmethod
    def visitBreak_statement(__ctx: BiastGrammarParser.Break_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#break_statement."""
        return CBreakStatement()

    @staticmethod
    def visitAsm_volatile_qualifier(__ctx: BiastGrammarParser.Asm_volatile_qualifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_volatile_qualifier."""
        return CAsmQualifier.VOLATILE

    @staticmethod
    def visitAsm_inline_qualifier(__ctx: BiastGrammarParser.Asm_inline_qualifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_inline_qualifier."""
        return CAsmQualifier.INLINE

    @staticmethod
    def visitAsm_goto_qualifier(__ctx: BiastGrammarParser.Asm_goto_qualifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_goto_qualifier."""
        return CAsmQualifier.GOTO

    @staticmethod
    def visitAsm_gotolabels(ctx: BiastGrammarParser.Asm_gotolabelsContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_gotolabels."""
        return list(label.text for label in ctx.labels)

    @staticmethod
    def visitAssignment_operator(ctx: BiastGrammarParser.Assignment_operatorContext):
        """ Visit a parse tree produced by BiastGrammarParser#assignment_operator."""
        return CAssignmentOperator(ctx.getChild(0).getText())

    @staticmethod
    def visitMacro_expr(ctx: BiastGrammarParser.Macro_exprContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_expr."""
        return CMacroExpression(ctx.MACROEXPR().getText()[1: -5], [])

    @staticmethod
    def visitIdentifier_expression(ctx: BiastGrammarParser.Identifier_expressionContext):
        """ # Visit a parse tree produced by BiastGrammarParser#identifier_expression."""
        return CIdentifierExpression(ctx.CNAME().getText())

    @staticmethod
    def visitChar_constant(ctx: BiastGrammarParser.Char_constantContext):
        """ Visit a parse tree produced by BiastGrammarParser#char_constant."""
        return CCharConstant(ctx.CHAR_LITERAL().getText()[1: -1])

    @staticmethod
    def visitStorage_class_specifier(ctx: BiastGrammarParser.Storage_class_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#storage_class_specifier."""
        return CStorageQualifier(ctx.getText())

    @staticmethod
    def visitType_qualifier(ctx: BiastGrammarParser.Type_qualifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#type_qualifier."""
        return CTypeQualifier(ctx.getText())

    @staticmethod
    def visitSingle_lex_spec(ctx: BiastGrammarParser.Single_lex_specContext):
        """ Visit a parse tree produced by BiastGrammarParser#single_lex_spec."""
        return CTypeSpecifier(ctx.getText())

    @staticmethod
    def visitType_alias_specifier(ctx: BiastGrammarParser.Type_alias_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#type_alias_specifier."""
        return CAliasType(ctx.CNAME().getText())

    @staticmethod
    def visitEmpty_field(__ctx: BiastGrammarParser.Empty_fieldContext):
        """ Visit a parse tree produced by BiastGrammarParser#empty_field."""
        return None

    @staticmethod
    def visitNamed_declarator(ctx: BiastGrammarParser.Named_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#named_declarator."""
        return CDeclarator(ctx.CNAME().getText())

    @staticmethod
    def visitEmpty_parameter_declarations(
            __ctx: BiastGrammarParser.Empty_parameter_declarationsContext):
        """ Visit a parse tree produced by BiastGrammarParser#empty_parameter_declarations."""
        return [], True

    @staticmethod
    def visitNamed_enum_item(ctx: BiastGrammarParser.Named_enum_itemContext):
        """ Visit a parse tree produced by BiastGrammarParser#named_enum_item."""
        return CEnumItem(name=ctx.CNAME().getText(), value=None)

    @staticmethod
    def visitString_literal(ctx: BiastGrammarParser.String_literalContext):
        """ Visit a parse tree produced by BiastGrammarParser#string_literal."""
        content = ""
        for str_l in ctx.sls:
            content += str_l.text[1:-1]
        return content

    @staticmethod
    def visitEmpty_attr(__ctx: BiastGrammarParser.Empty_attrContext):
        """ Visit a parse tree produced by BiastGrammarParser#empty_attr."""
        return None

    @staticmethod
    def visitAsm_keyword(__ctx: BiastGrammarParser.Asm_keywordContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_keyword."""
        return None

    @staticmethod
    def __pass_as_flatlist(args):
        ret = []
        for arg in args:
            if isinstance(arg, list):
                ret += arg
            else:
                ret.append(arg)
        return ret

    @staticmethod
    def __get_attributes(*args) -> List[CAttribute]:
        attributes: List[CAttribute] = []
        for arg in args:
            attributes += list(attr for attr in arg if isinstance(attr, CAttribute))
        return attributes

    @staticmethod
    def __merge_parameters(
        parameters: List[Union[CParameterDeclaration, CMacroParameterDeclaration]],
        supplements: Dict[str, CVariableDeclaration]) -> \
            List[CParameterDeclaration]:
        for param in parameters:
            if isinstance(param, CMacroParameterDeclaration) and supplements:
                raise AttributeError(f"cannot match macro parameter to parameter declarations")
            if not isinstance(param, CMacroParameterDeclaration) and param.name is not None:
                continue
            if not isinstance(param, CMacroParameterDeclaration) and \
                    isinstance(param.type, CAliasType):
                # In a function definition, a parameter must has its name. Hence here
                # the type alias should be treated as its name.
                param.name = param.type.name
                # search for the real type
                if param.name in supplements:
                    param.type = supplements[param.name].type
                    del supplements[param.name]
                else:
                    raise KeyError(f"parameter type of {param.name} not found")
            elif not isinstance(param, CMacroParameterDeclaration):
                raise AttributeError(f"parameter name omited")
            else:
                continue
        if len(supplements) != 0:
            raise ValueError(f"parameter names of {supplements} are missing")
        return parameters

    @staticmethod
    def __get_storage_qualifiers(items) -> Set[CStorageQualifier]:
        """ A transform handler to deal with corresponding grammar rule"""
        ret = set()
        for item in items:
            if isinstance(item, CStorageQualifier):
                ret.add(item)
        return ret

    @staticmethod
    def __is_typename(name: str) -> bool:
        if name.startswith('uint') and name.endswith('_t'):
            return True
        if name.startswith('int') and name.endswith('_t'):
            return True

        return False

    @staticmethod
    def __extract_float(content: str):
        """ A transform handler to deal with corresponding grammar rule"""
        content = content.lower()
        suffixes = set()

        if content.endswith('f'):
            content = content[:-1]
            suffixes.add(CFloatConstant.Suffix.FLOAT)
        elif content.endswith('l'):
            content = content[:-1]
            suffixes.add(CFloatConstant.Suffix.LONGDOUBLE)

        return CFloatConstant(str(content), suffixes)

    @staticmethod
    def __extract_integer(content: str, base: CIntegerBase):
        """ A transform handler to deal with corresponding grammar rule"""
        content = content.lower()
        suffixes = set()

        while content[-1] in ('u', 'l'):
            if content.endswith('u'):
                content = content[:-1]
                suffixes.add(CIntegerConstant.Suffix.UNSIGNED)
            elif content.endswith('ll'):
                content = content[:-2]
                suffixes.add(CIntegerConstant.Suffix.LONGLONG)
            elif content.endswith('l'):
                content = content[:-1]
                suffixes.add(CIntegerConstant.Suffix.LONG)

        if content == '0':
            # 0 is parsed as an oct integer constant
            base = CIntegerBase.DEC

        return CIntegerConstant(int(content, base.value), base, suffixes)

    def visitTranslation_unit(self, ctx: BiastGrammarParser.Translation_unitContext):
        """ Visit a parse tree produced by BiastGrammarParser#translation_unit."""
        ret: List[Union[CFunction, CDeclaration]] = []
        for decl in ctx.decls:
            unit = self.visit(decl)
            if isinstance(unit, CFunction):
                ret.append(unit)
            elif isinstance(unit, List):
                ret += unit
        return CTranslationUnit(ret)

    def visitExternal_declaration(self, ctx: BiastGrammarParser.External_declarationContext):
        """ Visit a parse tree produced by BiastGrammarParser#external_declaration."""
        return self.visitChildren(ctx)

    def visitFunction_definition(self, ctx: BiastGrammarParser.Function_definitionContext):
        """ Visit a parse tree produced by BiastGrammarParser#function_definition."""
        rettype_specifiers = self.visit(ctx.declaration_specifiers())
        declarator = self.visit(ctx.declarator())
        supplement_decls = self.visit(ctx.declarations()) if ctx.declarations() is not None else []
        body = self.visit(ctx.compound_statement())
        attributes = []
        if ctx.attribute_specifiers() is not None:
            attributes = self.visit(ctx.attribute_specifiers())

        if not isinstance(declarator.decorator, CFunctionDecorator):
            raise TypeError(f"{declarator} does not declare any function")

        func_type = declarator.get_type(rettype_specifiers)
        return_type = func_type.return_type.add_qualifiers(func_type.qualifiers)
        orig_parameters = declarator.decorator.parameters
        supplements: Dict[str, CVariableDeclaration] = {}
        if supplement_decls is not None:
            supplements = {decl.name: decl for decl in chain(*supplement_decls)}

        parameters = self.__merge_parameters(orig_parameters, supplements)
        variable_length_argument = declarator.decorator.variable_length_argument

        ret = CFunction(
            name=declarator.name,
            parameters=parameters,
            variable_length_argument=variable_length_argument,
            return_type=return_type,
            body=body if body.body != [] else None,
            inline=CFunctionSpecifier.INLINE in rettype_specifiers,
            storage_qualifiers=self.__get_storage_qualifiers(rettype_specifiers)
        )
        ret.attributes = self.__get_attributes(
            rettype_specifiers, declarator.attributes, attributes or []
        )

        return ret

    def visitC_declaration(self, ctx: BiastGrammarParser.C_declarationContext):
        """ Visit a parse tree produced by BiastGrammarParser#c_declaration."""
        specifiers = self.visit(ctx.declaration_specifiers())
        declarators_opt = ctx.init_declarators()

        if declarators_opt is None and CStorageQualifier.TYPEDEF in specifiers:
            raise NotImplementedError(f"Anonymous type alias is not supported currently.")

        if declarators_opt is None:
            # According to Section 6.7 in C99, a declaration declares no declarators
            # iff. it is a function parameter, a struct, an union, etc.
            declarator = CAbstractDeclarator()
            decl = CVariableDeclaration(
                name=None,
                type=declarator.get_type(specifiers),
                storage_qualifiers=self.__get_storage_qualifiers(specifiers)
            )
            decl.attributes = self.__get_attributes(specifiers, declarator.attributes)
            return [decl]

        if CStorageQualifier.TYPEDEF in specifiers:
            specifiers.remove(CStorageQualifier.TYPEDEF)
            type_decls: List[CDeclaration] = []
            for type_declarator in self.visit(declarators_opt):
                type_decl = CTypeDeclaration(
                    name=type_declarator.name,
                    type=type_declarator.get_type(specifiers)
                )
                type_decl.attributes = self.__get_attributes(specifiers, type_declarator.attributes)
                type_decls.append(type_decl)
            return type_decls

        var_decls: List[CDeclaration] = []
        for var_declarator in self.visit(declarators_opt):
            var_decl = CVariableDeclaration(
                name=var_declarator.name,
                type=var_declarator.get_type(specifiers),
                initializer=var_declarator.initializer,
                storage_qualifiers=self.__get_storage_qualifiers(specifiers)
            )
            var_decl.attributes = self.__get_attributes(specifiers, var_declarator.attributes)
            var_decls.append(var_decl)
        return var_decls

    def visitMacro_declaration(self, ctx: BiastGrammarParser.Macro_declarationContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_declaration."""
        macro_name = ctx.MACRODECL().getText()[1:-5]
        arguments_opt = None
        if ctx.builtin_parameters() is not None:
            arguments_opt = self.visit(ctx.builtin_parameters())
        return [CMacroDeclaration(
            name=None,
            type=CType(),
            macro_name=macro_name,
            arguments=arguments_opt or [])]

    def visitDeclarations(self, ctx: BiastGrammarParser.DeclarationsContext):
        """ Visit a parse tree produced by BiastGrammarParser#declarations."""
        return list(self.visit(decl) for decl in ctx.decls)

    def visitNontype_declaration_specifier(
            self, ctx: BiastGrammarParser.Nontype_declaration_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#nontype_declaration_specifier."""
        return self.visitChildren(ctx)

    def visitNontypename_declaration_specifier(
            self, ctx: BiastGrammarParser.Nontypename_declaration_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#nontypename_declaration_specifier. """
        return self.visitChildren(ctx)

    def visitNontype_declaration_specifiers(
            self, ctx: BiastGrammarParser.Nontype_declaration_specifiersContext):
        """ Visit a parse tree produced by BiastGrammarParser#nontype_declaration_specifiers."""
        return self.__pass_as_flatlist(list(self.visit(elt) for elt in ctx.spcs))

    def visitNontypename_declaration_specifiers(
            self, ctx: BiastGrammarParser.Nontypename_declaration_specifiersContext):
        """ Visit a parse tree produced by BiastGrammarParser#nontypename_declaration_specifiers."""
        return self.__pass_as_flatlist(list(self.visit(elt) for elt in ctx.spcs))

    def visitDeclaration_specifiers_alias(
            self, ctx: BiastGrammarParser.Declaration_specifiers_aliasContext):
        """ Visit a parse tree produced by BiastGrammarParser#declaration_specifiers_alias."""
        ret = [self.visit(ctx.type_alias_specifier())]
        if ctx.nontype_declaration_specifiers() is None:
            return ret

        return ret + self.visit(ctx.nontype_declaration_specifiers())

    def visitDeclaration_specifiers_nonname(
            self, ctx: BiastGrammarParser.Declaration_specifiers_nonnameContext):
        """ Visit a parse tree produced by BiastGrammarParser#declaration_specifiers_nonname."""
        ret = [self.visit(ctx.direct_type_specifier())]
        if ctx.nontypename_declaration_specifiers() is None:
            return ret

        return ret + self.visit(ctx.nontypename_declaration_specifiers())

    def visitDeclaration_specifiers_nontype(
            self, ctx: BiastGrammarParser.Declaration_specifiers_nontypeContext):
        """ Visit a parse tree produced by BiastGrammarParser#declaration_specifiers_nontype."""
        ret = self.visit(ctx.nontype_declaration_specifier())
        if not isinstance(ret, list):
            ret = [ret]
        if ctx.declaration_specifiers() is None:
            return ret

        return ret + self.visit(ctx.declaration_specifiers())

    def visitInit_declarators(self, ctx: BiastGrammarParser.Init_declaratorsContext):
        """ Visit a parse tree produced by BiastGrammarParser#init_declarators."""
        return list(self.visit(decl) for decl in ctx.decltor)

    def visitInit_declarator(self, ctx: BiastGrammarParser.Init_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#init_declarator."""
        decltor = self.visit(ctx.declarator())
        ret = CInitDeclarator(decltor.name, decltor.decorator)
        ret.attributes = decltor.attributes
        init_opt = ctx.initializer()
        attr_opt = ctx.attribute_specifiers()
        if init_opt is not None:
            ret.initializer = self.visit(init_opt)
            if attr_opt is not None:
                ret.attributes = self.__get_attributes(decltor.attributes, self.visit(attr_opt))
        return ret

    def visitExpr_initializer(self, ctx: BiastGrammarParser.Expr_initializerContext):
        """ Visit a parse tree produced by BiastGrammarParser#expr_initializer."""
        return self.visitChildren(ctx)

    def visitBracket_initializer(self, ctx: BiastGrammarParser.Bracket_initializerContext):
        """ Visit a parse tree produced by BiastGrammarParser#bracket_initializer."""
        if ctx.designated_initializers() is not None:
            return self.visit(ctx.designated_initializers())

        return CConstructorExpression([])

    def visitDesignated_initializers(self, ctx: BiastGrammarParser.Designated_initializersContext):
        """ Visit a parse tree produced by BiastGrammarParser#designated_initializers."""
        return CConstructorExpression(list(self.visit(init) for init in ctx.inits))

    def visitDesignated_initializer(self, ctx: BiastGrammarParser.Designated_initializerContext):
        """ Visit a parse tree produced by BiastGrammarParser#designated_initializer."""
        expr = self.visit(ctx.initializer())
        if ctx.designation() is not None:
            return (self.visit(ctx.designation()), expr)

        return (CDesignation(), expr)

    def visitDesignation(self, ctx: BiastGrammarParser.DesignationContext):
        """ Visit a parse tree produced by BiastGrammarParser#designation."""
        return CDesignation(list(self.visit(dsnr) for dsnr in ctx.dsnrs))

    def visitRange_designator(self, ctx: BiastGrammarParser.Range_designatorContext):
        """ Visit a parse tree produced by BiastGrammarParser#range_designator."""
        index = self.visit(ctx.constant_expression(0))
        end_index_opt = ctx.constant_expression(1)
        if end_index_opt is None:
            return CIndexDesignator(index)

        return CRangeDesignator(index, self.visit(end_index_opt))

    def visitStatement(self, ctx: BiastGrammarParser.StatementContext):
        """ Visit a parse tree produced by BiastGrammarParser#statement."""
        return self.visitChildren(ctx)

    def visitExpression_statement(self, ctx: BiastGrammarParser.Expression_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#expression_statement."""
        return CExpressionStatement(self.visit(ctx.expression()))

    def visitMacro_statement(self, ctx: BiastGrammarParser.Macro_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_statement."""
        name = ctx.MACROSTMT().getText()[1: -5]
        args_opt = ctx.macro_parameters()
        if args_opt is not None:
            return CMacroStatement(name, self.visit(args_opt))

        return CMacroStatement(name, [])

    def visitLabelled_statement(self, ctx: BiastGrammarParser.Labelled_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#labelled_statement."""
        return CLabelledStatement(ctx.CNAME().getText(), self.visit(ctx.statement()))

    def visitSwitch_body(self, ctx: BiastGrammarParser.Switch_bodyContext):
        """ Visit a parse tree produced by BiastGrammarParser#switch_body."""
        ret = []
        count = ctx.getChildCount()
        for i in range(count)[1: -1]:
            ret.append(self.visit(ctx.getChild(i)))
        return ret

    def visitNon_default_case_statement(
            self, ctx: BiastGrammarParser.Non_default_case_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#non_default_case_statement."""
        return CCaseStatement(self.visit(ctx.constant_expression()), self.visit(ctx.statement()))

    def visitDefault_case_statement(self, ctx: BiastGrammarParser.Default_case_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#default_case_statement."""
        return CDefaultCaseStatement(self.visit(ctx.statement()))

    def visitCompound_statement(self, ctx: BiastGrammarParser.Compound_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#compound_statement."""
        compound = CCompoundStatement([])
        count = ctx.getChildCount()
        for i in range(count)[1:-1]:
            stmt = self.visit(ctx.getChild(i))
            if isinstance(stmt, list):
                compound.body += stmt
            else:
                compound.body.append(stmt)
        return compound

    def visitIf_statement(self, ctx: BiastGrammarParser.If_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#if_statement."""
        return CIfStatement(self.visit(ctx.condition()), self.visit(ctx.statement()), None)

    def visitIf_statement_else(self, ctx: BiastGrammarParser.If_statement_elseContext):
        """ Visit a parse tree produced by BiastGrammarParser#if_statement_else."""
        return CIfStatement(self.visit(ctx.condition()),
                            self.visit(ctx.statement(0)), self.visit(ctx.statement(1)))

    def visitSwitch_statement(self, ctx: BiastGrammarParser.Switch_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#switch_statement."""
        controller = self.visit(ctx.condition())
        body = self.visit(ctx.switch_body())
        labelled_statements: List[Tuple[Optional[CExpression], List[CStatementOrDeclaration]]] = []
        for item in body:
            if isinstance(item, CCaseStatement):
                labelled_statements.append((item.value, [item.stmt]))
            elif isinstance(item, CDefaultCaseStatement):
                labelled_statements.append((None, [item.stmt]))
            elif len(labelled_statements) == 0:
                raise ValueError(f"statement {item} will never be executed")
            else:
                labelled_statements[-1][1].append(item)

        cases = []
        default = None
        for (val, stmts) in labelled_statements:
            if val is not None:
                cases.append((val, stmts))
            else:
                default = stmts

        return CSwitchStatement(controller, cases, default)

    def visitWhile_statement(self, ctx: BiastGrammarParser.While_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#while_statement."""
        return CWhileStatement(self.visit(ctx.condition()), self.visit(ctx.statement()))

    def visitDo_while_statement(self, ctx: BiastGrammarParser.Do_while_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#do_while_statement."""
        return CDoWhileStatement(self.visit(ctx.statement()), self.visit(ctx.condition()))

    def visitFor_statement(self, ctx: BiastGrammarParser.For_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#for_statement."""
        declaration = self.visit(ctx.decl) if ctx.decl is not None else None
        condition = self.visit(ctx.cond) if ctx.cond is not None else None
        incrementor = self.visit(ctx.incr) if ctx.incr is not None else None
        body = self.visit(ctx.statement())
        return CForStatement(declaration, condition, incrementor, body)

    def visitFor_statement_decl(self, ctx: BiastGrammarParser.For_statement_declContext):
        """ Visit a parse tree produced by BiastGrammarParser#for_statement_decl."""
        declaration = self.visit(ctx.declaration())
        condition = self.visit(ctx.cond) if ctx.cond is not None else None
        incrementor = self.visit(ctx.incr) if ctx.incr is not None else None
        body = self.visit(ctx.statement())
        return CForStatement(declaration, condition, incrementor, body)

    def visitReturn_statement(self, ctx: BiastGrammarParser.Return_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#return_statement."""
        value = self.visit(ctx.expression()) if ctx.expression() is not None else None
        return CReturnStatement(value)

    def visitAsm_statement(self, ctx: BiastGrammarParser.Asm_statementContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_statement."""
        qualifiers = self.visit(ctx.asm_qualifiers())
        template = self.visit(ctx.string_literal())
        output_operands: List[CAsmOperand] = self.visit(ctx.op1) if ctx.op1 is not None else []
        input_operands: List[CAsmOperand] = self.visit(ctx.op2) if ctx.op2 is not None else []
        clobbers = self.visit(ctx.asm_clobbers()) if ctx.asm_clobbers() is not None else []
        gotolabels = self.visit(ctx.asm_gotolabels()) if ctx.asm_gotolabels() is not None else []

        return CAsmStatement(
            template,
            output_operands,
            input_operands,
            clobbers,
            gotolabels,
            CAsmQualifier.VOLATILE in qualifiers,
            CAsmQualifier.INLINE in qualifiers,
        )

    def visitAsm_qualifiers(self, ctx: BiastGrammarParser.Asm_qualifiersContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_qualifiers."""
        return list(self.visit(asm) for asm in ctx.asms)

    def visitAsm_operands(self, ctx: BiastGrammarParser.Asm_operandsContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_operands."""
        return list(self.visit(op) for op in ctx.ops)

    def visitAsm_operand(self, ctx: BiastGrammarParser.Asm_operandContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_operand."""
        name = ctx.CNAME().getText() if ctx.CNAME() is not None else None
        constraint = self.visit(ctx.string_literal())
        cexpr = self.visit(ctx.expression())
        return CAsmOperand(name, constraint, cexpr)

    def visitAsm_clobbers(self, ctx: BiastGrammarParser.Asm_clobbersContext):
        """ Visit a parse tree produced by BiastGrammarParser#asm_clobbers."""
        return list(self.visit(c) for c in ctx.clobber)

    def visitCondition(self, ctx: BiastGrammarParser.ConditionContext):
        """ Visit a parse tree produced by BiastGrammarParser#condition."""
        return self.visit(ctx.expression())

    def visitConstant_expression(self, ctx: BiastGrammarParser.Constant_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#constant_expression."""
        return self.visitChildren(ctx)

    def visitSequence_expression(self, ctx: BiastGrammarParser.Sequence_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#sequence_expression."""
        head = self.visit(ctx.expression())
        tail = self.visit(ctx.assignment_expression())
        if isinstance(head, CSequenceExpression):
            head.sequence.append(tail)
            return head
        return CSequenceExpression([head, tail])

    def visitRegular_expression(self, ctx: BiastGrammarParser.Regular_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_expression."""
        return self.visitChildren(ctx)

    def visitRegular_assign(self, ctx: BiastGrammarParser.Regular_assignContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_assign."""
        return self.visitChildren(ctx)

    def visitFull_assign(self, ctx: BiastGrammarParser.Full_assignContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_assign."""
        return CAssignmentExpression(self.visit(ctx.unary_expression()),
                                     self.visit(ctx.assignment_operator()),
                                     self.visit(ctx.assignment_expression()))

    def visitRegular_condition(self, ctx: BiastGrammarParser.Regular_conditionContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_condition."""
        return self.visitChildren(ctx)

    def visitTernary_expression(self, ctx: BiastGrammarParser.Ternary_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#ternary_expression."""
        return CConditionalExpression(self.visit(ctx.logical_or_expression()),
                                      self.visit(ctx.expression()),
                                      self.visit(ctx.conditional_expression()))

    def visitRegular_lor(self, ctx: BiastGrammarParser.Regular_lorContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_lor."""
        return self.visitChildren(ctx)

    def visitFull_lor(self, ctx: BiastGrammarParser.Full_lorContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_lor."""
        return CBinaryExpression(self.visit(ctx.logical_or_expression()),
                                 CBinaryOperator(ctx.LOR().getText()),
                                 self.visit(ctx.logical_and_expression()))

    def visitRegular_land(self, ctx: BiastGrammarParser.Regular_landContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_land."""
        return self.visitChildren(ctx)

    def visitFull_land(self, ctx: BiastGrammarParser.Full_landContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_land."""
        return CBinaryExpression(self.visit(ctx.logical_and_expression()),
                                 CBinaryOperator(ctx.LAND().getText()),
                                 self.visit(ctx.or_expression()))

    def visitRegular_bor(self, ctx: BiastGrammarParser.Regular_borContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_bor."""
        return self.visitChildren(ctx)

    def visitFull_bor(self, ctx: BiastGrammarParser.Full_borContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_bor."""
        return CBinaryExpression(self.visit(ctx.or_expression()),
                                 CBinaryOperator(ctx.BOR().getText()),
                                 self.visit(ctx.xor_expression()))

    def visitFull_xor(self, ctx: BiastGrammarParser.Full_xorContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_xor."""
        return CBinaryExpression(self.visit(ctx.xor_expression()),
                                 CBinaryOperator(ctx.XOR().getText()),
                                 self.visit(ctx.and_expression()))

    def visitRegular_xor(self, ctx: BiastGrammarParser.Regular_xorContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_xor."""
        return self.visitChildren(ctx)

    def visitFull_band(self, ctx: BiastGrammarParser.Full_bandContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_band."""
        return CBinaryExpression(self.visit(ctx.and_expression()),
                                 CBinaryOperator(ctx.BAND().getText()),
                                 self.visit(ctx.equality_expression()))

    def visitRegular_band(self, ctx: BiastGrammarParser.Regular_bandContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_band."""
        return self.visitChildren(ctx)

    def visitRegular_eq(self, ctx: BiastGrammarParser.Regular_eqContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_eq."""
        return self.visitChildren(ctx)

    def visitFull_eq(self, ctx: BiastGrammarParser.Full_eqContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_eq."""
        if ctx.EQ() is not None:
            opr = CBinaryOperator(ctx.EQ().getText())
        else:
            opr = CBinaryOperator(ctx.NE().getText())
        return CBinaryExpression(self.visit(ctx.equality_expression()), opr,
                                 self.visit(ctx.relational_expression()))

    def visitFull_compare(self, ctx: BiastGrammarParser.Full_compareContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_compare."""
        if ctx.LT() is not None:
            opr = CBinaryOperator(ctx.LT().getText())
        elif ctx.GT() is not None:
            opr = CBinaryOperator(ctx.GT().getText())
        elif ctx.LE() is not None:
            opr = CBinaryOperator(ctx.LE().getText())
        else:
            opr = CBinaryOperator(ctx.GE().getText())
        return CBinaryExpression(self.visit(ctx.relational_expression()), opr,
                                 self.visit(ctx.shift_expression()))

    def visitRegular_compare(self, ctx: BiastGrammarParser.Regular_compareContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_compare."""
        return self.visitChildren(ctx)

    def visitRegualr_shift(self, ctx: BiastGrammarParser.Regualr_shiftContext):
        """ Visit a parse tree produced by BiastGrammarParser#regualr_shift."""
        return self.visitChildren(ctx)

    def visitFull_shift(self, ctx: BiastGrammarParser.Full_shiftContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_shift."""
        if ctx.RSHIFT() is not None:
            opr = CBinaryOperator(ctx.RSHIFT().getText())
        else:
            opr = CBinaryOperator(ctx.LSHIFT().getText())
        return CBinaryExpression(self.visit(ctx.shift_expression()), opr,
                                 self.visit(ctx.additive_expression()))

    def visitRegular_add(self, ctx: BiastGrammarParser.Regular_addContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_add."""
        return self.visitChildren(ctx)

    def visitFull_add(self, ctx: BiastGrammarParser.Full_addContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_add."""
        if ctx.ADD() is not None:
            opr = CBinaryOperator(ctx.ADD().getText())
        else:
            opr = CBinaryOperator(ctx.SUB().getText())
        return CBinaryExpression(self.visit(ctx.additive_expression()), opr,
                                 self.visit(ctx.multiplicative_expression()))

    def visitRegular_mul(self, ctx: BiastGrammarParser.Regular_mulContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_mul."""
        return self.visitChildren(ctx)

    def visitFull_mul(self, ctx: BiastGrammarParser.Full_mulContext):
        """ Visit a parse tree produced by BiastGrammarParser#full_mul."""
        if ctx.MUL() is not None:
            opr = CBinaryOperator(ctx.MUL().getText())
        elif ctx.DIV() is not None:
            opr = CBinaryOperator(ctx.DIV().getText())
        else:
            opr = CBinaryOperator(ctx.MOD().getText())
        return CBinaryExpression(self.visit(ctx.multiplicative_expression()), opr,
                                 self.visit(ctx.cast_expression()))

    def visitRegualr_cast(self, ctx: BiastGrammarParser.Regualr_castContext):
        """ Visit a parse tree produced by BiastGrammarParser#regualr_cast."""
        return self.visitChildren(ctx)

    def visitExplicit_cast_expression(
            self, ctx: BiastGrammarParser.Explicit_cast_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#explicit_cast_expression."""
        return CCastExpression(self.visit(ctx.type_name()), self.visit(ctx.cast_expression()))

    def visitRegular_unary(self, ctx: BiastGrammarParser.Regular_unaryContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_unary."""
        return self.visitChildren(ctx)

    def visitSelfinc_unary(self, ctx: BiastGrammarParser.Selfinc_unaryContext):
        """ Visit a parse tree produced by BiastGrammarParser#selfinc_unary."""
        if ctx.SELFINC() is not None:
            opr = ctx.SELFINC().getText()
        else:
            opr = ctx.SELFDEC().getText()
        return CUnaryExpression(CUnaryOperator(opr), self.visit(ctx.unary_expression()))

    def visitAdd_unary(self, ctx: BiastGrammarParser.Add_unaryContext):
        """ Visit a parse tree produced by BiastGrammarParser#add_unary."""
        if ctx.ADD() is not None:
            opr = ctx.ADD().getText()
        elif ctx.SUB() is not None:
            opr = ctx.SUB().getText()
        elif ctx.BNOT() is not None:
            opr = ctx.BNOT().getText()
        else:
            opr = ctx.LNOT().getText()
        return CUnaryExpression(CUnaryOperator(opr), self.visit(ctx.cast_expression()))

    def visitBand_unary(self, ctx: BiastGrammarParser.Band_unaryContext):
        """ Visit a parse tree produced by BiastGrammarParser#band_unary."""
        return CUnaryExpression(CUnaryOperator(ctx.BAND().getText()),
                                self.visit(ctx.cast_expression()))

    def visitMul_unary(self, ctx: BiastGrammarParser.Mul_unaryContext):
        """ Visit a parse tree produced by BiastGrammarParser#mul_unary."""
        return CUnaryExpression(CUnaryOperator(ctx.MUL().getText()),
                                self.visit(ctx.cast_expression()))

    def visitSizeof_unary(self, ctx: BiastGrammarParser.Sizeof_unaryContext):
        """ Visit a parse tree produced by BiastGrammarParser#sizeof_unary."""
        return CUnaryExpression(CUnaryOperator(ctx.SIZEOF().getText()),
                                self.visit(ctx.unary_expression()))

    def visitSizeof_type(self, ctx: BiastGrammarParser.Sizeof_typeContext):
        """ Visit a parse tree produced by BiastGrammarParser#sizeof_type."""
        return CUnaryExpression(CUnaryOperator(ctx.SIZEOF().getText()),
                                self.visit(ctx.type_name()))

    def visitArgument_expression_list(
            self, ctx: BiastGrammarParser.Argument_expression_listContext):
        """ Visit a parse tree produced by BiastGrammarParser#argument_expression_list."""
        return list(self.visit(assign) for assign in ctx.assigns)

    def visitMacro_call(self, ctx: BiastGrammarParser.Macro_callContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_call."""
        macro = ctx.MACROEXPR().getText()[1: -5]
        args = self.visit(ctx.builtin_parameters())
        return CMacroExpression(macro, args)

    def visitArray_reference(self, ctx: BiastGrammarParser.Array_referenceContext):
        """ Visit a parse tree produced by BiastGrammarParser#array_reference."""
        return CElementExpression(self.visit(ctx.postfix_expression()),
                                  self.visit(ctx.expression()))

    def visitPostfix_unary_expression(
            self, ctx: BiastGrammarParser.Postfix_unary_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#postfix_unary_expression."""
        if ctx.SELFINC() is not None:
            opr = ctx.SELFINC().getText()
        else:
            opr = ctx.SELFDEC().getText()
        return CUnaryExpression(CPostUnaryOperator(opr), self.visit(ctx.postfix_expression()))

    def visitObject_member(self, ctx: BiastGrammarParser.Object_memberContext):
        """ Visit a parse tree produced by BiastGrammarParser#object_member."""
        return CMemberExpression(self.visit(ctx.postfix_expression()), ctx.CNAME().getText())

    def visitFunction_call(self, ctx: BiastGrammarParser.Function_callContext):
        """ Visit a parse tree produced by BiastGrammarParser#function_call."""
        callee = self.visit(ctx.postfix_expression())
        args_opt = []
        if ctx.argument_expression_list() is not None:
            args_opt = self.visit(ctx.argument_expression_list())

        if len(args_opt) == 1 and isinstance(callee, CIdentifierExpression) and \
                self.__is_typename(callee.name):
            return CCastExpression(CAliasType(callee.name), args_opt[0])

        return CCallExpression(callee, args_opt)

    def visitPri_expression(self, ctx: BiastGrammarParser.Pri_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#pri_expression."""
        return self.visitChildren(ctx)

    def visitPointer_object_member(self, ctx: BiastGrammarParser.Pointer_object_memberContext):
        """ Visit a parse tree produced by BiastGrammarParser#pointer_object_member."""
        return CMemberExpression(
            CUnaryExpression(CUnaryOperator.DEREFERENCE, self.visit(ctx.postfix_expression())),
            ctx.CNAME().getText())

    def visitCompound_literal(self, ctx: BiastGrammarParser.Compound_literalContext):
        """ Visit a parse tree produced by BiastGrammarParser#compound_literal."""
        return CCastExpression(self.visit(ctx.type_name()),
                               self.visit(ctx.designated_initializers()))

    def visitBuiltin_expression(self, ctx: BiastGrammarParser.Builtin_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#builtin_expression."""
        return CMacroExpression(ctx.BUILTIN().getText(), self.visit(ctx.builtin_parameters()))

    def visitBuiltin_parameter(self, ctx: BiastGrammarParser.Builtin_parameterContext):
        """ Visit a parse tree produced by BiastGrammarParser#builtin_parameter."""
        return self.visitChildren(ctx)

    def visitBuiltin_parameters(self, ctx: BiastGrammarParser.Builtin_parametersContext):
        """ Visit a parse tree produced by BiastGrammarParser#builtin_parameters."""
        return list(self.visit(param) for param in ctx.params)

    def visitMacro_parameters(self, ctx: BiastGrammarParser.Macro_parametersContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_parameters."""
        return self.visitChildren(ctx)

    def visitConstant_as_pri_expression(
            self, ctx: BiastGrammarParser.Constant_as_pri_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#constant_as_pri_expression."""
        return self.visitChildren(ctx)

    def visitStrings(self, ctx: BiastGrammarParser.StringsContext):
        """ Visit a parse tree produced by BiastGrammarParser#strings."""
        return CStringLiteral(self.visit(ctx.string_literal()))

    def visitSigle_expression(self, ctx: BiastGrammarParser.Sigle_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#sigle_expression."""
        return self.visit(ctx.expression())

    def visitCompound_expression(self, ctx: BiastGrammarParser.Compound_expressionContext):
        """ Visit a parse tree produced by BiastGrammarParser#compound_expression."""
        return CCompoundExpression(self.visit(ctx.compound_statement()))

    def visitInteger_constant(self, ctx: BiastGrammarParser.Integer_constantContext):
        """ Visit a parse tree produced by BiastGrammarParser#integer_constant."""
        return self.visitChildren(ctx)

    def visitFloating_constant(self, ctx: BiastGrammarParser.Floating_constantContext):
        """ Visit a parse tree produced by BiastGrammarParser#floating_constant."""
        return self.__extract_float(ctx.FLOAT_LITERAL().getText())

    def visitDecimal_literal(self, ctx: BiastGrammarParser.Decimal_literalContext):
        """ Visit a parse tree produced by BiastGrammarParser#decimal_literal."""
        return self.__extract_integer(ctx.INT_LITERAL().getText(), CIntegerBase.DEC)

    def visitHexadecimal_literal(self, ctx: BiastGrammarParser.Hexadecimal_literalContext):
        """ Visit a parse tree produced by BiastGrammarParser#hexadecimal_literal."""
        return self.__extract_integer(ctx.HEX_LITERAL().getText(), CIntegerBase.HEX)

    def visitOctal_literal(self, ctx: BiastGrammarParser.Octal_literalContext):
        """ Visit a parse tree produced by BiastGrammarParser#octal_literal."""
        return self.__extract_integer(ctx.OCT_LITERAL().getText(), CIntegerBase.OCT)

    def visitBinary_literal(self, ctx: BiastGrammarParser.Binary_literalContext):
        """ Visit a parse tree produced by BiastGrammarParser#binary_literal."""
        return self.__extract_integer(ctx.BIN_LITERAL().getText(), CIntegerBase.BIN)

    def visitType_specifier(self, ctx: BiastGrammarParser.Type_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#type_specifier."""
        return self.visitChildren(ctx)

    def visitType_macro_specifier(self, ctx: BiastGrammarParser.Type_macro_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#type_macro_specifier."""
        name = ctx.MACROTYPE().getText()[1: -5]
        if ctx.params is not None:
            arguments = list(self.visit(param) for param in ctx.params)
        else:
            arguments = []
        return CMacroType(name, arguments)

    def visitReturn_single(self, ctx: BiastGrammarParser.Return_singleContext):
        """ Visit a parse tree produced by BiastGrammarParser#single_spec."""
        return self.visitChildren(ctx)

    def visitTypeof_specifier(self, ctx: BiastGrammarParser.Typeof_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#typeof_specifier."""
        if ctx.type_name() is not None:
            typ_or_expr = self.visit(ctx.type_name())
        else:
            typ_or_expr = self.visit(ctx.expression())
        return CTypeOf(typ_or_expr)

    def visitNamed_struct_specifier(self, ctx: BiastGrammarParser.Named_struct_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#named_struct_specifier."""
        attributes: List[CAttribute]
        attributes = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        name = ctx.CNAME().getText()
        fields = None
        attributes_end: List[CAttribute] = []
        if ctx.field_declarations() is not None:
            fields = self.visit(ctx.field_declarations())
            attributes_end = self.visit(ctx.attr_end) if ctx.attr_end is not None else []

        ret = CStructType(name=name, field_decls=fields)
        ret.attributes = self.__get_attributes(attributes, attributes_end)
        return ret

    def visitAnonymous_struct_specifier(
            self, ctx: BiastGrammarParser.Anonymous_struct_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#anonymous_struct_specifier."""
        attributes: List[CAttribute]
        attributes = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        fields = self.visit(ctx.field_declarations())
        attributes_end: List[CAttribute]
        attributes_end = self.visit(ctx.attr_end) if ctx.attr_end is not None else []

        ret = CStructType(name=None, field_decls=fields)
        ret.attributes = self.__get_attributes(attributes, attributes_end)
        return ret

    def visitNamed_union_specifier(self, ctx: BiastGrammarParser.Named_union_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#named_union_specifier."""
        attributes: List[CAttribute]
        attributes = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        name = ctx.CNAME().getText()
        fields = None
        attributes_end: List[CAttribute] = []
        if ctx.field_declarations() is not None:
            fields = self.visit(ctx.field_declarations())
            attributes_end = self.visit(ctx.attr_end) if ctx.attr_end is not None else []

        ret = CUnionType(name=name, field_decls=fields)
        ret.attributes = self.__get_attributes(attributes, attributes_end)
        return ret

    def visitAnonymous_union_specifier(
            self, ctx: BiastGrammarParser.Anonymous_union_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#anonymous_union_specifier."""
        attributes: List[CAttribute]
        attributes = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        fields = self.visit(ctx.field_declarations())
        attributes_end: List[CAttribute]
        attributes_end = self.visit(ctx.attr_end) if ctx.attr_end is not None else []

        ret = CUnionType(name=None, field_decls=fields)
        ret.attributes = self.__get_attributes(attributes, attributes_end)
        return ret

    def visitAnonymous_enum_specifier(self, ctx: BiastGrammarParser.Anonymous_enum_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#anonymous_enum_specifier."""
        attributes: List[CAttribute]
        attributes = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        fields = self.visit(ctx.enumerators())
        attributes_end: List[CAttribute]
        attributes_end = self.visit(ctx.attr_end) if ctx.attr_end is not None else []

        ret = CEnumType(name=None, enum_items=fields)
        ret.attributes = self.__get_attributes(attributes, attributes_end)
        return ret

    def visitNamed_enum_specifier(self, ctx: BiastGrammarParser.Named_enum_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#named_enum_specifier."""
        attributes: List[CAttribute]
        attributes = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        name = ctx.CNAME().getText()
        fields = None
        attributes_end: List[CAttribute] = []
        if ctx.enumerators() is not None:
            fields = self.visit(ctx.enumerators())
            attributes_end = self.visit(ctx.attr_end) if ctx.attr_end is not None else []

        ret = CEnumType(name=name, enum_items=fields)
        ret.attributes = self.__get_attributes(attributes, attributes_end)
        return ret

    def visitField_declarations(self, ctx: BiastGrammarParser.Field_declarationsContext):
        """ Visit a parse tree produced by BiastGrammarParser#field_declarations."""
        fields = list(self.visit(arg) for arg in ctx.f_decls if arg is not None)
        return list(chain(*fields))

    def visitRegular_field_declaration(
            self, ctx: BiastGrammarParser.Regular_field_declarationContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_field_declaration."""
        sps = self.visit(ctx.specifier_qualifiers())
        decls = []
        for fdeclarator in self.visit(ctx.field_declarators()):
            decl = CFieldDeclaration(
                name=fdeclarator.name,
                type=fdeclarator.get_type(sps or []),
                bitwidth=fdeclarator.bitwidth)
            decl.attributes = self.__get_attributes(sps or [], fdeclarator.attributes)
            decls.append(decl)
        return decls

    def visitUnamed_struct_field(self, ctx: BiastGrammarParser.Unamed_struct_fieldContext):
        """ Visit a parse tree produced by BiastGrammarParser#unamed_struct_field."""
        struct_sp = self.visit(ctx.struct_specifier())
        ret = CFieldDeclaration(None, struct_sp, None)
        ret.attributes = struct_sp.attributes
        return [ret]

    def visitUnamed_union_field(self, ctx: BiastGrammarParser.Unamed_union_fieldContext):
        """ Visit a parse tree produced by BiastGrammarParser#unamed_union_field."""
        union_sp = self.visit(ctx.union_specifier())
        ret = CFieldDeclaration(None, union_sp, None)
        ret.attributes = union_sp.attributes
        return [ret]

    def visitMacro_field(self, ctx: BiastGrammarParser.Macro_fieldContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_field."""
        name = ctx.MACRODECL().getText()[1: -5]
        args = self.visit(ctx.builtin_parameters()) if ctx.builtin_parameters() is not None else []
        return [CMacroFieldDeclaration(name=None, type=CType(),
                                       macro_name=name, arguments=args, bitwidth=None)]

    def visitField_declarators(self, ctx: BiastGrammarParser.Field_declaratorsContext):
        """ Visit a parse tree produced by BiastGrammarParser#field_declarators."""
        return list(self.visit(f) for f in ctx.fields)

    def visitBitfield_descriptor(self, ctx: BiastGrammarParser.Bitfield_descriptorContext):
        """ Visit a parse tree produced by BiastGrammarParser#bitfield_descriptor."""
        return self.visitChildren(ctx)

    def visitNamed_field_declarator(self, ctx: BiastGrammarParser.Named_field_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#named_field_declarator."""
        declarator = self.visit(ctx.declarator())
        bit = None
        if ctx.bitfield_descriptor() is not None:
            bit = self.visit(ctx.bitfield_descriptor())
        ret = CFieldDeclarator(declarator.name, declarator.decorator, bit)
        ret.attributes = declarator.attributes
        return ret

    def visitAnonymous_bit_field_declarator(
            self, ctx: BiastGrammarParser.Anonymous_bit_field_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#anonymous_bit_field_declarator."""
        return CFieldDeclarator(name=None, bitwidth=self.visit(ctx.bitfield_descriptor()))

    def visitPointer_descriptors(self, ctx: BiastGrammarParser.Pointer_descriptorsContext):
        """ Visit a parse tree produced by BiastGrammarParser#pointer_descriptors."""
        return list(self.visit(d) for d in ctx.des)

    def visitPointer_descriptor(self, ctx: BiastGrammarParser.Pointer_descriptorContext):
        """ Visit a parse tree produced by BiastGrammarParser#pointer_descriptor."""
        return list(self.visit(q) for q in ctx.tqs)

    def visitPointer_abstract_declarator(
            self, ctx: BiastGrammarParser.Pointer_abstract_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#pointer_abstract_declarator."""
        attr = []
        if ctx.attribute_specifiers() is not None:
            attr = self.visit(ctx.attribute_specifiers())
        declarator = CAbstractDeclarator()
        for pointer_qualifiers in reversed(self.visit(ctx.pointer_descriptors())):
            declarator.decorator = CPointerDecorator(
                qualifiers=set(pointer_qualifiers),
                base_decorator=declarator.decorator)
        declarator.attributes = self.__get_attributes(declarator.attributes, attr)
        return declarator

    def visitPointer_complex_abstract_declarator(
            self, ctx: BiastGrammarParser.Pointer_complex_abstract_declaratorContext):
        """
        Visit a parse tree produced by BiastGrammarParser#pointer_complex_abstract_declarator.
        """
        declarator = self.visit(ctx.direct_abstract_declarator())
        attr: List[CAttribute] = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        attr_end: List[CAttribute] = self.visit(ctx.attr_end) if ctx.attr_end is not None else []
        if ctx.pointer_descriptors() is not None:
            for pointer_qualifiers in reversed(self.visit(ctx.pointer_descriptors())):
                declarator.decorator = CPointerDecorator(
                    qualifiers=set(pointer_qualifiers),
                    base_decorator=declarator.decorator)
        declarator.attributes = self.__get_attributes(attr, declarator.attributes, attr_end)
        return declarator

    def visitNested_array_abstract_declarator(
            self, ctx: BiastGrammarParser.Nested_array_abstract_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#nested_array_abstract_declarator."""
        declarator = self.visit(ctx.direct_abstract_declarator())
        declarator.decorator = CArrayDecorator(
            qualifiers=set(),
            capacity=self.visit(ctx.expression()) if ctx.expression() is not None else None,
            base_decorator=declarator.decorator)
        return declarator

    def visitNested_function_abstract_declarator(
            self, ctx: BiastGrammarParser.Nested_function_abstract_declaratorContext):
        """
        Visit a parse tree produced by BiastGrammarParser#nested_function_abstract_declarator.
        """
        parameters, vla = self.visit(ctx.parameter_declarations())
        declarator = self.visit(ctx.direct_abstract_declarator())
        if len(parameters) == 0:
            vla = True
        elif parameters == [CParameterDeclaration(None, CVoidType())]:
            vla = False
            parameters = []
        declarator.decorator = CFunctionDecorator(
            qualifiers=set(),
            parameters=parameters,
            base_decorator=declarator.decorator,
            variable_length_argument=vla)
        return declarator

    def visitSingle_declarator(self, ctx: BiastGrammarParser.Single_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#single_declarator."""
        return self.visit(ctx.abstract_declarator())

    def visitArray_abstract_declarator(
            self, ctx: BiastGrammarParser.Array_abstract_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#array_abstract_declarator."""
        declarator = CAbstractDeclarator()
        declarator.decorator = CArrayDecorator(
            qualifiers=set(),
            capacity=self.visit(ctx.expression()) if ctx.expression() is not None else None,
            base_decorator=None)
        return declarator

    def visitFunction_abstract_declarator(
            self, ctx: BiastGrammarParser.Function_abstract_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#function_abstract_declarator."""
        parameters, vla = self.visit(ctx.parameter_declarations())
        declarator = CAbstractDeclarator()
        if len(parameters) == 0:
            vla = True
        elif parameters == [CParameterDeclaration(None, CVoidType())]:
            vla = False

            parameters = []
        declarator.decorator = CFunctionDecorator(
            qualifiers=set(),
            parameters=parameters,
            base_decorator=None,
            variable_length_argument=vla)
        return declarator

    def visitDeclarator(self, ctx: BiastGrammarParser.DeclaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#declarator."""
        declarator = self.visit(ctx.direct_declarator())
        attr: List[CAttribute] = self.visit(ctx.attr_head) if ctx.attr_head is not None else []
        attr_end: List[CAttribute] = self.visit(ctx.attr_end) if ctx.attr_end is not None else []
        if ctx.pointer_descriptors() is not None:
            for pointer_qualifiers in reversed(self.visit(ctx.pointer_descriptors())):
                if isinstance(declarator.decorator, CFunctionDecorator):
                    declarator.decorator.return_ptr_level += 1
                else:
                    declarator.decorator = CPointerDecorator(
                        qualifiers=set(pointer_qualifiers),
                        base_decorator=declarator.decorator)
        declarator.attributes = self.__get_attributes(attr, declarator.attributes, attr_end)
        return declarator

    def visitFunction_declarator(self, ctx: BiastGrammarParser.Function_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#function_declarator."""
        direct_declarator = self.visit(ctx.direct_declarator())
        parameters, vla = self.visit(ctx.parameter_declarations())
        if len(parameters) == 0:
            vla = True
        elif parameters == [CParameterDeclaration(None, CVoidType())]:
            vla = False
            parameters = []
        direct_declarator.decorator = CFunctionDecorator(
            qualifiers=set(),
            parameters=parameters,
            base_decorator=direct_declarator.decorator,
            variable_length_argument=vla)
        return direct_declarator

    def visitBrace_declarator(self, ctx: BiastGrammarParser.Brace_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#brace_declarator."""
        return self.visit(ctx.declarator())

    def visitArray_declarator(self, ctx: BiastGrammarParser.Array_declaratorContext):
        """ Visit a parse tree produced by BiastGrammarParser#array_declarator."""
        direct_declarator = self.visit(ctx.direct_declarator())
        capacity_opt = self.visit(ctx.expression()) if ctx.expression() is not None else None
        direct_declarator.decorator = CArrayDecorator(
            qualifiers=set(),
            capacity=capacity_opt,
            base_decorator=direct_declarator.decorator)
        return direct_declarator

    def visitRegular_parameter_declarations(
            self, ctx: BiastGrammarParser.Regular_parameter_declarationsContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_parameter_declarations."""
        args = list(self.visit(p) for p in ctx.pds)
        vla = ctx.AND_MORE() is not None
        return args, vla

    def visitRegular_parameter_declaration(
            self, ctx: BiastGrammarParser.Regular_parameter_declarationContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_parameter_declaration."""
        sps = []
        if ctx.declaration_specifiers() is not None:
            sps = self.visit(ctx.declaration_specifiers())
        declarator_opt = ctx.declarator() or ctx.abstract_declarator()
        if declarator_opt is not None:
            declarator = self.visit(declarator_opt)
        else:
            declarator = CAbstractDeclarator()
        ret = CParameterDeclaration(
            name=declarator.name,
            type=declarator.get_type(sps),)
        ret.attributes = self.__get_attributes(sps, declarator.attributes)
        return ret

    def visitMacro_parameter(self, ctx: BiastGrammarParser.Macro_parameterContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_parameter."""
        macro = ctx.MACRODECL().getText()[1: -5]
        args = self.visit(ctx.builtin_parameters()) if ctx.builtin_parameters() is not None else []
        return CMacroParameterDeclaration(
            name=None,
            type=CType(),
            macro_name=macro,
            arguments=args)

    def visitEnumerators(self, ctx: BiastGrammarParser.EnumeratorsContext):
        """ Visit a parse tree produced by BiastGrammarParser#enumerators."""
        return list(self.visit(e) for e in ctx.etr)

    def visitAssigned_enum_item(self, ctx: BiastGrammarParser.Assigned_enum_itemContext):
        """ Visit a parse tree produced by BiastGrammarParser#assigned_enum_item."""
        return CEnumItem(name=ctx.CNAME().getText(), value=self.visit(ctx.constant_expression()))

    def visitType_name(self, ctx: BiastGrammarParser.Type_nameContext):
        """ Visit a parse tree produced by BiastGrammarParser#type_name."""
        declarator = CAbstractDeclarator()
        if ctx.abstract_declarator() is not None:
            declarator = self.visit(ctx.abstract_declarator())
        sps = self.visit(ctx.specifier_qualifiers())
        if sps is None:
            raise ValueError(f"empty string is invalid to match a type specifier")
        return declarator.get_type(sps)

    def visitSpecifiers_qualifiers_alias(
            self, ctx: BiastGrammarParser.Specifiers_qualifiers_aliasContext):
        """ Visit a parse tree produced by BiastGrammarParser#specifiers_qualifiers_alias."""
        qualifiers = []
        if ctx.specifier_qualifiers() is not None:
            qualifiers = self.visit(ctx.specifier_qualifiers())
        return [self.visit(ctx.getChild(0))] + qualifiers

    def visitSpecifiers_qualifiers(self, ctx: BiastGrammarParser.Specifiers_qualifiersContext):
        """ Visit a parse tree produced by BiastGrammarParser#specifiers_qualifiers."""
        qualifiers = list(self.visit(q) for q in ctx.qs)
        return [self.visit(ctx.direct_type_specifier())] + qualifiers

    def visitDirect_specifier_qualifier(
            self, ctx: BiastGrammarParser.Direct_specifier_qualifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#direct_specifier_qualifiers."""
        return self.visitChildren(ctx)

    def visitAttribute_specifiers(self, ctx: BiastGrammarParser.Attribute_specifiersContext):
        """ Visit a parse tree produced by BiastGrammarParser#attribute_specifiers."""
        count = ctx.getChildCount()
        ret = []
        for i in range(count):
            arg = self.visit(ctx.getChild(i))
            if isinstance(arg, CMacroAttribute):
                ret.append(arg)
            else:
                ret += arg
        return ret

    def visitRegular_attribute_specifier(
            self, ctx: BiastGrammarParser.Regular_attribute_specifierContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_attribute_specifier."""
        attr = self.visit(ctx.attributes()) if ctx.attributes() is not None else []
        return attr

    def visitMacro_attribute(self, ctx: BiastGrammarParser.Macro_attributeContext):
        """ Visit a parse tree produced by BiastGrammarParser#macro_attribute."""
        args = self.visit(ctx.builtin_parameters()) if ctx.builtin_parameters() is not None else []
        return CMacroAttribute(macro_name=ctx.MACROATTR().getText()[1: -5], arguments=args)

    def visitAttributes(self, ctx: BiastGrammarParser.AttributesContext):
        """ Visit a parse tree produced by BiastGrammarParser#attributes."""
        attrs = list(self.visit(attr) for attr in ctx.attrs)
        return list(attr for attr in attrs if attr is not None)

    def visitRegular_attr(self, ctx: BiastGrammarParser.Regular_attrContext):
        """ Visit a parse tree produced by BiastGrammarParser#regular_attr."""
        parameters = None
        if ctx.attribute_parameters() is not None:
            parameters = self.visit(ctx.attribute_parameters())
        return CClassicalAttribute(ctx.CNAME().getText(), parameters)

    def visitAttribute_parameters(self, ctx: BiastGrammarParser.Attribute_parametersContext):
        """ Visit a parse tree produced by BiastGrammarParser#attribute_parameters."""
        ret = None
        if ctx.argument_expression_list() is not None:
            ret = self.visit(ctx.argument_expression_list())
        return ret
