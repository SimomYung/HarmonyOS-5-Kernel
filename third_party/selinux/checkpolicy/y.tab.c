/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 32 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"

#include <sys/types.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <sepol/policydb/expand.h>
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/services.h>
#include <sepol/policydb/conditional.h>
#include <sepol/policydb/hierarchy.h>
#include <sepol/policydb/polcaps.h>
#include "queue.h"
#include "checkpolicy.h"
#include "module_compiler.h"
#include "policy_define.h"

extern policydb_t *policydbp;
extern unsigned int pass;

extern char yytext[];
extern int yylex(void);
extern int yywarn(const char *msg);
extern int yyerror(const char *msg);

typedef int (* require_func_t)(int pass);


#line 107 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "y.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PATH = 3,                       /* PATH  */
  YYSYMBOL_QPATH = 4,                      /* QPATH  */
  YYSYMBOL_FILENAME = 5,                   /* FILENAME  */
  YYSYMBOL_CLONE = 6,                      /* CLONE  */
  YYSYMBOL_COMMON = 7,                     /* COMMON  */
  YYSYMBOL_CLASS = 8,                      /* CLASS  */
  YYSYMBOL_CONSTRAIN = 9,                  /* CONSTRAIN  */
  YYSYMBOL_VALIDATETRANS = 10,             /* VALIDATETRANS  */
  YYSYMBOL_INHERITS = 11,                  /* INHERITS  */
  YYSYMBOL_SID = 12,                       /* SID  */
  YYSYMBOL_ROLE = 13,                      /* ROLE  */
  YYSYMBOL_ROLEATTRIBUTE = 14,             /* ROLEATTRIBUTE  */
  YYSYMBOL_ATTRIBUTE_ROLE = 15,            /* ATTRIBUTE_ROLE  */
  YYSYMBOL_ROLES = 16,                     /* ROLES  */
  YYSYMBOL_TYPEALIAS = 17,                 /* TYPEALIAS  */
  YYSYMBOL_TYPEATTRIBUTE = 18,             /* TYPEATTRIBUTE  */
  YYSYMBOL_TYPEBOUNDS = 19,                /* TYPEBOUNDS  */
  YYSYMBOL_TYPE = 20,                      /* TYPE  */
  YYSYMBOL_TYPES = 21,                     /* TYPES  */
  YYSYMBOL_ALIAS = 22,                     /* ALIAS  */
  YYSYMBOL_ATTRIBUTE = 23,                 /* ATTRIBUTE  */
  YYSYMBOL_EXPANDATTRIBUTE = 24,           /* EXPANDATTRIBUTE  */
  YYSYMBOL_BOOL = 25,                      /* BOOL  */
  YYSYMBOL_TUNABLE = 26,                   /* TUNABLE  */
  YYSYMBOL_IF = 27,                        /* IF  */
  YYSYMBOL_ELSE = 28,                      /* ELSE  */
  YYSYMBOL_TYPE_TRANSITION = 29,           /* TYPE_TRANSITION  */
  YYSYMBOL_TYPE_MEMBER = 30,               /* TYPE_MEMBER  */
  YYSYMBOL_TYPE_CHANGE = 31,               /* TYPE_CHANGE  */
  YYSYMBOL_ROLE_TRANSITION = 32,           /* ROLE_TRANSITION  */
  YYSYMBOL_RANGE_TRANSITION = 33,          /* RANGE_TRANSITION  */
  YYSYMBOL_SENSITIVITY = 34,               /* SENSITIVITY  */
  YYSYMBOL_DOMINANCE = 35,                 /* DOMINANCE  */
  YYSYMBOL_DOM = 36,                       /* DOM  */
  YYSYMBOL_DOMBY = 37,                     /* DOMBY  */
  YYSYMBOL_INCOMP = 38,                    /* INCOMP  */
  YYSYMBOL_CATEGORY = 39,                  /* CATEGORY  */
  YYSYMBOL_LEVEL = 40,                     /* LEVEL  */
  YYSYMBOL_RANGE = 41,                     /* RANGE  */
  YYSYMBOL_MLSCONSTRAIN = 42,              /* MLSCONSTRAIN  */
  YYSYMBOL_MLSVALIDATETRANS = 43,          /* MLSVALIDATETRANS  */
  YYSYMBOL_USER = 44,                      /* USER  */
  YYSYMBOL_NEVERALLOW = 45,                /* NEVERALLOW  */
  YYSYMBOL_ALLOW = 46,                     /* ALLOW  */
  YYSYMBOL_AUDITALLOW = 47,                /* AUDITALLOW  */
  YYSYMBOL_AUDITDENY = 48,                 /* AUDITDENY  */
  YYSYMBOL_DONTAUDIT = 49,                 /* DONTAUDIT  */
  YYSYMBOL_ALLOWXPERM = 50,                /* ALLOWXPERM  */
  YYSYMBOL_AUDITALLOWXPERM = 51,           /* AUDITALLOWXPERM  */
  YYSYMBOL_DONTAUDITXPERM = 52,            /* DONTAUDITXPERM  */
  YYSYMBOL_NEVERALLOWXPERM = 53,           /* NEVERALLOWXPERM  */
  YYSYMBOL_SOURCE = 54,                    /* SOURCE  */
  YYSYMBOL_TARGET = 55,                    /* TARGET  */
  YYSYMBOL_SAMEUSER = 56,                  /* SAMEUSER  */
  YYSYMBOL_FSCON = 57,                     /* FSCON  */
  YYSYMBOL_PORTCON = 58,                   /* PORTCON  */
  YYSYMBOL_NETIFCON = 59,                  /* NETIFCON  */
  YYSYMBOL_NODECON = 60,                   /* NODECON  */
  YYSYMBOL_IBPKEYCON = 61,                 /* IBPKEYCON  */
  YYSYMBOL_IBENDPORTCON = 62,              /* IBENDPORTCON  */
  YYSYMBOL_PIRQCON = 63,                   /* PIRQCON  */
  YYSYMBOL_IOMEMCON = 64,                  /* IOMEMCON  */
  YYSYMBOL_IOPORTCON = 65,                 /* IOPORTCON  */
  YYSYMBOL_PCIDEVICECON = 66,              /* PCIDEVICECON  */
  YYSYMBOL_DEVICETREECON = 67,             /* DEVICETREECON  */
  YYSYMBOL_FSUSEXATTR = 68,                /* FSUSEXATTR  */
  YYSYMBOL_FSUSETASK = 69,                 /* FSUSETASK  */
  YYSYMBOL_FSUSETRANS = 70,                /* FSUSETRANS  */
  YYSYMBOL_GENFSCON = 71,                  /* GENFSCON  */
  YYSYMBOL_U1 = 72,                        /* U1  */
  YYSYMBOL_U2 = 73,                        /* U2  */
  YYSYMBOL_U3 = 74,                        /* U3  */
  YYSYMBOL_R1 = 75,                        /* R1  */
  YYSYMBOL_R2 = 76,                        /* R2  */
  YYSYMBOL_R3 = 77,                        /* R3  */
  YYSYMBOL_T1 = 78,                        /* T1  */
  YYSYMBOL_T2 = 79,                        /* T2  */
  YYSYMBOL_T3 = 80,                        /* T3  */
  YYSYMBOL_L1 = 81,                        /* L1  */
  YYSYMBOL_L2 = 82,                        /* L2  */
  YYSYMBOL_H1 = 83,                        /* H1  */
  YYSYMBOL_H2 = 84,                        /* H2  */
  YYSYMBOL_NOT = 85,                       /* NOT  */
  YYSYMBOL_AND = 86,                       /* AND  */
  YYSYMBOL_OR = 87,                        /* OR  */
  YYSYMBOL_XOR = 88,                       /* XOR  */
  YYSYMBOL_CTRUE = 89,                     /* CTRUE  */
  YYSYMBOL_CFALSE = 90,                    /* CFALSE  */
  YYSYMBOL_IDENTIFIER = 91,                /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 92,                    /* NUMBER  */
  YYSYMBOL_EQUALS = 93,                    /* EQUALS  */
  YYSYMBOL_NOTEQUAL = 94,                  /* NOTEQUAL  */
  YYSYMBOL_IPV4_ADDR = 95,                 /* IPV4_ADDR  */
  YYSYMBOL_IPV6_ADDR = 96,                 /* IPV6_ADDR  */
  YYSYMBOL_MODULE = 97,                    /* MODULE  */
  YYSYMBOL_VERSION_IDENTIFIER = 98,        /* VERSION_IDENTIFIER  */
  YYSYMBOL_REQUIRE = 99,                   /* REQUIRE  */
  YYSYMBOL_OPTIONAL = 100,                 /* OPTIONAL  */
  YYSYMBOL_POLICYCAP = 101,                /* POLICYCAP  */
  YYSYMBOL_PERMISSIVE = 102,               /* PERMISSIVE  */
  YYSYMBOL_FILESYSTEM = 103,               /* FILESYSTEM  */
  YYSYMBOL_DEFAULT_USER = 104,             /* DEFAULT_USER  */
  YYSYMBOL_DEFAULT_ROLE = 105,             /* DEFAULT_ROLE  */
  YYSYMBOL_DEFAULT_TYPE = 106,             /* DEFAULT_TYPE  */
  YYSYMBOL_DEFAULT_RANGE = 107,            /* DEFAULT_RANGE  */
  YYSYMBOL_LOW_HIGH = 108,                 /* LOW_HIGH  */
  YYSYMBOL_LOW = 109,                      /* LOW  */
  YYSYMBOL_HIGH = 110,                     /* HIGH  */
  YYSYMBOL_GLBLUB = 111,                   /* GLBLUB  */
  YYSYMBOL_112_ = 112,                     /* '{'  */
  YYSYMBOL_113_ = 113,                     /* '}'  */
  YYSYMBOL_114_ = 114,                     /* ';'  */
  YYSYMBOL_115_ = 115,                     /* ':'  */
  YYSYMBOL_116_ = 116,                     /* ','  */
  YYSYMBOL_117_ = 117,                     /* '('  */
  YYSYMBOL_118_ = 118,                     /* ')'  */
  YYSYMBOL_119_ = 119,                     /* '-'  */
  YYSYMBOL_120_ = 120,                     /* '~'  */
  YYSYMBOL_121_ = 121,                     /* '*'  */
  YYSYMBOL_YYACCEPT = 122,                 /* $accept  */
  YYSYMBOL_policy = 123,                   /* policy  */
  YYSYMBOL_base_policy = 124,              /* base_policy  */
  YYSYMBOL_125_1 = 125,                    /* $@1  */
  YYSYMBOL_126_2 = 126,                    /* $@2  */
  YYSYMBOL_127_3 = 127,                    /* $@3  */
  YYSYMBOL_classes = 128,                  /* classes  */
  YYSYMBOL_class_def = 129,                /* class_def  */
  YYSYMBOL_initial_sids = 130,             /* initial_sids  */
  YYSYMBOL_initial_sid_def = 131,          /* initial_sid_def  */
  YYSYMBOL_access_vectors = 132,           /* access_vectors  */
  YYSYMBOL_opt_common_perms = 133,         /* opt_common_perms  */
  YYSYMBOL_common_perms = 134,             /* common_perms  */
  YYSYMBOL_common_perms_def = 135,         /* common_perms_def  */
  YYSYMBOL_av_perms = 136,                 /* av_perms  */
  YYSYMBOL_av_perms_def = 137,             /* av_perms_def  */
  YYSYMBOL_opt_default_rules = 138,        /* opt_default_rules  */
  YYSYMBOL_default_rules = 139,            /* default_rules  */
  YYSYMBOL_default_user_def = 140,         /* default_user_def  */
  YYSYMBOL_default_role_def = 141,         /* default_role_def  */
  YYSYMBOL_default_type_def = 142,         /* default_type_def  */
  YYSYMBOL_default_range_def = 143,        /* default_range_def  */
  YYSYMBOL_opt_mls = 144,                  /* opt_mls  */
  YYSYMBOL_mls = 145,                      /* mls  */
  YYSYMBOL_sensitivities = 146,            /* sensitivities  */
  YYSYMBOL_sensitivity_def = 147,          /* sensitivity_def  */
  YYSYMBOL_alias_def = 148,                /* alias_def  */
  YYSYMBOL_dominance = 149,                /* dominance  */
  YYSYMBOL_opt_categories = 150,           /* opt_categories  */
  YYSYMBOL_categories = 151,               /* categories  */
  YYSYMBOL_category_def = 152,             /* category_def  */
  YYSYMBOL_levels = 153,                   /* levels  */
  YYSYMBOL_level_def = 154,                /* level_def  */
  YYSYMBOL_mlspolicy = 155,                /* mlspolicy  */
  YYSYMBOL_mlspolicy_decl = 156,           /* mlspolicy_decl  */
  YYSYMBOL_mlsconstraint_def = 157,        /* mlsconstraint_def  */
  YYSYMBOL_mlsvalidatetrans_def = 158,     /* mlsvalidatetrans_def  */
  YYSYMBOL_te_rbac = 159,                  /* te_rbac  */
  YYSYMBOL_te_rbac_decl = 160,             /* te_rbac_decl  */
  YYSYMBOL_rbac_decl = 161,                /* rbac_decl  */
  YYSYMBOL_te_decl = 162,                  /* te_decl  */
  YYSYMBOL_attribute_def = 163,            /* attribute_def  */
  YYSYMBOL_expandattribute_def = 164,      /* expandattribute_def  */
  YYSYMBOL_type_def = 165,                 /* type_def  */
  YYSYMBOL_typealias_def = 166,            /* typealias_def  */
  YYSYMBOL_typeattribute_def = 167,        /* typeattribute_def  */
  YYSYMBOL_typebounds_def = 168,           /* typebounds_def  */
  YYSYMBOL_opt_attr_list = 169,            /* opt_attr_list  */
  YYSYMBOL_bool_def = 170,                 /* bool_def  */
  YYSYMBOL_tunable_def = 171,              /* tunable_def  */
  YYSYMBOL_bool_val = 172,                 /* bool_val  */
  YYSYMBOL_cond_stmt_def = 173,            /* cond_stmt_def  */
  YYSYMBOL_cond_else = 174,                /* cond_else  */
  YYSYMBOL_cond_expr = 175,                /* cond_expr  */
  YYSYMBOL_cond_expr_prim = 176,           /* cond_expr_prim  */
  YYSYMBOL_cond_pol_list = 177,            /* cond_pol_list  */
  YYSYMBOL_cond_rule_def = 178,            /* cond_rule_def  */
  YYSYMBOL_cond_transition_def = 179,      /* cond_transition_def  */
  YYSYMBOL_cond_te_avtab_def = 180,        /* cond_te_avtab_def  */
  YYSYMBOL_cond_allow_def = 181,           /* cond_allow_def  */
  YYSYMBOL_cond_auditallow_def = 182,      /* cond_auditallow_def  */
  YYSYMBOL_cond_auditdeny_def = 183,       /* cond_auditdeny_def  */
  YYSYMBOL_cond_dontaudit_def = 184,       /* cond_dontaudit_def  */
  YYSYMBOL_transition_def = 185,           /* transition_def  */
  YYSYMBOL_range_trans_def = 186,          /* range_trans_def  */
  YYSYMBOL_te_avtab_def = 187,             /* te_avtab_def  */
  YYSYMBOL_allow_def = 188,                /* allow_def  */
  YYSYMBOL_auditallow_def = 189,           /* auditallow_def  */
  YYSYMBOL_auditdeny_def = 190,            /* auditdeny_def  */
  YYSYMBOL_dontaudit_def = 191,            /* dontaudit_def  */
  YYSYMBOL_neverallow_def = 192,           /* neverallow_def  */
  YYSYMBOL_xperm_allow_def = 193,          /* xperm_allow_def  */
  YYSYMBOL_xperm_auditallow_def = 194,     /* xperm_auditallow_def  */
  YYSYMBOL_xperm_dontaudit_def = 195,      /* xperm_dontaudit_def  */
  YYSYMBOL_xperm_neverallow_def = 196,     /* xperm_neverallow_def  */
  YYSYMBOL_attribute_role_def = 197,       /* attribute_role_def  */
  YYSYMBOL_role_type_def = 198,            /* role_type_def  */
  YYSYMBOL_role_attr_def = 199,            /* role_attr_def  */
  YYSYMBOL_role_dominance = 200,           /* role_dominance  */
  YYSYMBOL_role_trans_def = 201,           /* role_trans_def  */
  YYSYMBOL_role_allow_def = 202,           /* role_allow_def  */
  YYSYMBOL_roles = 203,                    /* roles  */
  YYSYMBOL_role_def = 204,                 /* role_def  */
  YYSYMBOL_roleattribute_def = 205,        /* roleattribute_def  */
  YYSYMBOL_opt_constraints = 206,          /* opt_constraints  */
  YYSYMBOL_constraints = 207,              /* constraints  */
  YYSYMBOL_constraint_decl = 208,          /* constraint_decl  */
  YYSYMBOL_constraint_def = 209,           /* constraint_def  */
  YYSYMBOL_validatetrans_def = 210,        /* validatetrans_def  */
  YYSYMBOL_cexpr = 211,                    /* cexpr  */
  YYSYMBOL_cexpr_prim = 212,               /* cexpr_prim  */
  YYSYMBOL_213_4 = 213,                    /* $@4  */
  YYSYMBOL_214_5 = 214,                    /* $@5  */
  YYSYMBOL_215_6 = 215,                    /* $@6  */
  YYSYMBOL_216_7 = 216,                    /* $@7  */
  YYSYMBOL_217_8 = 217,                    /* $@8  */
  YYSYMBOL_218_9 = 218,                    /* $@9  */
  YYSYMBOL_219_10 = 219,                   /* $@10  */
  YYSYMBOL_220_11 = 220,                   /* $@11  */
  YYSYMBOL_221_12 = 221,                   /* $@12  */
  YYSYMBOL_222_13 = 222,                   /* $@13  */
  YYSYMBOL_223_14 = 223,                   /* $@14  */
  YYSYMBOL_224_15 = 224,                   /* $@15  */
  YYSYMBOL_225_16 = 225,                   /* $@16  */
  YYSYMBOL_op = 226,                       /* op  */
  YYSYMBOL_role_mls_op = 227,              /* role_mls_op  */
  YYSYMBOL_users = 228,                    /* users  */
  YYSYMBOL_user_def = 229,                 /* user_def  */
  YYSYMBOL_opt_mls_user = 230,             /* opt_mls_user  */
  YYSYMBOL_initial_sid_contexts = 231,     /* initial_sid_contexts  */
  YYSYMBOL_initial_sid_context_def = 232,  /* initial_sid_context_def  */
  YYSYMBOL_opt_dev_contexts = 233,         /* opt_dev_contexts  */
  YYSYMBOL_dev_contexts = 234,             /* dev_contexts  */
  YYSYMBOL_dev_context_def = 235,          /* dev_context_def  */
  YYSYMBOL_pirq_context_def = 236,         /* pirq_context_def  */
  YYSYMBOL_iomem_context_def = 237,        /* iomem_context_def  */
  YYSYMBOL_ioport_context_def = 238,       /* ioport_context_def  */
  YYSYMBOL_pci_context_def = 239,          /* pci_context_def  */
  YYSYMBOL_dtree_context_def = 240,        /* dtree_context_def  */
  YYSYMBOL_opt_fs_contexts = 241,          /* opt_fs_contexts  */
  YYSYMBOL_fs_contexts = 242,              /* fs_contexts  */
  YYSYMBOL_fs_context_def = 243,           /* fs_context_def  */
  YYSYMBOL_net_contexts = 244,             /* net_contexts  */
  YYSYMBOL_opt_port_contexts = 245,        /* opt_port_contexts  */
  YYSYMBOL_port_contexts = 246,            /* port_contexts  */
  YYSYMBOL_port_context_def = 247,         /* port_context_def  */
  YYSYMBOL_opt_ibpkey_contexts = 248,      /* opt_ibpkey_contexts  */
  YYSYMBOL_ibpkey_contexts = 249,          /* ibpkey_contexts  */
  YYSYMBOL_ibpkey_context_def = 250,       /* ibpkey_context_def  */
  YYSYMBOL_opt_ibendport_contexts = 251,   /* opt_ibendport_contexts  */
  YYSYMBOL_ibendport_contexts = 252,       /* ibendport_contexts  */
  YYSYMBOL_ibendport_context_def = 253,    /* ibendport_context_def  */
  YYSYMBOL_opt_netif_contexts = 254,       /* opt_netif_contexts  */
  YYSYMBOL_netif_contexts = 255,           /* netif_contexts  */
  YYSYMBOL_netif_context_def = 256,        /* netif_context_def  */
  YYSYMBOL_opt_node_contexts = 257,        /* opt_node_contexts  */
  YYSYMBOL_node_contexts = 258,            /* node_contexts  */
  YYSYMBOL_node_context_def = 259,         /* node_context_def  */
  YYSYMBOL_opt_fs_uses = 260,              /* opt_fs_uses  */
  YYSYMBOL_fs_uses = 261,                  /* fs_uses  */
  YYSYMBOL_fs_use_def = 262,               /* fs_use_def  */
  YYSYMBOL_opt_genfs_contexts = 263,       /* opt_genfs_contexts  */
  YYSYMBOL_genfs_contexts = 264,           /* genfs_contexts  */
  YYSYMBOL_genfs_context_def = 265,        /* genfs_context_def  */
  YYSYMBOL_266_17 = 266,                   /* $@17  */
  YYSYMBOL_ipv4_addr_def = 267,            /* ipv4_addr_def  */
  YYSYMBOL_xperms = 268,                   /* xperms  */
  YYSYMBOL_nested_xperm_set = 269,         /* nested_xperm_set  */
  YYSYMBOL_nested_xperm_list = 270,        /* nested_xperm_list  */
  YYSYMBOL_nested_xperm_element = 271,     /* nested_xperm_element  */
  YYSYMBOL_272_18 = 272,                   /* $@18  */
  YYSYMBOL_xperm = 273,                    /* xperm  */
  YYSYMBOL_security_context_def = 274,     /* security_context_def  */
  YYSYMBOL_opt_mls_range_def = 275,        /* opt_mls_range_def  */
  YYSYMBOL_mls_range_def = 276,            /* mls_range_def  */
  YYSYMBOL_mls_level_def = 277,            /* mls_level_def  */
  YYSYMBOL_id_comma_list = 278,            /* id_comma_list  */
  YYSYMBOL_tilde = 279,                    /* tilde  */
  YYSYMBOL_asterisk = 280,                 /* asterisk  */
  YYSYMBOL_names = 281,                    /* names  */
  YYSYMBOL_282_19 = 282,                   /* $@19  */
  YYSYMBOL_tilde_push = 283,               /* tilde_push  */
  YYSYMBOL_asterisk_push = 284,            /* asterisk_push  */
  YYSYMBOL_names_push = 285,               /* names_push  */
  YYSYMBOL_identifier_list_push = 286,     /* identifier_list_push  */
  YYSYMBOL_identifier_push = 287,          /* identifier_push  */
  YYSYMBOL_identifier_list = 288,          /* identifier_list  */
  YYSYMBOL_nested_id_set = 289,            /* nested_id_set  */
  YYSYMBOL_nested_id_list = 290,           /* nested_id_list  */
  YYSYMBOL_nested_id_element = 291,        /* nested_id_element  */
  YYSYMBOL_292_20 = 292,                   /* $@20  */
  YYSYMBOL_identifier = 293,               /* identifier  */
  YYSYMBOL_filesystem = 294,               /* filesystem  */
  YYSYMBOL_path = 295,                     /* path  */
  YYSYMBOL_filename = 296,                 /* filename  */
  YYSYMBOL_number = 297,                   /* number  */
  YYSYMBOL_number64 = 298,                 /* number64  */
  YYSYMBOL_ipv6_addr = 299,                /* ipv6_addr  */
  YYSYMBOL_policycap_def = 300,            /* policycap_def  */
  YYSYMBOL_permissive_def = 301,           /* permissive_def  */
  YYSYMBOL_module_policy = 302,            /* module_policy  */
  YYSYMBOL_module_def = 303,               /* module_def  */
  YYSYMBOL_version_identifier = 304,       /* version_identifier  */
  YYSYMBOL_avrules_block = 305,            /* avrules_block  */
  YYSYMBOL_avrule_decls = 306,             /* avrule_decls  */
  YYSYMBOL_avrule_decl = 307,              /* avrule_decl  */
  YYSYMBOL_require_block = 308,            /* require_block  */
  YYSYMBOL_require_list = 309,             /* require_list  */
  YYSYMBOL_require_decl = 310,             /* require_decl  */
  YYSYMBOL_require_class = 311,            /* require_class  */
  YYSYMBOL_require_decl_def = 312,         /* require_decl_def  */
  YYSYMBOL_require_id_list = 313,          /* require_id_list  */
  YYSYMBOL_optional_block = 314,           /* optional_block  */
  YYSYMBOL_315_21 = 315,                   /* $@21  */
  YYSYMBOL_optional_else = 316,            /* optional_else  */
  YYSYMBOL_optional_decl = 317,            /* optional_decl  */
  YYSYMBOL_else_decl = 318,                /* else_decl  */
  YYSYMBOL_avrule_user_defs = 319          /* avrule_user_defs  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1378

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  122
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  407
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  817

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   366


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     117,   118,   121,     2,   116,   119,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   115,   114,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   112,     2,   113,   120,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   165,   165,   166,   168,   170,   173,   168,   177,   178,
     180,   183,   184,   186,   189,   191,   192,   194,   195,   197,
     200,   201,   203,   205,   207,   210,   211,   213,   214,   215,
     216,   217,   218,   219,   220,   222,   224,   227,   229,   232,
     234,   237,   239,   241,   243,   245,   247,   249,   252,   253,
     255,   257,   258,   260,   262,   265,   267,   269,   272,   273,
     275,   276,   278,   280,   283,   284,   286,   288,   291,   292,
     294,   295,   297,   300,   303,   304,   306,   307,   308,   309,
     310,   311,   313,   314,   315,   316,   317,   318,   319,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   334,   337,   340,   342,   345,   348,   351,   354,   355,
     357,   360,   363,   365,   368,   371,   374,   376,   378,   381,
     384,   387,   390,   393,   396,   399,   403,   406,   408,   410,
     412,   415,   418,   421,   424,   428,   430,   432,   434,   437,
     441,   445,   449,   454,   456,   458,   460,   463,   465,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   478,   481,
     484,   487,   490,   493,   496,   499,   502,   505,   508,   511,
     514,   516,   518,   521,   524,   526,   529,   531,   534,   537,
     538,   540,   541,   543,   544,   546,   549,   552,   554,   557,
     560,   563,   566,   569,   572,   575,   575,   578,   578,   581,
     581,   584,   584,   587,   587,   590,   590,   593,   593,   596,
     596,   599,   599,   602,   605,   605,   608,   608,   611,   614,
     614,   617,   617,   620,   623,   626,   629,   632,   635,   639,
     641,   644,   646,   648,   650,   653,   654,   656,   659,   660,
     662,   663,   665,   668,   668,   670,   671,   673,   674,   675,
     676,   677,   679,   682,   684,   687,   689,   692,   695,   698,
     699,   701,   702,   704,   707,   709,   710,   712,   713,   715,
     717,   720,   721,   723,   724,   726,   728,   731,   732,   734,
     735,   737,   740,   741,   743,   744,   746,   749,   750,   752,
     753,   755,   757,   760,   761,   763,   764,   766,   768,   770,
     773,   774,   776,   777,   779,   781,   781,   783,   786,   789,
     791,   793,   795,   799,   801,   802,   804,   804,   805,   806,
     808,   811,   813,   814,   816,   818,   821,   823,   826,   827,
     829,   831,   833,   835,   837,   840,   843,   846,   846,   849,
     852,   855,   856,   857,   858,   859,   861,   862,   864,   867,
     868,   870,   872,   872,   874,   874,   874,   874,   876,   879,
     881,   884,   886,   889,   892,   905,   914,   917,   920,   925,
     930,   933,   935,   937,   939,   941,   942,   944,   945,   946,
     947,   948,   949,   951,   953,   954,   956,   957,   959,   962,
     963,   964,   965,   966,   967,   968,   969,   970,   972,   974,
     978,   977,   982,   984,   986,   989,   992,   993
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PATH", "QPATH",
  "FILENAME", "CLONE", "COMMON", "CLASS", "CONSTRAIN", "VALIDATETRANS",
  "INHERITS", "SID", "ROLE", "ROLEATTRIBUTE", "ATTRIBUTE_ROLE", "ROLES",
  "TYPEALIAS", "TYPEATTRIBUTE", "TYPEBOUNDS", "TYPE", "TYPES", "ALIAS",
  "ATTRIBUTE", "EXPANDATTRIBUTE", "BOOL", "TUNABLE", "IF", "ELSE",
  "TYPE_TRANSITION", "TYPE_MEMBER", "TYPE_CHANGE", "ROLE_TRANSITION",
  "RANGE_TRANSITION", "SENSITIVITY", "DOMINANCE", "DOM", "DOMBY", "INCOMP",
  "CATEGORY", "LEVEL", "RANGE", "MLSCONSTRAIN", "MLSVALIDATETRANS", "USER",
  "NEVERALLOW", "ALLOW", "AUDITALLOW", "AUDITDENY", "DONTAUDIT",
  "ALLOWXPERM", "AUDITALLOWXPERM", "DONTAUDITXPERM", "NEVERALLOWXPERM",
  "SOURCE", "TARGET", "SAMEUSER", "FSCON", "PORTCON", "NETIFCON",
  "NODECON", "IBPKEYCON", "IBENDPORTCON", "PIRQCON", "IOMEMCON",
  "IOPORTCON", "PCIDEVICECON", "DEVICETREECON", "FSUSEXATTR", "FSUSETASK",
  "FSUSETRANS", "GENFSCON", "U1", "U2", "U3", "R1", "R2", "R3", "T1", "T2",
  "T3", "L1", "L2", "H1", "H2", "NOT", "AND", "OR", "XOR", "CTRUE",
  "CFALSE", "IDENTIFIER", "NUMBER", "EQUALS", "NOTEQUAL", "IPV4_ADDR",
  "IPV6_ADDR", "MODULE", "VERSION_IDENTIFIER", "REQUIRE", "OPTIONAL",
  "POLICYCAP", "PERMISSIVE", "FILESYSTEM", "DEFAULT_USER", "DEFAULT_ROLE",
  "DEFAULT_TYPE", "DEFAULT_RANGE", "LOW_HIGH", "LOW", "HIGH", "GLBLUB",
  "'{'", "'}'", "';'", "':'", "','", "'('", "')'", "'-'", "'~'", "'*'",
  "$accept", "policy", "base_policy", "$@1", "$@2", "$@3", "classes",
  "class_def", "initial_sids", "initial_sid_def", "access_vectors",
  "opt_common_perms", "common_perms", "common_perms_def", "av_perms",
  "av_perms_def", "opt_default_rules", "default_rules", "default_user_def",
  "default_role_def", "default_type_def", "default_range_def", "opt_mls",
  "mls", "sensitivities", "sensitivity_def", "alias_def", "dominance",
  "opt_categories", "categories", "category_def", "levels", "level_def",
  "mlspolicy", "mlspolicy_decl", "mlsconstraint_def",
  "mlsvalidatetrans_def", "te_rbac", "te_rbac_decl", "rbac_decl",
  "te_decl", "attribute_def", "expandattribute_def", "type_def",
  "typealias_def", "typeattribute_def", "typebounds_def", "opt_attr_list",
  "bool_def", "tunable_def", "bool_val", "cond_stmt_def", "cond_else",
  "cond_expr", "cond_expr_prim", "cond_pol_list", "cond_rule_def",
  "cond_transition_def", "cond_te_avtab_def", "cond_allow_def",
  "cond_auditallow_def", "cond_auditdeny_def", "cond_dontaudit_def",
  "transition_def", "range_trans_def", "te_avtab_def", "allow_def",
  "auditallow_def", "auditdeny_def", "dontaudit_def", "neverallow_def",
  "xperm_allow_def", "xperm_auditallow_def", "xperm_dontaudit_def",
  "xperm_neverallow_def", "attribute_role_def", "role_type_def",
  "role_attr_def", "role_dominance", "role_trans_def", "role_allow_def",
  "roles", "role_def", "roleattribute_def", "opt_constraints",
  "constraints", "constraint_decl", "constraint_def", "validatetrans_def",
  "cexpr", "cexpr_prim", "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10",
  "$@11", "$@12", "$@13", "$@14", "$@15", "$@16", "op", "role_mls_op",
  "users", "user_def", "opt_mls_user", "initial_sid_contexts",
  "initial_sid_context_def", "opt_dev_contexts", "dev_contexts",
  "dev_context_def", "pirq_context_def", "iomem_context_def",
  "ioport_context_def", "pci_context_def", "dtree_context_def",
  "opt_fs_contexts", "fs_contexts", "fs_context_def", "net_contexts",
  "opt_port_contexts", "port_contexts", "port_context_def",
  "opt_ibpkey_contexts", "ibpkey_contexts", "ibpkey_context_def",
  "opt_ibendport_contexts", "ibendport_contexts", "ibendport_context_def",
  "opt_netif_contexts", "netif_contexts", "netif_context_def",
  "opt_node_contexts", "node_contexts", "node_context_def", "opt_fs_uses",
  "fs_uses", "fs_use_def", "opt_genfs_contexts", "genfs_contexts",
  "genfs_context_def", "$@17", "ipv4_addr_def", "xperms",
  "nested_xperm_set", "nested_xperm_list", "nested_xperm_element", "$@18",
  "xperm", "security_context_def", "opt_mls_range_def", "mls_range_def",
  "mls_level_def", "id_comma_list", "tilde", "asterisk", "names", "$@19",
  "tilde_push", "asterisk_push", "names_push", "identifier_list_push",
  "identifier_push", "identifier_list", "nested_id_set", "nested_id_list",
  "nested_id_element", "$@20", "identifier", "filesystem", "path",
  "filename", "number", "number64", "ipv6_addr", "policycap_def",
  "permissive_def", "module_policy", "module_def", "version_identifier",
  "avrules_block", "avrule_decls", "avrule_decl", "require_block",
  "require_list", "require_decl", "require_class", "require_decl_def",
  "require_id_list", "optional_block", "$@21", "optional_else",
  "optional_decl", "else_decl", "avrule_user_defs", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-714)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-232)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -33,   -17,   111,  -714,   179,  -714,   834,  -714,   259,  -714,
     -17,   198,  -714,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,    93,   -17,   -17,    92,    93,    93,    93,    93,    93,
      48,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      81,  -714,   -17,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,   508,  -714,  -714,  -714,
     113,  -714,  -714,  -714,  -714,  -714,   126,  -714,   -17,  -714,
     106,  -714,    10,   -17,   219,   330,   -17,   -17,    16,   254,
     -26,  -714,  -714,   -31,  -714,   248,  -714,   147,   248,   248,
      92,    92,   186,  -714,  -714,    93,    93,    93,    93,    93,
     288,    93,    93,    93,    93,    93,    93,    93,    93,    93,
     177,   257,   -17,   311,  -714,  -714,   834,  -714,  -714,   -17,
    -714,  -714,   350,   356,  -714,    93,   -17,   265,   286,  -714,
    -714,    93,   268,   300,   303,   283,   289,  -714,  -714,  -714,
     107,  -714,  -714,  -714,  -714,  -714,  -714,   310,  -714,   324,
     328,   249,    70,    92,    92,    92,    92,    92,  -714,   347,
     360,   372,    89,   170,   363,    20,  -714,   387,   162,   388,
     389,   390,   394,   395,   396,   404,   -17,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,    22,  -714,   358,   -17,
    -714,   491,  -714,   411,   417,   199,   -17,   350,  -714,  -714,
     416,   400,  -714,  -714,   -17,  -714,  -714,  -714,  -714,   428,
    -714,   -17,  -714,  -714,  -714,   -17,  -714,  -714,  -714,   249,
     174,   253,  -714,  -714,    24,    93,    93,    93,    93,   430,
      93,   431,   398,   432,  -714,   322,  -714,  -714,    93,  -714,
      93,    93,    93,    93,    93,    93,    93,    93,    93,  -714,
    -714,  -714,  -714,   321,    93,  -714,   -17,    93,    93,    93,
      93,   512,   199,  -714,  -714,  -714,  -714,    18,  -714,  -714,
    -714,  -714,  -714,  -714,    93,    93,    93,    93,    93,    93,
      93,   520,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
     -17,   -17,   -17,   -17,  -714,   -17,  -714,   -17,   -17,   288,
    -714,    93,    93,    93,    93,    93,   -17,   -17,   -17,   -17,
    -714,  -714,   -17,   509,   522,   -16,  -714,   402,   405,   425,
      25,   -17,   939,  -714,   447,  -714,  -714,  -714,  -714,  -714,
     -17,   -17,    93,    93,    93,    93,    93,    93,    93,   439,
    -714,     6,   448,   449,   451,   452,  -714,   400,    21,   453,
     454,   455,   456,   458,     2,     2,     2,     2,  -714,   -17,
     459,  -714,  -714,   462,  -714,  -714,   461,   463,   464,   465,
     466,   467,   267,   279,   468,    14,   -17,  -714,   620,  -714,
    -714,  -714,  -714,  -714,  -714,   208,  -714,   525,   471,   187,
     469,   470,   472,   473,   474,   476,   477,  -714,  -714,  -714,
     479,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,    32,   480,  -714,  -714,    32,  -714,   481,   482,   483,
     545,  -714,   834,  -714,  -714,  -714,  -714,  -714,  -714,   484,
     485,   486,   487,   488,   489,  -714,  -714,   490,   492,  -714,
     152,  -714,   -17,  -714,   -17,   550,   525,  -714,   -17,  -714,
      93,    93,    93,    93,    93,    93,    93,   119,  -714,  -714,
     232,  -714,   494,  -714,  -714,  -714,  -714,  -714,  -714,   -17,
     496,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,    93,
      93,  -714,   475,  -714,  -714,  -714,  -714,   202,    28,   -17,
     351,  -714,  -714,   216,   -17,   -17,   -17,    93,    93,    93,
      93,  -714,  -714,  -714,  -714,  -714,  -714,    93,   173,   593,
    -714,  -714,  -714,   497,   376,    93,    93,  -714,   450,  -714,
    -714,  -714,  -714,    15,   498,   500,   501,   502,   503,   504,
     528,   173,    46,    95,   244,  -714,   401,   401,   401,    46,
     401,   401,   401,   401,   401,    46,    46,    46,   173,   173,
     149,  -714,   -17,    86,  -714,  -714,  -714,   -17,    93,   173,
    -714,  -714,   507,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
     151,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,   551,  -714,  -714,   547,   549,  -714,  -714,   548,
    -714,  -714,   323,   542,   357,  -714,    83,   173,   173,  -714,
     -17,   528,  -714,   352,   571,  -714,   339,   173,   157,  -714,
    -714,   121,   121,   121,   121,  -714,   121,   121,   121,   121,
    -714,   121,   121,  -714,   121,   121,   121,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,   543,  -714,   515,   528,   245,
     -17,   -17,   560,   352,  -714,  -714,  -714,   183,  -714,   363,
    -714,  -714,   241,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,   -17,   -17,
    -714,  -714,   -17,   -17,   -17,   245,   574,   560,  -714,  -714,
    -714,   217,  -714,   363,  -714,   521,   -17,   527,   534,   540,
     493,   -17,   346,   583,   574,  -714,  -714,  -714,  -714,   228,
     -17,  -714,  -714,  -714,  -714,  -714,  -714,   -15,   528,   528,
     564,   528,   528,   493,   596,   346,  -714,  -714,  -714,  -714,
    -714,  -714,   -17,   598,   583,  -714,  -714,  -714,   546,    80,
    -714,    88,   -17,  -714,    98,   103,   -17,   -17,   567,   612,
     596,  -714,  -714,   -17,   403,  -714,   598,  -714,  -714,   -17,
    -714,  -714,   -17,   528,  -714,  -714,   564,  -714,   528,  -714,
    -714,  -714,  -714,   528,   -17,  -714,   612,  -714,  -714,   -17,
     580,   567,  -714,  -714,   -17,  -714,   -17,   -17,   -17,   140,
     528,  -714,  -714,   -17,   -17,  -714,  -714,  -714,  -714,   528,
    -714,   -17,  -714,  -714,   -17,  -714,  -714
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     0,     2,     0,     3,     0,   358,     0,     1,
       0,     0,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,     0,   382,   377,   378,    89,    90,    91,    92,
      93,    94,    95,    96,   379,    97,    98,    99,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    82,    83,    88,
      84,    85,    86,    87,   100,   369,   407,   376,   380,   381,
       0,   364,   308,   371,   373,   372,     0,    10,     0,     9,
      16,    11,   109,     0,     0,     0,     0,     0,   109,     0,
       0,   330,   331,     0,   334,     0,   333,   332,     0,     0,
       0,     0,     0,   124,   125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   407,   375,   374,     0,   370,    13,     0,
      12,     5,     0,    15,    17,     0,     0,     0,     0,   328,
     167,     0,     0,     0,     0,   109,     0,   101,   355,   357,
       0,   352,   354,   336,   335,   112,   113,     0,   337,     0,
       0,   118,     0,     0,     0,     0,     0,     0,   127,     0,
       0,     0,     0,     0,     0,     0,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   389,   392,   390,
     391,   394,   395,   396,   397,   393,     0,   385,     0,     0,
     368,     0,   406,     0,     0,    26,     0,    14,    20,    18,
       0,   108,   169,   178,     0,    55,   105,   106,   107,     0,
     104,     0,   351,   353,   102,     0,   110,   111,   117,   119,
     120,   121,   122,   123,     0,     0,     0,     0,     0,     0,
       0,     0,   325,   327,   348,     0,   170,   175,     0,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   383,
     384,   386,   398,     0,     0,   400,     0,     0,     0,     0,
       0,    49,    25,    27,    28,    29,    30,     0,    21,   168,
     329,   103,   356,   338,     0,     0,     0,     0,     0,     0,
       0,   116,   126,   128,   129,   135,   136,   137,   138,   130,
       0,     0,     0,     0,   171,     0,   147,     0,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     388,   387,     0,   239,   403,     0,   349,     0,     0,     0,
       0,     0,     0,    48,     0,    51,    31,    32,    33,    34,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,     0,     0,     0,     0,     0,   324,   326,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,   405,   401,     0,    19,   350,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    81,     0,    74,
      77,    76,    78,    80,    79,     0,    52,    59,    23,     0,
       0,     0,     0,     0,     0,     0,     0,   127,   363,   144,
       0,   145,   146,   172,   148,   177,   162,   158,   159,   160,
     161,     0,     0,   310,   309,     0,   320,     0,     0,     0,
       0,   237,     0,    35,    36,    37,    38,    39,    40,     0,
       0,     0,     0,     0,     0,    47,    54,     0,     0,    75,
     180,   235,     0,    56,     0,     0,    58,    60,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,   143,   319,
       0,   314,   318,   163,   312,   311,   164,   165,   166,     0,
       0,    43,    41,    42,    46,    44,    45,    53,   367,     0,
       0,     6,   179,   181,   183,   184,   236,     0,     0,     0,
       0,    64,    61,     0,     0,     0,     0,     0,     0,     0,
       0,   115,   313,   315,   316,   238,   402,     0,     0,     0,
     182,    57,    63,     0,     0,     0,     0,    65,    50,    68,
      70,    71,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,     0,   260,   240,    62,    67,     0,     0,     0,
      69,   132,     0,   133,   134,   139,   140,   141,   142,   317,
       0,   232,   233,   234,   229,   230,   231,   218,   214,   219,
     216,   221,   195,   197,   199,   201,     0,   203,   205,   207,
     209,   211,     0,     0,     0,   188,     0,     0,     0,   186,
       0,     0,   241,   294,   259,   261,     0,     0,     0,   131,
     185,     0,     0,     0,     0,   192,     0,     0,     0,     0,
     193,     0,     0,   194,     0,     0,     0,   223,   227,   224,
     228,   225,   226,   187,   189,   190,   242,     0,     0,     0,
       0,     0,   301,   293,   295,   262,    66,     0,    73,     0,
     339,   340,     0,   343,   215,   341,   220,   217,   222,   196,
     198,   200,   202,   204,   206,   208,   210,   212,     0,     0,
     360,   359,     0,     0,     0,     0,   266,   300,   302,   296,
      72,     0,   346,     0,   344,     0,     0,     0,     0,     0,
       0,     0,   244,   283,   265,   267,   303,   342,   347,     0,
       0,   263,   297,   298,   299,   361,   362,     0,     0,     0,
       0,     0,     0,     0,   272,   243,   245,   247,   248,   249,
     250,   251,     0,   288,   282,   284,   268,   345,   323,     0,
     307,     0,     0,   365,     0,     0,     0,     0,     0,   278,
     271,   273,   246,     0,     0,   264,   287,   289,   285,     0,
     321,   305,     0,     0,   269,   252,     0,   253,     0,   255,
     257,   258,   366,     0,     0,     7,   277,   279,   274,     0,
       0,     0,   290,   322,     0,   304,     0,     0,     0,     0,
       0,   280,   286,     0,     0,   306,   270,   254,   256,     0,
     275,     0,   291,   292,     0,   281,   276
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -714,  -714,  -714,  -714,  -714,  -714,  -714,   665,  -714,   587,
    -714,  -714,  -714,   535,  -714,   478,  -714,  -714,   397,   399,
     407,   408,  -714,  -714,  -714,   336,   -93,  -714,  -714,  -714,
     218,  -714,   172,  -714,   145,  -714,  -714,  -714,   287,  -298,
    -273,  -714,  -714,  -714,  -714,  -714,  -714,   -52,  -714,  -714,
     392,  -257,  -714,   -22,  -714,   269,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,   375,  -177,  -714,  -714,  -714,   194,  -714,  -714,  -511,
    -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,  -714,  -714,  -714,  -112,  -169,  -714,  -359,  -714,  -714,
     124,  -714,  -714,   -37,  -714,  -714,  -714,  -714,  -714,  -714,
    -714,    76,  -714,  -714,  -714,   -13,  -714,  -714,   -58,  -714,
    -714,   -83,  -714,  -714,   -40,  -714,  -714,   -60,  -714,  -714,
      44,  -714,  -714,    11,  -714,  -673,    54,  -368,  -714,   229,
    -714,  -403,  -371,  -714,  -311,  -274,   -94,  -350,  -272,   800,
    -714,  -714,  -714,  -168,     8,  -183,  -341,   -41,  -714,   552,
    -714,    -1,    19,   -20,   175,    41,   -61,  -713,  -714,  -714,
    -714,  -714,  -714,  -129,  -714,   640,  -238,  -714,   511,  -714,
    -714,  -714,  -252,  -714,  -714,  -714,  -714,   590
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     2,     3,     4,   215,   529,    11,    12,    90,    91,
     141,   142,   143,   144,   217,   218,   281,   282,   283,   284,
     285,   286,   342,   343,   344,   345,   152,   407,   465,   466,
     467,   510,   511,   538,   539,   540,   541,   398,   399,    44,
      45,    46,    47,    48,    49,    50,    51,   147,    52,    53,
     167,    54,   360,   112,   113,   244,   302,   303,   304,   305,
     306,   307,   308,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,   185,   186,    73,   501,   502,   503,   504,   505,   570,
     571,   636,   637,   638,   639,   641,   642,   644,   645,   646,
     631,   633,   632,   634,   596,   597,   460,   133,   380,   573,
     574,   734,   735,   736,   737,   738,   739,   740,   741,   623,
     624,   625,   712,   713,   714,   715,   759,   760,   761,   785,
     786,   787,   743,   744,   745,   765,   766,   767,   662,   663,
     664,   696,   697,   698,   794,    84,   432,   433,   480,   481,
     550,   434,   656,   770,   251,   252,   148,   103,   104,   105,
     235,   672,   673,   674,   701,   675,   335,   106,   160,   161,
     231,   107,   692,   727,   420,   436,   754,   783,   403,    74,
       5,     6,    86,    75,    76,    77,    78,   206,   207,   208,
     209,   273,    79,   334,   382,    80,   383,   135
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       8,   255,   153,   154,   365,   155,   309,   213,   257,    87,
     409,   418,    92,    93,    94,    95,    96,    97,    98,    99,
     418,   108,   109,   114,   435,   435,   435,   435,   482,   350,
     196,   145,   485,   184,   184,   197,   151,   198,   151,   461,
     590,   131,   199,   366,   400,   200,   156,   201,   202,    85,
     151,   791,   221,   294,   295,   296,   203,   615,   616,   159,
       7,   204,   163,   479,     1,     7,   205,   484,   628,   401,
     297,   298,   299,   300,     7,     7,     7,   482,   804,   392,
     393,   100,   591,   592,   593,   402,   100,   138,   171,   172,
     404,   790,   149,   158,    81,   149,   149,   384,   572,   162,
     400,   506,   164,   229,   749,   440,   654,   655,   598,   114,
     114,     9,   479,   139,   431,   599,   667,   803,    88,   159,
     419,   507,   101,    40,    81,   401,   146,   513,   456,   581,
     351,   211,   146,   256,   425,   269,   394,   301,   214,   594,
     595,   402,   532,   621,   431,   149,   404,   589,   294,   295,
     296,   239,   240,   241,   242,   243,   173,   174,   175,   162,
     120,   499,   500,   176,   177,   297,   298,   299,   300,   617,
     618,     7,   114,   114,   114,   114,   114,   110,   525,     7,
       7,   249,   253,     7,     7,   196,   552,    10,   238,     7,
     197,   257,   198,   130,     7,   268,   132,   199,     7,   771,
     200,   653,   201,   202,   248,   100,    10,   773,   272,   111,
      88,   203,   254,   101,   102,   287,   204,   776,    40,   100,
     232,   205,   778,   290,   367,   136,   158,   553,   554,   555,
     292,     7,   521,   669,   293,   617,   618,   617,   618,   309,
     137,   101,   102,   617,   618,   556,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   600,   568,   809,
     173,     7,   175,   619,   601,   630,   168,   176,   177,   617,
     618,   668,   173,   174,   175,   336,   259,   260,     7,   176,
     177,   670,   670,   670,   670,   250,   670,   670,   670,   670,
     569,   670,   670,     7,   670,   670,   670,   700,   178,     7,
     469,   184,   457,   277,   278,   279,   280,     7,   254,   361,
     362,   363,   364,   490,   253,   531,   253,   149,   706,   254,
     462,   707,   708,   709,    81,   374,   375,   376,   377,   542,
     717,   378,   254,   150,   385,   721,   690,   165,   166,   173,
     395,   747,   176,   177,   431,   522,   176,   177,   691,   408,
     336,    81,   151,   703,    82,   132,   750,    83,   216,   671,
     671,   671,   671,   139,   671,   671,   671,   671,   157,   671,
     671,   210,   671,   671,   671,   449,   450,   451,   253,   222,
     774,   775,   226,   777,   779,   780,   781,   452,   453,   454,
     606,   509,   789,   535,   536,   458,   612,   613,   614,   146,
     223,   795,   224,   230,   463,   647,   648,   649,   385,   729,
     730,   731,   732,   733,   227,   533,   224,   228,   802,   224,
     659,   660,   661,   805,   234,   806,   807,   808,   810,   437,
     438,   439,   812,   813,   319,   331,   320,   332,   236,   651,
     815,   652,   237,   816,   602,   603,   604,   605,   607,   608,
     609,   610,   611,   666,   254,   224,   386,   387,   793,   388,
     389,   336,   245,   508,   676,   677,   678,   336,   679,   680,
     681,   682,   271,   683,   684,   246,   685,   686,   687,   390,
     391,   341,   405,   626,   499,   500,   702,   247,   253,   704,
     576,   577,   535,   536,   594,   595,   725,   726,    82,   782,
     169,   170,   258,   261,   262,   263,   385,   274,   534,   264,
     265,   266,   385,   543,   544,   545,   224,   317,   718,   267,
     702,    13,    14,    15,   275,    16,    17,    18,    19,   276,
     289,    20,    21,    22,    23,    24,   718,    25,    26,    27,
      28,    29,   291,    30,   314,   316,   341,   318,   359,   379,
     381,   417,   132,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   421,   422,   464,   423,   424,   426,   427,   428,
     429,   620,   430,   441,   442,   443,   149,   444,   445,   446,
     447,   448,   455,   468,   470,   471,   489,   472,   473,   474,
     509,   475,   476,   478,   483,   486,   487,   488,   491,   492,
     493,   494,   495,   496,   497,   572,   498,    40,    41,   526,
      42,   575,   583,   524,   584,   585,   586,   587,   588,   657,
      81,   629,    43,  -231,   635,   640,   650,   643,   621,   617,
     688,   695,   711,    13,    14,    15,   720,    16,    17,    18,
      19,   722,   742,    20,    21,    22,    23,    24,   723,    25,
      26,    27,    28,    29,   724,    30,   753,   758,   764,   693,
     694,   769,   658,   782,   132,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   784,    82,    89,   140,   219,   346,
     406,   347,   537,   580,   512,   459,   477,   705,   657,   348,
     349,   657,   657,   657,   368,   288,   530,   622,   762,   689,
     665,   746,   788,   801,   768,   657,   792,   699,   716,   523,
     728,   719,   233,   757,   710,   797,   134,   270,   582,   748,
      41,   396,    42,   212,     0,     0,   657,     0,     0,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,   763,     0,     0,     0,     0,     0,     0,   772,     0,
     657,   657,     0,   657,   657,   657,   657,     0,     0,     0,
       0,     0,   657,     0,     0,     0,     0,     0,   253,   751,
     752,   657,   755,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   800,     0,     0,     0,     0,   657,     0,
       0,     0,     0,   657,     0,   657,   657,   657,   657,     0,
       0,     0,   657,   657,     0,     0,     0,     0,     0,     0,
     657,     0,     0,   657,   796,     0,     0,     0,     0,   798,
       0,     0,     0,     0,   799,   115,   116,   117,   118,   119,
       0,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,   811,     0,     0,     0,     0,     0,    13,    14,    15,
     814,    16,    17,    18,    19,     0,     0,    20,    21,    22,
      23,    24,     0,    25,    26,    27,    28,    29,     0,    30,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   179,   180,   181,   182,   183,
       0,   187,   188,   189,   190,   191,   192,   193,   194,   195,
       0,     0,     0,    40,    41,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,   220,     0,     0,    43,     0,
       0,   225,    13,    14,    15,     0,    16,    17,    18,    19,
       0,     0,    20,    21,    22,    23,    24,     0,    25,    26,
      27,    28,    29,     0,    30,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    41,
     396,    42,     0,     0,     0,   310,   311,   312,   313,     0,
     315,     0,     0,   397,     0,     0,     0,     0,   321,     0,
     322,   323,   324,   325,   326,   327,   328,   329,   330,     0,
       0,     0,     0,     0,   333,     0,     0,   337,   338,   339,
     340,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,   353,   354,   355,   356,   357,
     358,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   369,   370,   371,   372,   373,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   410,   411,   412,   413,   414,   415,   416,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
     528,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,   547,   548,
     549,     0,     0,     0,     0,     0,     0,   551,     0,     0,
       0,     0,     0,     0,     0,   578,   579,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   627
};

static const yytype_int16 yycheck[] =
{
       1,   184,    96,    97,   315,    98,   244,   136,   185,    10,
     351,     5,    13,    14,    15,    16,    17,    18,    19,    20,
       5,    22,    23,    24,   374,   375,   376,   377,   431,    11,
       8,    21,   435,    13,    13,    13,    22,    15,    22,   398,
     551,    42,    20,   317,   342,    23,    98,    25,    26,     8,
      22,   764,   146,    29,    30,    31,    34,   568,   569,   100,
      91,    39,   103,   431,    97,    91,    44,   435,   579,   342,
      46,    47,    48,    49,    91,    91,    91,   480,   791,    54,
      55,   112,    36,    37,    38,   342,   112,    88,   110,   111,
     342,   764,    93,   119,    92,    96,    97,   113,    12,   100,
     398,   460,   103,   155,   119,   379,   617,   618,    13,   110,
     111,     0,   480,     7,   112,    20,   627,   790,    12,   160,
     114,   462,   120,    99,    92,   398,   116,   468,   114,   114,
     112,   132,   116,   113,   113,   113,   111,   113,   139,    93,
      94,   398,   114,    57,   112,   146,   398,   550,    29,    30,
      31,   173,   174,   175,   176,   177,    86,    87,    88,   160,
     112,     9,    10,    93,    94,    46,    47,    48,    49,    86,
      87,    91,   173,   174,   175,   176,   177,    85,   489,    91,
      91,   182,   183,    91,    91,     8,    13,     8,   118,    91,
      13,   368,    15,   112,    91,   196,    44,    20,    91,   119,
      23,   118,    25,    26,   115,   112,     8,   119,   209,   117,
      12,    34,    91,   120,   121,   216,    39,   119,    99,   112,
     113,    44,   119,   224,   318,   112,   119,    54,    55,    56,
     231,    91,   113,   112,   235,    86,    87,    86,    87,   477,
     114,   120,   121,    86,    87,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    13,    85,   119,
      86,    91,    88,   114,    20,   114,   119,    93,    94,    86,
      87,   114,    86,    87,    88,   276,   114,   115,    91,    93,
      94,   631,   632,   633,   634,   115,   636,   637,   638,   639,
     117,   641,   642,    91,   644,   645,   646,   114,   112,    91,
     113,    13,   395,   104,   105,   106,   107,    91,    91,   310,
     311,   312,   313,   442,   315,   113,   317,   318,   689,    91,
     112,   692,   693,   694,    92,   326,   327,   328,   329,   113,
     113,   332,    91,   114,   335,   706,    91,    89,    90,    86,
     341,   113,    93,    94,   112,   113,    93,    94,   103,   350,
     351,    92,    22,   112,    95,    44,   727,    98,     8,   631,
     632,   633,   634,     7,   636,   637,   638,   639,   114,   641,
     642,   114,   644,   645,   646,   108,   109,   110,   379,   114,
     751,   752,   114,   754,   755,   756,   757,   108,   109,   110,
     559,    40,   763,    42,    43,   396,   565,   566,   567,   116,
     114,   772,   116,   114,   405,    82,    83,    84,   409,    63,
      64,    65,    66,    67,   114,   508,   116,   114,   789,   116,
      68,    69,    70,   794,   114,   796,   797,   798,   799,   375,
     376,   377,   803,   804,   112,   114,   114,   116,   114,    82,
     811,    84,   114,   814,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   114,    91,   116,    54,    55,   769,    54,
      55,   462,   115,   464,   632,   633,   634,   468,   636,   637,
     638,   639,   114,   641,   642,   115,   644,   645,   646,    54,
      55,    34,    35,   577,     9,    10,   669,   115,   489,   672,
     114,   115,    42,    43,    93,    94,     3,     4,    95,    96,
     108,   109,   115,   115,   115,   115,   507,    16,   509,   115,
     115,   115,   513,   514,   515,   516,   116,   119,   701,   115,
     703,    13,    14,    15,   113,    17,    18,    19,    20,   112,
     114,    23,    24,    25,    26,    27,   719,    29,    30,    31,
      32,    33,   114,    35,   114,   114,    34,   115,    28,    40,
      28,   112,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,   114,   114,    39,   114,   114,   114,   114,   114,
     114,   572,   114,   114,   112,   114,   577,   114,   114,   114,
     114,   114,   114,   112,   115,   115,    41,   115,   115,   115,
      40,   115,   115,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,    12,   114,    99,   100,   113,
     102,   114,   114,   119,   114,   114,   114,   114,   114,   620,
      92,   114,   114,    76,    73,    76,    84,    79,    57,    86,
     115,    71,    58,    13,    14,    15,   115,    17,    18,    19,
      20,   114,    59,    23,    24,    25,    26,    27,   114,    29,
      30,    31,    32,    33,   114,    35,    92,    61,    60,   660,
     661,   115,   621,    96,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    62,    95,    11,    90,   143,   282,
     344,   282,   510,   538,   466,   398,   417,   688,   689,   282,
     282,   692,   693,   694,   319,   217,   502,   573,   735,   658,
     624,   714,   760,   786,   744,   706,   766,   663,   697,   480,
     711,   703,   160,   733,   695,   776,    76,   206,   543,   720,
     100,   101,   102,   133,    -1,    -1,   727,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,   742,    -1,    -1,    -1,    -1,    -1,    -1,   749,    -1,
     751,   752,    -1,   754,   755,   756,   757,    -1,    -1,    -1,
      -1,    -1,   763,    -1,    -1,    -1,    -1,    -1,   769,   728,
     729,   772,   731,   732,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   784,    -1,    -1,    -1,    -1,   789,    -1,
      -1,    -1,    -1,   794,    -1,   796,   797,   798,   799,    -1,
      -1,    -1,   803,   804,    -1,    -1,    -1,    -1,    -1,    -1,
     811,    -1,    -1,   814,   773,    -1,    -1,    -1,    -1,   778,
      -1,    -1,    -1,    -1,   783,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,   800,    -1,    -1,    -1,    -1,    -1,    13,    14,    15,
     809,    17,    18,    19,    20,    -1,    -1,    23,    24,    25,
      26,    27,    -1,    29,    30,    31,    32,    33,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,    -1,    99,   100,    -1,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   145,    -1,    -1,   114,    -1,
      -1,   151,    13,    14,    15,    -1,    17,    18,    19,    20,
      -1,    -1,    23,    24,    25,    26,    27,    -1,    29,    30,
      31,    32,    33,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,    -1,    -1,    -1,   245,   246,   247,   248,    -1,
     250,    -1,    -1,   114,    -1,    -1,    -1,    -1,   258,    -1,
     260,   261,   262,   263,   264,   265,   266,   267,   268,    -1,
      -1,    -1,    -1,    -1,   274,    -1,    -1,   277,   278,   279,
     280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,   295,   296,   297,   298,   299,
     300,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   321,   322,   323,   324,   325,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   352,   353,   354,   355,   356,   357,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     470,   471,   472,   473,   474,   475,   476,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   499,
     500,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   517,   518,   519,
     520,    -1,    -1,    -1,    -1,    -1,    -1,   527,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   535,   536,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   578
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    97,   123,   124,   125,   302,   303,    91,   293,     0,
       8,   128,   129,    13,    14,    15,    17,    18,    19,    20,
      23,    24,    25,    26,    27,    29,    30,    31,    32,    33,
      35,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      99,   100,   102,   114,   161,   162,   163,   164,   165,   166,
     167,   168,   170,   171,   173,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   205,   301,   305,   306,   307,   308,   314,
     317,    92,    95,    98,   267,   297,   304,   293,    12,   129,
     130,   131,   293,   293,   293,   293,   293,   293,   293,   293,
     112,   120,   121,   279,   280,   281,   289,   293,   293,   293,
      85,   117,   175,   176,   293,   281,   281,   281,   281,   281,
     112,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     112,   293,    44,   229,   307,   319,   112,   114,   293,     7,
     131,   132,   133,   134,   135,    21,   116,   169,   278,   293,
     114,    22,   148,   278,   278,   148,   169,   114,   119,   289,
     290,   291,   293,   289,   293,    89,    90,   172,   119,   172,
     172,   175,   175,    86,    87,    88,    93,    94,   112,   281,
     281,   281,   281,   281,    13,   203,   204,   281,   281,   281,
     281,   281,   281,   281,   281,   281,     8,    13,    15,    20,
      23,    25,    26,    34,    39,    44,   309,   310,   311,   312,
     114,   293,   319,   305,   293,   126,     8,   136,   137,   135,
     281,   278,   114,   114,   116,   281,   114,   114,   114,   169,
     114,   292,   113,   291,   114,   282,   114,   114,   118,   175,
     175,   175,   175,   175,   177,   115,   115,   115,   115,   293,
     115,   276,   277,   293,    91,   287,   113,   204,   115,   114,
     115,   115,   115,   115,   115,   115,   115,   115,   293,   113,
     310,   114,   293,   313,    16,   113,   112,   104,   105,   106,
     107,   138,   139,   140,   141,   142,   143,   293,   137,   114,
     293,   114,   293,   293,    29,    30,    31,    46,    47,    48,
      49,   113,   178,   179,   180,   181,   182,   183,   184,   308,
     281,   281,   281,   281,   114,   281,   114,   119,   115,   112,
     114,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   114,   116,   281,   315,   288,   293,   281,   281,   281,
     281,    34,   144,   145,   146,   147,   140,   141,   142,   143,
      11,   112,   281,   281,   281,   281,   281,   281,   281,    28,
     174,   293,   293,   293,   293,   276,   277,   278,   203,   281,
     281,   281,   281,   281,   293,   293,   293,   293,   293,    40,
     230,    28,   316,   318,   113,   293,    54,    55,    54,    55,
      54,    55,    54,    55,   111,   293,   101,   114,   159,   160,
     161,   162,   173,   300,   314,    35,   147,   149,   293,   288,
     281,   281,   281,   281,   281,   281,   281,   112,     5,   114,
     296,   114,   114,   114,   114,   113,   114,   114,   114,   114,
     114,   112,   268,   269,   273,   279,   297,   268,   268,   268,
     277,   114,   112,   114,   114,   114,   114,   114,   114,   108,
     109,   110,   108,   109,   110,   114,   114,   148,   293,   160,
     228,   229,   112,   293,    39,   150,   151,   152,   112,   113,
     115,   115,   115,   115,   115,   115,   115,   177,   114,   269,
     270,   271,   273,   114,   269,   273,   114,   114,   114,    41,
     305,   114,   114,   114,   114,   114,   114,   114,   114,     9,
      10,   206,   207,   208,   209,   210,   229,   288,   293,    40,
     153,   154,   152,   288,   281,   281,   281,   281,   281,   281,
     281,   113,   113,   271,   119,   276,   113,   281,   281,   127,
     208,   113,   114,   148,   293,    42,    43,   154,   155,   156,
     157,   158,   113,   293,   293,   293,   281,   281,   281,   281,
     272,   281,    13,    54,    55,    56,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    85,   117,
     211,   212,    12,   231,   232,   114,   114,   115,   281,   281,
     156,   114,   296,   114,   114,   114,   114,   114,   114,   273,
     211,    36,    37,    38,    93,    94,   226,   227,    13,    20,
      13,    20,   226,   226,   226,   226,   227,   226,   226,   226,
     226,   226,   227,   227,   227,   211,   211,    86,    87,   114,
     293,    57,   232,   241,   242,   243,   278,   281,   211,   114,
     114,   222,   224,   223,   225,    73,   213,   214,   215,   216,
      76,   217,   218,    79,   219,   220,   221,    82,    83,    84,
      84,    82,    84,   118,   211,   211,   274,   293,   297,    68,
      69,    70,   260,   261,   262,   243,   114,   211,   114,   112,
     279,   280,   283,   284,   285,   287,   285,   285,   285,   285,
     285,   285,   285,   285,   285,   285,   285,   285,   115,   297,
      91,   103,   294,   293,   293,    71,   263,   264,   265,   262,
     114,   286,   287,   112,   287,   293,   274,   274,   274,   274,
     294,    58,   244,   245,   246,   247,   265,   113,   287,   286,
     115,   274,   114,   114,   114,     3,     4,   295,   293,    63,
      64,    65,    66,    67,   233,   234,   235,   236,   237,   238,
     239,   240,    59,   254,   255,   256,   247,   113,   293,   119,
     274,   297,   297,    92,   298,   297,   297,   295,    61,   248,
     249,   250,   235,   293,    60,   257,   258,   259,   256,   115,
     275,   119,   293,   119,   274,   274,   119,   274,   119,   274,
     274,   274,    96,   299,    62,   251,   252,   253,   250,   274,
     267,   299,   259,   276,   266,   274,   297,   298,   297,   297,
     293,   253,   274,   267,   299,   274,   274,   274,   274,   119,
     274,   297,   274,   274,   297,   274,   274
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   122,   123,   123,   125,   126,   127,   124,   128,   128,
     129,   130,   130,   131,   132,   133,   133,   134,   134,   135,
     136,   136,   137,   137,   137,   138,   138,   139,   139,   139,
     139,   139,   139,   139,   139,   140,   140,   141,   141,   142,
     142,   143,   143,   143,   143,   143,   143,   143,   144,   144,
     145,   146,   146,   147,   147,   148,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   154,   154,   155,   155,
     156,   156,   157,   158,   159,   159,   160,   160,   160,   160,
     160,   160,   161,   161,   161,   161,   161,   161,   161,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   163,   164,   165,   165,   166,   167,   168,   169,   169,
     170,   171,   172,   172,   173,   174,   174,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   177,   177,   178,   178,
     178,   179,   179,   179,   179,   180,   180,   180,   180,   181,
     182,   183,   184,   185,   185,   185,   185,   186,   186,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   201,   202,   203,   203,   204,   204,   205,   206,
     206,   207,   207,   208,   208,   209,   210,   211,   211,   211,
     211,   211,   212,   212,   212,   213,   212,   214,   212,   215,
     212,   216,   212,   217,   212,   218,   212,   219,   212,   220,
     212,   221,   212,   212,   222,   212,   223,   212,   212,   224,
     212,   225,   212,   212,   212,   212,   212,   212,   212,   226,
     226,   227,   227,   227,   227,   228,   228,   229,   230,   230,
     231,   231,   232,   233,   233,   234,   234,   235,   235,   235,
     235,   235,   236,   237,   237,   238,   238,   239,   240,   241,
     241,   242,   242,   243,   244,   245,   245,   246,   246,   247,
     247,   248,   248,   249,   249,   250,   250,   251,   251,   252,
     252,   253,   254,   254,   255,   255,   256,   257,   257,   258,
     258,   259,   259,   260,   260,   261,   261,   262,   262,   262,
     263,   263,   264,   264,   265,   266,   265,   265,   267,   268,
     268,   268,   268,   269,   270,   270,   272,   271,   271,   271,
     273,   274,   275,   275,   276,   276,   277,   277,   278,   278,
     279,   280,   281,   281,   281,   281,   281,   282,   281,   283,
     284,   285,   285,   285,   285,   285,   286,   286,   287,   288,
     288,   289,   290,   290,   291,   292,   291,   291,   293,   294,
     294,   295,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   304,   304,   305,   306,   306,   307,   307,   307,
     307,   307,   307,   308,   309,   309,   310,   310,   311,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   313,   313,
     315,   314,   316,   316,   317,   318,   319,   319
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     0,     0,     0,    19,     1,     2,
       2,     1,     2,     2,     2,     1,     0,     1,     2,     5,
       1,     2,     5,     4,     7,     1,     0,     1,     1,     1,
       1,     2,     2,     2,     2,     4,     4,     4,     4,     4,
       4,     5,     5,     5,     5,     5,     5,     4,     1,     0,
       5,     1,     2,     4,     3,     2,     2,     4,     1,     0,
       1,     2,     4,     3,     1,     2,     5,     3,     1,     2,
       1,     1,     5,     4,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     5,     4,     4,     4,     4,     2,     0,
       4,     4,     1,     1,     6,     4,     0,     3,     2,     3,
       3,     3,     3,     3,     1,     1,     2,     0,     1,     1,
       1,     8,     7,     7,     7,     1,     1,     1,     1,     7,
       7,     7,     7,     8,     7,     7,     7,     5,     7,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     7,     7,
       7,     7,     7,     8,     8,     8,     8,     3,     5,     4,
       4,     5,     7,     4,     1,     2,     3,     5,     4,     1,
       0,     1,     2,     1,     1,     5,     4,     3,     2,     3,
       3,     1,     3,     3,     3,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     1,     0,     4,     0,     4,     2,     0,
       4,     0,     4,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     6,     4,     0,
       1,     2,     3,     1,     0,     1,     2,     1,     1,     1,
       1,     1,     3,     3,     5,     3,     5,     3,     3,     1,
       0,     1,     2,     5,     3,     1,     0,     1,     2,     4,
       6,     1,     0,     1,     2,     4,     6,     1,     0,     1,
       2,     4,     1,     0,     1,     2,     4,     1,     0,     1,
       2,     4,     4,     1,     0,     1,     2,     4,     4,     4,
       1,     0,     1,     2,     6,     0,     7,     4,     1,     1,
       1,     2,     2,     3,     1,     2,     0,     4,     1,     1,
       1,     6,     2,     0,     3,     1,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     2,     2,     0,     4,     1,
       1,     1,     3,     1,     2,     4,     1,     2,     1,     1,
       2,     3,     1,     2,     1,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     2,
       4,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     2,     1,     2,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       0,     6,     4,     0,     1,     1,     2,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* $@1: %empty  */
