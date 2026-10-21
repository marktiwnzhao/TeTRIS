/*
	Package main - transpiled by c2go version: v0.26.0 Erbium 2020-03-17

	If you have found any issues, please raise an issue at:
	https://github.com/elliotchance/c2go/
*/

package main

import "os"

// main - transpiled function from  /home/zzj/TeTRIS/audit/repeat_runs_review/pp_probe/runner.c:1
func main() {
	var x byte = byte(int32(1))
	var pp byte = func() byte {
		x ^= byte(byte(int32(2)))
		return x
	}()
	os.Exit(int(int32(pp)))
}
