//
//  main.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstring>
#include "general.h"
#include "lexer.h"
#include "parser.h"



FILE *inFile;           // inpu
int lineno;             // line number

void init() {
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SourceCode.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_1.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_2.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_3.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_4.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_5.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_6.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_7.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_8.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_9.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_10.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_11.c", "r+");
//    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_12.c", "r+");
    inFile = fopen("/Users/ultragtx/Desktop/Software4/CMMCompiler/Resource/SorceCodePart/SourceCode_13.c", "r+");
    
    if (inFile == NULL) {
        printf("[ERROR]: Can't open file!\n");
    }
    
    lineno = 0;
}

void test_lex() {
    int type;
    int intValue;
    char *strValue;
    
    while (lexOne(&type, &intValue, &strValue) == LexReturnType_OK) {
        if (type < KeywordType_PlaceHolder) {
            printf("keyword ");
        }
        else if (type < PunctuatorType_PlaceHolder) {
            printf("punctua ");
        }
        else if (type == GeneralType_Identifier) {
            printf("identif ");
        }
        else if (type == GeneralType_Constant) {
            printf("constat ");
        }
        else if (type == GeneralType_StringLiteral) {
            printf("stringl ");
        }
    }
}

int main(int argc, const char * argv[])
{
    init();
    
    //test_lex();
    parse();
    
    fclose(inFile);

    return 0;
}

