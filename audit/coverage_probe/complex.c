struct S { int x; double y; };
int probe(int x) { struct S a={x,2.5}; switch(x) {case 1: return a.x; case 2: return (int)a.y; default: while(x>0) x--; return x;} }
