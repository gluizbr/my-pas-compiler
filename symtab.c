//
// Created by gabriel on 05/11/2019.
//
/****@<symtab.c>::*****/

#include "include/symtab.h"
#include "include/lexer.h"
#include <string.h>

SYMTAB symtab[MAXSYMTAB];
/* symtab_lookup looks up for a given symbol and, if existing returns the current symbol descriptor*/
int symtab_lookup(char *symbol) {
  int stbd;
  for (stbd = symtab_descriptor - 1; stbd > 0; stbd--){
    if(strcmp(symbol, symtab[stbd].name) == 0){
      break;
    }
  }
  return stbd;
}

/* symtab_append does apend a given symbol just defined in declarative scope */
//type_descriptor will be updated to more complex structure
int symtab_append(char *symbol) {
  strcpy(symtab[symtab_descriptor].name, symbol);
  return symtab_descriptor++;
}

int symtab_descriptor;

int symtab_type_range(int type_descriptor) {
  return 0;
}