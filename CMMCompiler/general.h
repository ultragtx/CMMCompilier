//
//  general.h
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#ifndef CMMCompiler_general_h
#define CMMCompiler_general_h

#include <iostream>
#include <stack>

#define LEX_BUFF_SIZE 128

typedef enum {
    LexReturnType_OK,
    LexReturnType_EOF,
}LexReturnType;

typedef enum {
    KeywordType_Int = 100,
    KeywordType_Float,
    KeywordType_Double,
    KeywordType_Char,
    KeywordType_Void,
    KeywordType_If,
    KeywordType_Else,
    KeywordType_While,
    KeywordType_Return,
    KeywordType_Main,
    KeywordType_Printf,
    KeywordType_Scanf,
    KeywordType_For,
    KeywordType_PlaceHolder,
    
}KeywordType;

const int NumOfKeywords = KeywordType_PlaceHolder - KeywordType_Int;

const char * const KeyWords[NumOfKeywords] = {
    "int",
    "float",
    "double",
    "char",
    "void",
    "if",
    "else",
    "while",
    "return",
    "main",
    "printf",
    "scanf",
    "for",
};

typedef enum {
    PunctuatorType_LBracket = 200,                      //[
    PunctuatorType_RBracket,                            //]
    PunctuatorType_LParenthese,                         //(
    PunctuatorType_RParenthese,                         //(
    PunctuatorType_LBrace,                              //{
    PunctuatorType_RBrace,                              //}
    PunctuatorType_Plusplus,                            //++
    PunctuatorType_Minusminus,                          //--
    PunctuatorType_Multiple,                            //*
    PunctuatorType_Divide,                              ///
    PunctuatorType_Plus,                                //+
    PunctuatorType_Minus,                               //-
    PunctuatorType_Exclamation,                         //!
    PunctuatorType_Percent,                             //%
    PunctuatorType_Less,                                //<
    PunctuatorType_More,                                //>
    PunctuatorType_And,                                 //&
    PunctuatorType_Or,                                  //|
    PunctuatorType_Semicolon,                           //;
    PunctuatorType_Comma,                               //,
    PunctuatorType_Well,                                //#
    PunctuatorType_Euqal,                               //=
    PunctuatorType_LE,                                  //<=
    PunctuatorType_BE,                                  //>=
    PunctuatorType_EE,                                  //==
    PunctuatorType_NE,                                  //!=
    PunctuatorType_PlaceHolder,
    
}PunctuatorType;

const int NumOfPunctuators = PunctuatorType_PlaceHolder - PunctuatorType_LBracket;

const char * const Punctuators[NumOfPunctuators] = {
    "[",
    "]",
    "(",
    ")",
    "{",
    "}",
    "++",
    "--",
    "*",
    "/",
    "+",
    "-",
    "!",
    "%%",
    "<",
    ">",
    "&",
    "|",
    ";",
    ",",
    "#",
    "=",
    "<=",
    ">=",
    "==",
    "!=",
};

typedef enum {
    GeneralType_Identifier = 300,
    GeneralType_Constant,
    GeneralType_StringLiteral,
    GeneralType_PlaceHolder,
    GeneralType_EOF,
}GeneralType;

extern FILE *inFile;           // inpu
extern int lineno;             // line number

typedef enum {
    ES_Main = 1000,             //main
    ES_LP,                      //(
    ES_RP,                      //)
    ES_Id,                      //identifier
    ES_Const,                   //constant
    ES_StrL,                    //string_literal
    ES_LB,                      //[
    ES_RB,                      //]
    ES_PP,                      //++
    ES_MM,                      //--
    ES_Mul,                     //*
    ES_Div,                     ///
    ES_Add,                     //+
    ES_Minu,                    //-
    ES_Less,                    //<
    ES_More,                    //>
    ES_LE,                      //<=
    ES_BE,                      //>=
    ES_EE,                      //==
    ES_NE,                      //!=
    ES_Eq,                      //=
    ES_Comm,                    //,
    ES_Semi,                    //semi
    ES_Int,                     //int
    ES_Float,                   //float
    ES_LLB,                     //{
    ES_RLB,                     //}
    ES_If,                      //if
    ES_Else,                    //else
    ES_For,                     //for
    ES_Ret,                     //return
    ES_Pri,                     //printf
    ES_Sca,                     //scanf
    ES_And,                     //&
    ES_End,                     //$
    ES_PlaceHolder,
}EndingSymbol;

const int NumOfEndingSymbol = ES_PlaceHolder - ES_Main;

typedef enum {
    MS_Program = ES_PlaceHolder,
    MS_Primary_Expression,
    MS_Postfix_Expression,
    MS_Multiplicative_Expression,
    MS_Additive_Expression,
    MS_Relational_Expression,
    MS_Equality_Expression,
    MS_Assignment_Expression,
    MS_Assignment_Operator,
    MS_Expression,
    MS_Constant_Expression,
    MS_Declaration,
    MS_Declaration_Specifiers,
    MS_Init_Declarator_List,
    MS_Init_Declarator,
    MS_Type_Specifier,
    MS_Declarator,
    MS_Direct_Declarator,
    MS_Initializer,
    MS_Initializer_List,
    MS_Statement,
    MS_Compound_Statement,
    MS_Block_Item_List,
    MS_Block_Item,
    MS_Expression_Statement,
    MS_Selection_Statement,
    MS_Iteration_Statement,
    MS_Jump_Statement,
    MS_Printf_Statement,
    MS_Printf_Params,
    MS_Scanf_Statement,
    MS_Scanf_Params,
    MS_PlaceHolder,
}MiddleSymbol;

const int NumOfMiddleSymbol = MS_PlaceHolder - MS_Program;

struct ParserElem{
    int symbol;
    int endingSymbol = 0; // 0 for middle symbol without reduce
    int intValue; // const value; size; symbol table index
    char *strValue = NULL;
    ParserElem *firstChild = NULL; // child
    ParserElem *next = NULL; // brother
};

#endif
