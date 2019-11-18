/**@<parser.c>::**/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "lexer.h"
#include "tokens.h"
#include "interface.h"
#include "keywords.h"
#include "parser.h"

/*********************************************************************
MyPascal is a name for the academic
project mypas, which is a simplified
and modified Pascal

Syntax::

Initial symbol: mypas

mypas -> PROGRAM ID ';' body .
*/
void mypas(void)
{
    match(PROGRAM);
    match(ID);
	match(';'); /*  missing ; match from previous version: mypas-skeleton */
    body();
}
/************************* Program Body ***************************
body -> declarative blkstmt
*/
void body(void)
{
    declarative();
    blkstmt();
}
/************************ Declaration of Initial Variables ********************************
declarative -> [ VAR varclause { varclause } ] methods
*/
void
declarative(void)
{
    if (lookahead == VAR) {
        match(VAR);
        varclause();
        while (lookahead == ID) {
            varclause();
        }
    }
    methods();
}
/************************ Recognizes single variables, groups of variables and its types ******************
varclause -> varlist ':' typespec ;
*/
void
varclause(void)
{
    varlist();
    match(':');
    typespec();
    match(';');
}

/************************* Groups variables ******************************
varlist -> varspec { , varspec }
*/
void
varlist(void)
{
    match(ID);
    while (lookahead == ',') {
        match(',');
        match(ID);
    }
}
/************************ Inspect Types ***********************************
typespec -> INTEGER | REAL | DOUBLE | BOOLEAN
*/
void
typespec(void)
{
    switch (lookahead) {
    case INTEGER:
    case REAL:
    case DOUBLE:
        match(lookahead);   /* Breaks were missing in this switch from last version: mypas-skeleton */
		break;
    default:
        match(BOOLEAN);
		break;
    }
}
/************ Recognizes functions or procedures blocks *********************
methods -> { procedure | function }
*/
void
methods(void)
{
    while (lookahead == PROCEDURE || lookahead == FUNCTION) {
        switch (lookahead) {
        case PROCEDURE:
            procedure();
            break;
        default:
            function();
        }
    }
}
/************************** Procedure Inspect ********************************
procedure -> PROCEDURE ID formalparm ';' body ;
*/
void
procedure(void)
{
    match(PROCEDURE);
    match(ID);
    formalparm();
    match(';');
    body();
    match(';');
}
/****************** Functions and procedures parameters **********************

/***********
 * formalparm -> [ '(' argdef {';' argdef } ')' ] 
************/

void formalparm(void){

	if (lookahead == '(') {
		match('(');
		argdef();
		
		while (lookahead == ';') {
			match(';');
			argdef();
			match(')');
		}
		match(')');
	}
}

/***********
 * argdef -> [ VAR  ] ID { ',' ID } ':' typespec  
 *
************/

void argdef(void) {

	if (lookahead == VAR) 
		match(VAR);

/********* This is the last version's (mypas-skeleton) code. Changed it to varlist() down below to avoid redundancy.
	match(ID);
	while (lookahead == ',') { 
		match(','); 
		match(ID); 
	}
****************************/

	varlist();

	match(':');
	typespec();
}

