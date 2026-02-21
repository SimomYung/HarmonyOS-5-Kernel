/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Biast grammar
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 15 17:10:55 2022
 */

grammar BiastGrammar;

translation_unit : decls+=external_declaration* EOF;

external_declaration: function_definition
    | declaration
    | SEMICOLON
    ;

function_definition: declaration_specifiers declarator declarations? compound_statement attribute_specifiers?;

function_specifier: INLINE;

declaration: declaration_specifiers (init_declarators)? SEMICOLON              # c_declaration
    | MACRODECL (LBRACE builtin_parameters RBRACE)?                            # macro_declaration
    ;

declarations: decls+=declaration+;

nontype_declaration_specifier: storage_class_specifier
    | type_qualifier
    | function_specifier
    | attribute_specifier
    ;

nontypename_declaration_specifier: nontype_declaration_specifier
    | direct_type_specifier
    ;

// qualifier and specifiers without type specifiers (e.g. T and int)
nontype_declaration_specifiers: spcs+=nontype_declaration_specifier+;

nontypename_declaration_specifiers: spcs+=nontypename_declaration_specifier+;

declaration_specifiers: type_alias_specifier nontype_declaration_specifiers?    # declaration_specifiers_alias
    | direct_type_specifier nontypename_declaration_specifiers?                 # declaration_specifiers_nonname
    | nontype_declaration_specifier declaration_specifiers?                     # declaration_specifiers_nontype
    ;

init_declarators: decltor+=init_declarator (COMMA decltor+=init_declarator)*;

init_declarator: declarator (ASSIGN initializer attribute_specifiers?)?;

initializer: assignment_expression                                             # expr_initializer
    | LBRACKET designated_initializers? COMMA? RBRACKET                        # bracket_initializer
    ;

designated_initializers: inits+=designated_initializer (COMMA inits+=designated_initializer)*;

designated_initializer: designation? initializer;

designation: dsnrs+=designator+ '=';

designator: LSQUARE constant_expression ('...' constant_expression)? RSQUARE   # range_designator
          | DOT CNAME                                                          # member_designator
          ;

statement: expression_statement
         | labelled_statement
         | compound_statement
         | selection_statement
         | iteration_statement
         | control_statement
         | asm_statement
         | null_statement
         | macro_statement
         ;

expression_statement: expression SEMICOLON;

null_statement: SEMICOLON;

macro_statement: MACROSTMT (LBRACE macro_parameters RBRACE)?;

labelled_statement: CNAME COLON statement;

switch_body: LBRACKET (statement|case_statement)* RBRACKET;

case_statement: CASE constant_expression COLON statement                      # non_default_case_statement
              | DEFAULT COLON statement                                       # default_case_statement
              ;

compound_statement: LBRACKET (statement|declaration)* RBRACKET;

selection_statement: IF condition statement                                   # if_statement
    | IF condition statement ELSE statement                                   # if_statement_else
    | SWITCH condition switch_body                                            # switch_statement
    ;

iteration_statement: WHILE condition statement                                # while_statement
    | DO statement WHILE condition SEMICOLON                                  # do_while_statement
    | FOR LBRACE decl=expression? SEMICOLON cond=expression? SEMICOLON incr=expression? RBRACE statement       # for_statement
    | FOR LBRACE declaration cond=expression? SEMICOLON incr=expression? RBRACE statement                      # for_statement_decl
    ;

control_statement: GOTO CNAME SEMICOLON                                       # goto_statement
    | CONTINUE SEMICOLON                                                      # continue_statement
    | BREAK SEMICOLON                                                         # break_statement
    | RETURN expression? SEMICOLON                                            # return_statement
    ;

asm_statement: asm_keyword asm_qualifiers LBRACE
    string_literal (op1=asm_operands (op2=asm_operands (asm_clobbers asm_gotolabels?)?)?)?
    RBRACE SEMICOLON;

asm_qualifiers: asms+=asm_qualifier*;

asm_qualifier: VOLATILE                                                       # asm_volatile_qualifier
             | INLINE                                                         # asm_inline_qualifier
             | GOTO                                                           # asm_goto_qualifier
             ;

