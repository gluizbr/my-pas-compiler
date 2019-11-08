/*
 * @<main.c>::
 * Authors:
 * Grupo 1
 * Gabriel Luiz
 * Leonardo Cabbau
 */

/** system include libraries **/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/** NOTE: compile with -I. directive flag **/
#include "include/parser.h"

void mypas(void);

FILE *source,
    *object;

int
main(int argc, char *argv[]) {

  if (argc > 1) {
    char *ext = strrchr(argv[1], '.');
    if (!ext) {
      return 0;
    } else {
      if(((strcmp(ext, ".mypas")) == 0) || ((strcmp(ext, ".pas"))) == 0){
        source = fopen(argv[1], "r");
        if (source == NULL) {
          exit(-2);
        }
      } else {
        printf("Invalid extension. Only valid .mypas and .pas extensions");
        return 0;
      }
    }

  } else {
    source = stdin;
  }
  object = stdout;

  lookahead = gettoken(source);

  mypas();

  return 0;
}
