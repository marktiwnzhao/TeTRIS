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

// set_var - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/issue_921_replay/id_000801/runner.c:10
func set_var(gi int32, gf float32, gc byte) {
	global_int = gi
	global_float = gf
	global_char = gc
}

// func1 - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/issue_921_replay/id_000801/runner.c:11
func func1() int32 {
	var local_int int32 = int32(1)
	var local_float float32 = float32(1)
	var local_char byte = byte(int32(1))
	var p_p byte = byte(int32(1))
	var ft_ptr *float32 = &why
	var int_ptr *int32 = &minee.integerMember
	local_int += global_int*int32(3) + int32(56) + int32((float32((global_int * int32(3))) - float32((local_int)))) + (int32(int16((local_int)))-int32(int16((global_int*int32(3)))))*int32((float64((local_int))*float64((local_int))))
	local_float += 1
	minee.integerMember = local_int + int32(4) + int32(56) + int32((float32((minee.integerMember))+float32((whatif.integerMember))))*int32((uint32((*int_ptr))*uint32((local_int+int32(4))))) - (func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(uint16((*int_ptr))) < int32(uint16((local_int + int32(4)))))) + int32(56) + int32((float32((minee.integerMember))-float32((local_int+int32(4)+int32(56)+int32((float32((minee.integerMember))+float32((whatif.integerMember))))*int32((uint32((*int_ptr))*uint32((local_int+int32(4)))))-(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(uint16((*int_ptr))) < int32(uint16((local_int+int32(4))))))))))*int32((float64((minee.integerMember))-float64((local_int+int32(4)+int32(56)+int32((float32((minee.integerMember))+float32((whatif.integerMember))))*int32((uint32((*int_ptr))*uint32((local_int+int32(4)))))-(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(uint16((*int_ptr))) < int32(uint16((local_int+int32(4))))))))))
	*int_ptr = global_int*int32(2) + minee.integerMember + int32(56)*(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(int16((*int_ptr))) <= int32(int16((whatif.integerMember)))))
	*ft_ptr = float32(float64(float32(minee.integerMember)) + 2.19 + float64(int32(56)) - float64(((*ft_ptr) * float32((float64(float32(minee.integerMember)) + 2.19)))) - float64(float32((float64((*ft_ptr)) * (float64(float32(minee.integerMember)) + 2.19)))))
	*int_ptr = -(^(local_int * global_int))
	local_int = int32(global_char) + int32(1) + int32(56)*int32((uint32((local_int))|uint32((int32(global_char)+int32(1))))) + int32(56) - int32((int64((local_int)) - int64((int32(global_char) + int32(1) + int32(56)*int32((uint32((local_int))|uint32((int32(global_char)+int32(1))))))))) - int32((float64((local_int)) * float64((int32(global_char) + int32(1) + int32(56)*int32((uint32((local_int))|uint32((int32(global_char)+int32(1))))))))) + int32((float64((int32(global_char) + int32(1) + int32(56)*int32((uint32((local_int))|uint32((int32(global_char)+int32(1))))))) - float64((int32(global_char) + int32(1) + int32(56)*int32((uint32((local_int))|uint32((int32(global_char)+int32(1)))))))))
	whatif.important = float32(float64(*ft_ptr+minee.important) + 5.45 + float64(float32(int32(56))*((whatif.important)+(minee.important))))
	local_float = func() float32 {
		func() float32 {
			*ft_ptr -= *ft_ptr
			return *ft_ptr
		}()
		return &ft_ptr
	}()
	set_var(local_int, local_float, local_char)
	return int32(0)
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/issue_921_replay/id_000801/runner.c:31
func main() {
	func1()
	return
}
