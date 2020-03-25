add_library(manda_runtime
    ansi_printer.cpp ansi_printer.hpp
    vm_options.cpp vm_options.hpp error_reporter.cpp error_reporter.hpp)
target_include_directories(manda_runtime PUBLIC ${JIT_INCLUDE_DIR})
