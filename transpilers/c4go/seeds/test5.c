
int global_int;
float global_float;
char global_char;

float p=4.0;
double x=5.6;
int globe = 1;
int func1 () {
    int local_int = 2;
    float local_float = 2.0;
    char local_char = 1;  
    int s_3  = 2;


    do {
        local_int = s_3 + global_float;
        s_3++;
        int new_p = 1;
        local_int = x + local_char;
        
         if(local_int>=450){
            float new_x = 1.05 ;
            x = new_p + new_x;
            new_x = local_char + local_int;
            local_float -= x + new_x ; 
         }
         else {
            double y_y = local_float;
            local_int = new_p + (int)((double)globe);
            local_float += x + y_y ; 
         }

         globe = new_p + s_3;
    }
    while (s_3 < 5);
    
    
    local_float = local_char + local_int;

    local_int = globe + (int)local_float;

    return 0;
}
