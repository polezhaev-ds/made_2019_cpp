#include <iostream>
#include <random>
#include <vector>
#include "ThreadPool.h"

struct A {};

void foo(const A&) {}

int main() {
    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    task2.get();

    return 0;
}