//
//  lexer.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "lexer.h"

char lexBuff[LEX_BUFF_SIZE];

int typeOfWord(char *word) {
    for (int i = 0; i < NumOfKeywords; i++) {
        if (!strcmp(word, KeyWords[i])) {
            return KeywordType_Int + i;
        }
    }
    return -1;
}

int lexOne() {
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
        }
        else if (elemCh == EOF) {                       //end of file
            return LexReturnType_EOF;
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
                return LexReturnType_OK;
            }
            else {              //integer
                int intConst = atoi(lexBuff);
                // TODO: get int constant
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
            if (typeOfWord(lexBuff) != -1) {    // keyword
                
            }
            else {                              // identifier
                printf("identifier ");
            }
            
            return LexReturnType_OK;
        }
        else if (elemCh == '\"'){                       //string literal
            while (1) {
                elemCh = fgetc(inFile);
                if (elemCh != '\"') {
                    lexBuff[buffIndex++] = elemCh;
                }
                else {
                    break;
                }
            }
            
            // TODO: string literal
            
            return LexReturnType_OK;
        }
        else {                                          //punctuator
            /*switch (elemCh) {
             case '[':
             
             break;
             case ']':
             
             break;
             case '(':
             
             break;
             case ')':
             
             break;
             case '{':
             
             break;
             case '}':
             
             break;
             case '+':   //++
             
             break;
             case '-':   //--
             
             break;
             case '*':
             
             break;
             case '/':
             
             break;
             case '%':
             
             break;
             case '<':
             
             break;
             case '>':
             
             break;
             case '=':
             
             break;
             case '!':
             
             break;
             case '&':
             
             break;
             case '|':
             
             break;
             case ';':
             
             break;
             case ',':
             
             break;
             case '#':
             
             break;
             
             default:
             printf("ERROR: Wrong punctuator");
             break;
             }*/
            
            lexBuff[buffIndex++] = elemCh;
            
            elemCh = fgetc(inFile);
            if (elemCh == '+' || elemCh == '-') {
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
                    
                }
            }
        }
        
    }
    
    return 0;
}