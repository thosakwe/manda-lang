add_library(manda_runtime
    vm_options.cpp vm_options.hpp
    interpreter.cpp interpreter.hpp
    ansi_printer.cpp ansi_printer.hpp)
target_include_directories(manda_runtime PUBLIC ${JIT_INCLUDE_DIR})
