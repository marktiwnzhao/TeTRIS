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
 int local_int = 1 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 char p_p = 1 ;
 float * ft_ptr = & why ;
 int * int_ptr = & minee .integerMember ;
 local_int += global_int * 3 + 56 - ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) | ( ( unsigned long long int ) ( global_int * 3 ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( local_int ) ) * ( ( double ) ( global_int * 3 ) ) ) ) ) + 56 * ((int)((((double)(global_int * 3 + 56 - ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) | ( ( unsigned long long int ) ( global_int * 3 ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( local_int ) ) * ( ( double ) ( global_int * 3 ) ) ) ) ) )) - ((double)(local_int ))))) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) * ( ( unsigned long long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( double ) ( global_int ) ) + ( ( double ) ( local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long long int ) ( * int_ptr ) ) * ( ( unsigned long long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( float ) ( global_int ) ) + ( ( float ) ( local_int ) ) ) ) ) + ( ( int ) ( ( ( ( short int ) ( global_int ) ) > ( ( short int ) ( local_int ) ) ) ) ) ;
 * int_ptr = ( global_int * 2 ) + minee .integerMember ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 - ( ( float ) ( ( ( ( int ) ( * ft_ptr ) ) & ( ( int ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) + 56 + ((float)((((double)(*ft_ptr)) - ((double)(whatif.important))))) - ((float)((((short int)(*ft_ptr)) * ((short int)(whatif.important))))) + ((float)((((short int)(*ft_ptr)) & ((short int)(* ft_ptr ))))) ;
 p_p /= local_char + 56 + ((char)((((unsigned long long int)(p_p )) < ((unsigned long long int)(local_char))))) - ((char)((((long long int)(p_p )) <= ((long long int)(p_p ))))) - ((char)((((double)(p_p )) + ((double)(p_p ))))) ;
 local_int = global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) | ( ( unsigned long long int ) ( global_char + 1 ) ) ) ) ) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ( ( float ) ( ( ( ( double ) ( whatif .important ) ) * ( ( double ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 ) ) ) ) ) - ( ( float ) ( ( ( ( unsigned long long int ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 ) ) > ( ( unsigned long long int ) ( local_float ) ) ) ) ) + 56 + ((float)((((float)(*ft_ptr)) * ((float)(*ft_ptr))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 + ( ( float ) ( ( ( ( long long int ) ( local_float ) ) * ( ( long long int ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) ) ) ) - ( ( float ) ( ( ( ( unsigned long long int ) ( local_float ) ) * ( ( unsigned long long int ) ( local_float ) ) ) ) ) + 56 + ((float)((((unsigned long long int)(local_float )) >= ((unsigned long long int)(( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 + ( ( float ) ( ( ( ( long long int ) ( local_float ) ) * ( ( long long int ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) ) ) ) - ( ( float ) ( ( ( ( unsigned long long int ) ( local_float ) ) * ( ( unsigned long long int ) ( local_float ) ) ) ) ) ))))) + ((float)((((float)(local_float )) + ((float)(*ft_ptr))))) + ((float)((((float)(local_float )) - ((float)(local_float ))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
