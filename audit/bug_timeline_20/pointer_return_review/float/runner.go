/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "os"
import "unsafe"

// f - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/pointer_return_review/float/runner.c:1
func f() float32 {
	var x float32 = float32(int32(1))
	var p *float32 = &x
	x += func() float32 {
		func() float32 {
			*p += *p
			return *p
		}()
		return &p
	}()
	return x
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/pointer_return_review/float/runner.c:2
func main() {
	os.Exit(int(int32(f())))
}
