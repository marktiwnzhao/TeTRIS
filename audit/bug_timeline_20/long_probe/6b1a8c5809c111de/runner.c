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
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 2 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp + 56 - ( ( int ) ( ( ( ( double ) ( p ) ) - ( ( double ) ( c + pp ) ) ) ) ) + 56 + ((int)((((long long int)(d )) > ((long long int)(d ))))) * ((int)((((float)(i)) - ((float)(d ))))) ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 - ( ( int ) ( ( ( ( short int ) ( d ) ) > ( ( short int ) ( d - e ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( d ) ) * ( ( double ) ( d - e ) ) ) ) ) ;
 c ^= j + 56 + ( ( int ) ( ( ( ( float ) ( e ) ) * ( ( float ) ( j ) ) ) ) ) + 56 - ((int)((((short int)(c )) * ((short int)(c ))))) * ((int)((((short int)(j + 56 + ( ( int ) ( ( ( ( float ) ( e ) ) * ( ( float ) ( j ) ) ) ) ) )) < ((short int)(c ))))) * ((int)((((long long int)(j + 56 + ( ( int ) ( ( ( ( float ) ( e ) ) * ( ( float ) ( j ) ) ) ) ) )) + ((long long int)(c ))))) ;
 switch ( e > 20 ) {
 i = i + 10 + 56 * ( ( int ) ( ( ( ( float ) ( o ) ) * ( ( float ) ( i + 10 ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( f ) ) + ( ( double ) ( i ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( i + 10 ) ) ^ ( ( short int ) ( i ) ) ) ) ) + 56 + ((int)((((long long int)(i )) < ((long long int)(o))))) + ((int)((((double)(i )) * ((double)(o))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = ( k * 1 ) + step + 56 + ( ( int ) ( ( ( ( long long int ) ( b ) ) >= ( ( long long int ) ( ( k * 1 ) + step ) ) ) ) ) + 56 + ((int)((((float)(k )) + ((float)(( k * 1 ) + step + 56 + ( ( int ) ( ( ( ( long long int ) ( b ) ) >= ( ( long long int ) ( ( k * 1 ) + step ) ) ) ) ) ))))) ;
 }
 local_int = k ^ c + 56 + ( ( int ) ( ( ( ( short ) ( local_int ) ) ^ ( ( short ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( k ^ c ) ) + ( ( long long int ) ( local_int ) ) ) ) ) + 56 + ((int)((((short)(local_int )) + ((short)(o))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
