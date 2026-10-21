int global_int ;
 float global_float ;
 char global_char ;
 int MAX = 13 ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 2 ;
 float local_float = 2.000000 ;
 char local_char = 1 ;
 int s_3 = 5 ;
 s_3 = ( 3 + ( MAX - MAX % 3 ) ) * ( MAX / 3 ) / 2 ;
 int s_5 = 7 ;
 s_5 = ( 5 + ( MAX - MAX % 5 ) ) * ( MAX / 5 ) / 2 ;
 int s_15 = 3 ;
 s_15 = ( 15 + ( local_float - MAX % 15 ) ) * ( MAX / 15 ) / 2 ;
 s_15 ^= s_3 + local_char + 56 * ((int)((((long long int)(s_15 )) | ((long long int)(s_3 + local_char ))))) * ((int)((((float)(s_15 )) * ((float)(s_3 + local_char ))))) + ((int)((((float)(global_int)) - ((float)(s_3 + local_char ))))) ;
 local_float /= global_float ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
