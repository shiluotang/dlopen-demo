#include <cstdlib>

#include <string>
#include <stdexcept>

#include <dlfcn.h>

#include "dynamic_library.hpp"


namespace org {
    namespace sqg {

        void dynamic_library::throw_dlerror(std::string const &reason) {
            char const *error = ::dlerror();
            if (error)
                throw std::runtime_error(error);
            throw std::runtime_error(reason);
        }

        void dynamic_library::clear_dlerror() {
            (void) ::dlerror();
        }

        void* dynamic_library::search(std::string const &name) {
            clear_dlerror();
            void *symbol = ::dlsym(_M_handle, name.c_str());
            if (!symbol)
                throw_dlerror("dlsym");
            return symbol;
        }

        dynamic_library::dynamic_library(
                std::string const &path,
                int modes)
            : _M_path(path),
            _M_modes(modes),
            _M_handle(NULL) {
            _M_handle = ::dlopen(path.c_str(), modes);
            if (!_M_handle)
                throw_dlerror("dlopen");
        }


        dynamic_library::~dynamic_library() {
            if (_M_handle) {
                if (::dlclose(_M_handle) != 0) {
                    throw_dlerror("dlclose");
                }
            }
        }
    }
}

