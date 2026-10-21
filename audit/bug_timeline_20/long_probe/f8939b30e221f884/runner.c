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
 local_int += global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) < ( ( unsigned long long int ) ( global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) ) ) ) ) ) * ( ( int ) ( ( ( ( long long int ) ( global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) ) ) <= ( ( long long int ) ( global_int ) ) ) ) ) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) | ( ( unsigned short int ) ( local_int ) ) ) ) ) + 56 + ((int)((((double)(minee .integerMember )) - ((double)(*int_ptr))))) + ((int)((((int)(minee .integerMember )) - ((int)(*int_ptr))))) ;
 * int_ptr ^= ( global_int * 2 ) + minee .integerMember + 56 * ((int)((((unsigned long long int)(* int_ptr )) ^ ((unsigned long long int)(*int_ptr))))) + ((int)((((float)(* int_ptr )) + ((float)(*int_ptr))))) - ((int)((((unsigned short int)(minee.integerMember)) <= ((unsigned short int)(minee.integerMember))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 + ((float)((((unsigned short int)(*ft_ptr)) & ((unsigned short int)(( float ) minee .integerMember + 2.190000 ))))) - ((float)((((unsigned long long int)(*ft_ptr)) - ((unsigned long long int)(local_float))))) ;
 global_float = ( * ft_ptr ) * 2 + 56 + ( ( float ) ( ( ( ( double ) ( * ft_ptr ) ) * ( ( double ) ( local_float ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( * ft_ptr ) ) - ( ( float ) ( local_float ) ) ) ) ) + 56 - ((float)((((double)(why)) + ((double)(( * ft_ptr ) * 2 + 56 + ( ( float ) ( ( ( ( double ) ( * ft_ptr ) ) * ( ( double ) ( local_float ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( * ft_ptr ) ) - ( ( float ) ( local_float ) ) ) ) ) ))))) ;
 local_int = global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( long long int ) ( local_int ) ) + ( ( long long int ) ( global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( short ) ( local_int ) ) + ( ( short ) ( global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) | ( ( unsigned long long int ) ( local_float ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) + ( ( float ) ( local_float ) ) ) ) ) - ( ( float ) ( ( ( ( double ) ( whatif .important ) ) * ( ( double ) ( local_float ) ) ) ) ) + 56 * ((float)((((float)(whatif .important )) - ((float)(global_float))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float ;
 set_var(local_int,local_float,local_char);
return global_char += (~p_p) ;
 }
 
int main(){func1();return 0;}
