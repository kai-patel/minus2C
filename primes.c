void is_prime(int x) {
    int i = 2;
    while(i < x) {
        print_int(x % i);
        if(x % i > 0) {
            return;
        }
        i = i + 1;
    }
    print_int(x);
}

int main() {
    int x = 1;
    while(x < 100) {
        is_prime(x);
        x = x + 1;
    }
    return 0;
}
