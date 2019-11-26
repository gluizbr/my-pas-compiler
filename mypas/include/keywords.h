/*****@<keywords.h>::****/
enum{
    BEGIN = 16384,
    PROGRAM,
    VAR,
    INTEGER, //1
    LONG, //2
    REAL, //3
    DOUBLE, //4
    BOOLEAN, //5
    CHAR, //6
    STRING,  //7
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
    NOT,
    END,
};

/*
 * @ iskeywords:: 
 */
int iskeywords(char * name);
