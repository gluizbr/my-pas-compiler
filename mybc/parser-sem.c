/*
 * @<parser.c>::
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/parser.h"
#include "include/tokens.h"
#include "include/lexer.h"
#include "include/vmachine.h"
#include <string.h>
#include "include/main.h"

/*
 * lexer-to-parser interface 
 */
int lookahead;

int gettoken(FILE *);
/*
 * adopted grammar: LL(1) for simple expressions 
 */
/*************************************************
 * expr -> term rest
 * rest -> '+'term rest 
 *      | '-'term rest 
 *      | <empty>
 * term -> fact quoc
 * quoc -> '*'fact quoc 
 *      | '/'fact quoc 
 *      | <empty>
 * fact -> ID [ := expr ] 
 *      | DEC 
 *      | '(' expr ')'
 *************************************************/
/*
 * grammar emulation functions 
 */
int isOPLUS(void) {
  switch (lookahead) {
    case '+':
      return '+';
    case '-':
      return '-';
    default:
      return 0;
  }
}

int isOTIMES(void) {
  switch (lookahead) {
    case '*':
      return '*';
    case '/':
      return '/';
    default:
      return 0;
  }
}

void cmdln(void) {
  switch (lookahead) {
    case '+':
    case '-':
    case '(':
    case ID:
    case FLT:
    case UINT: {
      __expr:
      expr();

      /*************************/
      if (lookahead == ';') {
        match(';');
        goto __expr;
      }

      /*[print the last result]*/
      fprintf(object, "%lg \n", acc);
    }
      break;
    default:;
  }
}

void mybc(void) {
  while (lookahead != EOF && lookahead != EXIT) {
    cmdln();
    match('\n');
  }
}

extern double vmem[];

int symtb_add(char *);

void
expr(void) {
  /*[*/int oplus = 0, otimes = 0, oneg = 0;/*]*/
  /*[*/char vrname[MAXIDLEN + 1];
  int vradress = 0;/*]*/
  double arg1 = 0, saved = 0;
  if (lookahead == '-' || lookahead == '+') {
    oneg = lookahead;
    match(oneg);
  }
  _term:
  //term
  _fact:
  //fact
  if ((oplus && otimes)) {
    //save value if there is one pending operation
    // printf("\n saved %d %d \n", oplus, otimes);
    saved = arg1;
    // printf("\n what number saved is? %f \n", saved);
  }
  if ((oplus || otimes)) {
    // printf("\n is one of them? %d %d \n", oplus, otimes);
    arg1 = pop();
    // printf("\n what number is? %f \n", arg1);
  }
  switch (lookahead) {
    case ID:
      /***********************/
      strcpy(vrname, lexeme);
      /***********************/
      // /*[*/fprintf(object, " %s", lexeme);/*]*/
      match(ID);
      if (lookahead == ASGN) {
        match(ASGN);
        expr();
        //** an assignment from the right-side expression to the left side variable is required, then...**/
        // vradress = consultatabelasimboilos()
        vradress = symtb_add(vrname);
        vmem[vradress] = acc;
        /*****************************************/
      } else {
        //** a memory retrieveing from the variable is required, then...**/
        vradress = symtb_add(vrname);
        acc = vmem[vradress];
        /*****************************************/
      }
      break;
    case UINT:
      acc = atof(lexeme);
      // /*[*/fprintf(object, " uint %f", atof(lexeme));/*]*/
      match(UINT);
      break;
    case FLT:
      acc = atof(lexeme);
      // /*[*/fprintf(object, " flt %f", atof(lexeme));/*]*/
      match(FLT);
      break;
    default:
      match('(');
      expr();
      match(')');
  }

  if (otimes) {
    //if there if already an otimes
    //execop(arg1, arg2, op)
    // printf("\n pop on otimes \n");
    acc = execop(arg1, acc, otimes);
    // /*[*/fprintf(object, " exec(%c)", otimes);/*]*/
    /*[*/otimes = 0;/*]*/
  }

  /** abstract { oplus term }**/
  if (otimes = isOTIMES()) {
    // printf("\n push on otimes \n");
    push(acc);
    match(otimes);
    goto _fact;
  }

  /**** if oneg is set there is a pending invert operation **/
  if (oneg) {
    if (oneg == '-') acc = -acc;
    oneg = 0;
  }
  /*************************/

  if (oplus) {
    //if there if already an oplus
    //push(execop(arg1, arg2, oplus))
    // printf("\n pop on oplus \n");
    if (saved > 0 || saved < 0) {
      arg1 = saved;
      saved = 0;
    }
    acc = execop(arg1, acc, oplus);
    // /*[*/fprintf(object, " exec(%c)", oplus);/*]*/
    /*[*/oplus = 0;/*]*/
  }
  /** abstract { oplus term }**/
  if (oplus = isOPLUS()) {
    // printf("push on push %f", acc);
    push(acc);
    match(oplus);
    goto _term;
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
//        Should not exit or abort the program only skip this instruction
//        exit resetCalculator !!!@!@ should ignore this operation
    exit(-2);
  }
}
