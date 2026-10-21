int global_int ;
 float global_float ;
 char global_char ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 2 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 int a = 1 , b = 2 ;
 int c = 3 ;
 char pp = local_char + 2 ;
 int d = 3 , e = 2 , f = local_int , g = 0 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp + 56 + ( ( int ) ( ( ( ( short ) ( d ) ) | ( ( short ) ( c + pp ) ) ) ) ) - ( ( int ) ( ( ( ( short ) ( d ) ) * ( ( short ) ( c + pp ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned long long int ) ( d ) ) | ( ( unsigned long long int ) ( c + pp ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( unsigned long long int ) ( d ) ) & ( ( unsigned long long int ) ( c ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( d ) ) * ( ( double ) ( c ) ) ) ) ) ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 - ( ( int ) ( ( ( ( float ) ( c ) ) * ( ( float ) ( d - e ) ) ) ) ) * ( ( int ) ( ( ( ( long long int ) ( c ) ) - ( ( long long int ) ( d - e ) ) ) ) ) + 56 + ((int)((((float)(o)) + ((float)(d - e + 56 - ( ( int ) ( ( ( ( float ) ( c ) ) * ( ( float ) ( d - e ) ) ) ) ) * ( ( int ) ( ( ( ( long long int ) ( c ) ) - ( ( long long int ) ( d - e ) ) ) ) ) ))))) + ((int)((((short)(o)) & ((short)(g))))) ;
 local_float = global_float + 56 * ( ( float ) ( ( ( ( float ) ( local_float ) ) * ( ( float ) ( global_float ) ) ) ) ) + ( ( float ) ( ( ( ( int ) ( local_float ) ) < ( ( int ) ( local_float ) ) ) ) ) ;
 if ( e > 20 ) {
 i = i + 10 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( h ) ) > ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( unsigned long long int ) ( i ) ) * ( ( unsigned long long int ) ( i + 10 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( h ) ) > ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( i ) ) & ( ( long long int ) ( h ) ) ) ) ) + 56 * ((int)((((double)(p)) - ((double)(i + 10 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( h ) ) > ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( unsigned long long int ) ( i ) ) * ( ( unsigned long long int ) ( i + 10 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( h ) ) > ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( i ) ) & ( ( long long int ) ( h ) ) ) ) ) ))))) * ((int)((((float)(i)) + ((float)(i + 10 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( h ) ) > ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( unsigned long long int ) ( i ) ) * ( ( unsigned long long int ) ( i + 10 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( h ) ) > ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( i ) ) & ( ( long long int ) ( h ) ) ) ) ) ))))) * ((int)((((float)(p)) - ((float)(p))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = i ^= i + 56 + ((int)((((float)(e)) - ((float)(i ^= i ))))) ;
 }
 local_int = k ^ c + 56 * ( ( int ) ( ( ( ( float ) ( local_int ) ) + ( ( float ) ( k ^ c ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( local_int ) ) * ( ( double ) ( k ^ c ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( float ) ( d ) ) * ( ( float ) ( p ) ) ) ) ) + ( ( int ) ( ( ( ( short int ) ( local_int ) ) > ( ( short int ) ( p ) ) ) ) ) + 56 + ((int)((((float)(b)) + ((float)(k ^ c + 56 * ( ( int ) ( ( ( ( float ) ( local_int ) ) + ( ( float ) ( k ^ c ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( local_int ) ) * ( ( double ) ( k ^ c ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( float ) ( d ) ) * ( ( float ) ( p ) ) ) ) ) + ( ( int ) ( ( ( ( short int ) ( local_int ) ) > ( ( short int ) ( p ) ) ) ) ) ))))) + ((int)((((long long int)(b)) > ((long long int)(a))))) * ((int)((((short)(b)) ^ ((short)(a))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
