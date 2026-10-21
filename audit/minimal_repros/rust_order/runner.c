#include <stdio.h>
int g; int f(void) { g = 60; return 0; }
int main(void) { g += 56 - g + f(); printf("%d\n",g); return 0; }
