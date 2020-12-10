int main() {
    int x = 0;
    while(x < 10) {
        print_int(x);
        x = x + 1;
        if(x == 5) break;
    }
    return 0;
}
