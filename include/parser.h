/*****@<parser.h>::****/
#pragma once

#include "lexer.h"

typedef int type_t;

/*
 * @ declarative functions of parser.c::
 */
void mypas(void);
void declscope(void);
void varlst(void);
void vartype(void);
void procdecl(void);
void parmdef(void);
void stmblock(void);
void stmlst(void);
void stmt(void);
void ifstm(void);
void whilestm(void);
void repstm(void);
int isOPLUS(void);
void exprlst(void);
type_t expr(type_t parent_type);
type_t smpexpr(type_t parent_type);
type_t term(type_t parent_type);
type_t fact(type_t parent_type);
int isOTIMES(void);
int isNUM(void);

//type_t checKUint();
//type_t checkFlt();
int isNEG(void);

void match(token_t expected);

extern token_t lookahead;
extern int symtab_initial, symtab_final;

//line number