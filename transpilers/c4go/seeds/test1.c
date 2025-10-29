int global_int;
float global_float;
char global_char;

struct MyStruct {
    int integerMember;
    float important;
} minee, whatif;

float why, dis;
 int func1 () {
    int local_int = 1;
    float local_float = 1.0;
    char local_char = 1;
    
    char p_p = 1;

    float* ft_ptr = &why;
    int* int_ptr= &minee.integerMember;
    
    local_int += global_int * 3;
    local_float++;

    minee.integerMember = local_int + 4;

    *int_ptr = (global_int * 2) + minee.integerMember ;

    *ft_ptr = (float)minee.integerMember + 2.19 ;

    global_float = (*ft_ptr) * 2;
    
    local_int = global_char+1;

    whatif.important =  (*ft_ptr) + (float)minee.important + 5.45;

    local_float = (*ft_ptr) + 2.01 + whatif.important + global_float;


    return 0;
}