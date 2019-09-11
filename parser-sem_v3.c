/*
 * @<parser.c>::
 */

#include <stdio.h>
#include <stdlib.h>

#include <parser.h>
#include <tokens.h>
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

void
expr(void)
{
    /*[*/int oplus = 0, otimes = 0;/*]*/
_term:
    //term    
_fact:
    //fact
    switch (lookahead) {
    case ID:
        /*[*/fprintf(object, " id");/*]*/
        match(ID);
	if (lookahead == ASGN) {
		match(ASGN); expr();
	}
        break;
    case UINT:
        /*[*/fprintf(object, " uint");/*]*/
        match(UINT);
        break;
    case FLT:
        /*[*/fprintf(object, " flt");/*]*/
        match(FLT);
        break;
    default:
        match('(');
        expr();
        match(')');
    }
    
    if(otimes) {
        /*[*/fprintf(object, " exec(%c)", otimes);/*]*/
        /*[*/otimes = 0;/*]*/
    }

    /** abstract { oplus term }**/
    if(otimes = isOTIMES()) {
        match(otimes);
        goto _fact;
    }

    if(oplus) {
        /*[*/fprintf(object, " exec(%c)", oplus);/*]*/
        /*[*/oplus = 0;/*]*/
    }
    /** abstract { oplus term }**/
    if(oplus = isOPLUS()) {
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
