/**@<parser.c>::**/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "lexer.h"
#include "tokens.h"
#include "interface.h"
#include "keywords.h"
#include "parser.h"
#include "pseudocode.h"

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
    match('.');
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
        /** unary ops **/
        case '-':
        case '+':
        case NOT:
        /** constants **/      
        case UINT:
        case FLTP:
        case FALSE:
        case TRUE:
        /** variable, function or procedure **/
		case ID: /* <---- ID was missing in last version (mypas-skeleton) and was added here in factor case*/
		/** otherwise, it must be a '(' **/
        case '(':
            expr();
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

/***********************************************************
Semantic rule to translate ifstmt to a pseudocode:
======================================================
Target Language			||	Object Language   
	syntax rule			||	    synthesis rule
========================||============================
ifstmt ->				||	<ifstmt>.as <- 
------------------------------------------------------------------------------ 
	IF expr THEN 		||		<expr>.as
						||		jz .L<endif|else>
------------------------------------------------------------------------------
		stmt 			||		<stmt>.as
------------------------------------------------------------------------------
	[ ELSE 				||	[	jmp .L<endif>
						||	  .L<else>:
------------------------------------------------------------------------------
		stmt ]			||		<stmt>.as	]						
						||	  .L<endif>:
======================================================
* 
* Semantic Actions:
* ifstmt -> IF expr THEN 
* 				[[printf("\tjz .L%d\n", lblendif=lblelse=loopcount++);]]
* 				stmt
* 				[ ELSE
* 				  [[printf("\tjmp .L%d\n", lblendif=loopcount++);]]	 
* 			  	  [[printf(".L%d\n", lblelse);]]
* 				stmt ]
* 				  [[printf(".L%d\n", lblendif);]]
* 
*/

/**/size_t loopcount = 1;/**/

void
ifstmt(void)
{
	/**/size_t lblelse, lblendif;/**/
	/**/printf("# IF-clause:\n");/**/
    match(IF);
    expr(); /**///printf("\t<expr>.as\n");/**/
    match(THEN);
    
    /**/printf("\tjz .L%d\n", lblendif=lblelse=loopcount++);/**/
    
    stmt();  /**///printf("\t<stmt>.as\n");/**/
    if (lookahead == ELSE) {
        match(ELSE);
        
	/**/printf("\tjmp .L%d\n", lblendif=loopcount++);/**/
	/**/printf(".L%d\n", lblelse);/**/
        
        stmt();  /**///printf("\t<stmt>.as\n");/**/
    }
    
    /**/printf(".L%d\n", lblendif);/**/
    
	/**/printf("# END IF-clause\n");/**/
    
}
/************************* Inspects WHILE-DO statement structure **************************
*	whlstmt  -> WHILE expr DO stmt
* 
*	Semantic rule for while-statement: 
*
* 	whilestmt -> WHILE expr DO stmt
* 
* 	will be translated to
* 
* 	.L<whlhead>:
* 				<expr>.as
* 				gofalse .L<whltail>
* 				<stmt>.as
* 				goto .L<whlhead>
* 				
*	.L<whltail>: 
*/

void whlstmt(void)
{
 /**/size_t whlhead, whltail; /**/
 /**/printf("# WHILE-clause:\n");/**/
 match(WHILE);
 /**/printf(".L%d\n", whlhead = loopcount++);/**/
 expr();  /**///printf("\t<expr>.as\n");/**/
 /**/printf("\tjz .L%d\n", whltail=loopcount++);/**/
 match(DO);
 stmt();  /**///printf("\t<stmt>.as\n");/**/
 /**/printf("\tjmp .L%d\n", whlhead);/**/
 /**/printf(".L%d\n", whltail);/**/
 /**/printf("# END WHILE-clause\n");/**/
 
}
/************************* Inspects REPEAT-UNTIL statement structure **************************
*repstmt  -> REPEAT stmtlist UNTIL expr
* 
* Semantic rule for repeat-until: 
* 
* REPEAT stmtlist UNTIL expr
* 
* will be translated to
* 
* .L<replbl>:
* 				<stmtlist>.as
* 				<expr>.as
* 				jz .L<replbl>
* 
*/
void repstmt(void)
{
  /**/size_t replbl = loopcount++;/**/
  match(REPEAT); /**/printf("# REPEAT-clause:\n");/**/
  /**/printf(".L%d\n", replbl);/**/
  stmtlist();  /**///printf("<stmtlist>.as");/**/
  match(UNTIL); 
  /**/printf("# UNTIL");/**/
  expr();   /**///printf("<expr>.as");/**/
  /**/printf("\tjz .L%d\n", replbl);/**/

}

/********************************
 *  Imperative scope ends here
 * /

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
    int relop;
    smpexpr();
    if ((relop = isrelop())) {
        push("");
        smpexpr();
        execrelop("", relop);
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
    /**/int isotimes = 0, isoplus = 0;/**/
    switch (lookahead) {
    case '+':
        match('+');
        break;
    case '-':
    case NOT:
        /**/ isnot = lookahead;/**/
        match(lookahead);
        match(NOT);
        break;
    }

  T_entry:

  F_entry:

    factor();

	switch (isneg) {
	case '-':
		neg("");
		break;
	case NOT:
		notb();
		break;
	}; isneg = 0;
    /**/  isneg=0; /**/
    /**/ isnot=0; /**/
    /**/switch (isotimes){
            case AND: 
                andb(); 
            break;
            case DIV:
                idiv("l");
            break;
            case MOD:
                mod("l");
            break;
            case '/':
                idiv("");
            break;
            case '*':
                imul("");
            break;
        }
    /**/

    /**/switch (isoplus){
                case OR: 
                    orb(); 
                break;
                case DIV:
                    sub("l");
                break;
                case MOD:
                    mod("l");
                break;
                case '/':
                    idiv("");
                break;
                case '*':
                    imul("");
				case '+':
                    add("l");
                break;
            }
        /**/

    isotimes = 0;
    isoplus = 0;
        

    if (lookahead == '*' ||
        lookahead == '/' ||
        lookahead == DIV ||
        lookahead == NOT ||
        lookahead == AND) {
        
        /**/otimes = lookahead;/**/
        /**/push(""); /**/
        match(lookahead);
        goto F_entry;
    }

    if (lookahead == '+' || lookahead == '-') {
        /**/oplus = lookahead;/**/
        push("");
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
    } 
    
    else if(lookahead == '('){
		match('(');
		exprlist();
		match(')');
    }
    
    else { 
		; 
	}
}

/********************************************************************
 *  exprlist -> expr { ',' expr }
 */

void exprlist(void){
	
	_expr_start:
	expr();
	if (lookahead == ',') {
		match(',');
		goto _expr_start;
	
	}
}