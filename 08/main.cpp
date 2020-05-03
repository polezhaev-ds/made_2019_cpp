#include <iostream>
#include <iterator>
#include <algorithm>
#include "Vector.h"

int main() {

    Vector<int> v = {1, -1, 5, 6, 0, 4, 1, -5, -1};
    std::sort(v.begin(), v.end());
    for(auto x : v)
        std::cout << x << " ";
    std::cout << std::endl;
    Vector<int> vec(10, 5);
    auto oit = std::ostream_iterator<int>(std::cout, " ");
    for (std::size_t i = 0; i < vec.size(); ++i) {
        *oit++ = vec[i];
    }
    std::cout << std::endl;

    Vector<int> vec2(5);
    for (auto x : vec2)
        std::cout << x << " ";
    std::cout << std::endl;

    Vector<int> vec3 = {1, 2, 3, 4, 5};
    for (auto it = vec3.begin(); it != vec3.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    for (auto it = vec3.rbegin(); it != vec3.rend(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    for (auto i = 0; i < 10; ++i)
        vec3.push_back(i);
    std::cout << std::endl;
    for (auto it = vec3.begin(); it != vec3.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    std::sort(vec3.begin(), vec3.end());
    for (auto it = vec3.begin(); it != vec3.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;

    return 0;
}