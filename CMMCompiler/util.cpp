//
//  util.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "util.h"

EndingSymbol l2e(int lexSymbol) {       //lex symbol to ending symbol
    EndingSymbol result = ES_PlaceHolder;
    
    switch (lexSymbol) {
        case KeywordType_Main:
            result = ES_Main;
            break;
        case PunctuatorType_LParenthese:
            result = ES_LP;
            break;
        case PunctuatorType_RParenthese:
            result = ES_RP;
            break;
        case GeneralType_Identifier:
            result = ES_Id;
            break;
        case GeneralType_Constant:
            result = ES_Const;
            break;
        case GeneralType_StringLiteral:
            result = ES_StrL;
            break;
        case PunctuatorType_LBracket:
            result = ES_LB;
            break;
        case PunctuatorType_RBracket:
            result = ES_RB;
            break;
        case PunctuatorType_Plusplus:
            result = ES_PP;
            break;
        case PunctuatorType_Minusminus:
            result = ES_MM;
            break;
        case PunctuatorType_Multiple:
            result = ES_Mul;
            break;
        case PunctuatorType_Divide:
            result = ES_Div;
            break;
        case PunctuatorType_Plus:
            result = ES_Add;
            break;
        case PunctuatorType_Minus:
            result = ES_Minu;
            break;
        case PunctuatorType_Less:
            result = ES_Less;
            break;
        case PunctuatorType_More:
            result = ES_More;
            break;
        case PunctuatorType_LE:
            result = ES_LE;
            break;
        case PunctuatorType_BE:
            result = ES_BE;
            break;
        case PunctuatorType_EE:
            result = ES_EE;
            break;
        case PunctuatorType_NE:
            result = ES_NE;
            break;
        case PunctuatorType_Euqal:
            result = ES_Eq;
            break;
        case PunctuatorType_Comma:
            result = ES_Comm;
            break;
        case PunctuatorType_Semicolon:
            result = ES_Semi;
            break;
        case KeywordType_Int:
            result = ES_Int;
            break;
        case KeywordType_Float:
            result = ES_Float;
            break;
        case PunctuatorType_LBrace:
            result = ES_LLB;
            break;
        case PunctuatorType_RBrace:
            result = ES_RLB;
            break;
        case KeywordType_If:
            result = ES_If;
            break;
        case KeywordType_Else:
            result = ES_Else;
            break;
        case KeywordType_For:
            result = ES_For;
            break;
        case KeywordType_Return:
            result = ES_Ret;
            break;
        case KeywordType_Printf:
            result = ES_Pri;
            break;
        case KeywordType_Scanf:
            result = ES_Sca;
            break;
        case PunctuatorType_And:
            result = ES_And;
            break;
        case GeneralType_EOF:
            result = ES_End;
            break;
        default:
            printf("[ERROR] L2E Error!\n");
            break;
    }
    
    return result;
}

void error(const char *m) {
    //fprintf(stderr, "line %d: %s\n", lineno, m);
    printf("[ERROR]:line %d: %s\n", lineno, m);
    exit(1);
}
