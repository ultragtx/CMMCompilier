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
string tempStr;

int for_number = 0;
int if_number = 0;
int str_literal_num = 0;

void registerCodeCollector(ParserElem *elem) {
    codeCollectorStack.push(elem);
    tempStr.erase();
}

void genCode(const char *fmt, ...) {
    char textString[MAX_BUFFER] = {'\0'};
    
    va_list args;
    va_start ( args, fmt );
    vsnprintf ( textString, MAX_BUFFER, fmt, args );
    va_end ( args );
    string retStr = textString;
    
    //ParserElem *currentCollector = codeCollectorStack.top();
    //(currentCollector->code).append(retStr);
    tempStr.append(retStr);
}

void submitCode(string code) {
    //ParserElem *currentCollector = codeCollectorStack.top();
    //(currentCollector->code).append(code);
    tempStr.append(code);
}

void unregisterCodeCollector(ParserElem *elem) {
    ParserElem *currentCollector = codeCollectorStack.top();
    (currentCollector->code).erase();
    (currentCollector->code).append(tempStr);
    codeCollectorStack.pop();
}

string stringWithFormat(const char *fmt, ...) {
    char textString[MAX_BUFFER] = {'\0'};
    
    va_list args;
    va_start ( args, fmt );
    vsnprintf ( textString, MAX_BUFFER, fmt, args );
    va_end ( args );
    string retStr = textString;
    
    return retStr;
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
    
    elem->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem);
}

void translate_postifix_expression(ParserElem *elem1, ParserElem *elem2) { // "postfix_expression => postfix_expression [ expression ] ;"
    ParserElem *pos_exp = elem1;
    ParserElem *exp = elem2;
    
    registerCodeCollector(elem1);
    
    int baseAddr = symtable[pos_exp->intValue].addr;
    int typeSize = symtable[pos_exp->intValue].typeSize;
    
    if (exp->endingSymbol == ES_Id) {
        int idAddr = symtable[exp->intValue].addr;
        // did not check the type size
        //printf("[GENCODE]: movl %d(%%ebp), %%edx\n", -idAddr);
        genCode("[GENCODE]: movl %d(%%ebp), %%edx\n", -idAddr);
    }
    else if (exp->endingSymbol == ES_Const) {
        // won't happen in test code
    }
    else {
        printf("[WARNING %d]: translate_postifix_expression, situation not implemented\n", lineno);
    }
    
    genCode("[GENCODE]: movl %d(%%ebp, %%edx, %d), %%eax\n", -baseAddr, typeSize);
    //genCode("##\n");
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem1);
}

void translate_postifix_expression_1(ParserElem *elem1) { // "postfix_expression => postfix_expression++;"
    ParserElem *pos_exp = elem1;
    
    registerCodeCollector(elem1);
    
    int baseAddr = symtable[pos_exp->intValue].addr;
    int typeSize = symtable[pos_exp->intValue].typeSize;
    
    if (elem1->endingSymbol == ES_Id) {
        int idAddr = symtable[elem1->intValue].addr;
        genCode("[GENCODE]: addl $1, %d(%%ebp)\n", -idAddr);
    }
    else {
        printf("[WARNING %d]: translate_postifix_expression, situation not implemented\n", lineno);
    }
    
    genCode("[GENCODE]: movl %d(%%ebp, %%edx, %d), %%eax\n", -baseAddr, typeSize);
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem1);
}

void translate_multiplicative_expression(ParserElem *elem1, ParserElem *elem2, int type) { // "multiplicative_expression => multiplicative_expression * postfix_expression ;",
    
    static const char * const operators[2] = {"mull", "divl"};
    
    ParserElem *multiplicative_expression = elem1;
    ParserElem *postfix_expression = elem2;
    
    registerCodeCollector(elem1);
    
    if (multiplicative_expression->endingSymbol == ES_Code && postfix_expression->endingSymbol == ES_Code) {
        submitCode(postfix_expression->code);
        genCode("[GENCODE]: movl %%eax, %%ebx\n");
        //genCode("$$\n");
        submitCode(multiplicative_expression->code);
        genCode("[GENCODE]: %s %%ebx\n", operators[type]);
    }
    else if (multiplicative_expression->endingSymbol == ES_Id && postfix_expression->endingSymbol == ES_Id) {
        int addr1 = symtable[multiplicative_expression->intValue].addr;
        int addr2 = symtable[multiplicative_expression->intValue].addr;
        genCode("[GENCODE]: movl %d(%%ebp), %%eax\n", -addr1);
        genCode("[GENCODE]: movl %d(%%ebp), %%ebx\n", -addr2);
        genCode("[GENCODE]: %s %%ebx\n", operators[type]);
    }
    else {
        printf("[WARNING]: translate_multiplicative_expression, situation not implemented\n");
    }
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem2);
}

