#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <jni.h>

#include "../src/dynamic_library.hpp"

template <typename T, size_t N>
size_t countof(T (&a)[N]) {
    return N;
}

class JNI {
private:
    typedef jint JNICALL (*JNI_CreateJavaVM_fptr)(JavaVM **pvm, void **penv, void *args);
public:
    explicit JNI(std::string path)
        :_M_jvmlib(path), _M_jvm_creator(NULL) {
        _M_jvm_creator = reinterpret_cast<JNI_CreateJavaVM_fptr>(
                _M_jvmlib.search("JNI_CreateJavaVM"));
    }

    virtual ~JNI() { }

    jint CreateJavaVM(JavaVM **pvm, void **penv, void *args) {
        return _M_jvm_creator(pvm, penv, args);
    }
private:
    org::sqg::dynamic_library _M_jvmlib;
    JNI_CreateJavaVM_fptr _M_jvm_creator;
};

class envvars {
public:
    static bool exists(std::string const& name) {
        char *value = ::getenv(name.c_str());
        return !!value;
    }

    static std::string get(std::string const &name) {
        char *value = ::getenv(name.c_str());
        if (!value)
            throw std::runtime_error(name + " is not defined in environment yet!!!");
        return value;
    }
};

int main(int argc, char* argv[]) try {
    using namespace std;
    using namespace org::sqg;

    ::JavaVM *vm = NULL;
    ::JNIEnv *env = NULL;

    ::JavaVMInitArgs args;
    std::string vmargs[] = {
        "-verbose:jni",
        "-Djava.class.path=/Library/Java/JavaVirtualMachines/jdk1.8.0_112.jdk/Contents/Home/jre"
    };


    memset(&args, 0, sizeof(args));
    args.version = JNI_VERSION_1_8;
    //args.version = JNI_VERSION_1_6;
    args.ignoreUnrecognized = JNI_TRUE;
    std::vector< ::JavaVMOption > opts;
    for (int i = 0, n = countof(vmargs); i < n; ++i) {
        JavaVMOption opt = { const_cast<char*>(vmargs[i].c_str()), NULL };
        opts.push_back(opt);
    }
    args.nOptions = opts.size();
    if (args.nOptions < 1) {
        args.options = NULL;
    } else {
        args.options = &opts[0];
    }

    cout << "JAVA_HOME = " << envvars::get("JAVA_HOME") << endl;
    std::string path = envvars::get("JAVA_HOME") + "/jre/lib/server/libjvm.dylib";
    cout << "JVM library = " << path << endl;

    JNI jni(path);
    if (JNI_OK != jni.CreateJavaVM(&vm, reinterpret_cast<void**>(&env), &args))
        throw std::runtime_error("CreateJavaVM failed!");

    return EXIT_SUCCESS;
} catch (std::exception const &e) {
    std::cerr << "[C++ exception] " << e.what() << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "[C++ exception] " << "<UNKNOWN CAUSE>" << std::endl;
    return EXIT_FAILURE;
}
