/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "os"
import "unsafe"

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/expression_review/pointer_compound/runner.c:1
func main() {
	var x int32 = int32(1)
	var p *int32 = &x
	var y int32 = func() int32 {
		func() int32 {
			*p ^= *p
			return *p
		}()
		return &p
	}()
	os.Exit(int(y))
}
