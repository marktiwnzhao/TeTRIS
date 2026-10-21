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
 int d = 3 , e = 3 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp + 56 + ( ( int ) ( ( ( ( double ) ( d ) ) - ( ( double ) ( c + pp ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( d ) ) + ( ( float ) ( h ) ) ) ) ) - ( ( int ) ( ( ( ( long long int ) ( d ) ) - ( ( long long int ) ( c + pp ) ) ) ) ) ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( d ) ) <= ( ( unsigned long long int ) ( d - e ) ) ) ) ) + 56 - ((int)((((float)(d )) + ((float)(local_int))))) ;
 c ^= j + 56 * ( ( int ) ( ( ( ( double ) ( l ) ) - ( ( double ) ( j ) ) ) ) ) ;
 if ( e > 20 ) {
 i = i + 10 + 56 + ( ( int ) ( ( ( ( float ) ( i ) ) - ( ( float ) ( i + 10 ) ) ) ) ) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = ( k * 1 ) + step + 56 - ((int)((((short int)(k )) - ((short int)(c))))) - ((int)((((double)(k )) + ((double)(k ))))) ;
 }
 local_int = k ^ c ;
 set_var(local_int,local_float,local_char);
return global_char -= pp ;
 }
 
int main(){func1();return 0;}
