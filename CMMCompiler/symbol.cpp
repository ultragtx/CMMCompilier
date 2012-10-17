//
//  symbol.cpp
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-15.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#include "symbol.h"
#include "util.h"
#include <cstring>

#define STRMAX 999
#define SYMMAX 100

char lexemes[STRMAX];
int lastchar = -1;


entry symtable[SYMMAX];
int lastentry = 0;

int lookup(char *s) {
    int p;
    for (p = lastentry; p > 0; p--) {
        if (strcmp(symtable[p].lexptr, s) == 0) {
            return p;
        }
    }
    return 0;
}

int insert(char *s, int token) {
    int len;
    len = (int)strlen(s);
    
    if (lastentry + 1 >= SYMMAX) {
        error("symbol table full\n");
    }
    if (lastchar + len + 1 >= STRMAX) {
        error("lexemes arry full\n");
    }
    lastentry++;
    symtable[lastentry].token = l2e(token); // convert to parser type
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return lastentry;
}
