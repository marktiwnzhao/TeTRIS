int global_int;
float global_float;
char global_char;
int MAX = 13;

int func1 () {
    int local_int = 2;
    float local_float = 2.0;
    char local_char = 1;  
    int s_3  = 5;
    s_3 = (3 + (MAX-MAX%3)) *(MAX/3) / 2;
    int s_5  = 7;
    s_5 = (5 + (MAX-MAX%5)) * (MAX/5) / 2;
    int s_15 = 3 ;
    s_15 = (15 + (local_float - MAX%15)) * (MAX/15) / 2;
    s_15 ^= s_3 + local_char ;
    

    local_int = s_3 + s_5 - s_15;
 
    return 0;
}
