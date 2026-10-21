/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "os"
import "unsafe"
import "github.com/elliotchance/c2go/noarch"

// f - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/address_review/r/runner.c:1
func f() int32 {
	var x int32 = int32(1)
	return *noarch.Int32ToInt32((x + int32(1)))
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/address_review/r/runner.c:2
func main() {
	os.Exit(int(f()))
}
