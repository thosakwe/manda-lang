find_path(JIT_INCLUDE_DIR NAMES "jit/jit-plus.h" "jit/jit.h")
add_library(manda_runtime
    ../../include/manda_api.h
    object.cpp object.hpp
    worker.cpp worker.hpp
    task.cpp task.hpp
    vm.cpp vm.hpp
    module.cpp module.hpp
    type.cpp type.hpp
    symbol_table.cpp symbol_table.hpp
    module_compiler.cpp module_compiler.hpp
    late_bound_object.cpp late_bound_object.hpp
    ast_function.cpp ast_function.hpp
    number.cpp number.hpp
    vm_options.cpp vm_options.hpp
    interpreter.cpp interpreter.hpp
    ansi_printer.cpp ansi_printer.hpp
    tuple.cpp tuple.hpp
    void.cpp void.hpp
    function.cpp function.hpp
    builtin_function.cpp builtin_function.hpp
    core_library.cpp core_library.hpp
    bool.cpp bool.hpp
    string.cpp string.hpp
    char.cpp char.hpp
    type_resolver.cpp type_resolver.hpp
    object_resolver.cpp object_resolver.hpp
    any_type.cpp any_type.hpp
    jit_compiled_function.cpp jit_compiled_function.hpp
    manda_api_impl.hpp manda_api_impl.cpp
    garbage_collector.cpp garbage_collector.hpp
    generic_scope.hpp
    base_resolver.cpp base_resolver.hpp
    )
target_include_directories(manda_runtime PUBLIC ${JIT_INCLUDE_DIR})
