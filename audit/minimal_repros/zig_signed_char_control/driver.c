#include <stdio.h>
int probe(void) { signed char x = -1; return x + 1; }
int main(void) { printf("%d\n",probe()); }
