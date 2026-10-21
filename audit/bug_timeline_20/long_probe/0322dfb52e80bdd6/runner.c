int global_int ;
 float global_float ;
 char global_char ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 0 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 int a = 1 , b = 2 ;
 int c = 3 ;
 char pp = local_char + 2 ;
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 local_float /= global_float ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 * ( ( int ) ( ( ( ( short ) ( d ) ) > ( ( short ) ( d - e ) ) ) ) ) - ( ( int ) ( ( ( ( float ) ( global_int ) ) * ( ( float ) ( d ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( c ) ) > ( ( long long int ) ( d ) ) ) ) ) + 56 + ((int)((((double)(d )) + ((double)(d ))))) ;
 c ^= j + 56 - ((int)((((unsigned long long int)(e)) & ((unsigned long long int)(n))))) - ((int)((((double)(l)) + ((double)(n))))) ;
 if ( e > 20 ) {
 i = i + 10 + 56 + ((int)((((double)(b)) - ((double)(i + 10 ))))) * ((int)((((float)(h)) + ((float)(i + 10 ))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = ( k * 1 ) + step + 56 + ( ( int ) ( ( ( ( float ) ( c ) ) + ( ( float ) ( c ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( c ) ) + ( ( double ) ( c ) ) ) ) ) + ( ( int ) ( ( ( ( long long int ) ( c ) ) - ( ( long long int ) ( c ) ) ) ) ) ;
 }
 local_int = k ^ c + 56 * ((int)((((float)(local_int )) - ((float)(k ^ c ))))) + ((int)((((float)(local_int)) * ((float)(k ^ c ))))) - ((int)((((long long int)(e)) & ((long long int)(local_int))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