asm_operands: COLON (ops+=asm_operand (COMMA ops+=asm_operand)*)?;

asm_operand: (LSQUARE CNAME RSQUARE)? string_literal LBRACE expression RBRACE;

asm_clobbers: COLON (clobber+=string_literal (COMMA clobber+=string_literal)*)?;

asm_gotolabels: COLON (labels+=CNAME (COMMA labels+=CNAME)*)?;

condition: LBRACE expression RBRACE;

constant_expression: conditional_expression;

expression: assignment_expression                                             # regular_expression
          | expression COMMA assignment_expression                            # sequence_expression
          ;

assignment_expression: conditional_expression                                 # regular_assign
    | unary_expression assignment_operator assignment_expression              # full_assign
    ;

assignment_operator: ASSIGN|MUTASSIGN|DIVASSIGN|MODASSIGN
                   |ADDASSIGN|SUBASSIGN|RSHIFTASSIGN
                   |LSHIFTASSIGN|BANDASSIGN|BORASSIGN|XORASSIGN;

conditional_expression: logical_or_expression                                 # regular_condition
    | logical_or_expression QUESTION expression COLON conditional_expression  # ternary_expression
    ;

logical_or_expression: logical_and_expression                                 # regular_lor
    | logical_or_expression LOR logical_and_expression                        # full_lor
    ;

logical_and_expression: or_expression                                         # regular_land
    | logical_and_expression LAND or_expression                               # full_land
    ;

or_expression: xor_expression                                                # regular_bor
    | or_expression BOR xor_expression                                       # full_bor
    ;

xor_expression: and_expression                                               # regular_xor
    | xor_expression XOR and_expression                                      # full_xor
    ;

and_expression: equality_expression                                          # regular_band
    | and_expression BAND equality_expression                                # full_band
    ;

equality_expression: relational_expression                                   # regular_eq
    | equality_expression (EQ|NE) relational_expression                      # full_eq
    ;

relational_expression: shift_expression                                      # regular_compare
    | relational_expression (LT|GT|LE|GE) shift_expression                   # full_compare
    ;

shift_expression: additive_expression                                        # regualr_shift
    | shift_expression (RSHIFT|LSHIFT) additive_expression                   # full_shift
    ;

additive_expression: multiplicative_expression                               # regular_add
    | additive_expression (ADD|SUB) multiplicative_expression                # full_add
    ;

multiplicative_expression: cast_expression                                   # regular_mul
    | multiplicative_expression (MUL|DIV|MOD) cast_expression                # full_mul
    ;

cast_expression: unary_expression                                            # regualr_cast
    | LBRACE type_name RBRACE cast_expression                                # explicit_cast_expression
    ;

unary_expression: postfix_expression                                         # regular_unary
    | (SELFINC|SELFDEC) unary_expression                                     # selfinc_unary
    | (ADD|SUB|BNOT|LNOT) cast_expression                                    # add_unary
    | BAND cast_expression                                                   # band_unary
    | MUL cast_expression                                                    # mul_unary
    | SIZEOF unary_expression                                                # sizeof_unary
    | SIZEOF LBRACE type_name RBRACE                                         # sizeof_type
    ;

argument_expression_list: assigns+=assignment_expression (COMMA assigns+=assignment_expression)*;

postfix_expression: primary_expression                                          # pri_expression
    | postfix_expression LSQUARE expression RSQUARE                             # array_reference
    | postfix_expression LBRACE argument_expression_list? RBRACE                # function_call
    | postfix_expression DOT CNAME                                              # object_member
    | postfix_expression ARROW CNAME                                            # pointer_object_member
    | postfix_expression (SELFINC|SELFDEC)                                      # postfix_unary_expression
    | MACROEXPR                                                                 # macro_expr
    | MACROEXPR LBRACE builtin_parameters RBRACE                                # macro_call
    | LBRACE type_name RBRACE LBRACKET designated_initializers COMMA? RBRACKET  # compound_literal
    | BUILTIN LBRACE builtin_parameters RBRACE                                  # builtin_expression
    ;

builtin_parameter: assignment_expression
                 | type_name
                 ;

