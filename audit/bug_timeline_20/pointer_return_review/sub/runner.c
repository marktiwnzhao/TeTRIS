int f(void){int x=1;int *p=&x; x += *p -= *p; return x;}
int main(){return (int)f();}
