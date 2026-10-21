#include <stdio.h>
int probe(void) { float f = -1.0f; char x = (char)f; return x; }
int main(void) { printf("%d\n",probe()); }
