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
void
expr(void)
{
    term();
    rest();
}

void
rest(void)
{
    switch (lookahead) {
    case '+':
    case '-':
        match(lookahead);
        term();
        rest();
        break;
     case-1:case')':
		break;
     default:
        fprintf(stderr,
	"ilegal trailing symbol: '%c'\n", lookahead);
	exit(-3);
    }
}

void
term(void)
{
    fact();
    quoc();
}

void
quoc(void)
{
    switch (lookahead) {
    case '*':
    case '/':
        match(lookahead);
        fact();
        quoc();
        break;
     case-1:case')':
     case'+':case'-':
		break;
     default:
        fprintf(stderr,
	"ilegal trailing symbol: '%c'\n", lookahead);
	exit(-3);
    }
}

void
fact(void)
{
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
