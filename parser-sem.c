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
    int otimes = lookahead;
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
checkposfixed(int oplus, int otimes){
    if(oplus > 0) {
        /*[*/fprintf(object, " exec(%c)", oplus);/*]*/
    } else if(otimes > 0) {
        /*[*/fprintf(object, " exec(%c)", otimes);/*]*/
    }
}

void
expr(void)
{
    int oplus = 0;
    int otimes = 0;
_term:
    //term    
_fact:
    //fact
    switch (lookahead) {
    case ID:
        /*[*/fprintf(object, " id");/*]*/
        checkposfixed(oplus, otimes);
        otimes = 0;
        oplus = 0;
        match(ID);
	if (lookahead == ASGN) {
		match(ASGN); expr();
	}
        break;
    case UINT:
        /*[*/fprintf(object, " uint");/*]*/
        checkposfixed(oplus, otimes);
        otimes = 0;
        oplus = 0;
        match(UINT);
        break;
    case FLT:
        /*[*/fprintf(object, " flt");/*]*/
        checkposfixed(oplus, otimes);
        otimes = 0;
        oplus = 0;
        match(FLT);
        break;
    default:
        match('(');
        expr();
        match(')');
    }
    /** abstract { oplus term }**/
    if(otimes = isOTIMES()) {
        match(otimes);
        goto _fact;
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
