int global_int;
float global_float;
char global_char;

float why, dis;

long int i_l, j_l;

double i_d, j_d;

struct MyStruct {
    int intMemr;
    float important;
    long int stay;
    short int stay2;
    char st_char;

} minee, whatif;

struct MyStruct var1 = { 1, 2.00, 3, 2, 'b' };

union Data{
   int i;
};
union Data data1 = { 2 };


int reply(int q, float t){
    
    if(t>511.5) t= 3+dis;
    else t = 2 + why;

    if(q>58) q= 3+i_l;
    else q = 2 + j_l;

    global_int = q +2 ;
    global_float = t + 1.2;
    
    return t-q;
}

 int func1 () {
    int local_int = 1;
    float local_float = 1.0;
    char local_char = 1;
    
    char p_p = 1;
    int i_loop = 0;

    float* ft_ptr = &why;
    int* int_ptr= &minee.intMemr;

    *int_ptr = (global_int * 2) + var1.intMemr ;

    *ft_ptr = (float)minee.intMemr + 2.19 ;

    i_l = local_int + var1.important;

    local_int = data1.i + minee.intMemr ;

    local_int = reply(minee.intMemr, *ft_ptr) + 5 ;

    local_float = reply(*int_ptr, whatif.important) + 5 ;

    goto label_last;

    label_while: {

        local_char++;

        while(i_loop<5){
            local_float += i_d;
            i_loop++; 
        }

        i_d = 32 + j_d;

        j_l = minee.stay2 + whatif.intMemr;
  
    }

    label_switch: {
        int temp_local_int = local_int; 
        switch (temp_local_int % 3) {
            case 0:
                global_int += whatif.important;
                global_char = 'A';
                break;
            case 1:
                global_float = local_float + 5.0;
                minee.important = global_float;
                break;
            case 2:
                i_l += minee.important;
                j_l -= whatif.intMemr;
                break;
            default:
                global_int = 0;
                break;
        }

    }


    label_if_else: {

       if(i_loop==0){
            data1.i = 32; 
       }
       else if(i_loop>0 && i_loop<5) {
            local_float += 2.01;
            i_loop++;
       }
       else {
          why = dis + minee.important;

       }
        
    }



    label_if: {
        if(local_int == 123){
            local_float--;
            global_int++;
            local_int = i_l + global_int;
        }

        j_d = local_float + dis;
        dis = j_d;
        local_float = dis;
        i_l = whatif.stay + local_int;
        local_int = i_l + 5;
    }
    

    whatif.important =  (*ft_ptr) + (float)minee.important + 5.45;
    local_float = (*ft_ptr) + 2.01 + whatif.important + global_float;
    
    label_none:
    local_float = j_d + i_d; 
    local_int = i_l + global_int;
    return 0;

    label_last: goto label_while;
  
}

