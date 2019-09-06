add_library(manda_runtime
    value.hpp value.cpp
    object.cpp object.hpp
    worker.cpp worker.hpp
    task.cpp task.hpp
    vm.cpp vm.hpp
    module.cpp module.hpp
    type.cpp type.hpp
    symbol_table.cpp symbol_table.hpp
    module_compiler.cpp module_compiler.hpp late_bound_object.cpp late_bound_object.hpp function.cpp function.hpp number.cpp number.hpp vm_options.cpp vm_options.hpp interpreter.cpp interpreter.hpp ansi_printer.cpp ansi_printer.hpp tuple.cpp tuple.hpp)