#ifndef DLOPEN_DEMO_DYNAMIC_LIBRARY_HPP_INLCLUDED
#define DLOPEN_DEMO_DYNAMIC_LIBRARY_HPP_INLCLUDED

#include <string>
#include <dlfcn.h>

namespace org {
    namespace sqg {

        class dynamic_library {
        public:
            explicit dynamic_library(
                    std::string const &path,
                    int modes = RTLD_LAZY | RTLD_LOCAL);

            virtual ~dynamic_library();

            int modes() const { return _M_modes; }

            std::string path() const { return _M_path; }

            void* handle() { return _M_handle; }

            void* search(std::string const&);

        private:
            static void clear_dlerror();

            static void throw_dlerror(std::string const&);

        private:
            std::string _M_path;
            int _M_modes;
            void *_M_handle;
        };
    }
}

#endif // DLOPEN_DEMO_DYNAMIC_LIBRARY_HPP_INLCLUDED
