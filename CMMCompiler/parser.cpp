//
//  parser.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "lexer.h"
#include "parser.h"
#include "util.h"
#include "lalr.h"
#include "symbol.h"
#include <stack>

using namespace std;

typedef int ParserState;

typedef struct {
    int symbol;
    int intValue;
    char *strValue;
    
}ParserElem;


stack<ParserState> stateStack;
stack<ParserElem> elemStack;

ParserElem  nextParseElem() { // lexone
    ParserElem elem;
    // TODO: lex
    int type;
    int intValue;
    char *strValue;
    lexOne(&type, &intValue, &strValue);
    
    elem.symbol = l2e(type);
    if (type < KeywordType_PlaceHolder) {
        // do nothing
    }
    else if (type < PunctuatorType_PlaceHolder) {
        // do nothing
    }
    else if (type == GeneralType_Identifier) {
        elem.intValue = intValue;
    }
    else if (type == GeneralType_Constant) {
        elem.intValue = intValue;
    }
    else if (type == GeneralType_StringLiteral) {
        elem.strValue = strValue;
        // TODO: release strValue
    }
    else if (type == GeneralType_EOF) {
        // do nothing
    }
    else {
        printf("[ERROR]: lex type\n");
    }
    
    return elem;
}

int parse() {
    ParserState beginState = 0;
    ParserElem beginElem;
    beginElem.symbol = ES_End;
    
    stateStack.push(beginState);
    elemStack.push(beginElem);
    
    ParserElem currentElem;
    ParserState currentState;
    
    currentElem =  nextParseElem();
    currentState = stateStack.top();
    
    ParserElem tempElem;
    
    int action;
    int go;
    
    while (true) {
        
        action = ActionGotoTable[currentState][currentElem.symbol - ES_Main];

        if (action >= 0 && action < AG_Reduce_Base) {   // Step
            currentState = action;
            stateStack.push(currentState);
            elemStack.push(currentElem);
            currentElem =  nextParseElem();
            //printf("step to %d\n", action);
            //cout << "step to " << action << endl;
        }
        else if (action >= AG_Reduce_Base && action < AG_Accept) {  // Reduce by A -> b
            int popCount = ProductCount[action - AG_Reduce_Base];
            while (popCount > 0) { // pop |b| times
                popCount--;
                stateStack.pop();
                elemStack.pop();
            }
            currentState = stateStack.top();
            int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
            go = ActionGotoTable[currentState][gotoCol];
            
            currentState = go;
            tempElem.symbol = ProductSource[action - AG_Reduce_Base];
            // TODO: other fields add to tempElem?
            
            stateStack.push(currentState);
            elemStack.push(tempElem);
            
            //printf("reduce by %d", action - AG_Reduce_Base);
            cout << "reduced by " << action - AG_Reduce_Base << endl;
            cout << Productions[action - AG_Reduce_Base] << endl;
        }
        else if (action == AG_Accept) { //Accept
            //printf("[ACCEPTED]\n");
            cout << "[ACCEPTED]" << endl;
            break;
        }
        else {
            //printf("[ERROR] action error\n");
            cout << "[ERROR] action error" << endl;
            break;
        }
    }
    return 0;
}