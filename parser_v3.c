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
 * rest -> '+'term rest | '-'term rest | <empty>
 * term -> fact quoc
 * quoc -> '*'fact quoc | '/'fact quoc | <empty>
 * fact -> ID [ := expr ] | DEC | '(' expr ')'
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
    int oplus;
_term:
    int otimes;
_fact:
    //fact
    switch (lookahead) {
    case ID:
        match(ID);
	if (lookahead == ASGN) {
		match(ASGN); expr();
	}
        break;
    case DEC:
        match(DEC);
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
