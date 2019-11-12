/*****@<keywords.h>::****/
enum{
    BEGIN = 16384,
    PROGRAM,
    VAR,
    INTEGER,
    LONG,
    REAL,
    DOUBLE,
    BOOLEAN,
    CHAR,
    STRING,
    PROCEDURE,
    FUNCTION,
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    REPEAT,
    UNTIL,
    OR,
    DIV,
    MOD,
    AND,
    FALSE,
    TRUE,
    END,
};

/*
 * @ iskeywords:: 
 */
int iskeywords(char * name);