builtin_parameters: params+=builtin_parameter (COMMA params+=builtin_parameter)*;

macro_parameters: builtin_parameters;

primary_expression: constant                                                   # constant_as_pri_expression
                  | CNAME                                                      # identifier_expression
                  | string_literal                                             # strings
                  | LBRACE expression RBRACE                                   # sigle_expression
                  | LBRACE compound_statement RBRACE                           # compound_expression
                  ;

constant: integer_literal                                                      # integer_constant
        | CHAR_LITERAL                                                         # char_constant
        | FLOAT_LITERAL                                                        # floating_constant
        ;

integer_literal: INT_LITERAL                                                   # decimal_literal
               | HEX_LITERAL                                                   # hexadecimal_literal
               | OCT_LITERAL                                                   # octal_literal
               | BIN_LITERAL                                                   # binary_literal
               ;

storage_class_specifier: AUTO | REGISTER | STATIC | EXTERN | TYPEDEF;

type_qualifier: CONST | VOLATILE;

type_specifier: type_alias_specifier
    | direct_type_specifier
    ;

direct_type_specifier: VOID                                                   # single_lex_spec
    | BOOL                                                                    # single_lex_spec
    | CHAR                                                                    # single_lex_spec
    | SHORT                                                                   # single_lex_spec
    | INT                                                                     # single_lex_spec
    | LONG                                                                    # single_lex_spec
    | FLOAT                                                                   # single_lex_spec
    | DOUBLE                                                                  # single_lex_spec
    | SIGNED                                                                  # single_lex_spec
    | UNSIGNED                                                                # single_lex_spec
    | MACROTYPE (LBRACE params+=builtin_parameters RBRACE)?                   # type_macro_specifier
    | struct_specifier                                                        # return_single
    | union_specifier                                                         # return_single
    | enum_specifier                                                          # return_single
    | typeof_specifier                                                        # return_single
    ;

type_alias_specifier: CNAME;

typeof_specifier: TYPEOF LBRACE (type_name | expression) RBRACE;

struct_specifier: STRUCT attr_head=attribute_specifiers? CNAME (field_declarations attr_end=attribute_specifiers?)?    # named_struct_specifier
    | STRUCT attr_head=attribute_specifiers? field_declarations attr_end=attribute_specifiers?                         # anonymous_struct_specifier
    ;

union_specifier: UNION attr_head=attribute_specifiers? CNAME (field_declarations attr_end=attribute_specifiers?)?      # named_union_specifier
    | UNION attr_head=attribute_specifiers? field_declarations attr_end=attribute_specifiers?                          # anonymous_union_specifier
    ;

enum_specifier: ENUM attr_head=attribute_specifiers? enumerators attr_end=attribute_specifiers?    # anonymous_enum_specifier
    | ENUM attr_head=attribute_specifiers? CNAME (enumerators attr_end=attribute_specifiers?)?     # named_enum_specifier
    ;

field_declarations: LBRACKET f_decls+=field_declaration* RBRACKET;

field_declaration: specifier_qualifiers field_declarators SEMICOLON           # regular_field_declaration
    | struct_specifier SEMICOLON                                              # unamed_struct_field
    | union_specifier SEMICOLON                                               # unamed_union_field
    | SEMICOLON                                                               # empty_field
    | MACRODECL (LBRACE builtin_parameters RBRACE)?                           # macro_field
    ;

field_declarators: fields+=field_declarator (COMMA fields+=field_declarator)*;

bitfield_descriptor: COLON constant_expression;

field_declarator: declarator bitfield_descriptor?                             # named_field_declarator
                | bitfield_descriptor                                         # anonymous_bit_field_declarator
                ;

pointer_descriptors: des+=pointer_descriptor+;

pointer_descriptor: MUL tqs+=type_qualifier*;

abstract_declarator: pointer_descriptors attribute_specifiers?                                                         # pointer_abstract_declarator
    | pointer_descriptors? attr_head=attribute_specifiers? direct_abstract_declarator attr_end=attribute_specifiers?   # pointer_complex_abstract_declarator
    ;

