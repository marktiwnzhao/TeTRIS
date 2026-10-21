int global_int ;
 float global_float ;
 char global_char ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 2 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 int a = 1 , b = 3 ;
 int c = 3 ;
 char pp = local_char + 2 ;
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 local_float /= global_float ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 * ( ( int ) ( ( ( ( int ) ( d ) ) + ( ( int ) ( d - e ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( d ) ) - ( ( float ) ( d - e ) ) ) ) ) ;
 c ^= j + 56 + ( ( int ) ( ( ( ( short ) ( k ) ) > ( ( short ) ( j ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( k ) ) * ( ( double ) ( c ) ) ) ) ) + 56 * ((int)((((double)(c )) * ((double)(j))))) - ((int)((((double)(c )) - ((double)(c ))))) ;
 if ( e > 20 ) {
 i = i + 10 + 56 * ( ( int ) ( ( ( ( short ) ( i ) ) * ( ( short ) ( i + 10 ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( i ) ) * ( ( double ) ( i + 10 ) ) ) ) ) + 56 + ((int)((((float)(i )) + ((float)(i + 10 + 56 * ( ( int ) ( ( ( ( short ) ( i ) ) * ( ( short ) ( i + 10 ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( i ) ) * ( ( double ) ( i + 10 ) ) ) ) ) ))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = ( k * 1 ) + step + 56 - ( ( int ) ( ( ( ( double ) ( k ) ) - ( ( double ) ( global_int ) ) ) ) ) + ( ( int ) ( ( ( ( float ) ( ( k * 1 ) + step ) ) * ( ( float ) ( c ) ) ) ) ) ;
 }
 local_int = k ^ c + 56 + ( ( int ) ( ( ( ( float ) ( f ) ) + ( ( float ) ( o ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( f ) ) - ( ( double ) ( o ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( k ^ c ) ) * ( ( double ) ( o ) ) ) ) ) + 56 * ((int)((((double)(local_int )) * ((double)(n))))) - ((int)((((long long int)(c)) ^ ((long long int)(c))))) + ((int)((((unsigned short int)(c)) + ((unsigned short int)(c))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
