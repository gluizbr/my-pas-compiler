/*
 * @<parser.h>::
 */
extern int      lookahead;
int             gettoken(FILE *);
void            expr(void);
void            rest(void);
void            quoc(void);
void            term(void);
void            fact(void);
void            match(int);
