int global_int;
float global_float;
char global_char;

int func1(void) {
    int x = 0;
    switch (0) {
        x = 7;
    }
    global_int = x;
    return 0;
}

int main(){func1();return global_int;}
