//
//  translator.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-17.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "translator.h"
#include "parser.h"
#include "symbol.h"
#include <stack>
#include <string>

#define MAX_BUFFER 100

using namespace std;

stack<ParserElem*> codeCollectorStack;

void registerCodeCollector(ParserElem *elem) {
    codeCollectorStack.push(elem);
}

void genCode(const char *fmt, ...) {
    char textString[MAX_BUFFER] = {'\0'};
    
    va_list args;
    va_start ( args, fmt );
    vsnprintf ( textString, MAX_BUFFER, fmt, args );
    va_end ( args );
    string retStr = textString;
    
    ParserElem *currentCollector = codeCollectorStack.top();
    
    (currentCollector->code).append(retStr);
}

void submitCode(string code) {
    ParserElem *currentCollector = codeCollectorStack.top();
    
    (currentCollector->code).append(code);
}

void unregisterCodeCollector(ParserElem *elem) {
    codeCollectorStack.pop();
}

void translate_initializer(ParserElem *initializer, int count, int typeSize) { // actually assignment

    switch (initializer->endingSymbol) {
        case ES_Const:
            //printf("[GENCODE]: movl $%d, %%edx\n", count);
            //printf("[GENCODE]: movl $%d, %d(%%ebp, %%edx, %d)\n", initializer->intValue, -addrOffset, typeSize);
            genCode("[GENCODE]: movl $%d, %%edx\n", count);
            genCode("[GENCODE]: movl $%d, %d(%%ebp, %%edx, %d)\n", initializer->intValue, -addrOffset, typeSize);
            break;
            
        default:
            printf("[WARNING]: other types");
            break;
    }

}

void translate_initializer_list(ParserElem *elem, int typeSize) {
    
    ParserElem *initializer_list = elem;
    //registerCodeCollector(elem);
    
    stack<ParserElem*> s;
    
    int symbol = initializer_list->symbol;
    initializer_list = initializer_list -> firstChild;
    while (initializer_list->endingSymbol == 0 && initializer_list->symbol == symbol) { // list
        s.push(initializer_list);
        initializer_list = initializer_list -> firstChild;
    }
    
    //printf("%ld\n", s.size());
    
    int count = 0;
    translate_initializer(initializer_list, count++, typeSize);
    while (!s.empty()) {
        initializer_list = s.top();
        translate_initializer(initializer_list->next, count++, typeSize);
        s.pop();
    }
    
    //unregisterCodeCollector(elem);
    //submitCode(elem->code);
}

void translate_init_declarator(ParserElem *init_declarator, int typeSize) {
    ParserElem *declarator = init_declarator->firstChild;
    
    symtable[declarator->intValue].type = declarator->endingSymbol;
    symtable[declarator->intValue].typeSize = typeSize;
    
    int realSize = symtable[declarator->intValue].size * typeSize;
    addrOffset += realSize;
    symtable[declarator->intValue].addr = addrOffset;
    
    //printf("[GENCODE]: subl $%d, %%esp\n", realSize);
    genCode("[GENCODE]: subl $%d, %%esp\n", realSize);
    ParserElem *initializer = declarator->next;
    if (initializer != NULL) { // declarator = initializer
        if (initializer->endingSymbol == 0) { // { initializer_list }
            translate_initializer_list(initializer->firstChild, typeSize);
        }
        else { // assignment_expression
            translate_initializer(initializer, 0, typeSize);
        }
    }
}

void translate_init_declarator_list(ParserElem *elem, int typeSize) {
    ParserElem *init_declarator_list = elem;
    registerCodeCollector(elem);
    stack<ParserElem*> s;
    
    int symbol = init_declarator_list->symbol;
    init_declarator_list = init_declarator_list->firstChild;
    
    while (init_declarator_list->endingSymbol == 0 && init_declarator_list->symbol == symbol) { // list and same MS
        s.push(init_declarator_list);
        init_declarator_list = init_declarator_list->firstChild;
    }
    
    translate_init_declarator(init_declarator_list, typeSize);
    while (!s.empty()) {
        init_declarator_list = s.top();
        translate_init_declarator(init_declarator_list->next, typeSize);
        s.pop();
    }
    unregisterCodeCollector(elem);
}

void translate_assignment_expression(ParserElem *elem) {
    ParserElem *assignment_expression = elem;
    ParserElem *postfix_expression = elem->firstChild;
    ParserElem *assignment_exp = postfix_expression->next;
    
    if (postfix_expression->endingSymbol != 0) { // id or maybe etc, assume id only
        int addr = symtable[postfix_expression->intValue].addr;
    }
    else { //  id[const] or etc
        printf("[WARNING]: translate_assignment_expression, situation not implemented");
    }
}

void translate_expression(ParserElem *elem) {
    ParserElem *expression = elem;
    registerCodeCollector(elem);
    stack<ParserElem*> s;
    
    int symbol = expression->symbol;
    expression = expression->firstChild;
    
    while (expression->endingSymbol == 0 && expression->symbol == symbol) { // expression list
        s.push(expression);
        expression = expression->firstChild;
    }
    
    translate_assignment_expression(expression);
    
    while (!s.empty()) {
        expression = s.top();
        translate_assignment_expression(expression);
        s.pop();
    }
    unregisterCodeCollector(elem);
}


