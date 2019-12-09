#include <iostream>
#include "BigInt.h"

int main() {
    BigInt a("53453453453444444444444444444444445");
    BigInt b("-5345345345343454444444444444444444433333335345");
    BigInt c("45345345345456545756756734534534534534534534534534");
    BigInt y("-99999999999999999999999999999999999999999999999999999999999999999999");
    BigInt x = -a + b - c;
    x += b;
    x -= 5234234;
    x += 1234567890;
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "x != y: " << (x != y) << std::endl;
    std::cout << "x == y: " << (x == y) << std::endl;
    std::cout << "x < y: " << (x < y) << std::endl;
    std::cout << "x > y: " << (x > y) << std::endl;
    std::cout << "x <= y: " << (x <= y) << std::endl;
    std::cout << "x >= y: " << (x >= y) << std::endl;

    const BigInt u("-345345345345");
    const BigInt w("1111345345345222");
    std::cout << "u = " << u << std::endl;
    std::cout << "w = " << w << std::endl;
    std::cout << "u + w = " << u + w << std::endl;
    std::cout << "u - w = " << u - w << std::endl;
    std::cout << "-u = " << -u << std::endl;
    std::cout << "-w = " << -w << std::endl;
    std::cout << "u - 1234 = " << u - 1234 << std::endl;
    std::cout << "u - 1234 = " << u + 1234 << std::endl;
    std::cout << "1234 + w = " << 1234 + w << std::endl;
    std::cout << "1234 - w = " << 1234 - w << std::endl;

    BigInt z;
    std::cout << std::endl;
    std::cout << "Enter big integer z = ";
    std::cin >> z;
    std::cout << "z + 1 = " << z + 1 << std::endl;

    return 0;
}