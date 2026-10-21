int f(void){int x=1; return *(int*)(x+1);}
int main(){return f();}
