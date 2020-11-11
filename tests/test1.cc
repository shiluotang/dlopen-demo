#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include <dlfcn.h>

#include "../src/foo.h"

class dynamic_library {
public:
    explicit dynamic_library(
            std::string const &path,
            int modes = RTLD_LAZY | RTLD_LOCAL)
        : _M_path(path),
        _M_modes(modes),
        _M_handle(NULL) {
        _M_handle = ::dlopen(path.c_str(), modes);
        if (!_M_handle)
            throw_dlerror("dlopen");
    }
    virtual ~dynamic_library() {
        if (_M_handle) {
            if (::dlclose(_M_handle) != 0) {
                throw_dlerror("dlclose");
            }
        }
    }

    int modes() const { return _M_modes; }

    std::string path() const { return _M_path; }

    void* handle() { return _M_handle; }

    void* search(std::string const &name) {
        clear_dlerror();
        void *symbol = ::dlsym(_M_handle, name.c_str());
        if (!symbol)
            throw_dlerror("dlsym");
        return symbol;
    }

private:
    static void clear_dlerror() { (void) ::dlerror(); }
    static void throw_dlerror(std::string const &reason) {
        char const *error = ::dlerror();
        if (error)
            throw std::runtime_error(error);
        throw std::runtime_error(reason);
    }

private:
    std::string _M_path;
    int _M_modes;
    void *_M_handle;
};

template <typename T, size_t N> size_t countof(T (&a)[N]) { return N; }

int main(int argc, char* argv[]) try {
    using namespace std;

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
