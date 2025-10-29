int global_int;
float global_float;
char global_char;


int func1() {
    int local_int = 2;
    float local_float = 1.0;
    char local_char = 1;
	int a = 1, b = 2;
	int c = 3;
    char pp= local_char + 2 ;
    int d=3, e=2, f=local_int, g=3, h=1, i=local_float ,j=local_float , k= -1, l=1, m=1, n=1, o=1, p=1;

    d += c + pp;
    for ( ; a < h; a++) {
        a++;
        d = d - e;
        c ^= j;
        if (e > 20) {
            i = i + 10;
            c++;
        }
    }
    
    while (b--) {
       int step =1;
        k = ( k * 1 ) + step;
    }

    local_int = k ^ c ;

	return 0;
}
