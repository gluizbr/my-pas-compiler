/*
 * @<main.c>::
 */

/** system include libraries **/
#include <stdio.h>

/** NOTE: compile with -I. directive flag **/
#include <parser.h>


FILE           *source,
               *object;

int
main(int argc, char *argv[])
{
    source = stdin;
    object = stdout;

    lookahead = gettoken(source);

    expr();

    return 0;

}
