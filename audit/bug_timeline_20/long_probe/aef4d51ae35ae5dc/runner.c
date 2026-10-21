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
 local_int += global_int * 3 + 56 * ((int)((((double)(local_int )) * ((double)(global_int * 3 ))))) ;
 local_float ++ ;
 minee .integerMember = local_int + 4 ;
 global_char = (~global_char) ;
 * ft_ptr = ( float ) minee .integerMember + 2.190000 ;
 global_float = ( * ft_ptr ) * 2 + 56 + ((float)((((unsigned long long int)(*ft_ptr)) & ((unsigned long long int)(( * ft_ptr ) * 2 ))))) - ((float)((((int)(*ft_ptr)) + ((int)(*ft_ptr))))) ;
 local_int = global_char + 1 + 56 - ((int)((((unsigned long long int)(*int_ptr)) - ((unsigned long long int)(global_char + 1 ))))) + ((int)((((int)(*int_ptr)) + ((int)(global_char + 1 ))))) + ((int)((((float)(whatif.integerMember)) * ((float)(global_char + 1 ))))) ;
 whatif .important = ( * ft_ptr ) + ( float ) minee .important + 5.450000 + 56 - ((float)((((unsigned long long int)(whatif .important )) - ((unsigned long long int)(dis))))) ;
 local_float = ( * ft_ptr ) + 2.010000 + whatif .important + global_float + 56 - ((float)((((unsigned short int)(( * ft_ptr ) + 2.010000 + whatif .important + global_float )) <= ((unsigned short int)(( * ft_ptr ) + 2.010000 + whatif .important + global_float ))))) + ((float)((((long long int)(( * ft_ptr ) + 2.010000 + whatif .important + global_float )) + ((long long int)(dis))))) ;
 set_var(local_int,local_float,local_char);
return 0 ;
 }
 
int main(){func1();return 0;}
