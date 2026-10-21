/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

// Warning (SwitchStmt):  /home/zzj/TeTRIS/audit/bug_timeline_20/switch_review/runner.c:7 : Unsupport case

package main

import "os"

var global_int int32
var global_float float32
var global_char byte

// func1 - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/switch_review/runner.c:5
// Warning (SwitchStmt):  /home/zzj/TeTRIS/audit/bug_timeline_20/switch_review/runner.c:7 : Unsupport case
func func1() int32 {
	var x int32 = int32(0)
	x = int32(7)
	switch int32(0) {
	}
	global_int = x
	return int32(0)
}

// main - transpiled function from  /home/zzj/TeTRIS/audit/bug_timeline_20/switch_review/runner.c:14
func main() {
	func1()
	os.Exit(int(global_int))
}
