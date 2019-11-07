/*****@<parser.h>::****/
#pragma once

#include "lexer.h"

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
void expr(void);
void smpexpr(void);
int isOPLUS(void);
void term(void);
void exprlst(void);
void fact(void);
int isOTIMES(void);
int isNUM(void);

void match(token_t expected);

extern token_t lookahead;
extern int symtab_initial, symtab_final;

//line number