package main ;

var global_int int = 1;
var global_float float64 = 1.00;

var num (int);
var p, t, m int = -0o2, -0o3, -0o1;
var q float64 = 1.5 ;
var parray [3]int;

var p_p, q_q float64 = 1.1 , 1.2;
var qarray   [5]int;

var ptr, qtr *float64 = &global_float, &p_p;

type Player struct {
	t        int;
	i        int;
	abc, def float64;
	what, is int;
} ;

var mystruct Player;

var int_ptr, int_qtr *int = &p , &t;

func add(para, qara int, rara *float64, wait Player) int {

	var newp *int = &qara;

	*newp = para + wait.i;

	para = qara - (int)(*rara);

	wait.t = wait.what + para ;

	wait.i = wait.t + *newp ;

	return wait.i;
}


func func1() int {
	var local_int int = 1;
	var local_float float64 = 1.5;

	*qtr = p_p + mystruct.def - q;

	parray[0] = *int_ptr; 

	*ptr = q_q - mystruct.abc + *qtr; 
	
	local_float += *qtr + *ptr ;

	var fptr *float64 = &mystruct.abc;

	*fptr = local_float ;
	
	goto label_last;

  	label_start :
		 {
			*int_ptr = parray[0] + m;
			*int_qtr = *int_ptr + add(p,num,fptr,mystruct);
		}

	label_if :
	{
		if *int_qtr > 100 && t >50 {
			mystruct.what = local_int ;
			p_p = mystruct.abc - q_q ;
		} else {
			mystruct.t = *int_ptr;

		}


	}

	label_if_else :
	{

		if *int_qtr > 100 {
			mystruct.what = local_int + mystruct.is ;
		} else if mystruct.abc < 10.15 {
			mystruct.t = *int_ptr;
			
		} else {
			qarray[1] = *int_ptr + *int_qtr ;
		}

	}

	label_loop :
	{
		*fptr = local_float ;

		for m = 1; m<15 ; m++ {
			local_int = qarray[1] ;
		}

	}


  
    label_none :
    
	{   
		local_int += *int_qtr + t ;
		local_float += mystruct.abc + *qtr ;
		local_int += mystruct.t + qarray[1];
	    return 0;
	}
  
	
	label_last: goto label_start;
}

