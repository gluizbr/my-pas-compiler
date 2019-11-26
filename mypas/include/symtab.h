//
// Created by gabriel on 05/11/2019.
//
/****@<symtab.h>::*****/
#pragma once
//#todo check MAXSYMTAB definition
//#define MAXSYMTAB 0x100000000
#define MAXSYMTAB 1000000
#include "lexer.h"

/* symtab_lookup looks up for a given symbol and, if existing returns the current symbol descriptor*/
int symtab_lookup(char * symbol);

/* symtab_append does apend a given symbol just defined in declarative scope */
//type_descriptor will be updated to more complex structure
int symtab_append(char * symbol);

extern int symtab_descriptor;

int symtab_type_range(int type_descriptor);

typedef struct __symtab__ {
    char name[MAXIDLEN];
    int typedescriptor;
} SYMTAB;

extern SYMTAB symtab[MAXSYMTAB];