#line 168 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                          { if (define_policy(pass, 0) == -1) return -1; }
#line 2151 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 5: /* $@2: %empty  */
#line 170 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                          { if (pass == 1) { if (policydb_index_classes(policydbp)) return -1; }
                            else if (pass == 2) { if (policydb_index_others(NULL, policydbp, 0)) return -1; }}
#line 2158 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 6: /* $@3: %empty  */
#line 173 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                         { if (pass == 1) { if (policydb_index_bools(policydbp)) return -1;}
			   else if (pass == 2) { if (policydb_index_others(NULL, policydbp, 0)) return -1;}}
#line 2165 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 10: /* class_def: CLASS identifier  */
#line 181 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_class()) return -1;}
#line 2171 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 13: /* initial_sid_def: SID identifier  */
#line 187 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_initial_sid()) return -1;}
#line 2177 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 19: /* common_perms_def: COMMON identifier '{' identifier_list '}'  */
#line 198 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_common_perms()) return -1;}
#line 2183 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 22: /* av_perms_def: CLASS identifier '{' identifier_list '}'  */
#line 204 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_av_perms(FALSE)) return -1;}
#line 2189 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 23: /* av_perms_def: CLASS identifier INHERITS identifier  */
#line 206 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_av_perms(TRUE)) return -1;}
#line 2195 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 24: /* av_perms_def: CLASS identifier INHERITS identifier '{' identifier_list '}'  */
#line 208 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_av_perms(TRUE)) return -1;}
#line 2201 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 35: /* default_user_def: DEFAULT_USER names SOURCE ';'  */
#line 223 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_user(DEFAULT_SOURCE)) return -1; }
#line 2207 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 36: /* default_user_def: DEFAULT_USER names TARGET ';'  */
#line 225 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_user(DEFAULT_TARGET)) return -1; }
#line 2213 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 37: /* default_role_def: DEFAULT_ROLE names SOURCE ';'  */
#line 228 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_role(DEFAULT_SOURCE)) return -1; }
#line 2219 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 38: /* default_role_def: DEFAULT_ROLE names TARGET ';'  */
#line 230 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_role(DEFAULT_TARGET)) return -1; }
#line 2225 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 39: /* default_type_def: DEFAULT_TYPE names SOURCE ';'  */
#line 233 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_type(DEFAULT_SOURCE)) return -1; }
#line 2231 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 40: /* default_type_def: DEFAULT_TYPE names TARGET ';'  */
#line 235 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_type(DEFAULT_TARGET)) return -1; }
#line 2237 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 41: /* default_range_def: DEFAULT_RANGE names SOURCE LOW ';'  */
#line 238 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_SOURCE_LOW)) return -1; }
#line 2243 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 42: /* default_range_def: DEFAULT_RANGE names SOURCE HIGH ';'  */
#line 240 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_SOURCE_HIGH)) return -1; }
#line 2249 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 43: /* default_range_def: DEFAULT_RANGE names SOURCE LOW_HIGH ';'  */
#line 242 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_SOURCE_LOW_HIGH)) return -1; }
#line 2255 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 44: /* default_range_def: DEFAULT_RANGE names TARGET LOW ';'  */
#line 244 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_TARGET_LOW)) return -1; }
#line 2261 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 45: /* default_range_def: DEFAULT_RANGE names TARGET HIGH ';'  */
#line 246 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_TARGET_HIGH)) return -1; }
#line 2267 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 46: /* default_range_def: DEFAULT_RANGE names TARGET LOW_HIGH ';'  */
#line 248 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_TARGET_LOW_HIGH)) return -1; }
#line 2273 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 47: /* default_range_def: DEFAULT_RANGE names GLBLUB ';'  */
#line 250 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_default_range(DEFAULT_GLBLUB)) return -1; }
#line 2279 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 53: /* sensitivity_def: SENSITIVITY identifier alias_def ';'  */
#line 261 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_sens()) return -1;}
#line 2285 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 54: /* sensitivity_def: SENSITIVITY identifier ';'  */
#line 263 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_sens()) return -1;}
#line 2291 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 56: /* dominance: DOMINANCE identifier  */
#line 268 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_dominance()) return -1;}
#line 2297 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 57: /* dominance: DOMINANCE '{' identifier_list '}'  */
#line 270 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_dominance()) return -1;}
#line 2303 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 62: /* category_def: CATEGORY identifier alias_def ';'  */
#line 279 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_category()) return -1;}
#line 2309 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 63: /* category_def: CATEGORY identifier ';'  */
#line 281 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_category()) return -1;}
#line 2315 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 66: /* level_def: LEVEL identifier ':' id_comma_list ';'  */
#line 287 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_level()) return -1;}
#line 2321 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 67: /* level_def: LEVEL identifier ';'  */
#line 289 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_level()) return -1;}
#line 2327 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 72: /* mlsconstraint_def: MLSCONSTRAIN names names cexpr ';'  */
#line 298 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_constraint((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2333 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 73: /* mlsvalidatetrans_def: MLSVALIDATETRANS names cexpr ';'  */
#line 301 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_validatetrans((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2339 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 101: /* attribute_def: ATTRIBUTE identifier ';'  */
#line 335 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_attrib()) return -1;}
#line 2345 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 102: /* expandattribute_def: EXPANDATTRIBUTE names bool_val ';'  */
#line 338 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (expand_attrib()) return -1;}
#line 2351 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 103: /* type_def: TYPE identifier alias_def opt_attr_list ';'  */
#line 341 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_type(1)) return -1;}
#line 2357 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 104: /* type_def: TYPE identifier opt_attr_list ';'  */
#line 343 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_type(0)) return -1;}
#line 2363 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 105: /* typealias_def: TYPEALIAS identifier alias_def ';'  */
#line 346 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_typealias()) return -1;}
#line 2369 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 106: /* typeattribute_def: TYPEATTRIBUTE identifier id_comma_list ';'  */
#line 349 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_typeattribute()) return -1;}
#line 2375 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 107: /* typebounds_def: TYPEBOUNDS identifier id_comma_list ';'  */
#line 352 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_typebounds()) return -1;}
#line 2381 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 110: /* bool_def: BOOL identifier bool_val ';'  */
#line 358 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_bool_tunable(0)) return -1; }
#line 2387 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 111: /* tunable_def: TUNABLE identifier bool_val ';'  */
#line 361 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_bool_tunable(1)) return -1; }
#line 2393 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 112: /* bool_val: CTRUE  */
#line 364 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("T",0)) return -1; }
#line 2399 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 113: /* bool_val: CFALSE  */
#line 366 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("F",0)) return -1; }
#line 2405 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 114: /* cond_stmt_def: IF cond_expr '{' cond_pol_list '}' cond_else  */
#line 369 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (pass == 2) { if (define_conditional((cond_expr_t*)(yyvsp[-4].ptr), (avrule_t*)(yyvsp[-2].ptr), (avrule_t*)(yyvsp[0].ptr)) < 0) return -1;  }}
#line 2411 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 115: /* cond_else: ELSE '{' cond_pol_list '}'  */
#line 372 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = (yyvsp[-1].ptr); }
#line 2417 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 116: /* cond_else: %empty  */
#line 374 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = NULL; }
#line 2423 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 117: /* cond_expr: '(' cond_expr ')'  */
#line 377 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = (yyvsp[-1].ptr);}
#line 2429 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 118: /* cond_expr: NOT cond_expr  */
#line 379 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_NOT, (yyvsp[0].ptr), 0);
			  if ((yyval.ptr) == 0) return -1; }
