/*
 * @<parser.c>::
 */

#include <stdio.h>
#include <stdlib.h>

#include <parser.h>
#include <tokens.h>
#include <lexer.h>
#include <vmachine.h>
#include <string.h>
#include <main.h>
/*
 * lexer-to-parser interface 
 */
int             lookahead;
int             gettoken(FILE *);
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

int isOTIMES(void){
    switch (lookahead) {
    case '*':
        return '*';
    case '/':
        return '/';
    default:
        return 0;
    }
}

void mybc(void) {

__expr:
    expr();

    /*[print the last result]*/    
    fprintf(object, "%lg \n", acc);
    /*************************/
    if(lookahead == ';') {
        match(';'); goto __expr;
    }
}

extern double vmem[];
int symtb_add(char *);
void
expr(void)
{
    /*[*/int oplus = 0, otimes = 0, oneg = 0;/*]*/
    /*[*/char vrname[MAXIDLEN+1]; int vradress = 0;/*]*/
    double arg1 = 0;
    if (lookahead == '-' || lookahead == '+') {
        oneg = lookahead;
        match(oneg);
    }
_term:
    //term    
_fact:
    //fact
    // if there is an oplus or otimes
    // (oplus || otimes)
    // double arg1 = pop()
    if(oplus || otimes){
        arg1 = pop();
    }
    switch (lookahead) {
    case ID:
        /***********************/
        strcpy(vrname, lexeme);
        /***********************/
        /*[*/fprintf(object, " %s", lexeme);/*]*/
        match(ID);
        if (lookahead == ASGN) {
            match(ASGN); expr();
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
        /*[*/fprintf(object, " uint %f", atof(lexeme));/*]*/
        match(UINT);
        break;
    case FLT:
        acc = atof(lexeme);
        /*[*/fprintf(object, " flt %f", atof(lexeme));/*]*/
        match(FLT);
        break;
    default:
        match('(');
        expr();
        match(')');
    }
    //if(arg1 = 0) {
    //push value to stack (push(atof(lexeme))
    //}

    if(otimes) {
        //if there if already an otimes
        //execop(arg1, arg2, op)
        acc = execop(pop(), acc, otimes);
        /*[*/fprintf(object, " exec(%c)", otimes);/*]*/
        /*[*/otimes = 0;/*]*/
    }

    /** abstract { oplus term }**/
    if(otimes = isOTIMES()) {
        push(acc);
        match(otimes);
        goto _fact;
    }

    /**** if oneg is set there is a pending invert operation **/
    if(oneg) {
        if (oneg == '-') acc = -acc;
        oneg = 0;
    }
    /*************************/

    if(oplus) {
        //if there if already an oplus
        //push(execop(arg1, arg2, oplus))
        acc = execop(pop(), acc, oplus);
        /*[*/fprintf(object, " exec(%c)", oplus);/*]*/
        /*[*/oplus = 0;/*]*/
    }
    /** abstract { oplus term }**/
    if(oplus = isOPLUS()) {
        printf("before push %f", acc);
        push(acc);
        match(oplus);
        goto _term;
    }
}

/*
 * lexer-to-parser interface: 
 */
void
match(int expected)
{
    if (lookahead == expected) {
        lookahead = gettoken(source);
    } else {
        fprintf(stderr,
                "%d seen while %d was expected\n", lookahead, expected);
        exit(-2);
    }
}
