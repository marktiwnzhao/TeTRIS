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
 local_int += global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) < ( ( unsigned long long int ) ( global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) ) ) ) ) ) * ( ( int ) ( ( ( ( long int ) ( global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) ) ) <= ( ( long int ) ( global_int ) ) ) ) ) + 56 * ((int)((((double)(local_int )) + ((double)(whatif.integerMember))))) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) | ( ( unsigned short int ) ( local_int ) ) ) ) ) + 56 * ((int)((((unsigned long long int)(minee .integerMember )) >= ((unsigned long long int)(local_int + 4 + 56 + ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) | ( ( unsigned short int ) ( local_int ) ) ) ) ) ))))) ;
 * int_ptr ^= ( global_int * 2 ) + minee .integerMember + 56 - ((int)((((double)(minee.integerMember)) - ((double)(*int_ptr))))) - ((int)((((unsigned long long int)(minee.integerMember)) & ((unsigned long long int)(*int_ptr))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 - ( ( float ) ( ( ( ( unsigned short int ) ( * ft_ptr ) ) - ( ( unsigned short int ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) + ( ( float ) ( ( ( ( double ) ( global_float ) ) - ( ( double ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) + 56 + ((float)((((unsigned short int)(dis)) >= ((unsigned short int)(* ft_ptr ))))) * ((float)((((unsigned short int)(dis)) >= ((unsigned short int)(*ft_ptr))))) ;
 global_float = ( * ft_ptr ) * 2 + 56 + ( ( float ) ( ( ( ( double ) ( * ft_ptr ) ) * ( ( double ) ( local_float ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( * ft_ptr ) ) - ( ( float ) ( local_float ) ) ) ) ) + 56 - ((float)((((long long int)(global_float )) - ((long long int)(dis))))) + ((float)((((float)(global_float )) + ((float)(dis))))) ;
 local_int = global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( long long int ) ( local_int ) ) + ( ( long long int ) ( global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( short ) ( local_int ) ) + ( ( short ) ( global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( float ) ( global_int ) ) * ( ( float ) ( local_int ) ) ) ) ) + 56 * ((int)((((float)(local_int )) + ((float)(global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( long long int ) ( local_int ) ) + ( ( long long int ) ( global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) + ( ( int ) ( ( ( ( short ) ( local_int ) ) + ( ( short ) ( global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ) ) ) ) ) + 56 * ( ( int ) ( ( ( ( float ) ( global_int ) ) * ( ( float ) ( local_int ) ) ) ) ) ))))) * ((int)((((float)(global_int)) * ((float)(local_int ))))) - ((int)((((double)(local_int )) * ((double)(local_int ))))) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) | ( ( unsigned long long int ) ( local_float ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) + ( ( float ) ( local_float ) ) ) ) ) - ( ( float ) ( ( ( ( double ) ( whatif .important ) ) * ( ( double ) ( local_float ) ) ) ) ) + 56 - ((float)((((long int)(whatif .important )) + ((long int)(( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) | ( ( unsigned long long int ) ( local_float ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) + ( ( float ) ( local_float ) ) ) ) ) - ( ( float ) ( ( ( ( double ) ( whatif .important ) ) * ( ( double ) ( local_float ) ) ) ) ) ))))) - ((float)((((int)(whatif .important )) + ((int)(why))))) ;
 local_float = *ft_ptr += *ft_ptr ;
 return 0 ;
 }
 