direct_abstract_declarator: LBRACE abstract_declarator RBRACE                 # single_declarator
    | LSQUARE expression? RSQUARE                                             # array_abstract_declarator
    | direct_abstract_declarator LSQUARE expression? RSQUARE                  # nested_array_abstract_declarator
    | LBRACE parameter_declarations RBRACE                                    # function_abstract_declarator
    | direct_abstract_declarator LBRACE parameter_declarations RBRACE         # nested_function_abstract_declarator
    ;

declarator: pointer_descriptors? attr_head=attribute_specifiers? direct_declarator attr_end=attribute_specifiers?;

direct_declarator: CNAME                                                       # named_declarator
                 | LBRACE declarator RBRACE                                    # brace_declarator
                 | direct_declarator LSQUARE expression? RSQUARE               # array_declarator
                 | direct_declarator LBRACE parameter_declarations RBRACE      # function_declarator
                 ;

parameter_declarations:                                                                   # empty_parameter_declarations
    | pds+=parameter_declaration (COMMA pds+=parameter_declaration)* (COMMA AND_MORE)?    # regular_parameter_declarations
    ;

parameter_declaration: declaration_specifiers (declarator|abstract_declarator)?  # regular_parameter_declaration
    | MACRODECL (LBRACE builtin_parameters RBRACE)?                              # macro_parameter
    ;

enumerators: LBRACKET etr+=enumerator (COMMA etr+=enumerator)* COMMA? RBRACKET;

enumerator: CNAME                                                             # named_enum_item
          | CNAME ASSIGN constant_expression                                  # assigned_enum_item
          ;

type_name: specifier_qualifiers abstract_declarator?;

specifier_qualifiers: (type_qualifier | type_alias_specifier) specifier_qualifiers?       # specifiers_qualifiers_alias
                    | direct_type_specifier qs+=direct_specifier_qualifier*               # specifiers_qualifiers
                    ;

string_literal: sls+=STRING_LITERAL+;

direct_specifier_qualifier: type_qualifier|direct_type_specifier;

attribute_specifiers: attribute_specifier+;

attribute_specifier: ATTRIBUTE LBRACE LBRACE (attributes|) RBRACE RBRACE      # regular_attribute_specifier
                   | MACROATTR (LBRACE builtin_parameters RBRACE)?            # macro_attribute
                   ;

attributes: attrs+=attribute (COMMA attrs+=attribute)*;

attribute:                                                                    # empty_attr
         | CNAME (LBRACE attribute_parameters RBRACE)?                        # regular_attr
         ;

attribute_parameters: argument_expression_list?;

BUILTIN : '__builtin_va_arg'
          | '__builtin_offsetof'
          | '__builtin_types_compatible_p'
          ;
SIZEOF : 'sizeof'
       | '__sizeof__'
       ;
TYPEOF      : 'typeof' | '__typeof__';
MACROEXPR       : '@' CNAME ':expr';
MACROTYPE       : '@' CNAME ':type';
MACROSTMT       : '@' CNAME ':stmt';
MACRODECL       : '@' CNAME ':decl';
MACROATTR       : '@' CNAME ':attr';

HEX_LITERAL     : HEX_PREFIX HEX_DIGIT+ INTEGER_SUFFIX?;
CHAR_LITERAL    : SQUOTE C_CHAR SQUOTE;
STRING_LITERAL  : DQUOTE S_CHAR* DQUOTE;
FLOAT_LITERAL   : DIGIT* DOT DIGIT+ EXPONENT? FLOATING_SUFFIX?
                | DIGIT+ DOT EXPONENT? FLOATING_SUFFIX?
                | DIGIT+ EXPONENT FLOATING_SUFFIX?
                ;

LOR         : '||';
LAND        : '&&';
LE          : '<=';
GE          : '>=';
LNOT        : '!';
BOR         : '|';
XOR         : '^';
BAND        : '&';
BNOT        : '~';
LT          : '<';
GT          : '>';

ATTRIBUTE : '__attribute__';

VOID        : 'void';
CHAR        : 'char';
SHORT       : 'short';
INT         : 'int';
LONG        : 'long';
FLOAT       : 'float';
DOUBLE      : 'double';
SIGNED      : 'signed';
UNSIGNED    : 'unsigned';
BOOL        : '_Bool';
STRUCT      : 'struct';
UNION       : 'union';
ENUM        : 'enum';

