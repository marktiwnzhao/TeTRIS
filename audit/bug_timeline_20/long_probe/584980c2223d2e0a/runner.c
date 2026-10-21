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
 local_int += global_int * 3 + 56 + ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) > ( ( unsigned long long int ) ( global_int * 3 ) ) ) ) ) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 + ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) + ( ( double ) ( local_int + 4 ) ) ) ) ) + ( ( int ) ( ( ( ( float ) ( minee .integerMember ) ) + ( ( float ) ( minee .integerMember ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( unsigned short int ) ( minee .integerMember ) ) <= ( ( unsigned short int ) ( minee .integerMember ) ) ) ) ) + 56 - ((int)((((float)(minee .integerMember )) * ((float)(global_int))))) + ((int)((((double)(global_int)) + ((double)(minee .integerMember ))))) ;
 dis -= ( dis * whatif .important ) + 56 * ( ( float ) ( ( ( ( double ) ( ( dis * whatif .important ) ) ) + ( ( double ) ( ( dis * whatif .important ) ) ) ) ) ) + 56 + ((float)((((double)(dis )) * ((double)(why))))) * ((float)((((double)(dis )) - ((double)(why))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 + ( ( float ) ( ( ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) - ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) + 56 * ( ( float ) ( ( ( ( unsigned short int ) ( * ft_ptr ) ) + ( ( unsigned short int ) ( ( float ) minee .integerMember + 2.190000 + 56 + ( ( float ) ( ( ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) - ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) ) ) ) ) ) - ( ( float ) ( ( ( ( unsigned short int ) ( * ft_ptr ) ) <= ( ( unsigned short int ) ( ( float ) minee .integerMember + 2.190000 + 56 + ( ( float ) ( ( ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) - ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) ) ) ) ) ) - ( ( float ) ( ( ( ( double ) ( * ft_ptr ) ) * ( ( double ) ( ( float ) minee .integerMember + 2.190000 + 56 + ( ( float ) ( ( ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) - ( ( float ) ( ( float ) minee .integerMember + 2.190000 ) ) ) ) ) ) ) ) ) ) ;
 global_float = ( * ft_ptr ) * 2 + 56 - ( ( float ) ( ( ( ( double ) ( global_float ) ) * ( ( double ) ( ( * ft_ptr ) * 2 ) ) ) ) ) + 56 + ((float)((((double)(global_float )) + ((double)(global_float ))))) * ((float)((((double)(global_float )) * ((double)(global_float ))))) ;
 local_int = global_char + 1 + 56 - ( ( int ) ( ( ( ( short ) ( local_int ) ) * ( ( short ) ( local_int ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned short int ) ( global_char + 1 ) ) - ( ( unsigned short int ) ( global_char + 1 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) > ( ( unsigned long long int ) ( * int_ptr ) ) ) ) ) + 56 * ((int)((((float)(local_int )) + ((float)(global_char + 1 + 56 - ( ( int ) ( ( ( ( short ) ( local_int ) ) * ( ( short ) ( local_int ) ) ) ) ) * ( ( int ) ( ( ( ( unsigned short int ) ( global_char + 1 ) ) - ( ( unsigned short int ) ( global_char + 1 ) ) ) ) ) - ( ( int ) ( ( ( ( unsigned long long int ) ( local_int ) ) > ( ( unsigned long long int ) ( * int_ptr ) ) ) ) ) ))))) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) - ( ( float ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 ) ) ) ) ) * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) >= ( ( unsigned long long int ) ( why ) ) ) ) ) + 56 + ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) >= ( ( unsigned long long int ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) - ( ( float ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 ) ) ) ) ) * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) >= ( ( unsigned long long int ) ( why ) ) ) ) ) ) ) ) ) ) + 56 * ((float)((((unsigned long long int)(global_float)) * ((unsigned long long int)(( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) - ( ( float ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 ) ) ) ) ) * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) >= ( ( unsigned long long int ) ( why ) ) ) ) ) + 56 + ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) >= ( ( unsigned long long int ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) - ( ( float ) ( ( * ft_ptr ) + ( float ) minee .important + 5.450000 ) ) ) ) ) * ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) >= ( ( unsigned long long int ) ( why ) ) ) ) ) ) ) ) ) ) ))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 * ( ( float ) ( ( ( ( unsigned long long int ) ( * ft_ptr ) ) ^ ( ( unsigned long long int ) ( ( * ft_ptr ) + 2.010000 + whatif .important + global_float ) ) ) ) ) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
