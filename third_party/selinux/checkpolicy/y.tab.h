/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_USR1_HMOS_FOR_SYSTEM_SRC_INCREMENT_SOURCECODE_THIRD_PARTY_SELINUX_CHECKPOLICY_Y_TAB_H_INCLUDED
# define YY_YY_USR1_HMOS_FOR_SYSTEM_SRC_INCREMENT_SOURCECODE_THIRD_PARTY_SELINUX_CHECKPOLICY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PATH = 258,                    /* PATH  */
    QPATH = 259,                   /* QPATH  */
    FILENAME = 260,                /* FILENAME  */
    CLONE = 261,                   /* CLONE  */
    COMMON = 262,                  /* COMMON  */
    CLASS = 263,                   /* CLASS  */
    CONSTRAIN = 264,               /* CONSTRAIN  */
    VALIDATETRANS = 265,           /* VALIDATETRANS  */
    INHERITS = 266,                /* INHERITS  */
    SID = 267,                     /* SID  */
    ROLE = 268,                    /* ROLE  */
    ROLEATTRIBUTE = 269,           /* ROLEATTRIBUTE  */
    ATTRIBUTE_ROLE = 270,          /* ATTRIBUTE_ROLE  */
    ROLES = 271,                   /* ROLES  */
    TYPEALIAS = 272,               /* TYPEALIAS  */
    TYPEATTRIBUTE = 273,           /* TYPEATTRIBUTE  */
    TYPEBOUNDS = 274,              /* TYPEBOUNDS  */
    TYPE = 275,                    /* TYPE  */
    TYPES = 276,                   /* TYPES  */
    ALIAS = 277,                   /* ALIAS  */
    ATTRIBUTE = 278,               /* ATTRIBUTE  */
    EXPANDATTRIBUTE = 279,         /* EXPANDATTRIBUTE  */
    BOOL = 280,                    /* BOOL  */
    TUNABLE = 281,                 /* TUNABLE  */
    IF = 282,                      /* IF  */
    ELSE = 283,                    /* ELSE  */
    TYPE_TRANSITION = 284,         /* TYPE_TRANSITION  */
    TYPE_MEMBER = 285,             /* TYPE_MEMBER  */
    TYPE_CHANGE = 286,             /* TYPE_CHANGE  */
    ROLE_TRANSITION = 287,         /* ROLE_TRANSITION  */
    RANGE_TRANSITION = 288,        /* RANGE_TRANSITION  */
    SENSITIVITY = 289,             /* SENSITIVITY  */
    DOMINANCE = 290,               /* DOMINANCE  */
    DOM = 291,                     /* DOM  */
    DOMBY = 292,                   /* DOMBY  */
    INCOMP = 293,                  /* INCOMP  */
    CATEGORY = 294,                /* CATEGORY  */
    LEVEL = 295,                   /* LEVEL  */
    RANGE = 296,                   /* RANGE  */
    MLSCONSTRAIN = 297,            /* MLSCONSTRAIN  */
    MLSVALIDATETRANS = 298,        /* MLSVALIDATETRANS  */
    USER = 299,                    /* USER  */
    NEVERALLOW = 300,              /* NEVERALLOW  */
    ALLOW = 301,                   /* ALLOW  */
    AUDITALLOW = 302,              /* AUDITALLOW  */
    AUDITDENY = 303,               /* AUDITDENY  */
    DONTAUDIT = 304,               /* DONTAUDIT  */
    ALLOWXPERM = 305,              /* ALLOWXPERM  */
    AUDITALLOWXPERM = 306,         /* AUDITALLOWXPERM  */
    DONTAUDITXPERM = 307,          /* DONTAUDITXPERM  */
    NEVERALLOWXPERM = 308,         /* NEVERALLOWXPERM  */
    SOURCE = 309,                  /* SOURCE  */
    TARGET = 310,                  /* TARGET  */
    SAMEUSER = 311,                /* SAMEUSER  */
    FSCON = 312,                   /* FSCON  */
    PORTCON = 313,                 /* PORTCON  */
    NETIFCON = 314,                /* NETIFCON  */
    NODECON = 315,                 /* NODECON  */
    IBPKEYCON = 316,               /* IBPKEYCON  */
    IBENDPORTCON = 317,            /* IBENDPORTCON  */
    PIRQCON = 318,                 /* PIRQCON  */
    IOMEMCON = 319,                /* IOMEMCON  */
    IOPORTCON = 320,               /* IOPORTCON  */
    PCIDEVICECON = 321,            /* PCIDEVICECON  */
    DEVICETREECON = 322,           /* DEVICETREECON  */
    FSUSEXATTR = 323,              /* FSUSEXATTR  */
    FSUSETASK = 324,               /* FSUSETASK  */
    FSUSETRANS = 325,              /* FSUSETRANS  */
    GENFSCON = 326,                /* GENFSCON  */
    U1 = 327,                      /* U1  */
    U2 = 328,                      /* U2  */
    U3 = 329,                      /* U3  */
    R1 = 330,                      /* R1  */
    R2 = 331,                      /* R2  */
    R3 = 332,                      /* R3  */
    T1 = 333,                      /* T1  */
    T2 = 334,                      /* T2  */
    T3 = 335,                      /* T3  */
    L1 = 336,                      /* L1  */
    L2 = 337,                      /* L2  */
    H1 = 338,                      /* H1  */
    H2 = 339,                      /* H2  */
    NOT = 340,                     /* NOT  */
    AND = 341,                     /* AND  */
    OR = 342,                      /* OR  */
    XOR = 343,                     /* XOR  */
    CTRUE = 344,                   /* CTRUE  */
    CFALSE = 345,                  /* CFALSE  */
    IDENTIFIER = 346,              /* IDENTIFIER  */
    NUMBER = 347,                  /* NUMBER  */
    EQUALS = 348,                  /* EQUALS  */
    NOTEQUAL = 349,                /* NOTEQUAL  */
    IPV4_ADDR = 350,               /* IPV4_ADDR  */
    IPV6_ADDR = 351,               /* IPV6_ADDR  */
    MODULE = 352,                  /* MODULE  */
    VERSION_IDENTIFIER = 353,      /* VERSION_IDENTIFIER  */
    REQUIRE = 354,                 /* REQUIRE  */
    OPTIONAL = 355,                /* OPTIONAL  */
    POLICYCAP = 356,               /* POLICYCAP  */
    PERMISSIVE = 357,              /* PERMISSIVE  */
    FILESYSTEM = 358,              /* FILESYSTEM  */
    DEFAULT_USER = 359,            /* DEFAULT_USER  */
    DEFAULT_ROLE = 360,            /* DEFAULT_ROLE  */
    DEFAULT_TYPE = 361,            /* DEFAULT_TYPE  */
    DEFAULT_RANGE = 362,           /* DEFAULT_RANGE  */
    LOW_HIGH = 363,                /* LOW_HIGH  */
    LOW = 364,                     /* LOW  */
    HIGH = 365,                    /* HIGH  */
    GLBLUB = 366                   /* GLBLUB  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define PATH 258
#define QPATH 259
#define FILENAME 260
#define CLONE 261
#define COMMON 262
#define CLASS 263
#define CONSTRAIN 264
#define VALIDATETRANS 265
#define INHERITS 266
#define SID 267
#define ROLE 268
#define ROLEATTRIBUTE 269
#define ATTRIBUTE_ROLE 270
#define ROLES 271
#define TYPEALIAS 272
#define TYPEATTRIBUTE 273
#define TYPEBOUNDS 274
#define TYPE 275
#define TYPES 276
#define ALIAS 277
#define ATTRIBUTE 278
#define EXPANDATTRIBUTE 279
#define BOOL 280
#define TUNABLE 281
#define IF 282
#define ELSE 283
#define TYPE_TRANSITION 284
#define TYPE_MEMBER 285
#define TYPE_CHANGE 286
#define ROLE_TRANSITION 287
#define RANGE_TRANSITION 288
#define SENSITIVITY 289
#define DOMINANCE 290
#define DOM 291
#define DOMBY 292
#define INCOMP 293
#define CATEGORY 294
#define LEVEL 295
#define RANGE 296
#define MLSCONSTRAIN 297
#define MLSVALIDATETRANS 298
#define USER 299
#define NEVERALLOW 300
#define ALLOW 301
#define AUDITALLOW 302
#define AUDITDENY 303
#define DONTAUDIT 304
#define ALLOWXPERM 305
#define AUDITALLOWXPERM 306
#define DONTAUDITXPERM 307
#define NEVERALLOWXPERM 308
#define SOURCE 309
#define TARGET 310
#define SAMEUSER 311
#define FSCON 312
#define PORTCON 313
#define NETIFCON 314
#define NODECON 315
#define IBPKEYCON 316
#define IBENDPORTCON 317
#define PIRQCON 318
#define IOMEMCON 319
#define IOPORTCON 320
#define PCIDEVICECON 321
#define DEVICETREECON 322
#define FSUSEXATTR 323
#define FSUSETASK 324
#define FSUSETRANS 325
#define GENFSCON 326
#define U1 327
#define U2 328
#define U3 329
#define R1 330
#define R2 331
#define R3 332
#define T1 333
#define T2 334
#define T3 335
#define L1 336
#define L2 337
#define H1 338
#define H2 339
#define NOT 340
#define AND 341
#define OR 342
#define XOR 343
#define CTRUE 344
#define CFALSE 345
#define IDENTIFIER 346
#define NUMBER 347
#define EQUALS 348
#define NOTEQUAL 349
#define IPV4_ADDR 350
#define IPV6_ADDR 351
#define MODULE 352
#define VERSION_IDENTIFIER 353
#define REQUIRE 354
#define OPTIONAL 355
#define POLICYCAP 356
#define PERMISSIVE 357
#define FILESYSTEM 358
#define DEFAULT_USER 359
#define DEFAULT_ROLE 360
#define DEFAULT_TYPE 361
#define DEFAULT_RANGE 362
#define LOW_HIGH 363
#define LOW 364
#define HIGH 365
#define GLBLUB 366

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 68 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/policy_parse.y"

	unsigned int val;
	uint64_t val64;
	uintptr_t valptr;
	void *ptr;
        require_func_t require_func;

#line 297 "/usr1/hmos_for_system/src/increment/sourcecode/third_party/selinux/checkpolicy/y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_USR1_HMOS_FOR_SYSTEM_SRC_INCREMENT_SOURCECODE_THIRD_PARTY_SELINUX_CHECKPOLICY_Y_TAB_H_INCLUDED  */
