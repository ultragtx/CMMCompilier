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

#define ES_List 0

#define MAX_CHILDS 5

using namespace std;

typedef int ParserState;

stack<ParserState> stateStack;
stack<ParserElem*> elemStack;

int addrOffset = 0;
int registerFlag = 0;

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
            reduceTable[action - AG_Reduce_Base](action);
            continue;
            
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
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    //
    
    ParserElem *pos_exp = elems[0];
    ParserElem *exp = elems[2];
    int baseAddr = symtable[pos_exp->intValue].addr;
    int typeSize = symtable[pos_exp->intValue].typeSize;

    if (exp->endingSymbol == ES_Id) {
        int idAddr = symtable[exp->intValue].addr;
        // did not see the type size
        printf("[GENCODE]: movl %d(%%ebp), %%edx\n", -idAddr);
    }
    else if (exp->endingSymbol == ES_Const) {
        // won't happen in test code
    }
    else {
        printf("[WARNING] reduce07 other types\n");
    }
    
    if (registerFlag == 0) {
        printf("[GENCODE]: movl %d(%%ebp, %%edx, %d), %%eax\n", -baseAddr, typeSize);
    }
    else {
        printf("[GENCODE]: movl %d(%%ebp, %%edx, %d), %%ebx\n", -baseAddr, typeSize);
    }
    registerFlag = (registerFlag + 1) %2;
    
    reduceElem->endingSymbol = 0;
    //
    
    delete elems[1];
    delete elems[3];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce08(int action) {		//    "postfix_expression => postfix_expression ++ ;",
    
}

void reduce09(int action) {		//    "postfix_expression => postfix_expression -- ;",
    
}

void reduce10(int action) {		//    "",
    
}

void reduce11(int action) {		//    "multiplicative_expression => postfix_expression ;",
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
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    
    //
    
    ParserElem *multiplicative_expression = elems[0];
    if (multiplicative_expression->endingSymbol == 0) { // code generated result in %exa or %exb depend on registerFlag
        // assume both mul_exp and pos_exp generated
        printf("[GENCODE]: mull %%ebx\n");
        registerFlag = 0;
    }
    else {
        printf("[WARNING]: reduce12, unimplemented situation\n");
    }
    
    reduceElem->endingSymbol = 0;
    //
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce13(int action) {		//    "multiplicative_expression => multiplicative_expression / postfix_expression ;",
    
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
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    //
    
    
    ParserElem *multiplicative_expression = elems[2];
    ParserElem *additive_expression = elems[0];
    if (multiplicative_expression->endingSymbol == 0) { // code generated result in %exa
        // assume multiplicative_expression generated
        if (additive_expression->endingSymbol == ES_Id) {
            int addr = symtable[additive_expression->intValue].addr;
            printf("[GENCODE]: movl %d(%%ebp), %%ebx\n", -addr);
        }
        else {
            printf("[WARNING]: reduce15, (1)unimplemented situation\n");
        }
        printf("[GENCODE]: addl %%ebx, %%eax\n");
        registerFlag = 0;
    }
    else {
        printf("[WARNING]: reduce15, (2)unimplemented situation\n");
    }
    
    delete elems[1];
    
    reduceElem->endingSymbol = 0;
    //
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce16(int action) {		//    "additive_expression => additive_expression - multiplicative_expression ;",
    
}

void reduce17(int action) {		//    "relational_expression => additive_expression ;",
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
    
}

void reduce19(int action) {		//    "relational_expression => relational_expression > additive_expression ;",
    
}

void reduce20(int action) {		//    "relational_expression => relational_expression <= additive_expression ;",
    
}

void reduce21(int action) {		//    "relational_expression => relational_expression >= additive_expression ;",
    
}

void reduce22(int action) {		//    "equality_expression => relational_expression ;",
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

void reduce25(int action) {		//    "assignment_expression => equality_expression ;",
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
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    //
    ParserElem *postfix_expression = elems[0];
    ParserElem *assignment_expression = elems[2];
    
    int addr = symtable[postfix_expression->intValue].addr;
    
    if (assignment_expression->endingSymbol == 0) { // assignment_expression code generated and result in %eax
        printf("[GENCODE]: movl %%eax, %d(%%ebp)\n", -addr);
    }
    else {
        int constVal = elems[2]->intValue;
        
        printf("[GENCODE]: movl $%d, %d(%%ebp)\n", constVal, -addr);
    }
    
    delete elems[1];
    //
    
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

void reduce28(int action) {		//    "expression => assignment_expression ;",
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
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    int typeSize = elems[0]->intValue;
    
    translate_init_declarator_list(elems[1], typeSize);
    
    /*ParserElem *next = elems[1]->firstChild;
    
    ParserElem *declarator = next->firstChild;
    
    symtable[declarator->intValue].type = declarator->endingSymbol;
    symtable[declarator->intValue].typeSize = typeSize;
    
    int realSize = symtable[declarator->intValue].size * typeSize;
    
    addrOffset += realSize;
    symtable[declarator->intValue].addr = addrOffset;
    
    printf("[GENCODE]: subl $%d, %%esp\n", realSize);
    
    ParserElem *initializer = declarator->next;
    if (initializer != NULL) { // declarator = initializer
        if (initializer->endingSymbol == 0) { // { initializer_list }
            translate_initializer_list(initializer, typeSize);
        }
        else { // assignment_expression
            
        }
        
    }*/
    
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
    
}

void reduce49(int action) {		//    "statement => iteration_statement ;",
    
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
    
    ParserElem *reduceElem = new ParserElem;
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    reduceElem->firstChild = elems[0];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce52(int action) {		//    "block_item_list => block_item ;",
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
    
    ParserElem *reduceElem = elems[0];
    reduceElem->symbol = ProductSource[action - AG_Reduce_Base];
    
    currentState = go;
    
    stateStack.push(currentState);
    elemStack.push(reduceElem);
}

void reduce54(int action) {		//    "block_item => declaration ;",
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
    
}

void reduce57(int action) {		//    "expression_statement => scanf_statement semi ;",
    
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
    
}

void reduce61(int action) {		//    "iteration_statement => for ( expression semi expression semi expression ) statement ; // here are 2 semi",
    
}

void reduce62(int action) {		//    "jump_statement => return expression semi ;	// here is a semi",
    
}

void reduce63(int action) {		//    "printf_statement => printf ( printf_params ) ;",
    
}

void reduce64(int action) {		//    "printf_params => string_literal ;",
    
}

void reduce65(int action) {		//    "printf_params => printf_params , identifier ;",
    
}

void reduce66(int action) {		//    "scanf_statement => scanf ( scanf_params ) ;",
    
}

void reduce67(int action) {		//    "scanf_params => string_literal ;",
    
}

void reduce68(int action) {		//    "scanf_params =>	scanf_params , & identifier ;",
    
}

