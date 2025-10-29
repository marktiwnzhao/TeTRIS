
var global_int:Int = 1;
var global_float:Float = 0.05;

var pp:Int = -4;
var way:Float = 0.5;

typedef Person = {
	var name:String;
	var age:Int;
}

class Runner {

	static var m:Float = -.05;
	static var local_int:Int = 1;
  static var local_float:Float = 0.05;
  
 
	static function func1():Int {
		pp = pp - 6;
		if (pp > local_int)
			pp = local_int + 2 ;
		else
			pp = global_int -2 ;
		
	
	  var myStruct:Person = {name: "ALICE", age: 15};
		var sum:Int = 0;
		var i:Int = 1;

	  i = myStruct.age + pp ;

		myStruct.age = i + local_int ;

		m += way + 1.0 ;

		local_int -= pp + myStruct.age ;


		if (local_int < 0) {
			myStruct.age = local_int + sum ;
		}
			
		else if (local_int == 0) {
			 local_float = m + way ; 
		}
		else {
			 pp = local_int + myStruct.age ;
		}
		

		do {
			sum = i + pp;
			i++;
		} while (i <= 5);

		sum=local_int+1;

		local_int = sum + pp;
		local_float = m + global_float;

		return 0;
	}

	
}