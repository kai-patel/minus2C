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

function cplus ( int a ) {
    int cplusa ( int b) { return a+b; }
    return cplusa ;
}

int main() {
    print_int(times2(times2(10)));
    print_int(doublefunc(times2));
    print_int(doublefunc(square));
    print_int(cplus(5)(6));
    return 0;
}

