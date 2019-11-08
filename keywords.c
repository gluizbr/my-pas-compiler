//
// Created by gabriel on 29/10/2019.
//

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
    "end",
};

/*******************************************************************************
iskeywords:
        * Function that verify 
********************************************************************************/
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
