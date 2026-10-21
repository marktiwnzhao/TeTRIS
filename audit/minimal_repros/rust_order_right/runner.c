#include <stdio.h>
int g; int f(void) { g = 60; return 0; }
int main(void) { int rhs=56-g; int r=f(); g=g+rhs+r; printf("%d\n",g); return 0; }
