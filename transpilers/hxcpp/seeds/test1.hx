
var global_int:Int = 1;
var global_float:Float = 0.05;

var pp:Int = -4;
var way:Float = 0.5;

typedef Person = {
	var p1:Float;
	var age:Int;
}

typedef Tetris = {
	var dd:Float;
	var stat:Int;
  var ee:Float;
  var stat1:Int;
}

var myStruct2:Tetris = {dd: 1.15, stat:2, ee: 1.4, stat1: 1};

class Runner {

	static var m:Float = -.05;
	static var local_int:Int = 1;
  static var local_float:Float = 0.05;

  static function reply():Int {
    var ll:Int = -1 ;
    ll = myStruct2.stat + pp;
    return pp;
  }
  
 
	static function func1():Int {
		pp = pp - 6;
		
    if (pp > local_int) {
			pp = local_int + myStruct2.stat1 ;
      m = myStruct2.ee + way ;

    }
		else {
			pp = global_int - myStruct2.stat ;
      myStruct2.ee = m + local_float ;

    }
		
	
	  var myStruct:Person = {p1: 10.15, age: 15};
		var sum:Int = 0;
		var i:Int = 1;

	  i = myStruct.age + pp ;
    sum = reply() + i;
		m = way + myStruct2.ee ;
		
		do {
			sum = i + myStruct.age;
      local_float = myStruct.p1 + way;
			i++;
		} while (i <= 15);

		sum += local_int+1;

		local_int = sum + pp;
		local_float = m + global_float;

		return 0;
	}

	
}