//
//  translator.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-17.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "translator.h"
#include "parser.h"
#include <stack>

using namespace std;

void translate_initializer(ParserElem *initializer, int count, int typeSize) {
    switch (initializer->endingSymbol) {
        case ES_Const:
            printf("[GENCODE]: movl $%d, %%edx\n", count);
            printf("[GENCODE]: movl $%d, %d(%%ebp, %%edx, %d)\n", initializer->intValue, -addrOffset, typeSize);
            break;
            
        default:
            printf("[WARNING]: other types");
            break;
    }

}

void translate_initializer_list(ParserElem *initializer, int typeSize) {
    
    ParserElem *initializer_list = initializer;
    
    stack<ParserElem*> s;
    
    initializer_list = initializer_list -> firstChild;
    while (initializer_list->endingSymbol == 0) { // list
        s.push(initializer_list);
        initializer_list = initializer_list -> firstChild;
    }
    
    int count = 0;
    translate_initializer(initializer_list, count++, typeSize);
    while (!s.empty()) {
        initializer_list = s.top();
        translate_initializer(initializer_list->next, count++, typeSize);
        s.pop();
    }
    
}