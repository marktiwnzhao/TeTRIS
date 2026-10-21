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
 float local_float = 1 ;
 char local_char = 1 ;
 char p_p = 1 ;
 float * ft_ptr = & why ;
 int * int_ptr = & minee .integerMember ;
 local_int += *int_ptr += *int_ptr ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) + ( ( unsigned short int ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( int ) ( minee .integerMember ) ) | ( ( int ) ( * int_ptr ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( short int ) ( minee .integerMember ) ) >= ( ( short int ) ( local_int + 4 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) + ( ( unsigned short int ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( int ) ( minee .integerMember ) ) | ( ( int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) + 56 + ((int)((((unsigned short int)(local_int + 4 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) + ( ( unsigned short int ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( int ) ( minee .integerMember ) ) | ( ( int ) ( * int_ptr ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( short int ) ( minee .integerMember ) ) >= ( ( short int ) ( local_int + 4 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) + ( ( unsigned short int ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( int ) ( minee .integerMember ) ) | ( ( int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) )) + ((unsigned short int)(local_int + 4 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) + ( ( unsigned short int ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( int ) ( minee .integerMember ) ) | ( ( int ) ( * int_ptr ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( short int ) ( minee .integerMember ) ) >= ( ( short int ) ( local_int + 4 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) + ( ( unsigned short int ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( int ) ( minee .integerMember ) ) | ( ( int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) ))))) ;
 * int_ptr = ( global_int * 2 ) + minee .integerMember ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 * ( ( float ) ( ( ( ( float ) ( local_float ) ) * ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) + 56 - ( ( float ) ( ( ( ( unsigned short int ) ( * ft_ptr ) ) * ( ( unsigned short int ) ( * ft_ptr ) ) ) ) ) ;
 global_float = ( * ft_ptr ) * 2 + 56 + ((float)((((long int)(minee.important)) - ((long int)(minee.important))))) - ((float)((((long long int)(why)) >= ((long long int)(*ft_ptr))))) ;
 local_int = global_char + 1 + 56 * ((int)((((double)(local_int )) + ((double)(*int_ptr))))) * ((int)((((long int)(local_int )) ^ ((long int)(global_int))))) + ((int)((((unsigned long int)(local_int)) < ((unsigned long int)(global_int))))) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ( ( float ) ( ( ( ( double ) ( whatif .important ) ) + ( ( double ) ( whatif .important ) ) ) ) ) + 56 - ((float)((((float)(whatif .important )) - ((float)(( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ( ( float ) ( ( ( ( double ) ( whatif .important ) ) + ( ( double ) ( whatif .important ) ) ) ) ) ))))) * ((float)((((double)(whatif .important )) + ((double)(global_float))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 - ( ( float ) ( ( ( ( long int ) ( local_float ) ) >= ( ( long int ) ( * ft_ptr ) ) ) ) ) - ( ( float ) ( ( ( ( float ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) + ( ( float ) ( * ft_ptr ) ) ) ) ) - ( ( float ) ( ( ( ( float ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) - ( ( float ) ( * ft_ptr ) ) ) ) ) + 56 - ( ( float ) ( ( ( ( double ) ( local_float ) ) * ( ( double ) ( global_float ) ) ) ) ) + ( ( float ) ( ( ( ( long int ) ( local_float ) ) >= ( ( long int ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 - ( ( float ) ( ( ( ( long int ) ( local_float ) ) >= ( ( long int ) ( * ft_ptr ) ) ) ) ) - ( ( float ) ( ( ( ( float ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) + ( ( float ) ( * ft_ptr ) ) ) ) ) - ( ( float ) ( ( ( ( float ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) - ( ( float ) ( * ft_ptr ) ) ) ) ) ) ) ) ) ) ;
 return 0 ;
 }
 