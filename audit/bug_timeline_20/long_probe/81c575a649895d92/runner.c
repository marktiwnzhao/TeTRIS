int global_int ;
 float global_float ;
 char global_char ;
 struct MyStruct {
 int integerMember ;
 float important ;
 }
 minee , whatif ;
 float why , dis ;
 void set_var(int gi,float gf,char gc){global_int=gi;global_float=gf;global_char=gc;}
int func1 ( ) {
 int local_int = 2 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 char p_p = 1 ;
 float * ft_ptr = & why ;
 int * int_ptr = & minee .integerMember ;
 local_int += global_int * 3 + 56 * ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( global_int * 3 ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) > ( ( unsigned long long int ) ( * int_ptr ) ) ) ) ) + 56 * ((int)((((int)(*int_ptr)) & ((int)(global_int * 3 + 56 * ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( global_int * 3 ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) > ( ( unsigned long long int ) ( * int_ptr ) ) ) ) ) ))))) - ((int)((((int)(global_int * 3 + 56 * ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( global_int * 3 ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) > ( ( unsigned long long int ) ( * int_ptr ) ) ) ) ) )) | ((int)(global_int * 3 + 56 * ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( global_int * 3 ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) > ( ( unsigned long long int ) ( * int_ptr ) ) ) ) ) ))))) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) * ( ( unsigned long long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) ;
 * int_ptr = ( global_int * 2 ) + minee .integerMember + 56 * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) ^ ( ( short int ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) + 56 + ((int)((((float)(local_int)) * ((float)(( global_int * 2 ) + minee .integerMember + 56 * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) ^ ( ( short int ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) ))))) + ((int)((((double)(( global_int * 2 ) + minee .integerMember + 56 * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) ^ ( ( short int ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) )) - ((double)(( global_int * 2 ) + minee .integerMember + 56 * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) ^ ( ( short int ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) ))))) + ((int)((((double)(global_int)) * ((double)(local_int))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 + ( ( float ) ( ( ( ( float ) ( * ft_ptr ) ) + ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) - ( ( float ) ( ( ( ( unsigned short int ) ( ( float ) minee .integerMember + 2.190000 ) ) | ( ( unsigned short int ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) ;
 p_p /= local_char + 56 * ( ( char ) ( ( ( ( short int ) ( p_p ) ) - ( ( short int ) ( p_p ) ) ) ) ) - ( ( char ) ( ( ( ( float ) ( p_p ) ) - ( ( float ) ( local_char ) ) ) ) ) ;
 local_int = global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) | ( ( unsigned long long int ) ( global_char + 1 ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( float ) ( local_int ) ) + ( ( float ) ( global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) | ( ( unsigned long long int ) ( global_char + 1 ) ) ) ) ) ) ) ) ) ) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( short ) ( whatif .important ) ) * ( ( short ) ( whatif .important ) ) ) ) ) + 56 - ((float)((((unsigned long long int)(whatif .important )) - ((unsigned long long int)(( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( short ) ( whatif .important ) ) * ( ( short ) ( whatif .important ) ) ) ) ) ))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 + ( ( float ) ( ( ( ( double ) ( * ft_ptr ) ) * ( ( double ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) ) ) ) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
