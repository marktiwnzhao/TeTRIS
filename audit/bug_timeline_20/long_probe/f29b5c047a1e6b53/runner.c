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
 local_int += global_int * 3 + 56 - ( ( int ) ( ( ( ( double ) ( * int_ptr ) ) - ( ( double ) ( whatif .integerMember ) ) ) ) ) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 ;
 set_var(local_int,local_float,local_char);
return global_char += (~local_char) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 + 56 - ((float)((((short int)(* ft_ptr )) & ((short int)(( float ) minee .integerMember + 2.190000 ))))) ;
 global_float = ( * ft_ptr ) * 2 + 56 - ((float)((((int)(global_float )) & ((int)(( * ft_ptr ) * 2 ))))) ;
 local_int = global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) + 56 - ((int)((((double)(global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) )) * ((double)(global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ))))) - ((int)((((unsigned short int)(*int_ptr)) >= ((unsigned short int)(global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ))))) * ((int)((((short)(local_int)) > ((short)(global_char + 1 + 56 - ( ( int ) ( ( ( ( double ) ( local_int ) ) - ( ( double ) ( * int_ptr ) ) ) ) ) * ( ( int ) ( ( ( ( short int ) ( * int_ptr ) ) < ( ( short int ) ( * int_ptr ) ) ) ) ) ))))) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 - ((float)((((unsigned short int)(local_float )) * ((unsigned short int)(local_float ))))) ;
 return 0 ;
 }
 
int main(){func1();return 0;}
