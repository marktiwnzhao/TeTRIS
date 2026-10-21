#include <stdio.h>
char plain_char = -3;
signed char signed_char = -3;
unsigned char unsigned_char = (unsigned char)-3;
int main(void) {
  printf("%d:%d:%u\n", (int)plain_char, (int)signed_char, (unsigned)unsigned_char);
  return 0;
}
