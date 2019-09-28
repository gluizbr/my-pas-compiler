/*
 * @<main.c>::
 */

/** system include libraries **/
#include <stdio.h>
#include <tokens.h>
#include <ctype.h>

FILE *source;

/** NOTE: compile with -I. directive flag **/
#define UINT 1024
#define FLOAT 1025
#define EE 1026
#define HIST_SIZE 10

/*
 * REGEX:
 *          UINT = [1-9][0-9]* | 0
 *          FRAC = UINT'.'[0-9]* | '.'[0-9]+
 *          EE = [eE][\+\-]?[0-9]+
 *          FLT = UINT EE | FRAC EE?
 */
int isNum(FILE *tape) {
    int token;
    int head = getc(tape);
    if (isdigit(head) && head != '0') {
        token = UINT;
        while(isdigit(head = getc(tape)));
        if (head == '.') {
            while(isdigit(head = getc(tape)));
            ungetc(head, tape);
            token = FLOAT;
            return token;
        }
        ungetc(head, tape);
        return token;
    } else if (head == '.') {
        head = getc(tape);
        if (isdigit(head)) {
            while(isdigit(head = getc(tape)));
            token = FLOAT;
            ungetc(head, tape);
            return token;
        }
        ungetc(head, tape);
        ungetc('.', tape);
        return 0;
    }
    ungetc(head, tape);
    return 0;
}

int isEE(FILE * tape){
    int i = 0;
    int head[HIST_SIZE];
    head[i] = getc(tape);
    if (toupper(head[i]) == 'E') {
        i++;
        head[i] = getc(tape);
        if ((head[i] == '+') || (head[i] == '-')) {
            i++;
        } else {
            ungetc(head[i], tape); 
        }
        head[i] = getc(tape);
        if (isdigit(head[i])) {
            while(isdigit(head[i++] = getc(tape)));
            ungetc(head[i], tape);
            head[i] = 0;
            return EE;
        }
        for ( ; i > 0 ; i--) {
            ungetc(head[i], tape);
        }
    }
    ungetc(head[0], tape);
    return 0;
}

int isFLOAT(FILE * tape) {
    int num = 0, ee = 0;
    num = isNum(tape);
    if (num == 0) {
        printf("NaN \n");
        return 0;
    }
    if((ee = isEE(tape)) > 0) {
        printf("EE \n");
        return ee;
    }
    printf("UINT \n");
    return num;
}

int
main(int argc, char *argv[])
{
    source = stdin;
    printf(" \n The returns is %d \n", isFLOAT(source));
    return 0;
}
