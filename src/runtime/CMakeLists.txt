find_path(JIT_INCLUDE_DIR NAMES "jit/jit-plus.h" "jit/jit.h")
add_library(manda_runtime
    ../../include/manda_api.h
    module.cpp module.hpp
    type.cpp type.hpp
    module_compiler.cpp module_compiler.hpp
    ast_function.cpp ast_function.hpp
    number_type.cpp number_type.hpp
    vm_options.cpp vm_options.hpp
    interpreter.cpp interpreter.hpp
    ansi_printer.cpp ansi_printer.hpp
    tuple_type.cpp tuple_type.hpp
    void_type.cpp void_type.hpp
    function_type.cpp function_type.hpp
    core_library.cpp core_library.hpp
    bool_type.cpp bool_type.hpp
    string_type.cpp string_type.hpp
    char_type.cpp char_type.hpp
    type_resolver.cpp type_resolver.hpp
    any_type.cpp any_type.hpp
    scope.hpp scope.cpp
    base_resolver.cpp base_resolver.hpp
    array_type.cpp array_type.hpp symbol.cpp symbol.hpp)
target_include_directories(manda_runtime PUBLIC ${JIT_INCLUDE_DIR})
