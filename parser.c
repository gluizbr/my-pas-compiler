/***@<parser.c>::***/

/*******************************************************************************
 * Author: Prof. Eraldo Pereira Marinho, Ph.D.
 * Local standard date: ter out 29 09:38:03 -03 2019
 ******************************************************************************/

#include <ctype.h>
#include <stdlib.h>
#include "include/tokens.h"
#include "include/keywords.h"
#include "include/parser.h"
#include "include/lexer.h"
#include "include/main.h"

token_t lookahead;
char lexeme[MAXIDLEN + 1];

/*******************************************************************************
 Syntax for simplified / modified Pascal, namely MyPas Project

 EBNF Grammar

 mypas: initial nonterminal symbol

 mypas -> [ PROGRAM ID '(' input ',' output ')' ';' ] declscope stmblock '.'
*******************************************************************************/
void mypas(void) {
  if (lookahead == PROGRAM) {
    match(PROGRAM);
    match(ID);
    match('(');
    match(ID);
    match(',');
    match(ID);
    match(')');
    match(';');
  }

  declscope();
  stmblock();
  match('.');
}


/*******************************************************************************
--------------------------------------------------------------------------------
declscope -> { VAR varlst ':' vartype ';' } procdecl
*/
void declscope(void) {
  while (lookahead == VAR) {
    match(VAR);
    varlst();
    match(':');
    vartype();
    match(';');
  }
  procdecl();
}

/******************
varlst -> ID { ',' ID }
*/
void  varlst(void) {
  _varlst:
  match(ID);
  if (lookahead == ',') {
    match(',');
    goto _varlst;
  }
}

/*************************
vartype -> INTEGER | LONG | REAL | DOUBLE | BOOLEAN | CHAR | STRING
****************/
void vartype(void) {
  switch (lookahead) {
    case INTEGER:
      match(INTEGER);
      break;
    case LONG:
      match(LONG);
      break;
    case REAL:
      match(REAL);
      break;
    case DOUBLE:
      match(DOUBLE);
      break;
    case BOOLEAN:
      match(BOOLEAN);
      break;
    case CHAR:
      match(CHAR);
      break;
    default:
      match(STRING);
      break;
  }
}

/*********************
procdecl -> { PROCEDURE ID parmdef ';' declscope stmblock |
              FUNCTION ID parmdef ':' vartype ';' declscope stmblock }
***********/
void procdecl(void) {
  int isfunc = 0;
  while (lookahead == PROCEDURE || (isfunc = lookahead == FUNCTION)) {
    match(lookahead);
    match(ID);
    parmdef();
    if (isfunc != 0) {
      match(':');
      vartype();
    }
    match(';');
    declscope();
    stmblock();
  }
}

/**************
 * parmdef -> [ '(' [VAR] varlst ':' vartype { ';' [VAR] varlst ':' vartype }  ')' ]
*/
void parmdef(void) {
  if (lookahead = '(') {
    match('(');
    _parmdef:
    if (lookahead == VAR) {
      match(VAR);
    }
    varlst();
    match(':');
    vartype();
    if (lookahead == ';') {
      match(';');
      goto _parmdef;
    }
    match(')');
  }
}

/*************
stmblock -> BEGIN stmtlst END
**************/
void stmblock(void) {
  match(BEGIN);
  stmlst();
  match(END);
}

/*************
stmlst -> stmt { ';' stmt }
*******************************************************************************/
void stmlst(void) {
  stmt();
  while (lookahead == ';') {
    match(';');
    stmt();
  }
}

/*******************************************************************************
stmt -> stmblock | ifstm | whilestm | repstm | fact
*************/
void stmt(void) {
  switch (lookahead) {
    case BEGIN:
      stmblock();
      break;
    case IF:
      ifstm();
      break;
    case WHILE:
      whilestm();
      break;
    case REPEAT:
      repstm();
      break;
    default:
      fact();
  }
}

