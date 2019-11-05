/*
 * @<main.c>::
 * Authors:
 * Grupo 1
 * Gabriel Luiz
 * Leonardo Cabbau
 * Valeria
 */

/** system include libraries **/
#include <stdio.h>
#include <stdlib.h>
/** NOTE: compile with -I. directive flag **/
#include "include/parser.h"

void mypas(void);

FILE *source,
    *object;

int
main(int argc, char *argv[]) {

  if (argc > 1) {
    source = fopen(argv[1], "r");
    if (source == NULL) {
      exit(-2);
    }
  } else {
    source = stdin;
  }
  object = stdout;

  lookahead = gettoken(source);

  mypas();

  return 0;
}
