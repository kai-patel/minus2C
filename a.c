int factorial(int n) {
    if(n == 0) return 1;
    return n * factorial(n-1);
}

int main() {
    int x = 10;
    print_str("Factorial Result: ");
    print_int(factorial(x));
    return 0;
}