/**************
ifstm -> IF expr THEN stmt [ ELSE stmt ]
**************/
void ifstm(void) {
  match(IF);
  expr();
  match(THEN);
  stmt();
  if (lookahead == ELSE) {
    match(ELSE);
    stmt();
  }
}

/******************
whilestm -> WHILE expr DO stmt
 **********************/
void whilestm(void) {
  match(WHILE);
  expr();
  match(DO);
  stmt();
}

/****************
repstm -> REPEAT stmlst UNTIL expr
******************************************************************************/
void repstm(void) {
  match(REPEAT);
  stmlst();
  match(UNTIL);
  expr();
}


/*******************************************************************************
expr -> smpexpr [ RELOP smpexpr ]
*********************/
void expr(void) {
  smpexpr();
  if (lookahead == '<' || lookahead == NEQ || lookahead == LEQ || lookahead == '=' || lookahead == GEQ ||
      lookahead == '>') {
    match(lookahead);
    smpexpr();
  }
}

/***************************
smpexpr -> ['+'|'-'] term { OPLUS term }
***************************/
void smpexpr(void) {
  if (lookahead == '+' || lookahead == '-') {
    match(lookahead);
  }
  term();
  while (isOPLUS()) {
    match(lookahead);
    term();
  }
}


/**************************
OPLUS = " + | - " | OR
****************************/
int isOPLUS(void) {
  switch (lookahead) {
    case '+':
      return '+';
    case '-':
      return '-';
    case OR:
      return OR;
    default:
      return 0;
  }
}

/**************************
term -> fact { OTIMES fact }
**************************/
void term(void) {
  fact();
  while (isOTIMES()) {
    match(lookahead);
    fact();
  }
}

/*************************
OTIMES = " * | / " | DIV | MOD | AND
************************/
int isOTIMES(void) {
  switch (lookahead) {
    case '*':
      return '*';
    case '/':
      return '/';
    case DIV:
      return DIV;
    case MOD:
      return MOD;
    case AND:
      return AND;
    default:
      return 0;
  }
}

/***********************
fact ->   '(' expr ')'
	| NUM
	| CHR
	| STR
	| TRUE
	| FALSE
	| ID [ ":=" expr ]
 ********************/
void fact(void) {
  switch (lookahead) {
    case UINT:
    case FLT:
      match(lookahead);
      break;
    case CHR:
      match(CHR);
      break;
    case STR:
      match(STR);
      break;
    case TRUE:
      match(TRUE);
      break;
    case FALSE:
      match(FALSE);
      break;
    case ID:
      match(ID);
      if (lookahead == ASSGN) {
        match(ASSGN);
        expr();
      }
      break;
    default:
      match('(');
      expr();
      match(')');
      break;
  }
}

/**************
NUM = UINT | FLT
 *************/
int isNUM(void) {
  switch (lookahead) {
    case UINT:
      return UINT;
    case FLT:
      return FLT;
    default:
      return 0;
  }
}

/*
 * lexer-to-parser interface:
 */
void
match(int expected) {
  if (lookahead == expected) {
    lookahead = gettoken(source);
  } else {
    fprintf(stderr,
            "%d seen while %d was expected\n", lookahead, expected);
    exit(-2);
  }
}

/****************
Notes:: 
 1. low case variable means nonterminals whereas upper case means tokens, or
 terminal symbols. Of course, explicit ASCII characters are standard terminals;
 2. this is not Pascal, neither C.
*******************************************************************************/

/**** Atividade:
 * 1. Com base na sintaxe acima exposta, implemente um analisador sintático (parser)
 * para cada uma das produções. Observe que os tokens serão tratados no nível léxico
 * mas esses podem ser, em alguns casos, tratados diretamente no parser, na forma
 * de funções predicado.
 *
 * 2. Aumente o analisador léxico para os tokens que sejam mais bem definidos
 * na parte léxica do pré-compilador.
 ****/