void translate_additive_expression(ParserElem *elem1, ParserElem *elem2, int type) { // +, -
    static const char * const operators[2] = {"addl", "subl"};
    
    ParserElem *additive_expression = elem1;
    ParserElem *multiplicative_expression = elem2;
    
    registerCodeCollector(elem1);
    
    if (additive_expression->endingSymbol == ES_Id && multiplicative_expression->endingSymbol == ES_Code) {
        submitCode(multiplicative_expression->code);
        int addr = symtable[additive_expression->intValue].addr;
        genCode("[GENCODE]: movl %d(%%ebp), %%ebx\n", -addr);
        genCode("[GENCODE]: %s %%ebx, %%eax\n", operators[type]);
    }
    else if (additive_expression->endingSymbol == ES_Id && multiplicative_expression->endingSymbol == ES_Const) {
        int addr = symtable[additive_expression->intValue].addr;
        int constVal = multiplicative_expression->intValue;
        genCode("[GENCODE]: movl %d(%%ebp), %%eax\n", -addr);
        genCode("[GENCODE]: movl $%d, %%ebx\n", constVal);
        genCode("[GENCODE]: %s %%ebx, %%eax\n", operators[type]);
    }
    else if (additive_expression->endingSymbol == ES_Const && multiplicative_expression->endingSymbol == ES_Id) {
        int addr = symtable[multiplicative_expression->intValue].addr;
        int constVal = additive_expression->intValue;
        genCode("[GENCODE]: movl %d(%%ebp), %%ebx\n", -addr);
        genCode("[GENCODE]: movl $%d, %%eax\n", constVal);
        genCode("[GENCODE]: %s %%ebx, %%eax\n", operators[type]);
    }
    else {
        printf("[WARNING]: translate_additive_expression, situation not implemented\n");
    }
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem2);
}

void translate_assignment_expression(ParserElem *elem1, ParserElem *elem2) { // =
    ParserElem *postfix_expression = elem1;
    ParserElem *assignment_expression = elem2;
    
    registerCodeCollector(elem1);
    
    if (postfix_expression->endingSymbol == ES_Id && assignment_expression->endingSymbol == ES_Code) {
        submitCode(assignment_expression->code);
        int addr = symtable[postfix_expression->intValue].addr;
        genCode("[GENCODE]: movl %%eax, %d(%%ebp)\n", -addr);
    }
    else if (postfix_expression->endingSymbol == ES_Id && assignment_expression->endingSymbol == ES_Const) {
        int addr = symtable[postfix_expression->intValue].addr;
        int constVal = assignment_expression->intValue;
        genCode("[GENCODE]: movl $%d, %d(%%ebp)\n", constVal, -addr);
    }
    else {
        printf("[WARNING]: translate_additive_expression(2), situation not implemented\n");
    }
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem2);
}

void translate_relational_expression(ParserElem *elem1, ParserElem *elem2, int type) { // < > <= >=
    static const char * const operators[4] = {"jb", "ja", "jbe", "jae"};
    
    ParserElem *relational_expression = elem1;
    ParserElem *additive_expression = elem2;
    
    registerCodeCollector(elem1);
    
    if (relational_expression->endingSymbol == ES_Id && additive_expression->endingSymbol == ES_Const) {
        int addr = symtable[relational_expression->intValue].addr;
        int constVal = additive_expression->intValue;
        genCode("[GENCODE]: movl %d(%%ebp), %%eax\n", -addr);
        genCode("[GENCODE]: movl $%d, %%ebx\n", constVal);
        genCode("[GENCODE]: cmp %%eax, %%ebx\n");
        genCode("[GENCODE]: %s ", operators[type]); // waitng for a jump label
    }
    else {
        printf("[WARNING]: translate_relational_expression, situation not implemented\n");
    }
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem2);
}

void translate_iteration_statement(ParserElem *elem1, ParserElem *elem2, ParserElem *elem3, ParserElem *elem4) { // for
    ParserElem *expression1 = elem1;
    ParserElem *expression2 = elem2;
    ParserElem *expression3 = elem3;
    ParserElem *statement = elem4;
    
    registerCodeCollector(elem1);
    
    if (expression1->endingSymbol == ES_Code && expression2->endingSymbol == ES_Code && expression3->endingSymbol == ES_Code && statement->endingSymbol == ES_Code) {
        submitCode(expression1->code);
        genCode("[GENCODE]: for_begin_%d:\n", for_number);
        submitCode(expression2->code);
        genCode("for_end_%d\n", for_number);
        submitCode(statement->code);
        submitCode(expression3->code);
        genCode("[GENCODE]: jmp for_begin_%d\n", for_number);
        genCode("[GENCODE]: for_end_%d:\n", for_number);
        for_number++;
    }
    else {
        printf("[WARNING]: translate_iteration_statement, situation not implemented\n");
    }
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem2);
}

