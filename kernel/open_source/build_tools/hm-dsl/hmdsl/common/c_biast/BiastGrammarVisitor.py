# Generated from /usr1/hmos_for_chipset/src/increment/sourcecode/out/generic_generic_arm_64only/general_7525L_phone_standard/hongmeng/build/tmp/work/x86_64-linux/hm-dsl-native/git-r0/image/usr1/hmos_for_chipset/src/increment/sourcecode/out/generic_generic_arm_64only/general_7525L_phone_standard/hongmeng/build/tmp/work/x86_64-linux/hm-dsl-native/git-r0/recipe-sysroot-native/usr/bin/hm-dsl/hmdsl/common/c_biast/BiastGrammar.g4 by ANTLR 4.13.0
from antlr4 import *
if "." in __name__:
    from .BiastGrammarParser import BiastGrammarParser
else:
    from BiastGrammarParser import BiastGrammarParser

# This class defines a complete generic visitor for a parse tree produced by BiastGrammarParser.

class BiastGrammarVisitor(ParseTreeVisitor):

    # Visit a parse tree produced by BiastGrammarParser#translation_unit.
    def visitTranslation_unit(self, ctx:BiastGrammarParser.Translation_unitContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#external_declaration.
    def visitExternal_declaration(self, ctx:BiastGrammarParser.External_declarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#function_definition.
    def visitFunction_definition(self, ctx:BiastGrammarParser.Function_definitionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#function_specifier.
    def visitFunction_specifier(self, ctx:BiastGrammarParser.Function_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#c_declaration.
    def visitC_declaration(self, ctx:BiastGrammarParser.C_declarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_declaration.
    def visitMacro_declaration(self, ctx:BiastGrammarParser.Macro_declarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#declarations.
    def visitDeclarations(self, ctx:BiastGrammarParser.DeclarationsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#nontype_declaration_specifier.
    def visitNontype_declaration_specifier(self, ctx:BiastGrammarParser.Nontype_declaration_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#nontypename_declaration_specifier.
    def visitNontypename_declaration_specifier(self, ctx:BiastGrammarParser.Nontypename_declaration_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#nontype_declaration_specifiers.
    def visitNontype_declaration_specifiers(self, ctx:BiastGrammarParser.Nontype_declaration_specifiersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#nontypename_declaration_specifiers.
    def visitNontypename_declaration_specifiers(self, ctx:BiastGrammarParser.Nontypename_declaration_specifiersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#declaration_specifiers_alias.
    def visitDeclaration_specifiers_alias(self, ctx:BiastGrammarParser.Declaration_specifiers_aliasContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#declaration_specifiers_nonname.
    def visitDeclaration_specifiers_nonname(self, ctx:BiastGrammarParser.Declaration_specifiers_nonnameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#declaration_specifiers_nontype.
    def visitDeclaration_specifiers_nontype(self, ctx:BiastGrammarParser.Declaration_specifiers_nontypeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#init_declarators.
    def visitInit_declarators(self, ctx:BiastGrammarParser.Init_declaratorsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#init_declarator.
    def visitInit_declarator(self, ctx:BiastGrammarParser.Init_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#expr_initializer.
    def visitExpr_initializer(self, ctx:BiastGrammarParser.Expr_initializerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#bracket_initializer.
    def visitBracket_initializer(self, ctx:BiastGrammarParser.Bracket_initializerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#designated_initializers.
    def visitDesignated_initializers(self, ctx:BiastGrammarParser.Designated_initializersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#designated_initializer.
    def visitDesignated_initializer(self, ctx:BiastGrammarParser.Designated_initializerContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#designation.
    def visitDesignation(self, ctx:BiastGrammarParser.DesignationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#range_designator.
    def visitRange_designator(self, ctx:BiastGrammarParser.Range_designatorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#member_designator.
    def visitMember_designator(self, ctx:BiastGrammarParser.Member_designatorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#statement.
    def visitStatement(self, ctx:BiastGrammarParser.StatementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#expression_statement.
    def visitExpression_statement(self, ctx:BiastGrammarParser.Expression_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#null_statement.
    def visitNull_statement(self, ctx:BiastGrammarParser.Null_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_statement.
    def visitMacro_statement(self, ctx:BiastGrammarParser.Macro_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#labelled_statement.
    def visitLabelled_statement(self, ctx:BiastGrammarParser.Labelled_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#switch_body.
    def visitSwitch_body(self, ctx:BiastGrammarParser.Switch_bodyContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#non_default_case_statement.
    def visitNon_default_case_statement(self, ctx:BiastGrammarParser.Non_default_case_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#default_case_statement.
    def visitDefault_case_statement(self, ctx:BiastGrammarParser.Default_case_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#compound_statement.
    def visitCompound_statement(self, ctx:BiastGrammarParser.Compound_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#if_statement.
    def visitIf_statement(self, ctx:BiastGrammarParser.If_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#if_statement_else.
    def visitIf_statement_else(self, ctx:BiastGrammarParser.If_statement_elseContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#switch_statement.
    def visitSwitch_statement(self, ctx:BiastGrammarParser.Switch_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#while_statement.
    def visitWhile_statement(self, ctx:BiastGrammarParser.While_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#do_while_statement.
    def visitDo_while_statement(self, ctx:BiastGrammarParser.Do_while_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#for_statement.
    def visitFor_statement(self, ctx:BiastGrammarParser.For_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#for_statement_decl.
    def visitFor_statement_decl(self, ctx:BiastGrammarParser.For_statement_declContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#goto_statement.
    def visitGoto_statement(self, ctx:BiastGrammarParser.Goto_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#continue_statement.
    def visitContinue_statement(self, ctx:BiastGrammarParser.Continue_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#break_statement.
    def visitBreak_statement(self, ctx:BiastGrammarParser.Break_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#return_statement.
    def visitReturn_statement(self, ctx:BiastGrammarParser.Return_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_statement.
    def visitAsm_statement(self, ctx:BiastGrammarParser.Asm_statementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_qualifiers.
    def visitAsm_qualifiers(self, ctx:BiastGrammarParser.Asm_qualifiersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_volatile_qualifier.
    def visitAsm_volatile_qualifier(self, ctx:BiastGrammarParser.Asm_volatile_qualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_inline_qualifier.
    def visitAsm_inline_qualifier(self, ctx:BiastGrammarParser.Asm_inline_qualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_goto_qualifier.
    def visitAsm_goto_qualifier(self, ctx:BiastGrammarParser.Asm_goto_qualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_operands.
    def visitAsm_operands(self, ctx:BiastGrammarParser.Asm_operandsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_operand.
    def visitAsm_operand(self, ctx:BiastGrammarParser.Asm_operandContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_clobbers.
    def visitAsm_clobbers(self, ctx:BiastGrammarParser.Asm_clobbersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_gotolabels.
    def visitAsm_gotolabels(self, ctx:BiastGrammarParser.Asm_gotolabelsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#condition.
    def visitCondition(self, ctx:BiastGrammarParser.ConditionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#constant_expression.
    def visitConstant_expression(self, ctx:BiastGrammarParser.Constant_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#sequence_expression.
    def visitSequence_expression(self, ctx:BiastGrammarParser.Sequence_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_expression.
    def visitRegular_expression(self, ctx:BiastGrammarParser.Regular_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_assign.
    def visitRegular_assign(self, ctx:BiastGrammarParser.Regular_assignContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_assign.
    def visitFull_assign(self, ctx:BiastGrammarParser.Full_assignContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#assignment_operator.
    def visitAssignment_operator(self, ctx:BiastGrammarParser.Assignment_operatorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_condition.
    def visitRegular_condition(self, ctx:BiastGrammarParser.Regular_conditionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#ternary_expression.
    def visitTernary_expression(self, ctx:BiastGrammarParser.Ternary_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_lor.
    def visitRegular_lor(self, ctx:BiastGrammarParser.Regular_lorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_lor.
    def visitFull_lor(self, ctx:BiastGrammarParser.Full_lorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_land.
    def visitRegular_land(self, ctx:BiastGrammarParser.Regular_landContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_land.
    def visitFull_land(self, ctx:BiastGrammarParser.Full_landContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_bor.
    def visitRegular_bor(self, ctx:BiastGrammarParser.Regular_borContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_bor.
    def visitFull_bor(self, ctx:BiastGrammarParser.Full_borContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_xor.
    def visitFull_xor(self, ctx:BiastGrammarParser.Full_xorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_xor.
    def visitRegular_xor(self, ctx:BiastGrammarParser.Regular_xorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_band.
    def visitFull_band(self, ctx:BiastGrammarParser.Full_bandContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_band.
    def visitRegular_band(self, ctx:BiastGrammarParser.Regular_bandContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_eq.
    def visitRegular_eq(self, ctx:BiastGrammarParser.Regular_eqContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_eq.
    def visitFull_eq(self, ctx:BiastGrammarParser.Full_eqContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_compare.
    def visitFull_compare(self, ctx:BiastGrammarParser.Full_compareContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_compare.
    def visitRegular_compare(self, ctx:BiastGrammarParser.Regular_compareContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regualr_shift.
    def visitRegualr_shift(self, ctx:BiastGrammarParser.Regualr_shiftContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_shift.
    def visitFull_shift(self, ctx:BiastGrammarParser.Full_shiftContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_add.
    def visitRegular_add(self, ctx:BiastGrammarParser.Regular_addContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_add.
    def visitFull_add(self, ctx:BiastGrammarParser.Full_addContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_mul.
    def visitRegular_mul(self, ctx:BiastGrammarParser.Regular_mulContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#full_mul.
    def visitFull_mul(self, ctx:BiastGrammarParser.Full_mulContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regualr_cast.
    def visitRegualr_cast(self, ctx:BiastGrammarParser.Regualr_castContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#explicit_cast_expression.
    def visitExplicit_cast_expression(self, ctx:BiastGrammarParser.Explicit_cast_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_unary.
    def visitRegular_unary(self, ctx:BiastGrammarParser.Regular_unaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#selfinc_unary.
    def visitSelfinc_unary(self, ctx:BiastGrammarParser.Selfinc_unaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#add_unary.
    def visitAdd_unary(self, ctx:BiastGrammarParser.Add_unaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#band_unary.
    def visitBand_unary(self, ctx:BiastGrammarParser.Band_unaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#mul_unary.
    def visitMul_unary(self, ctx:BiastGrammarParser.Mul_unaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#sizeof_unary.
    def visitSizeof_unary(self, ctx:BiastGrammarParser.Sizeof_unaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#sizeof_type.
    def visitSizeof_type(self, ctx:BiastGrammarParser.Sizeof_typeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#argument_expression_list.
    def visitArgument_expression_list(self, ctx:BiastGrammarParser.Argument_expression_listContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_call.
    def visitMacro_call(self, ctx:BiastGrammarParser.Macro_callContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#array_reference.
    def visitArray_reference(self, ctx:BiastGrammarParser.Array_referenceContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#postfix_unary_expression.
    def visitPostfix_unary_expression(self, ctx:BiastGrammarParser.Postfix_unary_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#object_member.
    def visitObject_member(self, ctx:BiastGrammarParser.Object_memberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#function_call.
    def visitFunction_call(self, ctx:BiastGrammarParser.Function_callContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#pri_expression.
    def visitPri_expression(self, ctx:BiastGrammarParser.Pri_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#pointer_object_member.
    def visitPointer_object_member(self, ctx:BiastGrammarParser.Pointer_object_memberContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#compound_literal.
    def visitCompound_literal(self, ctx:BiastGrammarParser.Compound_literalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_expr.
    def visitMacro_expr(self, ctx:BiastGrammarParser.Macro_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#builtin_expression.
    def visitBuiltin_expression(self, ctx:BiastGrammarParser.Builtin_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#builtin_parameter.
    def visitBuiltin_parameter(self, ctx:BiastGrammarParser.Builtin_parameterContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#builtin_parameters.
    def visitBuiltin_parameters(self, ctx:BiastGrammarParser.Builtin_parametersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_parameters.
    def visitMacro_parameters(self, ctx:BiastGrammarParser.Macro_parametersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#constant_as_pri_expression.
    def visitConstant_as_pri_expression(self, ctx:BiastGrammarParser.Constant_as_pri_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#identifier_expression.
    def visitIdentifier_expression(self, ctx:BiastGrammarParser.Identifier_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#strings.
    def visitStrings(self, ctx:BiastGrammarParser.StringsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#sigle_expression.
    def visitSigle_expression(self, ctx:BiastGrammarParser.Sigle_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#compound_expression.
    def visitCompound_expression(self, ctx:BiastGrammarParser.Compound_expressionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#integer_constant.
    def visitInteger_constant(self, ctx:BiastGrammarParser.Integer_constantContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#char_constant.
    def visitChar_constant(self, ctx:BiastGrammarParser.Char_constantContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#floating_constant.
    def visitFloating_constant(self, ctx:BiastGrammarParser.Floating_constantContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#decimal_literal.
    def visitDecimal_literal(self, ctx:BiastGrammarParser.Decimal_literalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#hexadecimal_literal.
    def visitHexadecimal_literal(self, ctx:BiastGrammarParser.Hexadecimal_literalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#octal_literal.
    def visitOctal_literal(self, ctx:BiastGrammarParser.Octal_literalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#binary_literal.
    def visitBinary_literal(self, ctx:BiastGrammarParser.Binary_literalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#storage_class_specifier.
    def visitStorage_class_specifier(self, ctx:BiastGrammarParser.Storage_class_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#type_qualifier.
    def visitType_qualifier(self, ctx:BiastGrammarParser.Type_qualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#type_specifier.
    def visitType_specifier(self, ctx:BiastGrammarParser.Type_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#single_lex_spec.
    def visitSingle_lex_spec(self, ctx:BiastGrammarParser.Single_lex_specContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#type_macro_specifier.
    def visitType_macro_specifier(self, ctx:BiastGrammarParser.Type_macro_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#return_single.
    def visitReturn_single(self, ctx:BiastGrammarParser.Return_singleContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#type_alias_specifier.
    def visitType_alias_specifier(self, ctx:BiastGrammarParser.Type_alias_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#typeof_specifier.
    def visitTypeof_specifier(self, ctx:BiastGrammarParser.Typeof_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#named_struct_specifier.
    def visitNamed_struct_specifier(self, ctx:BiastGrammarParser.Named_struct_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#anonymous_struct_specifier.
    def visitAnonymous_struct_specifier(self, ctx:BiastGrammarParser.Anonymous_struct_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#named_union_specifier.
    def visitNamed_union_specifier(self, ctx:BiastGrammarParser.Named_union_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#anonymous_union_specifier.
    def visitAnonymous_union_specifier(self, ctx:BiastGrammarParser.Anonymous_union_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#anonymous_enum_specifier.
    def visitAnonymous_enum_specifier(self, ctx:BiastGrammarParser.Anonymous_enum_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#named_enum_specifier.
    def visitNamed_enum_specifier(self, ctx:BiastGrammarParser.Named_enum_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#field_declarations.
    def visitField_declarations(self, ctx:BiastGrammarParser.Field_declarationsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_field_declaration.
    def visitRegular_field_declaration(self, ctx:BiastGrammarParser.Regular_field_declarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#unamed_struct_field.
    def visitUnamed_struct_field(self, ctx:BiastGrammarParser.Unamed_struct_fieldContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#unamed_union_field.
    def visitUnamed_union_field(self, ctx:BiastGrammarParser.Unamed_union_fieldContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#empty_field.
    def visitEmpty_field(self, ctx:BiastGrammarParser.Empty_fieldContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_field.
    def visitMacro_field(self, ctx:BiastGrammarParser.Macro_fieldContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#field_declarators.
    def visitField_declarators(self, ctx:BiastGrammarParser.Field_declaratorsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#bitfield_descriptor.
    def visitBitfield_descriptor(self, ctx:BiastGrammarParser.Bitfield_descriptorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#named_field_declarator.
    def visitNamed_field_declarator(self, ctx:BiastGrammarParser.Named_field_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#anonymous_bit_field_declarator.
    def visitAnonymous_bit_field_declarator(self, ctx:BiastGrammarParser.Anonymous_bit_field_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#pointer_descriptors.
    def visitPointer_descriptors(self, ctx:BiastGrammarParser.Pointer_descriptorsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#pointer_descriptor.
    def visitPointer_descriptor(self, ctx:BiastGrammarParser.Pointer_descriptorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#pointer_abstract_declarator.
    def visitPointer_abstract_declarator(self, ctx:BiastGrammarParser.Pointer_abstract_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#pointer_complex_abstract_declarator.
    def visitPointer_complex_abstract_declarator(self, ctx:BiastGrammarParser.Pointer_complex_abstract_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#nested_array_abstract_declarator.
    def visitNested_array_abstract_declarator(self, ctx:BiastGrammarParser.Nested_array_abstract_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#nested_function_abstract_declarator.
    def visitNested_function_abstract_declarator(self, ctx:BiastGrammarParser.Nested_function_abstract_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#single_declarator.
    def visitSingle_declarator(self, ctx:BiastGrammarParser.Single_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#array_abstract_declarator.
    def visitArray_abstract_declarator(self, ctx:BiastGrammarParser.Array_abstract_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#function_abstract_declarator.
    def visitFunction_abstract_declarator(self, ctx:BiastGrammarParser.Function_abstract_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#declarator.
    def visitDeclarator(self, ctx:BiastGrammarParser.DeclaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#named_declarator.
    def visitNamed_declarator(self, ctx:BiastGrammarParser.Named_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#function_declarator.
    def visitFunction_declarator(self, ctx:BiastGrammarParser.Function_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#brace_declarator.
    def visitBrace_declarator(self, ctx:BiastGrammarParser.Brace_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#array_declarator.
    def visitArray_declarator(self, ctx:BiastGrammarParser.Array_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#empty_parameter_declarations.
    def visitEmpty_parameter_declarations(self, ctx:BiastGrammarParser.Empty_parameter_declarationsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_parameter_declarations.
    def visitRegular_parameter_declarations(self, ctx:BiastGrammarParser.Regular_parameter_declarationsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_parameter_declaration.
    def visitRegular_parameter_declaration(self, ctx:BiastGrammarParser.Regular_parameter_declarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_parameter.
    def visitMacro_parameter(self, ctx:BiastGrammarParser.Macro_parameterContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#enumerators.
    def visitEnumerators(self, ctx:BiastGrammarParser.EnumeratorsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#named_enum_item.
    def visitNamed_enum_item(self, ctx:BiastGrammarParser.Named_enum_itemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#assigned_enum_item.
    def visitAssigned_enum_item(self, ctx:BiastGrammarParser.Assigned_enum_itemContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#type_name.
    def visitType_name(self, ctx:BiastGrammarParser.Type_nameContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#specifiers_qualifiers_alias.
    def visitSpecifiers_qualifiers_alias(self, ctx:BiastGrammarParser.Specifiers_qualifiers_aliasContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#specifiers_qualifiers.
    def visitSpecifiers_qualifiers(self, ctx:BiastGrammarParser.Specifiers_qualifiersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#string_literal.
    def visitString_literal(self, ctx:BiastGrammarParser.String_literalContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#direct_specifier_qualifier.
    def visitDirect_specifier_qualifier(self, ctx:BiastGrammarParser.Direct_specifier_qualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#attribute_specifiers.
    def visitAttribute_specifiers(self, ctx:BiastGrammarParser.Attribute_specifiersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_attribute_specifier.
    def visitRegular_attribute_specifier(self, ctx:BiastGrammarParser.Regular_attribute_specifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#macro_attribute.
    def visitMacro_attribute(self, ctx:BiastGrammarParser.Macro_attributeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#attributes.
    def visitAttributes(self, ctx:BiastGrammarParser.AttributesContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#empty_attr.
    def visitEmpty_attr(self, ctx:BiastGrammarParser.Empty_attrContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#regular_attr.
    def visitRegular_attr(self, ctx:BiastGrammarParser.Regular_attrContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#attribute_parameters.
    def visitAttribute_parameters(self, ctx:BiastGrammarParser.Attribute_parametersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by BiastGrammarParser#asm_keyword.
    def visitAsm_keyword(self, ctx:BiastGrammarParser.Asm_keywordContext):
        return self.visitChildren(ctx)



del BiastGrammarParser