#line 2436 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 119: /* cond_expr: cond_expr AND cond_expr  */
#line 382 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_AND, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2443 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 120: /* cond_expr: cond_expr OR cond_expr  */
#line 385 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_OR, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return   -1; }
#line 2450 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 121: /* cond_expr: cond_expr XOR cond_expr  */
#line 388 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_XOR, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2457 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 122: /* cond_expr: cond_expr EQUALS cond_expr  */
#line 391 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_EQ, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2464 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 123: /* cond_expr: cond_expr NOTEQUAL cond_expr  */
#line 394 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_NEQ, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2471 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 124: /* cond_expr: cond_expr_prim  */
#line 397 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2477 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 125: /* cond_expr_prim: identifier  */
#line 400 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_expr(COND_BOOL,0, 0);
			  if ((yyval.ptr) == COND_ERR) return   -1; }
#line 2484 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 126: /* cond_pol_list: cond_pol_list cond_rule_def  */
#line 404 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_pol_list((avrule_t *)(yyvsp[-1].ptr), (avrule_t *)(yyvsp[0].ptr)); }
#line 2490 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 127: /* cond_pol_list: %empty  */
#line 406 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = NULL; }
#line 2496 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 128: /* cond_rule_def: cond_transition_def  */
#line 409 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2502 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 129: /* cond_rule_def: cond_te_avtab_def  */
#line 411 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2508 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 130: /* cond_rule_def: require_block  */
#line 413 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = NULL; }
#line 2514 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 131: /* cond_transition_def: TYPE_TRANSITION names names ':' names identifier filename ';'  */
#line 416 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_filename_trans() ;
                          if ((yyval.ptr) == COND_ERR) return -1;}
