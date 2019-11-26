#define MEMSIZE 1024*1024
#define STACKSIZE 16384

double execop(double arg1, double arg2, int op);
void push(double arg);
double pop(void);
extern double acc;
