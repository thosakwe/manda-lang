add_library(manda_runtime
    value.hpp value.cpp
    object.cpp object.hpp worker.cpp worker.hpp task.cpp task.hpp vm.cpp vm.hpp module.cpp module.hpp type.cpp type.hpp symbol_table.cpp symbol_table.hpp module_compiler.cpp module_compiler.hpp)