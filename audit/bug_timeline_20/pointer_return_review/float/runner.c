float f(void){float x=1;float *p=&x; x += *p += *p; return x;}
int main(){return (int)f();}
