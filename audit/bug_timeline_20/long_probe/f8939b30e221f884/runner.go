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

type MyStruct struct {
	integerMember int32
	important     float32
}

var minee MyStruct
var whatif MyStruct
var why float32
var dis float32

// set_var - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/f8939b30e221f884/runner.c:10
func set_var(gi int32, gf float32, gc byte) {
	global_int = gi
	global_float = gf
	global_char = gc
}

// func1 - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/f8939b30e221f884/runner.c:11
func func1() int32 {
	var local_int int32 = int32(1)
	_ = local_int
	var local_float float32 = float32(1)
	_ = local_float
	var local_char byte = byte(int32(1))
	_ = local_char
	var p_p byte = byte(int32(1))
	_ = p_p
	var ft_ptr *float32 = &why
	_ = ft_ptr
	var int_ptr *int32 = &minee.integerMember
	_ = int_ptr
	local_int += global_int*int32(3) + int32(56) - int32((float64((*int_ptr)) - float64((whatif.integerMember)))) + int32(56) - (func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(uint64((local_int)) < uint64((global_int*int32(3)+int32(56)-int32((float64((*int_ptr))-float64((whatif.integerMember))))))))*(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int64((global_int*int32(3)+int32(56)-int32((float64((*int_ptr))-float64((whatif.integerMember)))))) <= int64((global_int))))
	local_float += 1
	minee.integerMember = local_int + int32(4) + int32(56) + (int32(uint16((minee.integerMember))) | int32(uint16((local_int)))) + int32(56) + int32((float64((minee.integerMember)) - float64((*int_ptr)))) + ((minee.integerMember) - (*int_ptr))
	*int_ptr ^= global_int*int32(2) + minee.integerMember + int32(56)*int32((uint64((*int_ptr))^uint64((*int_ptr)))) + int32((float32((*int_ptr)) + float32((*int_ptr)))) - (func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(uint16((minee.integerMember))) <= int32(uint16((minee.integerMember)))))
	*ft_ptr = float32(float64(float32(minee.integerMember)) + 2.19 + float64(int32(56)) + float64(float32((int32(uint16((*ft_ptr))) & int32(uint16((float64(float32(minee.integerMember)) + 2.19)))))) - float64(float32((uint64((*ft_ptr)) - uint64((local_float))))))
	global_float = *ft_ptr*float32(int32(2)) + float32(int32(56)) + float32((float64((*ft_ptr))*float64((local_float))))*((*ft_ptr)-(local_float)) + float32(int32(56)) - float32((float64((why)) + float64((*ft_ptr*float32(int32(2)) + float32(int32(56)) + float32((float64((*ft_ptr))*float64((local_float))))*((*ft_ptr)-(local_float))))))
	local_int = int32(global_char) + int32(1) + int32(56) - int32((float64((local_int))-float64((*int_ptr))))*(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(int16((*int_ptr))) < int32(int16((*int_ptr))))) + int32(56) - int32((int64((local_int)) + int64((int32(global_char) + int32(1) + int32(56) - int32((float64((local_int))-float64((*int_ptr))))*(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(int16((*int_ptr))) < int32(int16((*int_ptr))))))))) + (int32(int16((local_int))) + int32(int16((int32(global_char) + int32(1) + int32(56) - int32((float64((local_int))-float64((*int_ptr))))*(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(int16((*int_ptr))) < int32(int16((*int_ptr)))))))))
	whatif.important = float32(float64(*ft_ptr+minee.important) + 5.45 + float64(float32(int32(56))*float32((uint64((whatif.important))|uint64((local_float))))*((whatif.important)+(local_float))) - float64(float32((float64((whatif.important)) * float64((local_float))))) + float64(float32(int32(56))*((whatif.important)-(global_float))))
	local_float = float32(float64(*ft_ptr) + 2.01 + float64(whatif.important) + float64(global_float))
	set_var(local_int, local_float, local_char)
	return int32(func() byte {
		global_char += ^byte(int32(p_p))
		return global_char
	}())
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/f8939b30e221f884/runner.c:31
func main() {
	func1()
    fmt.Printf("%d:%.2f:%d\n", global_int, global_float, global_char)
	return
}
