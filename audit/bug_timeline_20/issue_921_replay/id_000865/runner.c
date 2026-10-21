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
 local_int += global_int * 3 + 56 - ( ( int ) ( ( ( ( unsigned short int ) ( local_int ) ) - ( ( unsigned short int ) ( global_int * 3 ) ) ) ) ) + ( ( int ) ( ( ( ( float ) ( local_int ) ) * ( ( float ) ( global_int * 3 ) ) ) ) ) + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) + ( ( double ) ( local_int ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) + ( ( double ) ( local_int ) ) ) ) ) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 + 56 * ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) + ( ( double ) ( minee .integerMember ) ) ) ) ) * ( ( int ) ( ( ( ( short ) ( local_int + 4 ) ) * ( ( short ) ( global_int ) ) ) ) ) * ( ( int ) ( ( ( ( double ) ( local_int + 4 ) ) + ( ( double ) ( global_int ) ) ) ) ) + 56 + ( ( int ) ( ( ( ( double ) ( minee .integerMember ) ) * ( ( double ) ( * int_ptr ) ) ) ) ) - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) + ( ( double ) ( minee .integerMember ) ) ) ) ) ;
 local_float -= ( - dis ) + 56 - ((float)((((float)(( - dis ) )) - ((float)(why))))) + ((float)((((float)(whatif.important)) + ((float)(why))))) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 - ( ( float ) ( ( ( ( double ) ( minee .important ) ) * ( ( double ) ( minee .important ) ) ) ) ) + ( ( float ) ( ( ( ( float ) ( why ) ) + ( ( float ) ( minee .important ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( why ) ) - ( ( float ) ( minee .important ) ) ) ) ) + 56 * ((float)((((float)(*ft_ptr)) * ((float)(dis))))) ;
 global_float = ( * ft_ptr ) * 2 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) * ( ( float ) ( minee .important ) ) ) ) ) + ( ( float ) ( ( ( ( unsigned short int ) ( whatif .important ) ) & ( ( unsigned short int ) ( minee .important ) ) ) ) ) + ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) + ( ( unsigned long long int ) ( minee .important ) ) ) ) ) + 56 - ( ( float ) ( ( ( ( double ) ( global_float ) ) - ( ( double ) ( ( * ft_ptr ) * 2 + 56 * ( ( float ) ( ( ( ( float ) ( whatif .important ) ) * ( ( float ) ( minee .important ) ) ) ) ) + ( ( float ) ( ( ( ( unsigned short int ) ( whatif .important ) ) & ( ( unsigned short int ) ( minee .important ) ) ) ) ) + ( ( float ) ( ( ( ( unsigned long long int ) ( whatif .important ) ) + ( ( unsigned long long int ) ( minee .important ) ) ) ) ) ) ) ) ) ) * ( ( float ) ( ( ( ( float ) ( global_float ) ) - ( ( float ) ( * ft_ptr ) ) ) ) ) + ( ( float ) ( ( ( ( unsigned short int ) ( dis ) ) & ( ( unsigned short int ) ( * ft_ptr ) ) ) ) ) ;
 local_int = *int_ptr /= *int_ptr ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ( ( float ) ( ( ( ( int ) ( global_float ) ) < ( ( int ) ( minee .important ) ) ) ) ) + 56 * ((float)((((long long int)(whatif .important )) + ((long long int)(( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ( ( float ) ( ( ( ( int ) ( global_float ) ) < ( ( int ) ( minee .important ) ) ) ) ) ))))) * ((float)((((double)(whatif .important )) + ((double)(( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ( ( float ) ( ( ( ( int ) ( global_float ) ) < ( ( int ) ( minee .important ) ) ) ) ) ))))) * ((float)((((short)(local_float)) | ((short)(why))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 - ((float)((((float)(local_float )) * ((float)(( * ft_ptr ) + 2.010000 + whatif .important + global_float ))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
