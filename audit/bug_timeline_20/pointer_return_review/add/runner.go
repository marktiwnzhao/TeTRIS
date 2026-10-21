/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "os"
import "unsafe"

// f - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/pointer_return_review/add/runner.c:1
func f() int32 {
	var x int32 = int32(1)
	var p *int32 = &x
	x += func() int32 {
		func() int32 {
			*p += *p
			return *p
		}()
		return &p
	}()
	return x
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/pointer_return_review/add/runner.c:2
func main() {
	os.Exit(int(f()))
}
