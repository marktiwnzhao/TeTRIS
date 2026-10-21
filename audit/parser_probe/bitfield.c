struct S { unsigned int x:3; }; int func1(){struct S s={0}; s.x=5; return s.x;}
