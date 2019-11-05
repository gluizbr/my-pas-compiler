/*****@<keywords.h>::****/
// transformar tudo em string
enum{
    BEGIN = 16384,
    PROGRAM,
    VAR,
    INT,
    LONG,
    FLOAT,
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

int iskeywords(char * name);
