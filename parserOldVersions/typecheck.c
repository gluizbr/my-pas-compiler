#include <symtab.h>
#include <typecheck.h>

char * typecheck(char * var_name, char * acc)	// Compares var type with value type
{
	int vartype, acctype;

	Symbol varsymbol;
	int address = SymTab_lookup(var_name);

	if (address > -1)
		varsymbol = SymTab[address];
	else
	{
		;		// ERROR READING SYMTAB
	}

	vartype = varsymbol.type;

	if (acctype = typeID(acc))	// VERIFIES IF ACCUMULATOR HAS VARIABLE OR BOOLEAN 
	{
		if (acctype);
		else {
			int address = SymTab_lookup(acc);
			if (address > -1)
				acctype = SymTab[address].type;
			else
			{
				error(ESYMT);	//ERROR: COULD NOT FIND SYMBOL
			}
		}
	}
	else if (acctype = typeNUM(acc)); // VERIFIES IF ACCUMULATOR HAS NUMBER

	return flagcheck(vartype, acctype, acc);		
}

char * flagcheck(int* vartype, int acctype, char * acc)
{
	if ((acctype == UINT && vartype == INTEGER) ||
		((acctype == FLTP) && (vartype == DOUBLE || vartype == REAL || vartype == INTEGER)))//TYPE MATCH	
	{
		switch (acctype) {
		case UINT:

			switch (sizeof(atoi(acc)))
			{
			case 4:
				return "w";

			case 8:
				return "l";
			}
			break;

		case FLTP:
			if (vartype == DOUBLE)
				return "fl";
			else
				return "ql";

			break;
		}
	}
	else if (vartype == BOOLEAN)					//IF NOT ONE OF ABOVE, BOOLEAN TYPE
	{
		strcpy(acc, stack[stack_top--]);
		return "b";
	}
	
	error(ETYPE);	//ERROR: TYPE MISMATCH
	exit(-2);
}


token_t typeID(char * acc)
{
	token_t         keyword;
	if (isalpha(acc[0])) {
		int i;
		for (i = 1; isalnum(acc[i]) || acc[i] == '_'; i++);

		if ((keyword = iskeyword(acc)))
		{
			if (keyword == TRUE || keyword == FALSE)
				return BOOLEAN;
			else
				return 0;
		}

		return ID;
	}

	return 0;
}

/********** Check if next input is INTEGER or FLOAT
/*
 * NUM = UINT | FLTP
 * UINT = [1-9][0-9]* | 0
 * FLTP = ( UINT'.'FRAC? | '.'FRAC ) EE? | UINT EE
 *
 */

token_t
typeNUM(char * acc)
{
	token_t         token = 0;
	int             i;
	if (isdigit(acc[0])) {
		if (acc[0] == '0') {
			i = 1;
		}
		else {
			for (i = 1; isdigit(acc[i]); i++);
		}
		token = UINT;
		if ((acc[i]) == '.') {
			token = FLTP;
			i++;
			while (isdigit(acc[i])) i++;
		}
		else {
			;	// EPSILON TRANSITION IS SIMULATED HERE
		}
	}
	else if (acc[0] == '.') {
		if (isdigit(acc[1])) {
			i = 2;
			token = FLTP;
			while (isdigit(acc[i]))	i++;
		}
		else {
			; // EPSILON TRANSITION IS SIMULATED HERE 
		}
	}

	if (token) {
		i = typeEE(i, token, acc);
		return token;
	}
	// EPSILON TRANSITION IS SIMULATED HERE 
	return token;
}

int typeEE(int i, int token, char * acc) {
	int i0 = i;

	i = chktEE(i, acc);

	if (i > i0 && token == UINT) token = FLTP;

	return i;
}

int chktEE(int i, char * acc)
{
	int j = i;
	char next;

	if (acc[i] == 'E' || acc[i] == 'e') {
		next = acc[i + 1];
		j++;
		if (next == '+' || next == '-' || isdigit(next))
		{
			acc[j] = next;
			j++;
			while (isdigit(acc[j]))	j++;
		}
	}
	return j;
}