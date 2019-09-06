add_library(manda_runtime
    value.hpp value.cpp
    object.cpp object.hpp
    worker.cpp worker.hpp
    task.cpp task.hpp
    vm.cpp vm.hpp
    module.cpp module.hpp
    type.cpp type.hpp
    symbol_table.cpp symbol_table.hpp
    module_compiler.cpp module_compiler.hpp late_bound_object.cpp late_bound_object.hpp ast_function.cpp ast_function.hpp number.cpp number.hpp vm_options.cpp vm_options.hpp interpreter.cpp interpreter.hpp ansi_printer.cpp ansi_printer.hpp tuple.cpp tuple.hpp void.cpp void.hpp function.cpp function.hpp builtin_function.cpp builtin_function.hpp core_library.cpp core_library.hpp)