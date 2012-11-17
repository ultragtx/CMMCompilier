//
//  lexer.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "lexer.h"
#include "symbol.h"

char lexBuff[LEX_BUFF_SIZE];

void debugPrint(const char *str) {
    //printf("|%s| ", str);
    //fflush(stdout);
}

int typeOfWord(char *word) {
    for (int i = 0; i < NumOfKeywords; i++) {
        if (!strcmp(word, KeyWords[i])) {
            return KeywordType_Int + i;
        }
    }
    return -1;
}

int lexOne(int *type, int *intValue, char **strValue) {
    char elemCh;
    int buffIndex;
    
    while (1) {
        buffIndex = 0;          //clear lexBuff
        elemCh = fgetc(inFile);
        if (elemCh == ' ' || elemCh == '\t') {          //blank
            continue;
        }
        else if (elemCh == '\n') {                      //new line
            lineno++;
            debugPrint("\n");
            //printf("\n");
        }
        else if (elemCh == EOF) {                       //end of file
            *type = GeneralType_EOF;
            debugPrint("EOF");
            return LexReturnType_EOF;
        }
        else if (elemCh == '#') {                       //line start with '#'
            while (elemCh != '\n') {
                elemCh = fgetc(inFile);
            }
            lineno++;
            continue;
        }
        else if (isdigit(elemCh) || elemCh == '.') {    //number constant
            int dotCount = 0;
            while (isdigit(elemCh) || elemCh == '.') {  //did not chect dot count, assume this is correct
                if (elemCh == '.') {
                    dotCount++;
                }
                lexBuff[buffIndex++] = elemCh;
                elemCh = fgetc(inFile);
            }
            lexBuff[buffIndex] = '\0';
            
            if (elemCh != EOF) {
                ungetc(elemCh, inFile);
            }
            
            if (dotCount) {     //float or double
                double doubleConst = atof(lexBuff);
                // TODO: get double constant
                //printf("Float number [%lf] should not appear!", doubleConst);
                return LexReturnType_OK;
            }
            else {              //integer
                int intConst = atoi(lexBuff);
                // TODO: get int constant
                *type = GeneralType_Constant;
                *intValue = intConst;
                //debugPrint("constant");
                debugPrint(lexBuff);
                return LexReturnType_OK;
            }
        }
        else if (isalpha(elemCh) || elemCh == '_') {    //identifiers or keywords
            while (isalnum(elemCh) || elemCh == '_') {
                lexBuff[buffIndex++] = elemCh;
                elemCh = fgetc(inFile);
            }
            lexBuff[buffIndex] = '\0';
            
            if (elemCh != EOF) {
                ungetc(elemCh, inFile);
            }
            
            // TODO: keyword or identifier
            
            if ((*type = typeOfWord(lexBuff)) != -1) {    // keyword
                //debugPrint("Keyword");
                debugPrint(lexBuff);
            }
            else {                              // identifier
                *type = GeneralType_Identifier;
                *intValue = lookup(lexBuff);
                if (*intValue == 0) {
                    *intValue = insert(lexBuff, GeneralType_Identifier);
                }
                //debugPrint("identifier");
                debugPrint(lexBuff);
            }
            
            return LexReturnType_OK;
        }
        else if (elemCh == '\"'){                       //string literal
            while (1) {
                elemCh = fgetc(inFile);
                if (elemCh != '\"') {
                    lexBuff[buffIndex++] = elemCh;
                    if (elemCh == '%') {                //double %
                        lexBuff[buffIndex++] = elemCh;
                    }
                }
                else {
                    break;
                }
            }
            lexBuff[buffIndex] = '\0';
            
            *type = GeneralType_StringLiteral;
            
            size_t size = strlen(lexBuff) + 1;
            char *strl = (char *)malloc(size);
            if (strl == NULL) {
                printf("[ERROR]: malloc error for string_literal\n");
            }
            memcpy(strl, lexBuff, size);
            strl[size - 1] = '\0';
            *strValue = strl;
            //debugPrint("string_literal");
            debugPrint(lexBuff);
            return LexReturnType_OK;
        }
        else {                                          //punctuator
            lexBuff[buffIndex++] = elemCh;
            
            elemCh = fgetc(inFile);
            if (elemCh == '+' || elemCh == '-' || elemCh == '=') {
                lexBuff[buffIndex++] = elemCh;
            }
            else {
                ungetc(elemCh, inFile);
            }
            
            lexBuff[buffIndex] = '\0';
            
            for (int i = 0; i < NumOfPunctuators; i++) {
                if (!strcmp(lexBuff, Punctuators[i])) {
                    int puncType = PunctuatorType_LBracket + i;
                    // TODO: return puncType
                    *type = puncType;
                    //debugPrint("punctuator");
                    debugPrint(lexBuff);
                    return LexReturnType_OK;
                }
            }
        }
        
    }
    
    return LexReturnType_OK;
}