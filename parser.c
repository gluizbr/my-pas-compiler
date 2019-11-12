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
#include "include/symtab.h"
#include "include/default.h"

token_t lookahead;
int symtab_descriptor;
char lexeme[MAXIDLEN + 1];

/*******************************************************************************
 Syntax for simplified / modified Pascal, namely MyPas Project

 EBNF Grammar

 mypas: initial nonterminal symbol

 mypas -> [ PROGRAM ID '(' input ',' output ')' ';' ] declscope procdecl stmblock '.'
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
  procdecl();
  stmblock();
  match('.');
}

/**/ int symtab_initial, symtab_final; /**/
/*
 * @ declscope::
 */
/**************************************************************************************
declscope:
            *Function made for verify the pattern of an declarative scope.
    Pattern:
            *declscope -> { VAR varlst ':' vartype ';' } procdecl
****************************************************************************************/
void declscope(void) {
  while (lookahead == VAR) {
    match(VAR);
    /***/
    symtab_initial = symtab_descriptor;
    /***/
    varlst();
    /***/
    symtab_final = symtab_descriptor;
    /***/
    match(':');
    vartype();
    match(';');
  }
}

/******************
varlst -> ID { ',' ID }
*/
int fatalerrcount = 0;
/**/char *varname/**/;
/*
 * @ varlst::
 */
/**************************************************************************************
varlst:
            *Function made for verify the pattern of an variable list.
    Pattern:
            *varlst -> ID { ',' ID }
****************************************************************************************/
void varlst(void) {
  _varlst:
  /************* a variable must be registered *********/
  /**/
  if (symtab_lookup(lexeme)) {
    /********** symbol arread declared *******/
    fatalerrcount++;
  } else {
    /********** append new symbol on table ***********/
    symtab_append(lexeme);
  }
  /**/
  match(ID);
  if (lookahead == ',') {
    match(',');
    goto _varlst;
  }
}

/*
 * @ vartype::
 */
/**************************************************************************************
varlst:
            *Function made for verify the pattern of an variable type.
            *TYPES
                  -INTEGER
                  -LONG
                  -REAL
                  -DOUBLE
                  -BOOLEAN
                  -CHAR
                  -STRING
    Pattern:
            *vartype -> INTEGER | LONG | REAL | DOUBLE | BOOLEAN | CHAR | STRING
****************************************************************************************/
void vartype(void) {
  switch (lookahead) {
    case INTEGER:
      /***/
      symtab_type_range(1);
      /***/
      match(INTEGER);
      break;
    case LONG:
      /***/
      symtab_type_range(2);
      /***/
      match(LONG);
      break;
    case REAL:
      /***/
      symtab_type_range(3);
      /***/
      match(REAL);
      break;
    case DOUBLE:
      /***/
      symtab_type_range(4);
      /***/
      match(DOUBLE);
      break;
    case BOOLEAN:
      /***/
      symtab_type_range(5);
      /***/
      match(BOOLEAN);
      break;
    case CHAR:
      /***/
      symtab_type_range(6);
      /***/
      match(CHAR);
      break;
    default:
      /***/
      symtab_type_range(7);
      /***/
      match(STRING);
      break;
  }
}

/*
 * @ procdecl::
 */
/**************************************************************************************
procdecl:
            *Function made for verify the pattern of an procedure declaration.
    Pattern:
            *procdecl -> { PROCEDURE ID parmdef ';' declscope stmblock |
              FUNCTION ID parmdef ':' vartype ';' declscope stmblock }
****************************************************************************************/
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

/*
 * @ stmblock::
 */
/**************************************************************************************
stmblock:
            *Function made for verify the pattern of an statement block.
    Pattern:
            *stmblock -> BEGIN stmtlst END
****************************************************************************************/
void stmblock(void) {
  match(BEGIN);
  stmlst();
  match(END);
}

/*
 * @ stmlst::
 */
/**************************************************************************************
stmlst:
            *Function made for verify the pattern of an statement list.
    Pattern:
            *stmlst -> stmt { ';' stmt }
****************************************************************************************/
void stmlst(void) {
  stmt();
  while (lookahead == ';') {
    match(';');
    stmt();
  }
}

/*
 * @ stmt::
 */
/**************************************************************************************
stmt:
            *Function made for verify the pattern of an statement.
    Pattern:
            *stmt -> stmblock | ifstm | whilestm | repstm | fact
****************************************************************************************/
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

/*
 * @ ifstm::
 */
/**************************************************************************************
ifstm:
            *Function made for verify the pattern of an if statement.
    Pattern:
            *ifstm -> IF expr THEN stmt [ ELSE stmt ]
****************************************************************************************/
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

/*
 * @ whilestm::
 */
