#include <stdio.h>
int g; int f(void) { g = 60; return 0; }
int main(void) { int r=f(); g += 56 - g + r; printf("%d\n",g); return 0; }
