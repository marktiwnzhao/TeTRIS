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
 local_int += global_int * 3 + 56 + ((int)((((float)(global_int * 3 )) - ((float)(local_int))))) + ((int)((((short int)(local_int )) - ((short int)(global_int * 3 ))))) * ((int)((((double)(local_int )) * ((double)(local_int))))) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long int ) ( * int_ptr ) ) * ( ( unsigned long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) + 56 + ((int)((((float)(minee .integerMember )) - ((float)(local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long int ) ( * int_ptr ) ) * ( ( unsigned long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) ))))) * ((int)((((double)(minee .integerMember )) - ((double)(local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long int ) ( * int_ptr ) ) * ( ( unsigned long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) ))))) ;
 * int_ptr = ( global_int * 2 ) + minee .integerMember + 56 * ((int)((((short int)(* int_ptr )) <= ((short int)(whatif.integerMember))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 - ( ( float ) ( ( ( ( float ) ( * ft_ptr ) ) * ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) - ( ( float ) ( ( ( ( double ) ( * ft_ptr ) ) * ( ( double ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) ;
 * int_ptr = ( - ( ~ ( local_int * global_int ) ) ) ;
 local_int = global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long int ) ( local_int ) ) | ( ( unsigned long int ) ( global_char + 1 ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( long long int ) ( local_int ) ) - ( ( long long int ) ( global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long int ) ( local_int ) ) | ( ( unsigned long int ) ( global_char + 1 ) ) ) ) ) ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( local_int ) ) * ( ( double ) ( global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long int ) ( local_int ) ) | ( ( unsigned long int ) ( global_char + 1 ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long int ) ( local_int ) ) | ( ( unsigned long int ) ( global_char + 1 ) ) ) ) ) ) ) - ( ( double ) ( global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long int ) ( local_int ) ) | ( ( unsigned long int ) ( global_char + 1 ) ) ) ) ) ) ) ) ) ) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) + ( ( float ) ( minee .important ) ) ) ) ) ;
 local_float = *ft_ptr -= *ft_ptr ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
