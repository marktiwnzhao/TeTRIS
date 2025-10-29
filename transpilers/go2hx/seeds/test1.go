package main ;

var global_int int = 1;
var global_float float64 = 1.00;


type Smart struct {
	t        int;
	i        int;
	abc, def float64;
	what, is int;
} ;

var smartStruct Smart;

var int_array   [3]int;


var num (int);
var p, t, m int = -0o2, -0o3, -0o1;
var q float64 = 1.5 ;
var parray [3]int;

var p_p, q_q float64 = 1.1 , 1.2;

var ptr, qtr *float64 = &global_float, &p_p;

var int_ptr, int_qtr *int = &p , &t;




func func1() int {
	var local_int int = 1;
	var local_float float64 = 1.5;

	*qtr = p_p + smartStruct.def - q;

	parray[0] = *int_ptr; 

	*ptr = q_q - smartStruct.abc + *qtr; 
	
	local_float += *qtr + *ptr ;

	var fptr *float64 = &smartStruct.abc;

	*fptr = local_float ;
	
	goto label_last;

  	label_start :
		 {
			var num2 int = 1;
			num2 = num + m ;
			*int_ptr = parray[0] + num2;
			var num3 int = 1;
			num3 = num3 + *int_ptr;
			*int_qtr = num3  ;
		}

	label_if :
	{
		if *int_qtr > 100 && t >50 {
			smartStruct.what = local_int ;
			p_p = smartStruct.abc - q_q ;
		} else {
			smartStruct.t = *int_ptr;

		}


	}

	label_if_else :
	{

		if *int_qtr > 100 {
			smartStruct.what = local_int + smartStruct.is ;
		} else if smartStruct.abc < 10.15 {
			smartStruct.t = *int_ptr;
			
		} else {
			int_array[1] = *int_ptr + *int_qtr ;
		}

	}

	label_loop :
	{
		*fptr = local_float ;

		for m = 1; m<15 ; m++ {
			local_int = int_array[1] ;
		}

	}


  
    label_none :
    
	{  

		local_int += *int_qtr + t ;
		local_float += smartStruct.abc + *qtr ;
		local_int += smartStruct.t + int_array[1];
	    return 0;
	}
  
	
	label_last: goto label_start;
}