#line 2521 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 132: /* cond_transition_def: TYPE_TRANSITION names names ':' names identifier ';'  */
#line 419 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_compute_type(AVRULE_TRANSITION) ;
                          if ((yyval.ptr) == COND_ERR) return -1;}
#line 2528 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 133: /* cond_transition_def: TYPE_MEMBER names names ':' names identifier ';'  */
#line 422 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_compute_type(AVRULE_MEMBER) ;
                          if ((yyval.ptr) ==  COND_ERR) return -1;}
#line 2535 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 134: /* cond_transition_def: TYPE_CHANGE names names ':' names identifier ';'  */
#line 425 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_compute_type(AVRULE_CHANGE) ;
                          if ((yyval.ptr) == COND_ERR) return -1;}
#line 2542 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 135: /* cond_te_avtab_def: cond_allow_def  */
#line 429 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                          { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2548 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 136: /* cond_te_avtab_def: cond_auditallow_def  */
#line 431 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                          { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2554 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 137: /* cond_te_avtab_def: cond_auditdeny_def  */
#line 433 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                          { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2560 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 138: /* cond_te_avtab_def: cond_dontaudit_def  */
#line 435 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                          { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2566 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 139: /* cond_allow_def: ALLOW names names ':' names names ';'  */
#line 438 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_te_avtab(AVRULE_ALLOWED) ;
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2573 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 140: /* cond_auditallow_def: AUDITALLOW names names ':' names names ';'  */
#line 442 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_te_avtab(AVRULE_AUDITALLOW) ;
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2580 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 141: /* cond_auditdeny_def: AUDITDENY names names ':' names names ';'  */
#line 446 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_te_avtab(AVRULE_AUDITDENY) ;
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2587 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 142: /* cond_dontaudit_def: DONTAUDIT names names ':' names names ';'  */
#line 450 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = define_cond_te_avtab(AVRULE_DONTAUDIT);
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2594 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 143: /* transition_def: TYPE_TRANSITION names names ':' names identifier filename ';'  */
#line 455 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_filename_trans()) return -1; }
#line 2600 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 144: /* transition_def: TYPE_TRANSITION names names ':' names identifier ';'  */
#line 457 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_compute_type(AVRULE_TRANSITION)) return -1;}
#line 2606 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 145: /* transition_def: TYPE_MEMBER names names ':' names identifier ';'  */
#line 459 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_compute_type(AVRULE_MEMBER)) return -1;}
#line 2612 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 146: /* transition_def: TYPE_CHANGE names names ':' names identifier ';'  */
#line 461 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_compute_type(AVRULE_CHANGE)) return -1;}
#line 2618 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 147: /* range_trans_def: RANGE_TRANSITION names names mls_range_def ';'  */
#line 464 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_range_trans(0)) return -1; }
#line 2624 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 148: /* range_trans_def: RANGE_TRANSITION names names ':' names mls_range_def ';'  */
#line 466 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_range_trans(1)) return -1; }
#line 2630 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 158: /* allow_def: ALLOW names names ':' names names ';'  */
#line 479 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab(AVRULE_ALLOWED)) return -1; }
#line 2636 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 159: /* auditallow_def: AUDITALLOW names names ':' names names ';'  */
#line 482 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab(AVRULE_AUDITALLOW)) return -1; }
#line 2642 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 160: /* auditdeny_def: AUDITDENY names names ':' names names ';'  */
#line 485 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab(AVRULE_AUDITDENY)) return -1; }
#line 2648 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 161: /* dontaudit_def: DONTAUDIT names names ':' names names ';'  */
#line 488 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab(AVRULE_DONTAUDIT)) return -1; }
#line 2654 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 162: /* neverallow_def: NEVERALLOW names names ':' names names ';'  */
#line 491 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab(AVRULE_NEVERALLOW)) return -1; }
#line 2660 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 163: /* xperm_allow_def: ALLOWXPERM names names ':' names identifier xperms ';'  */
#line 494 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab_extended_perms(AVRULE_XPERMS_ALLOWED)) return -1; }
#line 2666 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 164: /* xperm_auditallow_def: AUDITALLOWXPERM names names ':' names identifier xperms ';'  */
#line 497 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab_extended_perms(AVRULE_XPERMS_AUDITALLOW)) return -1; }
#line 2672 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 165: /* xperm_dontaudit_def: DONTAUDITXPERM names names ':' names identifier xperms ';'  */
#line 500 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab_extended_perms(AVRULE_XPERMS_DONTAUDIT)) return -1; }
#line 2678 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 166: /* xperm_neverallow_def: NEVERALLOWXPERM names names ':' names identifier xperms ';'  */
#line 503 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_te_avtab_extended_perms(AVRULE_XPERMS_NEVERALLOW)) return -1; }
#line 2684 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 167: /* attribute_role_def: ATTRIBUTE_ROLE identifier ';'  */
#line 506 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_attrib_role()) return -1; }
#line 2690 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 168: /* role_type_def: ROLE identifier TYPES names ';'  */
#line 509 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_role_types()) return -1;}
#line 2696 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 169: /* role_attr_def: ROLE identifier opt_attr_list ';'  */
#line 512 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_role_attr()) return -1;}
#line 2702 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 171: /* role_trans_def: ROLE_TRANSITION names names identifier ';'  */
#line 517 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_role_trans(0)) return -1; }
#line 2708 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 172: /* role_trans_def: ROLE_TRANSITION names names ':' names identifier ';'  */
#line 519 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_role_trans(1)) return -1;}
#line 2714 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 173: /* role_allow_def: ALLOW names names ';'  */
#line 522 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_role_allow()) return -1; }
#line 2720 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 174: /* roles: role_def  */
#line 525 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2726 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 175: /* roles: roles role_def  */
#line 527 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.ptr) = merge_roles_dom((role_datum_t*)(yyvsp[-1].ptr), (role_datum_t*)(yyvsp[0].ptr)); if ((yyval.ptr) == 0) return -1;}
#line 2732 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 176: /* role_def: ROLE identifier_push ';'  */
#line 530 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {(yyval.ptr) = define_role_dom(NULL); if ((yyval.ptr) == 0) return -1;}
#line 2738 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 177: /* role_def: ROLE identifier_push '{' roles '}'  */
#line 532 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {(yyval.ptr) = define_role_dom((role_datum_t*)(yyvsp[-1].ptr)); if ((yyval.ptr) == 0) return -1;}
#line 2744 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 178: /* roleattribute_def: ROLEATTRIBUTE identifier id_comma_list ';'  */
#line 535 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_roleattribute()) return -1;}
#line 2750 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 185: /* constraint_def: CONSTRAIN names names cexpr ';'  */
#line 547 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_constraint((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2756 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 186: /* validatetrans_def: VALIDATETRANS names cexpr ';'  */
#line 550 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_validatetrans((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2762 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 187: /* cexpr: '(' cexpr ')'  */
#line 553 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = (yyvsp[-1].valptr); }
#line 2768 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 188: /* cexpr: NOT cexpr  */
#line 555 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NOT, (yyvsp[0].valptr), 0);
			  if ((yyval.valptr) == 0) return -1; }
