/*
 * @<lexer.c>::
 */

/*******************************************************
* Authors:
* Grupo 1
* Gabriel Luiz
* Leonardo Cabbau
* Valeria
********************************************************/

/**************************************************
 
 * Module for implementing lexical scanning methods
 
 **************************************************/

#include <stdio.h>
#include <ctype.h>
#include "include/lexer.h"
#include "include/tokens.h"
#include "include/keywords.h"
#include <string.h>

#define HIST_SIZE 10

/*******************************************************************************
linenumber:
            *Variable declared for the couting of the lines.
            *used in the function 'skipspaces()'
*****************************************************************************/
int linenumber = 1;

/*
 * @ skipspaces:: 
 */
/*******************************************************************************
skipspaces:
            *Function made for jump white-space character
            *Counter the number of lines
*****************************************************************************/
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
 * @ skipcomments:: 
 */
/*******************************************************************************
skipcomments:
            *Function made ignore left spaces
*****************************************************************************/
void skipcomments(FILE *tape) {
  int head;
  _skipspaces:
  skipspaces(tape);
  if ((head = getc(tape)) == '{') {
    while ((head = getc(tape)) != '}') {
      if (head == EOF) {
        return;
      }
    };
    goto _skipspaces;
  }
  ungetc(head, tape);
}

char lexeme[MAXIDLEN + 1];

/*
 * @ isID:: 
 */
/**************************************************************************************
isID:
            *Function made for verify if the tape matches with the pattern of an identifier
            and returns if is an identifier as a token.
    Pattern:
            *ID = [A-Za-z][A-Za-z0-9_]*
****************************************************************************************/
int
isID(FILE *tape) {

  token_t token;
  lexeme[0] = getc(tape);
  if (isalpha(lexeme[0])) {
    int i;
    for (i = 1; (isalnum(lexeme[i] = getc(tape)) || lexeme[i] == '_'); (i < MAXIDLEN) && i++);
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    if (token = iskeywords(lexeme)){
      return token;
    }
    return ID;
  }

  ungetc(lexeme[0], tape);
  return 0;
}

/*
 * @ isNum:: 
 */
/**************************************************************************************
isNum:
            *Function made for verify if the tape matches with the pattern of an number
            and returns it as a token.   
    Pattern:
           UINT = [1-9][0-9]* | 0
           FRAC = UINT'.'[0-9]* | '.'[0-9]+
           FLT = UINT EE | FRAC EE?          
****************************************************************************************/
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

/*
 * @ isEE:: 
 */
/**************************************************************************************
isEE:
            *Function made for verify if the tape matches with the pattern of an exponencial number
            and returns it as a token.         
    Pattern:
           EE = [eE][\+\-]?[0-9]+  
****************************************************************************************/
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

/*
 * @ isREAL:: 
 */
/**************************************************************************************
isREAL:
            *Function made for verify if the tape matches with the pattern of an real number
            and returns it as a token.
    Pattern:
           UINT = [1-9][0-9]* | 0
           FRAC = UINT'.'[0-9]* | '.'[0-9]+
           FLT = UINT EE | FRAC EE?  
           EE = [eE][\+\-]?[0-9]+  
****************************************************************************************/
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

/*
 * @ isASGN:: 
 */
/**************************************************************************************
isASGN:
            *Function made for verify if the tape is an assigment and returns a token.
    Pattern:
           :=
****************************************************************************************/
int isASGN(FILE *tape) {
  lexeme[0] = getc(tape);
  if (lexeme[0] == ':') {
    if ((lexeme[1] = getc(tape)) == '=') {
      lexeme[2] = 0;
      return ASSGN;
    }
    ungetc(lexeme[1], tape);
  }
  ungetc(lexeme[0], tape);
  return 0;
}

/*
 * @ isRELOP:: 
 */
/**************************************************************************************
isRELOP:
            *Function made for verify if the tape matches for a specified pattern.
    Pattern:
            RELOP = "<"" | NEQ | LEQ | "=" | GEQ | ">"
            NEQ = "<>"
            LEQ = "<="
            GEQ = ">="
****************************************************************************************/
int isRELOP(FILE *tape) {
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
  return 0;
}

/*
 * @ isSTR:: 
 */
/**************************************************************************************
isSTR:
            *Function made for verify the pattern and returns the appropriate token.
    Pattern:
            STR = \"CHR*\"
****************************************************************************************/
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

/*
 * @ isSTR:: 
 */
/**************************************************************************************
isSTR:
            *Function made for verify a pattern and returns the specific token.
    Pattern:
            CHR = \'[0x00-0xFF]\' (ASCII)
****************************************************************************************/
int isCHR(FILE *tape) {
  int i = 1;
  lexeme[0] = getc(tape);
  if (lexeme[0] == '\'') {
    lexeme[i] = getc(tape);
    i++;
    if ((lexeme[i] = getc(tape)) == '\'') {
      i++;
      lexeme[i] = 0;
      return CHR;
    }
    ungetc(lexeme[i], tape);
    ungetc(lexeme[1], tape);
    ungetc(lexeme[0], tape);
    return 0;
  }
  ungetc(lexeme[0], tape);
  return 0;
}


/*
 * @ gettoken:: 
 */
/**************************************************************************************
gettoken:
            *Function made for return the token value
            *lexer to parser interface: @ gettoken::
            *ignore left spaces
****************************************************************************************/
int
gettoken(FILE *source) {
  int token;

  skipcomments(source);

  /*****************************************************************************************
   * lexical analysers are called hereafter:
   *****************************************************************************************/
  if (token = isID(source)) {
    return token;
  }
  if (token = isREAL(source)){
    return token;
  }
  if (token = isASGN(source)) {
    return token;
  }
  if (token = isRELOP(source)) {
    return token;
  }
  if (token = isCHR(source)) {
    return token;
  }
  if (token = isSTR(source)) {
    return token;
  }

  token = getc(source);
  /*****************************************************************************************
   * return default token, say an ASCII value
   *****************************************************************************************/

  return token;
}