/**************************************************************************************
whilestm:
            *Function made for verify the pattern of an while statement.
    Pattern:
            *whilestm -> WHILE expr DO stmt
****************************************************************************************/
void whilestm(void) {
  match(WHILE);
  expr();
  match(DO);
  stmt();
}

/*
 * @ repstm::
 */
/**************************************************************************************
repstm:
            *Function made for verify the pattern of an while statement.
    Pattern:
            *repstm -> REPEAT stmlst UNTIL expr
****************************************************************************************/
void repstm(void) {
  match(REPEAT);
  stmlst();
  match(UNTIL);
  expr();
}

/*
 * @ expr::
 */
/**************************************************************************************
expr:
            *Function made for verify the pattern of an expression.
    Pattern:
            *expr -> smpexpr [ RELOP smpexpr ]
****************************************************************************************/
type_t expr(type_t parent_type) {
  /***/
  type_t t1, t2 = 0;
  /***/
  t1 = smpexpr(parent_type);
  if (lookahead == '<' || lookahead == NEQ || lookahead == LEQ || lookahead == '=' || lookahead == GEQ ||
      lookahead == '>') {
    match(lookahead);
    t2 = smpexpr(max(t1, parent_type));
  }
  if (t2) return t2;
  return t1;
}

/*
 * @ smpexpr::
 */
/**************************************************************************************
smpexpr:
            *Function made for verify the pattern of a sum expression.
    Pattern:
    #todo NOT
            *smpexpr -> ['+'|'-'|NOT] term { OPLUS term }
****************************************************************************************/
type_t smpexpr(type_t parent_type) {
  /***/
  type_t acctype = 0;
  /***/
  if (isNEG()) {
    match(lookahead);
  }
  _term:
  parent_type = max(parent_type, acctype);
  acctype = term(parent_type);
  if (isOPLUS()) {
    match(lookahead);
    goto _term;
  }
}

/*
 * @ isOPLUS::
 */
/**************************************************************************************
isOPLUS:
            *Function made for verify the pattern.
    Pattern:
            *OPLUS = " + | - " | OR
****************************************************************************************/
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

/*
 * @ term::
 */
/**************************************************************************************
term:
            *Function made for verify the pattern of a term.
    Pattern:
            *term -> fact { OTIMES fact }
****************************************************************************************/
type_t term(type_t parent_type) {
  /***/
  type_t acctype = 0;
  /***/
  _fact:
  parent_type = max(parent_type, acctype);
  acctype = fact(parent_type);
  if (isOTIMES()) {
    match(lookahead);
    goto _fact;
  }
}

/*
 * @ isOTIMES::
 */
/**************************************************************************************
isOTIMES:
            *Function made for verify the pattern.
    Pattern:
            *OTIMES = " * | / " | DIV | MOD | AND
****************************************************************************************/
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

/********************
   * EBNF:
   * exprlist -> expr { ',' expr }
   *
   */
   //*todo modificar tem que ter uma variavel
void exprlst(void) {
  _expr:
  expr(0);
  if (lookahead == ',') {
    match(',');
    goto _expr;
  }
}

/*
 * @ fact::
 */
/**************************************************************************************
fact:
            *Function made for verify the pattern of a factor.
    Pattern:
            *fact ->   '(' expr ')'
                      | NUM
                      | CHR
                      | STR
                      | TRUE
                      | FALSE
                      | ID [ ":=" expr ]
****************************************************************************************/


type_t fact(type_t parent_type) {

  /***/
  int var_descr;
  type_t acctype;
  /***/

  switch (lookahead) {
    case UINT:
      /***/
      acctype = max(parent_type, 1);
      /***/
      match(lookahead);
      break;
    case FLT:
      /***/
      //todo verificar o tipo se é ponto flutuante ou simples
      acctype = max(parent_type, 3);
      /***/
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
      /***/
      acctype = symtab[var_descr = symtab_lookup(lexeme)].typedescriptor;
      /***/
      match(ID);
      if (lookahead == ASSGN) {
        match(ASSGN);
        //todo validar se é compativel
        /***/
        acctype = expr(parent_type);
        /***/
      } else if (lookahead == '(') {
        match('(');
        exprlst();
        match(')');
      }
      break;
    default:
      match('(');
      acctype = expr(parent_type);
      match(')');
      break;
  }
  return max(acctype, parent_type);
}

/*
 * @ isNUM::
 */
/**************************************************************************************
isNUM:
            *Function made for verify the pattern of a number.
    Pattern:
            *NUM = UINT | FLT
****************************************************************************************/
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
 * @ match::
 */
/**************************************************************************************
match:
            *lexer-to-parser interface:
****************************************************************************************/
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