#line 2775 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 189: /* cexpr: cexpr AND cexpr  */
#line 558 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_AND, (yyvsp[-2].valptr), (yyvsp[0].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2782 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 190: /* cexpr: cexpr OR cexpr  */
#line 561 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_OR, (yyvsp[-2].valptr), (yyvsp[0].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2789 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 191: /* cexpr: cexpr_prim  */
#line 564 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = (yyvsp[0].valptr); }
#line 2795 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 192: /* cexpr_prim: U1 op U2  */
#line 567 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_USER, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2802 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 193: /* cexpr_prim: R1 role_mls_op R2  */
#line 570 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_ROLE, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2809 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 194: /* cexpr_prim: T1 op T2  */
#line 573 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_TYPE, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2816 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 195: /* $@4: %empty  */
#line 575 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2822 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 196: /* cexpr_prim: U1 op $@4 names_push  */
#line 576 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_USER, (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2829 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 197: /* $@5: %empty  */
#line 578 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2835 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 198: /* cexpr_prim: U2 op $@5 names_push  */
#line 579 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_USER | CEXPR_TARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2842 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 199: /* $@6: %empty  */
#line 581 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2848 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 200: /* cexpr_prim: U3 op $@6 names_push  */
#line 582 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_USER | CEXPR_XTARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2855 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 201: /* $@7: %empty  */
#line 584 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2861 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 202: /* cexpr_prim: R1 op $@7 names_push  */
#line 585 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_ROLE, (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2868 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 203: /* $@8: %empty  */
#line 587 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2874 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 204: /* cexpr_prim: R2 op $@8 names_push  */
#line 588 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_ROLE | CEXPR_TARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2881 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 205: /* $@9: %empty  */
#line 590 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2887 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 206: /* cexpr_prim: R3 op $@9 names_push  */
#line 591 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_ROLE | CEXPR_XTARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2894 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 207: /* $@10: %empty  */
#line 593 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2900 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 208: /* cexpr_prim: T1 op $@10 names_push  */
#line 594 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_TYPE, (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2907 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 209: /* $@11: %empty  */
#line 596 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2913 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 210: /* cexpr_prim: T2 op $@11 names_push  */
#line 597 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_TYPE | CEXPR_TARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2920 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 211: /* $@12: %empty  */
#line 599 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_separator(1)) return -1; }
#line 2926 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 212: /* cexpr_prim: T3 op $@12 names_push  */
#line 600 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_TYPE | CEXPR_XTARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2933 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 213: /* cexpr_prim: SAMEUSER  */
#line 603 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_USER, CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2940 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 214: /* $@13: %empty  */
#line 605 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { if (insert_separator(1)) return -1; }
#line 2946 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 215: /* cexpr_prim: SOURCE ROLE $@13 names_push  */
#line 606 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_ROLE, CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2953 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 216: /* $@14: %empty  */
#line 608 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { if (insert_separator(1)) return -1; }
#line 2959 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 217: /* cexpr_prim: TARGET ROLE $@14 names_push  */
#line 609 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_ROLE | CEXPR_TARGET), CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2966 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 218: /* cexpr_prim: ROLE role_mls_op  */
#line 612 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_ROLE, (yyvsp[0].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2973 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 219: /* $@15: %empty  */
#line 614 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { if (insert_separator(1)) return -1; }
#line 2979 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 220: /* cexpr_prim: SOURCE TYPE $@15 names_push  */
#line 615 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_TYPE, CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2986 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 221: /* $@16: %empty  */
#line 617 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { if (insert_separator(1)) return -1; }
#line 2992 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 222: /* cexpr_prim: TARGET TYPE $@16 names_push  */
#line 618 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_TYPE | CEXPR_TARGET), CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2999 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 223: /* cexpr_prim: L1 role_mls_op L2  */
#line 621 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L1L2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 3006 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 224: /* cexpr_prim: L1 role_mls_op H2  */
#line 624 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L1H2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 3013 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 225: /* cexpr_prim: H1 role_mls_op L2  */
#line 627 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_H1L2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 3020 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 226: /* cexpr_prim: H1 role_mls_op H2  */
#line 630 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_H1H2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 3027 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 227: /* cexpr_prim: L1 role_mls_op H1  */
#line 633 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L1H1, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 3034 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 228: /* cexpr_prim: L2 role_mls_op H2  */
#line 636 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L2H2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 3041 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 229: /* op: EQUALS  */
#line 640 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = CEXPR_EQ; }
#line 3047 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 230: /* op: NOTEQUAL  */
#line 642 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = CEXPR_NEQ; }
#line 3053 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 231: /* role_mls_op: op  */
#line 645 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = (yyvsp[0].valptr); }
#line 3059 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 232: /* role_mls_op: DOM  */
#line 647 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = CEXPR_DOM; }
#line 3065 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 233: /* role_mls_op: DOMBY  */
#line 649 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = CEXPR_DOMBY; }
#line 3071 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 234: /* role_mls_op: INCOMP  */
#line 651 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { (yyval.valptr) = CEXPR_INCOMP; }
#line 3077 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 237: /* user_def: USER identifier ROLES names opt_mls_user ';'  */
#line 657 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_user()) return -1;}
#line 3083 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 242: /* initial_sid_context_def: SID identifier security_context_def  */
#line 666 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_initial_sid_context()) return -1;}
#line 3089 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 252: /* pirq_context_def: PIRQCON number security_context_def  */
#line 680 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_pirq_context((yyvsp[-1].val))) return -1;}
#line 3095 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 253: /* iomem_context_def: IOMEMCON number64 security_context_def  */
#line 683 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_iomem_context((yyvsp[-1].val64),(yyvsp[-1].val64))) return -1;}
#line 3101 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 254: /* iomem_context_def: IOMEMCON number64 '-' number64 security_context_def  */
#line 685 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_iomem_context((yyvsp[-3].val64),(yyvsp[-1].val64))) return -1;}
#line 3107 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 255: /* ioport_context_def: IOPORTCON number security_context_def  */
#line 688 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ioport_context((yyvsp[-1].val),(yyvsp[-1].val))) return -1;}
#line 3113 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 256: /* ioport_context_def: IOPORTCON number '-' number security_context_def  */
#line 690 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ioport_context((yyvsp[-3].val),(yyvsp[-1].val))) return -1;}
#line 3119 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 257: /* pci_context_def: PCIDEVICECON number security_context_def  */
#line 693 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_pcidevice_context((yyvsp[-1].val))) return -1;}
#line 3125 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 258: /* dtree_context_def: DEVICETREECON path security_context_def  */
#line 696 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_devicetree_context()) return -1;}
#line 3131 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 263: /* fs_context_def: FSCON number number security_context_def security_context_def  */
#line 705 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_fs_context((yyvsp[-3].val),(yyvsp[-2].val))) return -1;}
#line 3137 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 269: /* port_context_def: PORTCON identifier number security_context_def  */
#line 716 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_port_context((yyvsp[-1].val),(yyvsp[-1].val))) return -1;}
#line 3143 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 270: /* port_context_def: PORTCON identifier number '-' number security_context_def  */
#line 718 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_port_context((yyvsp[-3].val),(yyvsp[-1].val))) return -1;}
#line 3149 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 275: /* ibpkey_context_def: IBPKEYCON ipv6_addr number security_context_def  */
#line 727 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ibpkey_context((yyvsp[-1].val),(yyvsp[-1].val))) return -1;}
#line 3155 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 276: /* ibpkey_context_def: IBPKEYCON ipv6_addr number '-' number security_context_def  */
#line 729 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ibpkey_context((yyvsp[-3].val),(yyvsp[-1].val))) return -1;}
#line 3161 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 281: /* ibendport_context_def: IBENDPORTCON identifier number security_context_def  */
#line 738 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ibendport_context((yyvsp[-1].val))) return -1;}
#line 3167 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 286: /* netif_context_def: NETIFCON identifier security_context_def security_context_def  */
#line 747 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_netif_context()) return -1;}
#line 3173 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 291: /* node_context_def: NODECON ipv4_addr_def ipv4_addr_def security_context_def  */
#line 756 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ipv4_node_context()) return -1;}
#line 3179 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 292: /* node_context_def: NODECON ipv6_addr ipv6_addr security_context_def  */
#line 758 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_ipv6_node_context()) return -1;}
#line 3185 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 297: /* fs_use_def: FSUSEXATTR filesystem security_context_def ';'  */
#line 767 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_fs_use(SECURITY_FS_USE_XATTR)) return -1;}
#line 3191 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 298: /* fs_use_def: FSUSETASK identifier security_context_def ';'  */
#line 769 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_fs_use(SECURITY_FS_USE_TASK)) return -1;}
#line 3197 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 299: /* fs_use_def: FSUSETRANS identifier security_context_def ';'  */
#line 771 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_fs_use(SECURITY_FS_USE_TRANS)) return -1;}
#line 3203 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 304: /* genfs_context_def: GENFSCON filesystem path '-' identifier security_context_def  */
#line 780 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_genfs_context(1)) return -1;}
#line 3209 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 305: /* $@17: %empty  */
#line 781 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                                           {insert_id("-", 0);}
#line 3215 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 306: /* genfs_context_def: GENFSCON filesystem path '-' '-' $@17 security_context_def  */
#line 782 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_genfs_context(1)) return -1;}
#line 3221 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 307: /* genfs_context_def: GENFSCON filesystem path security_context_def  */
#line 784 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_genfs_context(0)) return -1;}
#line 3227 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 308: /* ipv4_addr_def: IPV4_ADDR  */
#line 787 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3233 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 309: /* xperms: xperm  */
#line 790 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_separator(0)) return -1; }
#line 3239 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 310: /* xperms: nested_xperm_set  */
#line 792 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_separator(0)) return -1; }
#line 3245 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 311: /* xperms: tilde xperm  */
#line 794 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("~", 0)) return -1; }
#line 3251 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 312: /* xperms: tilde nested_xperm_set  */
#line 796 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("~", 0)) return -1;
			  if (insert_separator(0)) return -1; }
