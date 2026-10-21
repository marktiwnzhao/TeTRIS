int global_int ;
 float global_float ;
 char global_char ;
 struct MyStruct {
 int integerMember ;
 float important ;
 }
 minee , whatif ;
 float why , dis ;
 int func1 ( ) {
 int local_int = 1 ;
 float local_float = 1.000000 ;
 char local_char = 1 ;
 char p_p = 1 ;
 float * ft_ptr = & why ;
 int * int_ptr = & minee .integerMember ;
 local_int += *int_ptr -= *int_ptr ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long int ) ( * int_ptr ) ) * ( ( unsigned long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( * int_ptr ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) - ( ( double ) ( global_int ) ) ) ) ) + 56 + ((int)((((float)(minee .integerMember )) * ((float)(local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long int ) ( * int_ptr ) ) * ( ( unsigned long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( * int_ptr ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) - ( ( double ) ( global_int ) ) ) ) ) ))))) - ((int)((((unsigned long int)(minee .integerMember )) - ((unsigned long int)(local_int + 4 + 56 + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( whatif .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned long int ) ( * int_ptr ) ) * ( ( unsigned long int ) ( local_int + 4 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned short int ) ( * int_ptr ) ) < ( ( unsigned short int ) ( local_int + 4 ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( * int_ptr ) ) ) ) ) + ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) - ( ( double ) ( global_int ) ) ) ) ) ))))) * ((int)((((float)(minee .integerMember )) * ((float)(minee .integerMember ))))) ;
 * int_ptr = ( global_int * 2 ) + minee .integerMember + 56 * ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) + ( ( double ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) + ( ( int ) ( ( ( ( long long int ) ( * int_ptr ) ) ^ ( ( long long int ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) - ( ( int ) ( ( ( ( short ) ( * int_ptr ) ) & ( ( short ) ( ( global_int * 2 ) + minee .integerMember ) ) ) ) ) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 ;
 return p_p /= local_char ;
 local_int = global_char + 1 + 56 * ( ( int ) ( ( ( ( unsigned long int ) ( local_int ) ) | ( ( unsigned long int ) ( global_char + 1 ) ) ) ) ) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 * ( ( float ) ( ( ( ( double ) ( local_float ) ) - ( ( double ) ( why ) ) ) ) ) + ( ( float ) ( ( ( ( float ) ( why ) ) - ( ( float ) ( why ) ) ) ) ) + 56 + ((float)((((long long int)(local_float )) <= ((long long int)(dis))))) ;
 return 0 ;
 }
 