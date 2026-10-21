int global_int ;
 float global_float ;
 char global_char ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 3 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 int a = 1 , b = 2 ;
 int c = 3 ;
 char pp = local_char + 2 ;
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 local_char -= pp ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 - ( ( int ) ( ( ( ( short int ) ( d ) ) > ( ( short int ) ( d - e ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( d ) ) * ( ( double ) ( d - e ) ) ) ) ) + 56 + ((int)((((unsigned short int)(d - e + 56 - ( ( int ) ( ( ( ( short int ) ( d ) ) > ( ( short int ) ( d - e ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( d ) ) * ( ( double ) ( d - e ) ) ) ) ) )) & ((unsigned short int)(d))))) + ((int)((((short int)(c)) - ((short int)(n))))) ;
 c ^= j ;
 if ( e > 20 ) {
 i = i + 10 + 56 - ( ( int ) ( ( ( ( double ) ( k ) ) - ( ( double ) ( i + 10 ) ) ) ) ) - ( ( int ) ( ( ( ( int ) ( k ) ) - ( ( int ) ( i + 10 ) ) ) ) ) + 56 + ((int)((((unsigned long long int)(c)) - ((unsigned long long int)(j))))) + ((int)((((unsigned long long int)(c)) & ((unsigned long long int)(i ))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = ( k * 1 ) + step + 56 * ((int)((((float)(j)) * ((float)(( k * 1 ) + step ))))) ;
 }
 local_int = k ^ c + 56 + ( ( int ) ( ( ( ( short ) ( local_int ) ) ^ ( ( short ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( k ^ c ) ) + ( ( long long int ) ( local_int ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( float ) ( k ^ c + 56 + ( ( int ) ( ( ( ( short ) ( local_int ) ) ^ ( ( short ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( k ^ c ) ) + ( ( long long int ) ( local_int ) ) ) ) ) ) ) + ( ( float ) ( k ^ c + 56 + ( ( int ) ( ( ( ( short ) ( local_int ) ) ^ ( ( short ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( k ^ c ) ) + ( ( long long int ) ( local_int ) ) ) ) ) ) ) ) ) ) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