#line 3258 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 316: /* $@18: %empty  */
#line 804 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                { if (insert_id("-", 0)) return -1; }
#line 3264 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 320: /* xperm: number  */
#line 809 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3270 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 324: /* mls_range_def: mls_level_def '-' mls_level_def  */
#line 817 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (insert_separator(0)) return -1;}
#line 3276 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 325: /* mls_range_def: mls_level_def  */
#line 819 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (insert_separator(0)) return -1;}
#line 3282 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 326: /* mls_level_def: identifier ':' id_comma_list  */
#line 822 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (insert_separator(0)) return -1;}
#line 3288 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 327: /* mls_level_def: identifier  */
#line 824 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (insert_separator(0)) return -1;}
#line 3294 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 332: /* names: identifier  */
#line 834 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_separator(0)) return -1; }
#line 3300 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 333: /* names: nested_id_set  */
#line 836 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_separator(0)) return -1; }
#line 3306 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 334: /* names: asterisk  */
#line 838 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("*", 0)) return -1; 
			  if (insert_separator(0)) return -1; }
#line 3313 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 335: /* names: tilde identifier  */
#line 841 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("~", 0)) return -1;
			  if (insert_separator(0)) return -1; }
#line 3320 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 336: /* names: tilde nested_id_set  */
#line 844 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("~", 0)) return -1; 
			  if (insert_separator(0)) return -1; }
