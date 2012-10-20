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
#include <cstring>
#include "translator.h"

#define MAX_CHILDS 5

using namespace std;

typedef int ParserState;

stack<ParserState> stateStack;
stack<ParserElem*> elemStack;

int addrOffset = 0;

ParserElem * nextParseElem() { // lexone
    ParserElem *elem = new ParserElem;
    // TODO: lex
    int type;
    int intValue;
    char *strValue;
    lexOne(&type, &intValue, &strValue);
    
    elem->symbol = l2e(type);
    elem->endingSymbol = elem->symbol;
    if (type < KeywordType_PlaceHolder) {
//        elem->endingSymbol = elem->symbol;
    }
    else if (type < PunctuatorType_PlaceHolder) {
//        elem->endingSymbol = elem->symbol;
    }
    else if (type == GeneralType_Identifier) {
        elem->intValue = intValue;
    }
    else if (type == GeneralType_Constant) {
        elem->intValue = intValue;
    }
    else if (type == GeneralType_StringLiteral) {
        elem->strValue = strValue;
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
    ParserElem *beginElem = new ParserElem;
    beginElem->symbol = ES_End;
    
    stateStack.push(beginState);
    elemStack.push(beginElem);
    
    ParserElem *currentElem;
    ParserState currentState;
    
    currentElem =  nextParseElem();
    currentState = stateStack.top();
    
    
    
    int action;
    int go;
    
    while (true) {
        currentState = stateStack.top();
        action = ActionGotoTable[currentState][currentElem->symbol - ES_Main];

        if (action >= 0 && action < AG_Reduce_Base) {   // Step
            currentState = action;
            stateStack.push(currentState);
            elemStack.push(currentElem);
            currentElem =  nextParseElem();
            //printf("step to %d\n", action);
            //cout << "step to " << action << endl;
        }
        else if (action >= AG_Reduce_Base && action < AG_Accept) {  // Reduce by A -> b
//            cout << "reduced by " << action - AG_Reduce_Base << endl;
//            cout << Productions[action - AG_Reduce_Base] << endl;
//            reduceTable[action - AG_Reduce_Base](action);
//            continue;
            
            int popCount = ProductCount[action - AG_Reduce_Base];
            while (popCount > 0) { // pop |b| times
                popCount--;
                stateStack.pop();
                elemStack.pop();
            }
            currentState = stateStack.top();
            int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
            go = ActionGotoTable[currentState][gotoCol];
            
            ParserElem *reduceElem = new ParserElem;
            currentState = go;
            reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
            // TODO: other fields add to tempElem?
            
            stateStack.push(currentState);
            elemStack.push(reduceElem);
            
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

////////////////////////////////////////////////////////////////////////////////////

void reduce00(int action) {		//    "program' => program ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce01(int action) {		//    "program => type_specifier main ( ) compound_statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    cout << "---------------" << endl;
    cout << elems[4]->code;
    cout << "***************" << endl;
    
    delete elems[0];
    delete elems[1];
    delete elems[2];
    delete elems[3];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce02(int action) {		//    "primary_expression => identifier ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce03(int action) {		//    "primary_expression => constant ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce04(int action) {		//    "primary_expression => string_literal ;",
    
}

void reduce05(int action) {		//    "primary_expression => ( expression ) ;",
    
}

void reduce06(int action) {		//    "postfix_expression => primary_expression ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce07(int action) {		//    "postfix_expression => postfix_expression [ expression ] ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_postifix_expression(elems[0], elems[2]);

    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    delete elems[3];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce08(int action) {		//    "postfix_expression => postfix_expression ++ ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_postifix_expression_1(elems[0]);
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce09(int action) {		//    "postfix_expression => postfix_expression -- ;",
    
}

void reduce10(int action) {		//    "",
    
}

void reduce11(int action) {		//    "multiplicative_expression => postfix_expression ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce12(int action) {		//    "multiplicative_expression => multiplicative_expression * postfix_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_multiplicative_expression(elems[0], elems[2], 0); // 0 for mul 1 for div
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce13(int action) {		//    "multiplicative_expression => multiplicative_expression / postfix_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_multiplicative_expression(elems[0], elems[2], 1); // 0 for mul 1 for div
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce14(int action) {		//    "additive_expression => multiplicative_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce15(int action) {		//    "additive_expression => additive_expression + multiplicative_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_additive_expression(elems[0], elems[2], 0); // 0 for add 1 for sub
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce16(int action) {		//    "additive_expression => additive_expression - multiplicative_expression ;",
    
}

void reduce17(int action) {		//    "relational_expression => additive_expression ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce18(int action) {		//    "relational_expression => relational_expression < additive_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_relational_expression(elems[0], elems[2], 0); //0 for < 1 for > 2 for <= 3 for >=
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce19(int action) {		//    "relational_expression => relational_expression > additive_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_relational_expression(elems[0], elems[2], 1); //0 for < 1 for > 2 for <= 3 for >=
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce20(int action) {		//    "relational_expression => relational_expression <= additive_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_relational_expression(elems[0], elems[2], 2); //0 for < 1 for > 2 for <= 3 for >=
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce21(int action) {		//    "relational_expression => relational_expression >= additive_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_relational_expression(elems[0], elems[2], 3); //0 for < 1 for > 2 for <= 3 for >=
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce22(int action) {		//    "equality_expression => relational_expression ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce23(int action) {		//    "equality_expression => equality_expression == relational_expression ;",
    
}

void reduce24(int action) {		//    "equality_expression => equality_expression != relational_expression ;",
    
}

void reduce25(int action) {		//    "assignment_expression => equality_expression ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce26(int action) {		//    "assignment_expression => postfix_expression assignment_operator assignment_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_assignment_expression(elems[0], elems[2]);
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[1];

    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce27(int action) {		//    "assignment_operator =>	= ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce28(int action) {		//    "expression => assignment_expression ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce29(int action) {		//    "expression => expression , assignment_expression ;",
    /*int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    elems[0]->next = elems[2];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);*/
}

void reduce30(int action) {		//    "constant_expression => equality_expression ;",
    
}

void reduce31(int action) {		//    "declaration => declaration_specifiers init_declarator_list semi ;	// here is a semi",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    
    int typeSize = elems[0]->intValue;
    
    translate_init_declarator_list(elems[1], typeSize);
    
    ParserElem *reduceElem = elems[1];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->endingSymbol = ES_Code;
    reduceElem->code = elems[1]->code;
    
    delete elems[0];
    delete elems[2];
    
    /*cout << "---------------" << endl;
    cout << reduceElem->code;
    cout << "---------------" << endl;*/
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce32(int action) {		//    "declaration_specifiers => type_specifier ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce33(int action) {		//    "init_declarator_list =>	 init_declarator ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce34(int action) {		//    "init_declarator_list =>	 init_declarator_list , init_declarator ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    elems[0]->next = elems[2];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce35(int action) {		//    "init_declarator => declarator ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    //    reduceElem->endingSymbol = elems[0]->endingSymbol;
    //    reduceElem->intValue = elems[0]->intValue;
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce36(int action) {		//    "init_declarator => declarator = initializer ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    //    reduceElem->endingSymbol = elems[0]->endingSymbol;
    //    reduceElem->intValue = elems[0]->intValue;
    reduceElem->firstChild = elems[0];
    elems[0]->next = elems[2];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce37(int action) {		//    "type_specifier => int ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->intValue = sizeof(int);
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce38(int action) {		//    "type_specifier => float ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->intValue = sizeof(float);
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce39(int action) {		//    "declarator => direct_declarator ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce40(int action) {		//    "direct_declarator => identifier ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce41(int action) {		//    "direct_declarator => direct_declarator [ assignment_expression ] ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    int count = popCount;
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    symtable[elems[0]->intValue].size = elems[2]->intValue; // translate
    
    for (int i = 1; i < count; i++) {
        delete elems[i];
    }
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce42(int action) {		//    "initializer => assignment_expression ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce43(int action) {		//    "initializer => { initializer_list } ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];

    reduceElem->firstChild = elems[1];
    
    delete elems[0];
    delete elems[2];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce44(int action) {		//    "initializer_list =>	initializer ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    //    reduceElem->endingSymbol = elems[0]->endingSymbol;
    //    reduceElem->intValue = elems[0]->intValue;
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce45(int action) {		//    "initializer_list =>	initializer_list , initializer ;	// initializer may change to assignment_expression",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
//    reduceElem->endingSymbol = elems[0]->endingSymbol;
//    reduceElem->intValue = elems[0]->intValue;
    reduceElem->firstChild = elems[0];
    elems[0]->next = elems[2];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce46(int action) {		//    "statement => compound_statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce47(int action) {		//    "statement => expression_statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce48(int action) {		//    "statement => selection_statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce49(int action) {		//    "statement => iteration_statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce50(int action) {		//    "statement => jump_statement ;",
    
}

void reduce51(int action) {		//    "compound_statement => { block_item_list } ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];

    translate_block_item_list(elems[1]);
    
    ParserElem *reduceElem = elems[1];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->endingSymbol = ES_Code;
    
    delete elems[0];
    delete elems[2];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce52(int action) {		//    "block_item_list => block_item ;", // D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce53(int action) {		//    "block_item_list => block_item_list block_item ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    elems[0]->next = elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce54(int action) {		//    "block_item => declaration ;", //D
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce55(int action) {		//    "block_item => statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce56(int action) {		//    "expression_statement => printf_statement semi ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce57(int action) {		//    "expression_statement => scanf_statement semi ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce58(int action) {		//    "expression_statement => expression semi ;		// here is a semi",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce59(int action) {		//    "selection_statement => //if ( expression ) statement ;",
    
}

void reduce60(int action) {		//    "selection_statement => if ( expression ) statement else statement ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_selection_statement(elems[2], elems[4], elems[6]);
    
    ParserElem *reduceElem = elems[2];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[0];
    delete elems[1];
    delete elems[3];
    delete elems[4];
    delete elems[5];
    delete elems[6];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce61(int action) {		//    "iteration_statement => for ( expression semi expression semi expression ) statement ; // here are 2 semi",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_iteration_statement(elems[2], elems[4], elems[6], elems[8]);
    
    ParserElem *reduceElem = elems[2];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[0];
    delete elems[1];
    delete elems[3];
    delete elems[4];
    delete elems[5];
    delete elems[6];
    delete elems[7];
    delete elems[8];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce62(int action) {		//    "jump_statement => return expression semi ;	// here is a semi",
    
}

void reduce63(int action) {		//    "printf_statement => printf ( printf_params ) ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_printf_params(elems[2]);
    
    ParserElem *reduceElem = elems[2];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[0];
    delete elems[1];
    delete elems[3];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce64(int action) {		//    "printf_params => string_literal ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce65(int action) {		//    "printf_params => printf_params , identifier ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    elems[0]->next = elems[2];
    
    delete elems[1];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce66(int action) {		//    "scanf_statement => scanf ( scanf_params ) ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    translate_scanf_params(elems[2]);
    
    ParserElem *reduceElem = elems[2];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    delete elems[0];
    delete elems[1];
    delete elems[3];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce67(int action) {		//    "scanf_params => string_literal ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce68(int action) {		//    "scanf_params =>	scanf_params , & identifier ;",
    int popCount = ProductCount[action - AG_Reduce_Base];
    
    ParserElem *elems[popCount];
    while (popCount > 0) { // pop |b| times
        popCount--;
        elems[popCount] = elemStack.top();
        stateStack.pop();
        elemStack.pop();
    }
    ParserState currentState = stateStack.top();
    int gotoCol = ProductSource[action - AG_Reduce_Base] - MS_Program + NumOfEndingSymbol;
    int go = ActionGotoTable[currentState][gotoCol];
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    elems[0]->next = elems[3];
    
    delete elems[1];
    delete elems[2];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

