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
#include "include/lexer.h"
#include "include/tokens.h"
#include "include/keywords.h"
#include <string.h>

#define HIST_SIZE 10

int linenumber = 1;

/*
 * @ skipspaces:: 
 */
void
skipspaces(FILE *tape) {
  int head;

  while (isspace(head = getc(tape))) {
    if (head == '\n') {
      linenumber++;
    }
  }

  ungetc(head, tape);

}

/*
 * @ isID:: 
 */
char lexeme[MAXIDLEN + 1];

/*************
ID = [A-Za-z][A-Za-z0-9_]*
**************/
int
isID(FILE *tape) {

  token_t token;
  if (isalpha(lexeme[0] = getc(tape))) {
    int i;
    for (i = 1; (isalnum(lexeme[i] = getc(tape)) || lexeme[i] == '_'); (i < MAXIDLEN) && i++);
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    if (token = iskeywords(lexeme)) return token;
    return ID;
  }

  ungetc(lexeme[0], tape);
  return 0;
}

//.mypas <<erro
//.mypas <<mostrar


/*
 * REGEX:
 *          UINT = [1-9][0-9]* | 0
 *          FRAC = UINT'.'[0-9]* | '.'[0-9]+
 *          EE = [eE][\+\-]?[0-9]+
 *          FLT = UINT EE | FRAC EE?
 */
int isNum(FILE *tape, int *inum) {
  int token;
  lexeme[*inum] = getc(tape);
  if (isdigit(lexeme[*inum])) {
    token = UINT;
    if (lexeme[*inum] == '0') {
      (*inum)++;
      lexeme[*inum] = getc(tape);
    } else {
      (*inum)++;
      while (isdigit(lexeme[*inum] = getc(tape))) { (*inum)++; };
    }
    if (lexeme[*inum] == '.') {
      (*inum)++;
      while (isdigit(lexeme[*inum] = getc(tape))) { (*inum)++; };
      ungetc(lexeme[*inum], tape);
      token = FLT;
      lexeme[*inum] = 0;
      return token;
    }
    ungetc(lexeme[*inum], tape);
    lexeme[*inum] = 0;
    return token;
  } else if (lexeme[*inum] == '.') {
    (*inum)++;
    lexeme[*inum] = getc(tape);
    if (isdigit(lexeme[*inum])) {
      (*inum)++;
      while (isdigit(lexeme[*inum] = getc(tape))) { (*inum)++; };
      token = FLT;
      ungetc(lexeme[*inum], tape);
      lexeme[*inum] = 0;
      return token;
    }
    ungetc(lexeme[*inum], tape);
    ungetc('.', tape);
    return 0;
  }
  ungetc(lexeme[*inum], tape);
  return 0;
}

int isEE(FILE *tape, int *inum) {
  int firstnum = *inum;
  lexeme[*inum] = getc(tape);
  if (toupper(lexeme[*inum]) == 'E') {
    (*inum)++;
    lexeme[*inum] = getc(tape);
    if ((lexeme[*inum] == '+') || (lexeme[*inum] == '-')) {
      (*inum)++;
    } else {
      ungetc(lexeme[*inum], tape);
    }
    lexeme[*inum] = getc(tape);
    if (isdigit(lexeme[*inum])) {
      (*inum)++;
      while (isdigit(lexeme[*inum] = getc(tape))) { (*inum)++; };
      ungetc(lexeme[*inum], tape);
      lexeme[*inum] = 0;
      return FLT;
    }
    for (; *inum >= firstnum; *inum--) {
      ungetc(lexeme[*inum], tape);
    }
  }
  ungetc(lexeme[firstnum], tape);
  lexeme[firstnum] = 0;
  return 0;
}

int isREAL(FILE *tape) {
  int num = 0, ee = 0, inum = 0;
  num = isNum(tape, &inum);
  if (num == 0) {
    return 0;
  }
  if ((ee = isEE(tape, &inum)) > 0) {
    return ee;
  }
  return num;
}

int isASGN(FILE *tape) {
  lexeme[0] = getc(tape);
  if (lexeme[0] == ':') {
    if ((lexeme[1] = getc(tape)) == '=') {
      return ASSGN;
    }
  }
  ungetc(lexeme[0], tape);
  return 0;
}

/****************
Implemenntado no analisador lexico: isRELOP(){};
RELOP = "<"" | NEQ | LEQ | "=" | GEQ | ">"
NEQ = "<>"
LEQ = "<="
GEQ = ">="
****************/
token_t isRELOP(FILE *tape) {
  lexeme[2] = lexeme[1] = 0;
  switch (lexeme[0] = getc(tape)) {
    case '<':
      lexeme[1] = getc(tape);
      if (lexeme[1] == '=') return LEQ;
      if (lexeme[1] == '>') return NEQ;
      ungetc(lexeme[1], tape);
      lexeme[1] = 0;
      return lexeme[0];
    case '>':
      lexeme[1] = getc(tape);
      if (lexeme[1] == '=') return GEQ;
      ungetc(lexeme[1], tape);
      lexeme[1] = 0;
      return lexeme[0];
  };
  ungetc(lexeme[0], tape);
}

/*******************
STR = \"CHR*\"
**************/
int isSTR(FILE *tape) {
  int i = 1;
  lexeme[0] = getc(tape);
  if (lexeme[0] == '\"') {
    while ((lexeme[i] = getc(tape)) != '\"') {
      i++;
      if (lexeme[i] == EOF) {
        return EOF;
      }
    }
    lexeme[i] = 0;
    return STR;
  }
  ungetc(lexeme[0], tape);
  return 0;
}

/*******************
CHR = \'[0x00-0xFF]\' (ASCII)
**************/
int isCHR(FILE *tape) {
  int i = 1;
  lexeme[0] = getc(tape);
  if (lexeme[0] == '\'') {
    lexeme[i] = getc(tape);
    if ((lexeme[i] = getc(tape)) == '\'') {
      i++;
      lexeme[i] = 0;
      return CHR;
    }
    ungetc(lexeme[1], tape);
    ungetc(lexeme[0], tape);
    return 0;
  }
  ungetc(lexeme[0], tape);
  return 0;
}


/*
 * lexer to parser interface: @ gettoken:: 
 */

int
gettoken(FILE *source) {
  int token;

  /*
   * ignore left spaces
   */
  skipspaces(source);

  /*
   * lexical analysers are called hereafter:
   */

  if (token = isID(source))
    return token;
  if (token = isREAL(source))
    return token;
  if (token = isASGN(source))
    return token;
  if (token = isRELOP(source))
    return token;
  if (token = isCHR(source))
    return token;
  if (token = isSTR(source))
    return token;

  /*
   * return default token, say an ASCII value
   */
  return getc(source);
}