#line 3327 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 337: /* $@19: %empty  */
#line 846 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                         { if (insert_id("-", 0)) return -1; }
#line 3333 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 338: /* names: identifier '-' $@19 identifier  */
#line 847 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_separator(0)) return -1; }
#line 3339 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 339: /* tilde_push: tilde  */
#line 850 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("~", 1)) return -1; }
#line 3345 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 340: /* asterisk_push: asterisk  */
#line 853 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id("*", 1)) return -1; }
#line 3351 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 348: /* identifier_push: IDENTIFIER  */
#line 865 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext, 1)) return -1; }
#line 3357 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 355: /* $@20: %empty  */
#line 874 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                           { if (insert_id("-", 0)) return -1; }
#line 3363 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 358: /* identifier: IDENTIFIER  */
#line 877 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3369 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 359: /* filesystem: FILESYSTEM  */
#line 880 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3375 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 360: /* filesystem: IDENTIFIER  */
#line 882 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3381 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 361: /* path: PATH  */
#line 885 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3387 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 362: /* path: QPATH  */
#line 887 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { yytext[strlen(yytext) - 1] = '\0'; if (insert_id(yytext + 1,0)) return -1; }
#line 3393 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 363: /* filename: FILENAME  */
#line 890 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { yytext[strlen(yytext) - 1] = '\0'; if (insert_id(yytext + 1,0)) return -1; }
#line 3399 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 364: /* number: NUMBER  */
#line 893 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { unsigned long x;
			  errno = 0;
			  x = strtoul(yytext, NULL, 0);
			  if (errno)
			      return -1;
