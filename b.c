int times2(int a) {
    return a + a;
}

int square(int a) {
    return a*a;
}

int doublefunc(function f) {
    int x = 10;
    return f(f(x));
}

int main() {
    print_int(doublefunc(times2));
    print_int(doublefunc(square));
    return 0;
}

