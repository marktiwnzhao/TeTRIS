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

// set_var - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/e631847243675b01/runner.c:4
func set_var(gi int32, gf float32, gc byte) {
	global_int = gi
	global_float = gf
	global_char = gc
}

// func1 - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/e631847243675b01/runner.c:5
func func1() int32 {
	var local_int int32 = int32(2)
	_ = local_int
	var local_float float32 = float32(1)
	_ = local_float
	var local_char byte = byte(int32(1))
	_ = local_char
	var a int32 = int32(1)
	_ = a
	var b int32 = int32(2)
	_ = b
	var c int32 = int32(3)
	_ = c
	var pp byte = byte(int32(local_char) + int32(2))
	_ = pp
	var d int32 = int32(3)
	_ = d
	var e int32 = int32(2)
	_ = e
	var f int32 = local_int
	_ = f
	var g int32 = int32(3)
	_ = g
	var h int32 = int32(1)
	_ = h
	var i int32 = int32(local_float)
	_ = i
	var j int32 = int32(local_float)
	_ = j
	var k int32 = -int32(1)
	_ = k
	var l int32 = int32(1)
	_ = l
	var m int32 = int32(1)
	_ = m
	var n int32 = int32(1)
	_ = n
	var o int32 = int32(1)
	_ = o
	var p int32 = int32(1)
	_ = p
	d += c + int32(pp) + int32(56)*(int32(int16((n)))-int32(int16((i)))) + int32((float64((n)) + float64((i)))) + int32(56)*int32((float64((c+int32(pp)+int32(56)*(int32(int16((n)))-int32(int16((i))))+int32((float64((n))+float64((i))))))*float64((c+int32(pp)+int32(56)*(int32(int16((n)))-int32(int16((i))))+int32((float64((n))+float64((i)))))))) - (int32(uint16((c + int32(pp) + int32(56)*(int32(int16((n)))-int32(int16((i)))) + int32((float64((n)) + float64((i))))))) | int32(uint16((c + int32(pp) + int32(56)*(int32(int16((n)))-int32(int16((i)))) + int32((float64((n)) + float64((i))))))))
	for ; a < h; a++ {
		a += 1
		d = d - e
		c ^= j
		if e > int32(20) {
			i = i + int32(10) + int32(56)*int32((uint64((g))-uint64((i+int32(10))))) + (int32(int16((n))) | int32(int16((n)))) + int32(56) + (int32(uint16((i))) * int32(uint16((i + int32(10) + int32(56)*int32((uint64((g))-uint64((i+int32(10))))) + (int32(int16((n))) | int32(int16((n))))))))
			c += 1
		}
	}
	for func() int32 {
		defer func() {
			b -= 1
		}()
		return b
	}() != 0 {
		var step int32 = int32(1)
		_ = step
		local_float /= global_float
	}
	local_int = k ^ (c + int32(56) + int32((uint64((local_int)) ^ uint64((k ^ c)))) + int32((float32((local_int)) * float32((k ^ c)))) + int32((float32((k ^ c)) + float32((d)))) + int32(56) - int32((float32((k^(c+int32(56)+int32((uint64((local_int))^uint64((k^c))))+int32((float32((local_int))*float32((k^c))))+int32((float32((k^c))+float32((d)))))))-float32((k))))*(func(val bool) int32 {
		if val {
			return 1
		} else {
			return 0
		}
	}(int32(uint16((k^(c+int32(56)+int32((uint64((local_int))^uint64((k^c))))+int32((float32((local_int))*float32((k^c))))+int32((float32((k^c))+float32((d)))))))) >= int32(uint16((k))))) + int32((int64((k ^ (c + int32(56) + int32((uint64((local_int)) ^ uint64((k ^ c)))) + int32((float32((local_int)) * float32((k ^ c)))) + int32((float32((k ^ c)) + float32((d))))))) | int64((l)))))
	set_var(local_int, local_float, local_char)
	return int32(0)
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/long_probe/e631847243675b01/runner.c:34
func main() {
	func1()
    fmt.Printf("%d:%.2f:%d\n", global_int, global_float, global_char)
	return
}
