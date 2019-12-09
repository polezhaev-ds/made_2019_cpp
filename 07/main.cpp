#include <iostream>
#include <cassert>
#include "format.h"

int main() {

    try {
        auto text = Format("{1}+{1} = {0}", 2, "one");
        assert(text == "one+one = 2");

        std::cout << Format("{0}{1}{2}{3}{4}{5}", 1, true, '!', 22ul, "qwe", 2.3) << std::endl;
        std::cout << "Some demo outputs of format string:" << std::endl;
        std::cout << Format("234234") << std::endl;
        std::cout << Format("") << std::endl;
        std::cout << Format("abc") << std::endl;
        std::cout << Format("{0}", 4) << std::endl;
        std::cout << Format("{0}", 4.123) << std::endl;
        std::cout << Format("{0}", true) << std::endl;
        std::cout << Format("{0}", false) << std::endl;
        std::cout << Format("{0}", "abc") << std::endl;
        std::cout << Format("{0}", "") << std::endl;
        std::cout << Format("{0}", "  ") << std::endl;
        std::cout << Format("asdasd {0} 333asdasd", "abc") << std::endl;
        std::cout << Format("asdasd {0} 333asdasd {0}", "abc") << std::endl;
        std::cout << Format("{0}, {1}! {2}{2}", "Hello", "world", 777) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Runtime error exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }

    return 0;
}