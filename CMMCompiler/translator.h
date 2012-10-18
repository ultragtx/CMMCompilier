//
//  translator.h
//  CMMCompiler
//
//  Created by Xinrong Guo on 12-10-17.
//  Copyright (c) 2012年 Xinrong Guo. All rights reserved.
//

#ifndef __CMMCompiler__translator__
#define __CMMCompiler__translator__

#include <iostream>
#include "lalr.h"
#include "general.h"

void translate_initializer(ParserElem *initializer, int count, int typeSize);
void translate_initializer_list(ParserElem *initializer, int typeSize);
void translate_init_declarator(ParserElem *init_declarator, int typeSize);
void translate_init_declarator_list(ParserElem *elem, int typeSize);

void reduce00(int action);
void reduce01(int action);
void reduce02(int action);
void reduce03(int action);
void reduce04(int action);
void reduce05(int action);
void reduce06(int action);
void reduce07(int action);
void reduce08(int action);
void reduce09(int action);
void reduce10(int action);
void reduce11(int action);
void reduce12(int action);
void reduce13(int action);
void reduce14(int action);
void reduce15(int action);
void reduce16(int action);
void reduce17(int action);
void reduce18(int action);
void reduce19(int action);
void reduce20(int action);
void reduce21(int action);
void reduce22(int action);
void reduce23(int action);
void reduce24(int action);
void reduce25(int action);
void reduce26(int action);
void reduce27(int action);
void reduce28(int action);
void reduce29(int action);
void reduce30(int action);
void reduce31(int action);
void reduce32(int action);
void reduce33(int action);
void reduce34(int action);
void reduce35(int action);
void reduce36(int action);
void reduce37(int action);
void reduce38(int action);
void reduce39(int action);
void reduce40(int action);
void reduce41(int action);
void reduce42(int action);
void reduce43(int action);
void reduce44(int action);
void reduce45(int action);
void reduce46(int action);
void reduce47(int action);
void reduce48(int action);
void reduce49(int action);
void reduce50(int action);
void reduce51(int action);
void reduce52(int action);
void reduce53(int action);
void reduce54(int action);
void reduce55(int action);
void reduce56(int action);
void reduce57(int action);
void reduce58(int action);
void reduce59(int action);
void reduce60(int action);
void reduce61(int action);
void reduce62(int action);
void reduce63(int action);
void reduce64(int action);
void reduce65(int action);
void reduce66(int action);
void reduce67(int action);
void reduce68(int action);
void reduce69(int action);
void reduce70(int action);
void reduce71(int action);
void reduce72(int action);
void reduce73(int action);
void reduce74(int action);
void reduce75(int action);
void reduce76(int action);
void reduce77(int action);
void reduce78(int action);
void reduce79(int action);
void reduce80(int action);
void reduce81(int action);
void reduce82(int action);
void reduce83(int action);
void reduce84(int action);
void reduce85(int action);
void reduce86(int action);
void reduce87(int action);
void reduce88(int action);
void reduce89(int action);
void reduce90(int action);
void reduce91(int action);
void reduce92(int action);
void reduce93(int action);
void reduce94(int action);
void reduce95(int action);
void reduce96(int action);
void reduce97(int action);
void reduce98(int action);
void reduce99(int action);

void (* const reduceTable[NUM_PRODUCTION])(int) = {
    reduce00,
    reduce01,
    reduce02,
    reduce03,
    reduce04,
    reduce05,
    reduce06,
    reduce07,
    reduce08,
    reduce09,
    reduce10,
    reduce11,
    reduce12,
    reduce13,
    reduce14,
    reduce15,
    reduce16,
    reduce17,
    reduce18,
    reduce19,
    reduce20,
    reduce21,
    reduce22,
    reduce23,
    reduce24,
    reduce25,
    reduce26,
    reduce27,
    reduce28,
    reduce29,
    reduce30,
    reduce31,
    reduce32,
    reduce33,
    reduce34,
    reduce35,
    reduce36,
    reduce37,
    reduce38,
    reduce39,
    reduce40,
    reduce41,
    reduce42,
    reduce43,
    reduce44,
    reduce45,
    reduce46,
    reduce47,
    reduce48,
    reduce49,
    reduce50,
    reduce51,
    reduce52,
    reduce53,
    reduce54,
    reduce55,
    reduce56,
    reduce57,
    reduce58,
    reduce59,
    reduce60,
    reduce61,
    reduce62,
    reduce63,
    reduce64,
    reduce65,
    reduce66,
    reduce67,
    reduce68,
};

#endif /* defined(__CMMCompiler__translator__) */
