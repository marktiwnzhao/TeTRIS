/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "unsafe"

var global_int int32
var global_float float32
var global_char byte

type MyStruct struct {
	integerMember int32
	important     float32
}

var minee MyStruct
var whatif MyStruct
var why float32
var dis float32

// set_var - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/issue_921_replay/id_000865/runner.c:10
func set_var(gi int32, gf float32, gc byte) {
	global_int = gi
	global_float = gf
	global_char = gc
}

// func1 - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/issue_921_replay/id_000865/runner.c:11
func func1() int32 {
	var local_int int32 = int32(1)
	var local_float float32 = float32(1)
	var local_char byte = byte(int32(1))
	var p_p byte = byte(int32(1))
	var ft_ptr *float32 = &why
	var int_ptr *int32 = &minee.integerMember
	local_int += global_int*int32(3) + int32(56) - (int32(uint16((local_int))) - int32(uint16((global_int * int32(3))))) + int32((float32((local_int)) * float32((global_int * int32(3))))) + int32(56) - int32((float64((*int_ptr)) + float64((local_int)))) - int32((float64((*int_ptr)) + float64((local_int))))
	local_float += 1
	minee.integerMember = local_int + int32(4) + int32(56)*int32((float64((minee.integerMember))+float64((minee.integerMember))))*(int32(int16((local_int+int32(4))))*int32(int16((global_int))))*int32((float64((local_int+int32(4)))+float64((global_int)))) + int32(56) + int32((float64((minee.integerMember)) * float64((*int_ptr)))) - int32((float64((*int_ptr)) + float64((minee.integerMember))))
	local_float -= -dis + float32(int32(56)) - ((-dis) - (why)) + ((whatif.important) + (why))
	*ft_ptr = float32(float64(float32(minee.integerMember)) + 2.19 + float64(int32(56)) - float64(float32((float64((minee.important)) * float64((minee.important))))) + float64(((why)+(minee.important))*((why)-(minee.important))) + float64(float32(int32(56))*((*ft_ptr)*(dis))))
	global_float = *ft_ptr*float32(int32(2)) + float32(int32(56))*((whatif.important)*(minee.important)) + float32((int32(uint16((whatif.important))) & int32(uint16((minee.important))))) + float32((uint64((whatif.important)) + uint64((minee.important)))) + float32(int32(56)) - float32((float64((global_float))-float64((*ft_ptr*float32(int32(2))+float32(int32(56))*((whatif.important)*(minee.important))+float32((int32(uint16((whatif.important)))&int32(uint16((minee.important)))))+float32((uint64((whatif.important))+uint64((minee.important))))))))*((global_float)-(*ft_ptr)) + float32((int32(uint16((dis))) & int32(uint16((*ft_ptr)))))
	local_int = func() int32 {
		func() int32 {
			*int_ptr /= *int_ptr
			return *int_ptr
		}()
		return &int_ptr
	}()
	whatif.important = float32(float64(*ft_ptr+minee.important) + 5.45 + float64(int32(56)) - float64(float32((func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32((global_float)) < int32((minee.important)))))) + float64(float32(int32(56))*float32((int64((whatif.important))+int64((float64(*ft_ptr+minee.important)+5.45+float64(int32(56))-float64(float32((func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32((global_float)) < int32((minee.important))))))))))*float32((float64((whatif.important))+(float64(*ft_ptr+minee.important)+5.45+float64(int32(56))-float64(float32((func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32((global_float)) < int32((minee.important)))))))))*float32((int32(int16((local_float)))|int32(int16((why)))))))
	local_float = float32(float64(*ft_ptr) + 2.01 + float64(whatif.important) + float64(global_float) + float64(int32(56)) - float64(((local_float) * float32((float64(*ft_ptr) + 2.01 + float64(whatif.important) + float64(global_float))))))
	set_var(local_int, local_float, local_char)
	return int32(0)
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/issue_921_replay/id_000865/runner.c:31
func main() {
	func1()
	return
}
