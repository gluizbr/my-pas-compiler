/*
 * @<main.c>::
 */

/** system include libraries **/
#include <stdio.h>

/** NOTE: compile with -I. directive flag **/
#include <parser.h>
void mybc(void);

FILE           *source,
               *object;

int
main(int argc, char *argv[])
{
    source = stdin;

    object = stdout;

    lookahead = gettoken(source);

    mybc();

    return 0;

}
