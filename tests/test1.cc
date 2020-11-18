#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "../src/foo.h"

#include "../src/dynamic_library.hpp"

template <typename T, size_t N> size_t countof(T (&a)[N]) { return N; }

int main(int argc, char* argv[]) try {
    using namespace std;
    using namespace org::sqg;

    typedef void (*foo_fnptr)();

    std::string paths[] = {
        ".libs/libfoo1.dylib",
        ".libs/libfoo2.dylib",
    };
    dynamic_library lib1(paths[0]);
    dynamic_library lib2(paths[1]);
    reinterpret_cast<foo_fnptr>(lib1.search("foo"))();
    reinterpret_cast<foo_fnptr>(lib2.search("foo"))();
    reinterpret_cast<foo_fnptr>(lib1.search("foo"))();
    reinterpret_cast<foo_fnptr>(lib2.search("foo"))();

    return EXIT_SUCCESS;
} catch (std::exception const& e) {
    std::cerr << "[C++ exception] " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "[C++ exception] " << "<UNKNOWN CAUSE>" << std::endl;
    return EXIT_FAILURE;
}
