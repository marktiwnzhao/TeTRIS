/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "os"

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/expression_review/comma/runner.c:1
func main() {
	var c int32 = int32(1)
	var r int32 = func() int32 {
		c * int32(2)
		tempVar := &c
		return *tempVar
	}()
	os.Exit(int(r - int32(1)))
}
