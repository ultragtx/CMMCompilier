//
//  symbol.h
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-15.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#ifndef __CMMCompiler__symbol__
#define __CMMCompiler__symbol__

#include <iostream>

typedef struct {
    char *lexptr;
    int token;
    int addr;
    int size = 1; // real size if size * typeSize
    int type;
    int typeSize;
}entry;


int lookup(char *s);
int insert(char *s, int token);

extern entry symtable[];

#endif /* defined(__CMMCompiler__symbol__) */