/*********************** Function Inspect ****************************
function -> FUNCTION ID formalparm ':' typespec ';' body ;
*/
void
function(void)
{
    match(FUNCTION);
    match(ID);
    formalparm();
    match(':');
    typespec();
    match(';');
    body();
    match(';');
}
/*********************  Recognizes a list of statements *****************************
stmtlist -> stmt { ';' stmt }
*/
void
stmtlist(void)
{
    stmt();
    while (lookahead == ';') {
        match(';');
        stmt();
    }
}
/********************** Inspects statement structure *********************************
stmt ->   blkstmt
	| ifstmt
	| whlstmt
	| repstmt
	| factor
	| <empty>
*/
void
stmt(void)
{
    switch (lookahead) {
    case BEGIN:
        blkstmt();
        break;
    case IF:
        ifstmt();
        break;
    case WHILE:
        whlstmt();
        break;
    case REPEAT:
        repstmt();
        break;
    default:
        switch (lookahead) {
        case '-':
        case '+':
        case '(':
        case NOT:
        case UINT:
        case FLTP:
        case FALSE:
        case TRUE:
		case ID: /* <---- ID was missing in last version (mypas-skeleton) and was added here in factor case*/
            factor();
        }
        /*
         * <empty>
         */ ;
    }
}
/*********************** Recognizes the begining and ending of statements **************************
blkstmt  -> BEGIN stmtlist END
*/
void
blkstmt(void)
{
    match(BEGIN);
    stmtlist();
    match(END);
}
/************************* Inspects IF-ELSE statement structure **************************
ifstmt   -> IF expr THEN stmt [ ELSE stmt ]
*/
void
ifstmt(void)
{
    match(IF);
    expr();
    match(THEN);
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        stmt();
    }
}
/************************* Inspects WHILE-DO statement structure **************************
whlstmt  -> WHILE expr DO stmt
*/
void whlstmt(void)
{
 match(WHILE);
 expr();
 match(DO);
 stmt();
}
/************************* Inspects REPEAT-UNTIL statement structure **************************
repstmt  -> REPEAT stmtlist UNTIL expr
*/
void repstmt(void)
{
  match(REPEAT);
  stmtlist();
  match(UNTIL);
  expr();
}
/************************** Expressions Syntax ****************************
smpexpr -> [ + | - | NOT ] term { oplus term }

	oplus   -> + | - | OR

term    -> factor { otimes factor }

	otimes  -> * | / | DIV | MOD | AND

factor  -> variable [ ":=" expr ] | constant | ( expr )

variable -> ID

constant -> UINT | FLT | TRUE | FALSE

expr   -> smpexpr [ relop smpexpr ]

	relop ->    >
		  | >= # GEQ
		  | <
		  | <= # LEQ
		  | <> # NEQ
		  | =
*/


/********************** Recognizes Relational Operations ***************************

	relop ->    >
				| >= # GEQ
				| <
				| <= # LEQ
				| <> # NEQ
				| =

*/
int
isrelop(void)
{
    int             relop = lookahead;
    switch (lookahead) {
    case '>':
    case GEQ:
    case '<':
    case LEQ:
    case NEQ:
    case '=':
        match(lookahead);
        return relop;
    }
    return 0;
}
/************************ Recognizes expressions ************************************
expr   -> smpexpr [ relop smpexpr ]
*/
void
expr(void)
{
    smpexpr();
    if (isrelop()) {
        smpexpr();
    }
}
/****************************************************************

(1) recursive parser for smpexpression using the Wirth's syntatic
  diagram:
 
                     /---<----(+)---<----\
                     |                   |
                     |  /--<--(*)--<--\  |  
                     |  |             |  |
                     |  |   ------    |  |
 smpexpr->--['+''-']--------|fact|---------->---
                            ------
 
 factor ->  ID | UINT | FLT | ( expr )

**********************************************/
void
smpexpr(void)
{
    switch (lookahead) {
    case '+':
        match('+');
        break;
    case '-':
        match('-');
    }

  T_entry:

  F_entry:

    factor();

    if (lookahead == '*' || lookahead == '/') {
        match(lookahead);
        goto F_entry;
    }

    if (lookahead == '+' || lookahead == '-') {
        match(lookahead);
        goto T_entry;
    }

}

/***************** Recognizes factors ******************
 *		factor ->  ID | UINT | FLT | ( expr )
 */

void factor(void)
{
    switch (lookahead) {
    case ID:
        match(ID);
        assgn();
        break;
    case UINT:
    case FLTP:
        match(lookahead);
        break;
    default:
        match('(');
        expr();
        match(')');
    }
}

/***************** Recognizes assignment operations ******************
 *		variable [ ":=" expr ]
 */

int assgn(void)
{
    if (lookahead == ASGN) {
        match(ASGN);
        expr();
    } else {
    }
}
