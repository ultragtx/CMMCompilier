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
    PunctuatorType_PlaceHolder,
    
}PunctuatorType;

const int NumOfPunctuators = PunctuatorType_PlaceHolder - PunctuatorType_LBracket;

const char * const Punctuators[NumOfPunctuators] = {
    "[",
    "]",
    "(",
    "(",
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
};

typedef enum {
    GeneralType_Identifier = 300,
    GeneralType_Constant,
    GeneralType_StringLiteral,
    GeneralType_PlaceHolder,
}GeneralType;

extern FILE *inFile;           // inpu
extern int lineno;             // line number

#endif
