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
 char pp = 1 ;
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp + 56 + ( ( int ) ( ( ( ( short ) ( d ) ) > ( ( short ) ( c + pp ) ) ) ) ) ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e + 56 - ( ( int ) ( ( ( ( float ) ( d ) ) - ( ( float ) ( d - e ) ) ) ) ) ;
 c ^= j + 56 * ( ( int ) ( ( ( ( float ) ( j ) ) - ( ( float ) ( j ) ) ) ) ) ;
 if ( e > 20 ) {
 i = i + 10 + 56 + ( ( int ) ( ( ( ( unsigned short int ) ( a ) ) >= ( ( unsigned short int ) ( i + 10 ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( local_int ) ) * ( ( double ) ( i + 10 ) ) ) ) ) - ( ( int ) ( ( ( ( short int ) ( local_int ) ) & ( ( short int ) ( local_int ) ) ) ) ) + 56 + ((int)((((double)(i )) * ((double)(global_int))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 local_float /= (local_float - (((global_float + global_float) + local_float) * local_float)) ;
 }
 local_int = k ^ c + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( k ^ c ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) | ( ( unsigned long long int ) ( k ^ c ) ) ) ) ) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
