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
#include <tokens.h>

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

int
isID(FILE * tape)
{
    int             head = getc(tape);

    if (isalpha(head)) {
        while (isalnum(head = getc(tape)));
        ungetc(head, tape);
        return ID;
    }

    ungetc(head, tape);
    return 0;
}

int
isDEC(FILE * tape)
{
    int             head = getc(tape);

    if (isdigit(head)) {
        if (head == '0') {
            ;
        } else {
            while (isdigit(head = getc(tape)));
            ungetc(head, tape);
        }
        return DEC;
    }
    ungetc(head, tape);
    return 0;
}

int isASGN(FILE *tape)
{
	int head = getc(tape);

	if (head == ':') {
		if ( (head = getc(tape)) == '=' ) {
			return ASGN;
		}
		ungetc(head, tape);
		ungetc(':', tape);
		return 0;
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
    if (token = isDEC(source))
        return token;
    if (token = isASGN(source))
        return token;

    /*
     * return default token, say an ASCII value 
     */
    return getc(source);
}
