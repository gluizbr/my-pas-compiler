#include <stdlib.h>
#include <stdio.h>
#include "include/lexer.h"
#include <string.h>
#include "include/vmachine.h"

double vmem[MEMSIZE];
int vpm = 1;
char symtb[MEMSIZE][MAXIDLEN + 1];


int symtab_lookup(char *symbol) {
  //
  //percorre toda a tabela pra ver se existe o valor do symbol se existir retorna a posição i
  int i;
  for (i = 0; i < vpm; i++) {
    if (strcmp(symbol, symtb[i]) == 0) return i;
  }
  // percorre tudo procurando o valor
  return -1;
}

int symtb_add(char *symbol) {
  int i = symtab_lookup(symbol);

  if (i == -1) {
    strcpy(symtb[vpm], symbol);
    vpm = vpm + 1;
    return vpm - 1;
  }
  return i;
}

double execop(double arg1, double arg2, int op) {
  // printf("\n execop %f %f %c \n", arg1, arg2, op);
  switch (op) {
    case '+':
      // printf("\n sum %f \n", (arg1 + arg2));
      return (arg1 + arg2);
      break;
    case '-':
      // printf("\n sub %f \n", (arg1 - arg2));
      return (arg1 - arg2);
      break;
    case '*':
      // printf("\n mult %f \n", (arg1 * arg2));
      return (arg1 * arg2);
      break;
    case '/':
      if (arg2) {
        // printf("\n div %f \n", (arg1 / arg2));
        return (arg1 / arg2);
      }
      fprintf(stderr, "mybc: divison by zero.. exiting");
      return (-6);
  }
}

double acc = 0;

void push(double arg) {
  acc = arg;
}

double pop(void) {
  return acc;
}