void translate_selection_statement(ParserElem *elem1, ParserElem *elem2, ParserElem *elem3) { // if else
    ParserElem *expression = elem1;
    ParserElem *statement1 = elem2;
    ParserElem *statement2 = elem3;
    
    registerCodeCollector(elem1);
    
    if (expression->endingSymbol == ES_Code && statement1->endingSymbol == ES_Code && statement2->endingSymbol == ES_Code) {
        submitCode(expression->code);
        genCode("if_part_%d\n", if_number);
        submitCode(statement2->code);
        genCode("[GENCODE]: jmp if_end_%d\n", if_number);
        genCode("[GENCODE]: if_part_%d:\n", if_number);
        submitCode(statement1->code);
        genCode("[GENCODE]: if_end_%d:\n", if_number);
        
        if_number++;
    }
    else {
        printf("[WARNING]: translate_selection_statement, situation not implemented\n");
    }
    
    elem1->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem2);
}

/*void translate_assignment_expression(ParserElem *elem) {
    ParserElem *assignemt_expression = elem;
    if (assignemt_expression->endingSymbol == ES_Code) {
        submitCode(assignemt_expression->code);
    }
    else {
        printf("[ERROR %d]: translate_assignment_expression(1), code shuold be generated\n", lineno);
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
    
    elem->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem);
}*/

void translate_printf_params(ParserElem *elem) {
    ParserElem *printf_params = elem;
    registerCodeCollector(elem);
    
    stack<ParserElem*> s;
    stack<string> params;
    
    int symbol = printf_params->symbol;
    printf_params = printf_params->firstChild;
    
    while (printf_params->endingSymbol == 0 && printf_params->symbol == symbol) { // list
        s.push(printf_params);
        printf_params = printf_params->firstChild;
    }
    
    //params.push(stringWithFormat(printf_params->strValue)); not push
    // TODO:
    while (!s.empty()) {
        printf_params = s.top();
        int addr = symtable[printf_params->intValue].addr;
        params.push(stringWithFormat("[GENCODE]: pushl %d(%%ebp)\n", -addr));
        s.pop();
    }
    
    int offset = (int)params.size() + 1;
    
    while (!params.empty()) {
        submitCode(params.top());
        params.pop();
    }
    genCode("[GENCODE]: pushl $str_literal_%d\n", str_literal_num);\
    genCode("[GENCODE]: call printf\n");
    genCode("[GENCODE]: addl $%d, %%esp\n", offset * 4);
    
    str_literal_num++;
    
    elem->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem);
}

void translate_scanf_params(ParserElem *elem) {
    ParserElem *scanf_params = elem;
    registerCodeCollector(elem);
    
    stack<ParserElem*> s;
    stack<string> params;
    
    int symbol = scanf_params->symbol;
    scanf_params = scanf_params->firstChild;
    
    while (scanf_params->endingSymbol == 0 && scanf_params->symbol == symbol) { // list
        s.push(scanf_params);
        scanf_params = scanf_params->firstChild;
    }
    
    //params.push(stringWithFormat(printf_params->strValue)); not push
    // TODO:
    while (!s.empty()) {
        scanf_params = s.top();
        int addr = symtable[scanf_params->intValue].addr;
        params.push(stringWithFormat("[GENCODE]: pushl $%d\n", -addr));
        s.pop();
    }
    
    int offset = (int)params.size() + 1;
    
    while (!params.empty()) {
        submitCode(params.top());
        params.pop();
    }
    genCode("[GENCODE]: pushl $str_literal_%d\n", str_literal_num);\
    genCode("[GENCODE]: call scanf\n");
    genCode("[GENCODE]: addl $%d, %%esp\n", offset * 4);
    
    str_literal_num++;
    
    elem->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem);
}

void translate_jump_statement(ParserElem *elem) {
    elem->endingSymbol = ES_Code;
}

void translate_block_item(ParserElem *elem) {
    ParserElem *block_item = elem;
    
    if (block_item->endingSymbol == ES_Code) {
        submitCode(block_item->code);
    }
    else {
        printf("[ERROR]: translate_block_item, code shuold be generated\n");
    }
}

void translate_block_item_list(ParserElem *elem) {
    ParserElem *block_item_list = elem;
    registerCodeCollector(elem);
    
    stack<ParserElem*> s;
    
    int symbol = block_item_list->symbol;
    block_item_list = block_item_list -> firstChild;
    
    while (block_item_list->endingSymbol == 0 && block_item_list->symbol == symbol) { // list
        s.push(block_item_list);
        block_item_list = block_item_list -> firstChild;
    }
    
    //printf("%ld\n", s.size());
    
    translate_block_item(block_item_list);
    while (!s.empty()) {
        block_item_list = s.top();
        translate_block_item(block_item_list->next);
        s.pop();
    }
    
    elem->endingSymbol = ES_Code;
    
    unregisterCodeCollector(elem);
}

