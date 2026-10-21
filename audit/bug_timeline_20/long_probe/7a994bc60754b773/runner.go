/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main
import "fmt"

var global_int int32
var global_float float32
var global_char byte
var MAX int32 = int32(13)

// set_var - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/7a994bc60754b773/runner.c:5
func set_var(gi int32, gf float32, gc byte) {
	global_int = gi
	global_float = gf
	global_char = gc
}

// func1 - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/7a994bc60754b773/runner.c:6
func func1() int32 {
	var local_int int32 = int32(2)
	_ = local_int
	var local_float float32 = float32(2)
	_ = local_float
	var local_char byte = byte(int32(1))
	_ = local_char
	var s_3 int32 = int32(5)
	_ = s_3
	s_3 = (int32(3) + (MAX - MAX%int32(3))) * (MAX / int32(3)) / int32(2)
	var s_5 int32 = int32(7)
	_ = s_5
	s_5 = (int32(5) + (MAX - MAX%int32(5))) * (MAX / int32(5)) / int32(2)
	var s_15 int32 = int32(3)
	_ = s_15
	s_15 = int32((float32(int32(15)) + (local_float - float32(MAX%int32(15)))) * float32(MAX/int32(15)) / float32(int32(2)))
	s_15 ^= s_3 + int32(local_char) + int32(56)*int32((int64((s_15))|int64((s_3+int32(local_char)))))*int32((float32((s_15))*float32((s_3+int32(local_char))))) + int32((float32((global_int)) - float32((s_3 + int32(local_char)))))
	local_float /= global_float
	set_var(local_int, local_float, local_char)
	return int32(0)
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/7a994bc60754b773/runner.c:22
func main() {
	func1()
    fmt.Printf("%d:%.2f:%d\n", global_int, global_float, global_char)
	return
}
