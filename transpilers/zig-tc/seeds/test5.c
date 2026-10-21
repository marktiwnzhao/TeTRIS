int global_int = 1;
float global_float = 1.5;
char global_char = 1;

int globe =1;
float new_p = 1.01;
int func1() {
    int local_int = 1;
    float local_float = 1.0;
    char local_char = 'x';
    int glob_local = 1;
    float newp_loc = 1.2;

    local_int += global_int * 2;
    local_float += 2.5;
    local_char = global_char + 2;
    
    local_float += global_float;
    local_char = local_int + 1;

    if(1){
        if(2){
            if(3){
                new_p = 1 + local_float;
                glob_local = 2 + globe;
                local_int += globe + new_p;
                local_float += 2.5 + global_float;
                
            }
                new_p = 1 + local_float;
                glob_local = 2 + globe;
                local_int += globe + new_p;
                local_float += 2.5 + global_float;
        }
                new_p = 1 + local_float;
                glob_local = 2 + globe;
                local_int += globe + new_p;
                local_float += 2.5 + global_float;
    }

    if(1){
        do
        {
            if(1){
                if(2){
                     if(3){
                    new_p = 1 + local_float;
                    glob_local = 2 + globe;
                    local_int += globe + new_p;
                    local_float += 2.5 + global_float;
                    
                }
                    new_p = 1 + local_float;
                    glob_local = 2 + globe;
                    local_int += globe + new_p;
                    local_float += 2.5 + global_float;
            }
                new_p = 1 + local_float;
                glob_local = 2 + globe;
                local_int += globe + new_p;
                local_float += 2.5 + global_float;
        } 
        local_int++;
        } while (local_int< 15);
        
    }
    
    local_int -= global_int;
    local_float /= global_float;
    local_char = local_char - 1;

    return 0;
}

