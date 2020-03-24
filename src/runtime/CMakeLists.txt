add_library(manda_runtime
    ansi_printer.cpp ansi_printer.hpp
    interpreter.cpp interpreter.hpp
    vm_options.cpp vm_options.hpp)
target_include_directories(manda_runtime PUBLIC ${JIT_INCLUDE_DIR})