CONST       : 'const';
VOLATILE    : 'volatile';

SWITCH      : 'switch';
CASE        : 'case';
DEFAULT     : 'default';
IF          : 'if';
ELSE        : 'else';
WHILE       : 'while';
DO          : 'do';
FOR         : 'for';
asm_keyword : 'asm' | '__asm__';

GOTO        : 'goto';
BREAK       : 'break';
CONTINUE    : 'continue';
RETURN      : 'return';

AUTO        : 'auto';
REGISTER    : 'register';
STATIC      : 'static';
EXTERN      : 'extern';
TYPEDEF     : 'typedef';
INLINE      : 'inline';

SEMICOLON   : ';';
COLON       : ':';
QUESTION    : '?';
COMMA       : ',';
DOT         : '.';
SQUOTE      : '\'';
DQUOTE      : '"';
ARROW       : '->';

LBRACE      : '(';
RBRACE      : ')';
LBRACKET    : '{';
RBRACKET    : '}';
LSQUARE     : '[';
RSQUARE     : ']';

ADD         : '+';
SUB         : '-';
MUL         : '*';
DIV         : '/';
MOD         : '%';
EQ          : '==';
NE          : '!=';
LSHIFT      : '<<';
RSHIFT      : '>>';

ASSIGN      : '=';
MUTASSIGN   : '*=';
DIVASSIGN   : '/=';
MODASSIGN   : '%=';
ADDASSIGN   : '+=';
SUBASSIGN   : '-=';
RSHIFTASSIGN: '>>=';
LSHIFTASSIGN: '<<=';
BANDASSIGN  : '&=';
BORASSIGN   : '|=';
XORASSIGN   : '^=';

SELFINC     : '++';
SELFDEC     : '--';

AND_MORE    : '...';


WS : [ \t\f\r\n]+ -> skip;
CNAME           : ('_'|LETTER) ('_'|LETTER|DIGIT)*;


BIN_LITERAL     : BIN_PREFIX ('0' | '1')+ INTEGER_SUFFIX?;
INT_LITERAL     : NONZERO_DIGIT DIGIT* INTEGER_SUFFIX?;
OCT_LITERAL     : ZERO OCT_DIGIT* INTEGER_SUFFIX?;

EXPONENT        : ('e' | 'E') (ADD | SUB)? DIGIT+;

U_SUFFIX        : 'u' | 'U';
L_SUFFIX        : ('l' 'l'?) | ('L' 'L'?);

INTEGER_SUFFIX  : (U_SUFFIX L_SUFFIX? | L_SUFFIX U_SUFFIX?);
FLOATING_SUFFIX : 'f' | 'l' | 'F' | 'L';

HEX_DIGIT       : DIGIT | [a-f] | [A-F];

LETTER          : UCASE_LETTER | LCASE_LETTER;
ZERO            : '0';
fragment DIGIT  : [0-9];
NONZERO_DIGIT   : [1-9];
LCASE_LETTER    : [a-z];
UCASE_LETTER    : [A-Z];
HEX_PREFIX      : '0x' | '0X';
BIN_PREFIX      : '0b' | '0B';
OCT_DIGIT       : [0-7];

C_CHAR          : ~['\\]
                | ESCAPE_SEQUENCE
                ;

S_CHAR          : ~["\\]
                | ESCAPE_SEQUENCE
                ;

ESCAPE_SEQUENCE : '\\\'' | '\\"' | '\\?' | '\\\\'
                | '\\a' | '\\b' | '\\f' | '\\n'
                | '\\r' | '\\t' | '\\v'
                | '\\\\' OCT_DIGIT OCT_DIGIT? OCT_DIGIT?
                | '\\x' HEX_DIGIT+
                | '\\u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
                | '\\U' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
                ;
/** Comments inside .sysif are dropped for now.
 * But in case there might be any usage in the future, we still put them in HIDDEN channel. */
COMMENT : ('//' .*? '\n' | '/*' .*? '*/') -> channel(HIDDEN);
