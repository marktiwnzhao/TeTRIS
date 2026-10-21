int global_int ;
 float global_float ;
 char global_char ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 2 ;
 float local_float = 0 ;
 char local_char = 1 ;
 int a = 1 , b = 2 ;
 int c = 3 ;
 char pp = local_char + 2 ;
 int d = 3 , e = 2 , f = local_int , g = 3 , h = 1 , i = local_float , j = local_float , k = - 1 , l = 1 , m = 1 , n = 1 , o = 1 , p = 1 ;
 d += c + pp + 56 + ((int)((((long long int)(d )) >= ((long long int)(c + pp ))))) + ((int)((((float)(d )) + ((float)(c + pp ))))) ;
 for ( ;
 a < h ;
 a ++ ) {
 a ++ ;
 d = d - e ;
 c ^= j + 56 * ( ( int ) ( ( ( ( double ) ( c ) ) + ( ( double ) ( c ) ) ) ) ) ;
 if ( e > 20 ) {
 i = i + 10 + 56 - ( ( int ) ( ( ( ( unsigned long long int ) ( i ) ) * ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) + ( ( int ) ( ( ( ( unsigned short int ) ( i ) ) - ( ( unsigned short int ) ( i ) ) ) ) ) + 56 - ((int)((((float)(i )) - ((float)(i + 10 + 56 - ( ( int ) ( ( ( ( unsigned long long int ) ( i ) ) * ( ( unsigned long long int ) ( i + 10 ) ) ) ) ) + ( ( int ) ( ( ( ( unsigned short int ) ( i ) ) - ( ( unsigned short int ) ( i ) ) ) ) ) ))))) ;
 c ++ ;
 }
 }
 while ( b -- ) {
 int step = 1 ;
 local_char /= (~(-global_char)) ;
 }
 local_int = k ^ c + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( float ) ( local_int ) ) - ( ( float ) ( k ^ c ) ) ) ) ) - ( ( int ) ( ( ( ( int ) ( local_int ) ) ^ ( ( int ) ( k ^ c ) ) ) ) ) + 56 * ((int)((((unsigned long long int)(local_int )) + ((unsigned long long int)(k ^ c + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( float ) ( local_int ) ) - ( ( float ) ( k ^ c ) ) ) ) ) - ( ( int ) ( ( ( ( int ) ( local_int ) ) ^ ( ( int ) ( k ^ c ) ) ) ) ) ))))) * ((int)((((float)(local_int)) + ((float)(k ^ c + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( float ) ( local_int ) ) - ( ( float ) ( k ^ c ) ) ) ) ) - ( ( int ) ( ( ( ( int ) ( local_int ) ) ^ ( ( int ) ( k ^ c ) ) ) ) ) ))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
