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
//    TODO criar isCompact(t1, t2) -> para checkar se é compativel na hora de fazer a comparação
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
      fact(0);
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
/***/
  int lblcount = 1;
/***/
void ifstm(void) {
  match(IF);
  expr(0);
  /***/
  int lblendif, lblelse;
  fprintf(object, "\tjz .L%d\n", lblendif = lblelse = lblcount++);
  /***/

  match(THEN);
  stmt();
  if (lookahead == ELSE) {
    /***/
    fprintf(object, "\tjmp .L%d\n", lblendif = lblcount++);
    fprintf(object, "\t.L%d:\n", lblelse);
    /***/
    match(ELSE);
    stmt();
  }
  /***/
  fprintf(object, "\t.L%d:\n", lblendif);
  /***/
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
  /***/
  int lblwhile, lblwend;
  /***/
  match(WHILE);
  /***/
  fprintf(object, ".L%d:\n", lblwhile = lblcount++);
  /***/
  expr(0);
  /***/
  fprintf(object, "\tjz .L%d\n", lblwend = lblcount++);
  /***/
  match(DO);
  stmt();
  /***/
  fprintf(object, "\tjmp .L%d\n", lblwhile);
  fprintf(object, ".L%d:\n", lblwend);
  /***/
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
  /***/
  int lblrepeat;
  /***/
  match(REPEAT);
  /***/
  fprintf(object, ".L%d:\n", lblrepeat = lblcount++);
  /***/
  stmlst();
  match(UNTIL);
  expr(0);
  /***/
  fprintf(object, "\tjz .L%d\n", lblrepeat);
  /***/
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
    t2 = smpexpr(MAX(t1, parent_type));
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
  parent_type = MAX(parent_type, acctype);
  acctype = term(parent_type);
  if (isOPLUS()) {
    match(lookahead);
    goto _term;
  }
}

/**************************************************************************************
isNEG:
      isNEG -> ['+'|'-'|NOT]
****************************************************************************************/
int isNEG(void) {
  switch (lookahead) {
    case '+':
      return '+';
    case '-':
      return '-';
    case NOT:
      return NOT;
    default:
      return 0;
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
int adding = 0;
int isOPLUS(void) {
  /***/
  switch (adding) {
    case 1:
      fprintf(object, "\taddl %%eax, (%%esp)\n");
      fprintf(object, "\tpopl %%eax\n");
      adding=0;
      break;
    case 2:
      fprintf(object, "\tsubl %%eax, (%%esp)\n");
      fprintf(object, "\tpopl %%eax\n");
      adding=0;
      break;
    case 3:
    case 4:
      adding=0;
      break;
  }
  /***/
  switch (lookahead) {
    case '+':
      /***/
      if(adding == 0) {
        fprintf(object, "\tpushl %%eax\n");
        adding=1;
      }
      /***/
      return '+';
    case '-':
      /***/
      if(adding == 0) {
        fprintf(object, "\tpushl %%eax\n");
        adding=2;
      }
      /***/
      return '-';
    case OR:
      /***/
      if(adding == 0) {
        fprintf(object, "\tpushl %%eax\n");
        adding=3;
      }
      /***/
      return OR;
    default:
      adding=4;
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
  parent_type = MAX(parent_type, acctype);
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
int mul = 0;
int isOTIMES(void) {
  /***/
  switch (mul) {
    case 1:
      fprintf(object, "\timull (%%esp)\n");
      fprintf(object, "\taddl $4, %%esp\n");
      mul=0;
      break;
    case 2:
      fprintf(object, "\tpop %%eax\n");
      fprintf(object, "\tcltd\n");
      fprintf(object, "\tidivl %%ecx\n");
      mul=0;
      break;
    case 3:
//      fprintf(object, "\tsubl %%eax, (%%esp)\n");
      mul=0;
      break;
    case 4:
    case 5:
    case 6:
      mul = 0;
      break;
  }
  /***/
  switch (lookahead) {
    case '*':
      /***/
      if(mul == 0) {
        fprintf(object, "\tpushl %%eax\n");
        mul=1;
      }
      /***/
      return '*';
    case '/':
      /***/
      if(mul == 0) {
        fprintf(object, "\tpushl %%eax\n");
        mul=2;
      }
      /***/
      return '/';
    case DIV:
      /***/
      if(mul == 0) {
        fprintf(object, "\tpushl %%eax\n");
        mul=3;
      }
      /***/
      return DIV;
    case MOD:
      /***/
      if(mul == 0) {
        fprintf(object, "\tpushl %%eax\n");
        mul=4;
      }
      /***/
      return MOD;
    case AND:
      /***/
      if(mul == 0) {
        fprintf(object, "\tpushl %%eax\n");
        mul=5;
      }
      /***/
      return AND;
    default:
      mul=6;
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
  type_t var_type;
  type_t expr_type;

  type_t acctype;
  /***/
//todo while, if, etc colocar 0 só por em quanto

  switch (lookahead) {
    case UINT:
      /***/
      acctype = MAX(parent_type, checKUint());
      /***/
      match(lookahead);
      break;
    case FLT:
      /***/
      acctype = MAX(parent_type, checkFlt());
      /***/
      match(lookahead);
      break;
    case CHR:
      /***/
      acctype = MAX(parent_type, 6);
      /***/
      match(CHR);
      break;
    case STR:
      /***/
      acctype = MAX(parent_type, 7);
      /***/
      match(STR);
      break;
    case TRUE:
      /***/
      acctype = MAX(parent_type, 5);
      /***/
      match(TRUE);
      break;
    case FALSE:
      /***/
      acctype = MAX(parent_type, 5);
      /***/
      match(FALSE);
      break;
    case ID:
      /***/
      acctype = symtab[var_descr = symtab_lookup(lexeme)].typedescriptor;
      fprintf(object, "\tmovl %s, %%eax\n", lexeme);
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
        exprlst(acctype);
        match(')');
//        fprintf(varname)
      } else {
//        fprintf(object, , varname);
      }
      break;
    default:
      match('(');
      acctype = expr(parent_type);
      match(')');
      break;
  }
  return MAX(acctype, parent_type);
}

type_t checKUint() {
  double value = atof(lexeme);
  if (value >= -32768 && value <= 32767) {
    return 1;
  }
  return 2;
}

type_t checkFlt() {
  char *min = "1.5E-45";
  float value = strtof(lexeme, NULL);
  if (value >= strtof(min, NULL) && value <= strtof("3.4E38", NULL)) {
    return 3;
  }
  return 4;
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
