/*
 * @<lexer.c>::
 */

/*******************************************************
* Author: Eraldo Pereira Marinho, PhD
* Creation Date: Aug 13 19:20:59 -03 2019
********************************************************/

/*
 * Módulo para implementação de métodos de varredura léxica
 * *************************************************
 */

#include <stdio.h>
#include <ctype.h>
#include <lexer.h>
#include <tokens.h>
#define HIST_SIZE 10

/*
 * @ skipspaces:: 
 */
void
skipspaces(FILE * tape)
{
    int             head;

    while (isspace(head = getc(tape)));

    ungetc(head, tape);

}

/*
 * @ isID:: 
 */
//TODO
//change all head variable to use lexeme need to use 
char lexeme[MAXIDLEN+1];
int
isID(FILE * tape)
{
    if (isalpha(lexeme[0] = getc(tape))) {
        int i;
        for (i = 1 ; isalnum(lexeme[i] = getc(tape)) ; (i < MAXIDLEN) && i++);
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return ID;
    }

    ungetc(lexeme[0], tape);
    return 0;
}

/*
 * REGEX:
 *          UINT = [1-9][0-9]* | 0
 *          FRAC = UINT'.'[0-9]* | '.'[0-9]+
 *          EE = [eE][\+\-]?[0-9]+
 *          FLT = UINT EE | FRAC EE?
 */
int isNum(FILE *tape) {
    int token;
    int head = getc(tape);
    if (isdigit(head)) {
        token = UINT;
        if(head == '0') { head = getc(tape);}
        else {
            while(isdigit(head = getc(tape)));
        }
        if (head == '.') {
            while(isdigit(head = getc(tape)));
            ungetc(head, tape);
            token = FLT;
            return token;
        }
        ungetc(head, tape);
        return token;
    } else if (head == '.') {
        head = getc(tape);
        if (isdigit(head)) {
            while(isdigit(head = getc(tape)));
            token = FLT;
            ungetc(head, tape);
            return token;
        }
        ungetc(head, tape);
        ungetc('.', tape);
        return 0;
    }
    ungetc(head, tape);
    return 0;
}

int isEE(FILE * tape){
    int i = 0;
    int head[HIST_SIZE];
    head[i] = getc(tape);
    if (toupper(head[i]) == 'E') {
        i++;
        head[i] = getc(tape);
        if ((head[i] == '+') || (head[i] == '-')) {
            i++;
        } else {
            ungetc(head[i], tape); 
        }
        head[i] = getc(tape);
        if (isdigit(head[i])) {
            while(isdigit(head[i++] = getc(tape)));
            ungetc(head[i], tape);
            head[i] = 0;
            return FLT;
        }
        for ( ; i > 0 ; i--) {
            ungetc(head[i], tape);
        }
    }
    ungetc(head[0], tape);
    return 0;
}

int isFLOAT(FILE * tape) {
    int num = 0, ee = 0;
    num = isNum(tape);
    if (num == 0) {
        return 0;
    }
    if((ee = isEE(tape)) > 0) {
        return ee;
    }
    return num;
}

int isASGN(FILE *tape)
{
	int head = getc(tape);

	if (head == ':') {
		if ( (head = getc(tape)) == '=' ) {
			return ASGN;
		}
	}
	ungetc(head, tape);
	return 0;
}


/*
 * lexer to parser interface: @ gettoken:: 
 */

int
gettoken(FILE * source)
{
    int             token;

    /*
     * ignore left spaces 
     */
    skipspaces(source);

    /*
     * lexical analysers are called hereafter: 
     */   

    if (token = isID(source))
        return token;
    if (token = isFLOAT(source))
        return token;
    if (token = isASGN(source))
        return token;

    /*
     * return default token, say an ASCII value 
     */
    return getc(source);
}
