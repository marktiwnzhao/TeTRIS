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
 int d = 3 , e = 2 , f = local_int , g = 2 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp + 56 + ( ( int ) ( ( ( ( double ) ( d ) ) - ( ( double ) ( c + pp ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( d ) ) + ( ( float ) ( h ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( d ) ) - ( ( long long int ) ( c + pp ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( long long int ) ( k ) ) >= ( ( long long int ) ( c + pp + 56 + ( ( int ) ( ( ( ( double ) ( d ) ) - ( ( double ) ( c + pp ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( d ) ) + ( ( float ) ( h ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( d ) ) - ( ( long long int ) ( c + pp ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( short ) ( i ) ) < ( ( short ) ( k ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( i ) ) + ( ( float ) ( k ) ) ) ) ) ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( d ) ) <= ( ( unsigned long long int ) ( d - e ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( long long int ) ( d ) ) & ( ( long long int ) ( b ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( d ) ) & ( ( unsigned long long int ) ( b ) ) ) ) ) ;
 c ^= j + 56 * ( ( int ) ( ( ( ( double ) ( l ) ) - ( ( double ) ( j ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( l ) ) > ( ( unsigned long long int ) ( b ) ) ) ) ) + ( ( int ) ( ( ( ( int ) ( l ) ) + ( ( int ) ( b ) ) ) ) ) + 56 * ((int)((((unsigned long long int)(c )) < ((unsigned long long int)(j + 56 * ( ( int ) ( ( ( ( double ) ( l ) ) - ( ( double ) ( j ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( l ) ) > ( ( unsigned long long int ) ( b ) ) ) ) ) + ( ( int ) ( ( ( ( int ) ( l ) ) + ( ( int ) ( b ) ) ) ) ) ))))) - ((int)((((float)(c )) - ((float)(c ))))) - ((int)((((float)(c )) - ((float)(c ))))) ;
 if ( e > 20 ) {
 i = i + 10 + 56 + ( ( int ) ( ( ( ( float ) ( i ) ) - ( ( float ) ( i + 10 ) ) ) ) ) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = c ^= c + 56 - ((int)((((short)(j)) ^ ((short)(c ^= c ))))) - ((int)((((double)(j)) * ((double)(c ^= c ))))) ;
 }
 local_int = k ^ c + 56 + ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( k ^ c ) ) ) ) ) + 56 - ((int)((((short int)(h)) * ((short int)(k ^ c + 56 + ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( k ^ c ) ) ) ) ) ))))) - ((int)((((int)(h)) & ((int)(k ^ c + 56 + ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( k ^ c ) ) ) ) ) ))))) * ((int)((((unsigned short int)(h)) & ((unsigned short int)(k ^ c + 56 + ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( k ^ c ) ) ) ) ) ))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
