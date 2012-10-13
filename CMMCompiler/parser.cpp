//
//  parser.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-13.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "parser.h"
#include <stack>

using namespace std;

typedef int ParserState;

typedef struct {
    
}ParserElem;

stack<ParserState> stateStack;
stack<ParserElem> elemStack;

int parser() {
    ParserState beginState = 0;
    
    stateStack.push(beginState);
    // TODO: push ending to elemStack
    
    while (true) {
        // lex
        
    }
    
    return 0;
}



