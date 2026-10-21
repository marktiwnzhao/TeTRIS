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
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d += d - e + 56 * ( ( int ) ( ( ( ( float ) ( d ) ) + ( ( float ) ( d ) ) ) ) ) + ( ( int ) ( ( ( ( float ) ( d ) ) * ( ( float ) ( d ) ) ) ) ) ;
 global_float += local_float + 56 - ( ( float ) ( ( ( ( unsigned long long int ) ( global_float ) ) < ( ( unsigned long long int ) ( global_float ) ) ) ) ) ;
 if ( e > 20 ) {
 i = i + 10 + 56 * ( ( int ) ( ( ( ( double ) ( i ) ) + ( ( double ) ( i + 10 ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( double ) ( a ) ) + ( ( double ) ( i + 10 + 56 * ( ( int ) ( ( ( ( double ) ( i ) ) + ( ( double ) ( i + 10 ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( short int ) ( c ) ) <= ( ( short int ) ( local_int ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( a ) ) * ( ( float ) ( a ) ) ) ) ) + 56 + ((int)((((short int)(i )) < ((short int)(k))))) + ((int)((((unsigned long long int)(c)) > ((unsigned long long int)(j))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 k = ( k * 1 ) + step + 56 + ( ( int ) ( ( ( ( float ) ( k ) ) - ( ( float ) ( ( k * 1 ) + step ) ) ) ) ) + ( ( int ) ( ( ( ( unsigned short int ) ( g ) ) + ( ( unsigned short int ) ( g ) ) ) ) ) - ( ( int ) ( ( ( ( float ) ( k ) ) - ( ( float ) ( g ) ) ) ) ) ;
 }
 local_char = (-local_char) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
