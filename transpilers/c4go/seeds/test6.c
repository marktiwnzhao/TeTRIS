
int global_int;
float global_float;
char global_char;

float p=4.0;
double x=5.6;
int globe = 1;

int arr[4] = { 0, 2, 1, 3};
float d_arr[3] = { 1.1, 2.2, 1.4};

int func1 () {
    int local_int = 35;
    float local_float = 22.0;
    char local_char = 'a';  
    int s_3  = 5;
    
     

    do {
        d_arr[2] = global_float + arr[2];
        arr[2] = local_int + globe;
        local_int = s_3 + global_float;
        s_3++;
        int new_p = 1;
        local_int = x + local_char;

        global_float = d_arr[2];
        local_int = arr[2];
        local_float = global_float - 1.12;
        
        if(local_int>=45){
            float new_x = 1.05 ;
            x = new_p + new_x;
            new_x = local_char + local_int;
            local_float -= x + new_x ;
                    {
                        if(local_float>30.15){
                            int new_r = 3;
                            new_r = arr[2] - 1.05 + new_x;
                            local_float = d_arr[2] - new_x;
                            globe = new_p + arr[2];
                            local_int = globe - new_r;
                        }
                        else {
                            float new_F = arr[2];
                            new_F = local_float + 1.01;
                            arr[2] = new_F + 1;
                            local_float = new_F - arr[2];
                            local_int = arr[2] - local_float; 
                        }
                    }
         }
         else {
            double y_y = local_float;
            local_int = new_p + (int)((double)globe);
            local_float += x + y_y ; 
            d_arr[2] = global_float + arr[2];
            arr[2] = local_int + globe;
         }

         globe = new_p + s_3;
    }
    while (s_3 < 5);
    
    
    local_float = local_char + local_int;

    local_int = globe + (int)local_float;

    return 0;
}
