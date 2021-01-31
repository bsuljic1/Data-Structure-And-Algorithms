#include <iostream>

int fib_petlja(int n){
    if(n <= 0) return n;
    int pretprosli(0), prosli(1), rezultat;
    for(int i = 2; i <= n; i++){
        rezultat = pretprosli + prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}

int fib2_0(int n){
    if(n <= 0) return n;
    return fib_petlja(n);
}

int nzd(int x, int y){
    if(y == 0) return x;
    return nzd(y, x %= y);
}

int main() {
    std::cout << "Pripremna Zadaca Za Vjezbu 5, Zadatak 1";
    return 0;
}
