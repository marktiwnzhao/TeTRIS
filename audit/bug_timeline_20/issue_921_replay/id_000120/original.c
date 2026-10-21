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
 local_int += global_int * 3 + 56 + ( ( int ) ( ( ( ( short ) ( local_int ) ) > ( ( short ) ( global_int * 3 ) ) ) ) ) ;
 local_float ++ ;
 minee .integerMember = *int_ptr ^= *int_ptr ;
 * int_ptr = ( global_int * 2 ) + minee .integerMember + 56 - ((int)((((int)(* int_ptr )) | ((int)(( global_int * 2 ) + minee .integerMember ))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 * ( ( float ) ( ( ( ( int ) ( dis ) ) > ( ( int ) ( local_float ) ) ) ) ) + 56 - ((float)((((double)(* ft_ptr )) - ((double)(( float ) minee .integerMember + 2.190000 + 56 * ( ( float ) ( ( ( ( int ) ( dis ) ) > ( ( int ) ( local_float ) ) ) ) ) ))))) + ((float)((((long long int)(* ft_ptr )) > ((long long int)(local_float))))) ;
 global_float = ( * ft_ptr ) * 2 + 56 * ( ( float ) ( ( ( ( unsigned short int ) ( global_float ) ) ^ ( ( unsigned short int ) ( global_float ) ) ) ) ) + ( ( float ) ( ( ( ( short int ) ( dis ) ) ^ ( ( short int ) ( global_float ) ) ) ) ) + 56 + ((float)((((float)(global_float )) + ((float)(( * ft_ptr ) * 2 + 56 * ( ( float ) ( ( ( ( unsigned short int ) ( global_float ) ) ^ ( ( unsigned short int ) ( global_float ) ) ) ) ) + ( ( float ) ( ( ( ( short int ) ( dis ) ) ^ ( ( short int ) ( global_float ) ) ) ) ) ))))) ;
 local_int = global_char + 1 + 56 + ( ( int ) ( ( ( ( short int ) ( global_char + 1 ) ) + ( ( short int ) ( minee .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( float ) ( global_char + 1 ) ) + ( ( float ) ( minee .integerMember ) ) ) ) ) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 ;
 p_p /= ( - ( local_char * local_char ) ) + 56 + ((char)((((double)(p_p )) - ((double)(p_p ))))) - ((char)((((unsigned long int)(( - ( local_char * local_char ) ) )) ^ ((unsigned long int)(p_p ))))) - ((char)((((unsigned long int)(( - ( local_char * local_char ) ) )) - ((unsigned long int)(p_p ))))) ;
 return 0 ;
 }
 