#if ULONG_MAX > UINT_MAX
			  if (x > UINT_MAX)
			      return -1;
#endif
			  (yyval.val) = (unsigned int) x;
			}
#line 3415 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 365: /* number64: NUMBER  */
#line 906 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { unsigned long long x;
			  errno = 0;
			  x = strtoull(yytext, NULL, 0);
			  if (errno)
			      return -1;
			  (yyval.val64) = (uint64_t) x;
			}
#line 3427 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 366: /* ipv6_addr: IPV6_ADDR  */
#line 915 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3433 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 367: /* policycap_def: POLICYCAP identifier ';'  */
#line 918 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_polcap()) return -1;}
#line 3439 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 368: /* permissive_def: PERMISSIVE identifier ';'  */
#line 921 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        {if (define_permissive()) return -1;}
#line 3445 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 369: /* module_policy: module_def avrules_block  */
#line 926 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (end_avrule_block(pass) == -1) return -1;
                          if (policydb_index_others(NULL, policydbp, 0)) return -1;
                        }
#line 3453 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 370: /* module_def: MODULE identifier version_identifier ';'  */
#line 931 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (define_policy(pass, 1) == -1) return -1; }
#line 3459 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 371: /* version_identifier: VERSION_IDENTIFIER  */
#line 934 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3465 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 372: /* version_identifier: number  */
#line 936 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (insert_id(yytext,0)) return -1; }
#line 3471 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 388: /* require_class: CLASS identifier names  */
#line 960 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (require_class(pass)) return -1; }
#line 3477 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 389: /* require_decl_def: ROLE  */
#line 962 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_role; }
#line 3483 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 390: /* require_decl_def: TYPE  */
#line 963 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_type; }
#line 3489 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 391: /* require_decl_def: ATTRIBUTE  */
#line 964 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_attribute; }
#line 3495 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 392: /* require_decl_def: ATTRIBUTE_ROLE  */
#line 965 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                           { (yyval.require_func) = require_attribute_role; }
#line 3501 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 393: /* require_decl_def: USER  */
#line 966 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_user; }
#line 3507 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 394: /* require_decl_def: BOOL  */
#line 967 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_bool; }
#line 3513 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 395: /* require_decl_def: TUNABLE  */
#line 968 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_tunable; }
#line 3519 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 396: /* require_decl_def: SENSITIVITY  */
#line 969 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_sens; }
#line 3525 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 397: /* require_decl_def: CATEGORY  */
#line 970 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                                      { (yyval.require_func) = require_cat; }
#line 3531 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 398: /* require_id_list: identifier  */
#line 973 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if ((yyvsp[-1].require_func) (pass)) return -1; }
#line 3537 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 399: /* require_id_list: require_id_list ',' identifier  */
#line 975 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if ((yyvsp[-3].require_func) (pass)) return -1; }
#line 3543 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 400: /* $@21: %empty  */
#line 978 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (end_avrule_block(pass) == -1) return -1; }
#line 3549 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 401: /* optional_block: optional_decl '{' avrules_block '}' $@21 optional_else  */
#line 980 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (end_optional(pass) == -1) return -1; }
#line 3555 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 402: /* optional_else: else_decl '{' avrules_block '}'  */
#line 983 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (end_avrule_block(pass) == -1) return -1; }
#line 3561 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 404: /* optional_decl: OPTIONAL  */
#line 987 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (begin_optional(pass) == -1) return -1; }
#line 3567 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;

  case 405: /* else_decl: ELSE  */
#line 990 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"
                        { if (begin_optional_else(pass) == -1) return -1; }
#line 3573 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"
    break;


#line 3577 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

