/*
 * @<keywords.c>::
 * Authors:
 * Grupo 1
 * Gabriel Luiz
 * Leonardo Cabbau
 * Valeria
 */

/*****@<keywords.h>::****/
#include "include/keywords.h"

/*******************************************************************************
keyword:
        *Array of keywords that simulates a dictionary for the words used.
********************************************************************************/
char *keyword[] = {
    "begin",
    "program",
    "var",
    "integer",
    "long",
    "real",
    "double",
    "boolean",
    "char",
    "string",
    "procedure",
    "function",
    "if",
    "then",
    "else",
    "while",
    "do",
    "repeat",
    "until",
    "or",
    "div",
    "mod",
    "and",
    "false",
    "true",
    "not",
    "end",
};

/*
 * @ iskeywords:: 
 */
/**************************************************************************************
iskeywords:
            *Function made for return the token value passed by the lexeme
            *Used by int isID(FILE *tape){};
****************************************************************************************/
#include <string.h>
int iskeywords(char * name){
  int token;
  for (token = BEGIN; token < END + 1; token++){
    if((strcmp(keyword[token-BEGIN], name)) == 0){
      return token;
    }
  }
  return 0;
}
