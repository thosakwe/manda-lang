find_package(FLEX REQUIRED)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR})

flex_target(MandaScanner scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.hpp)

add_library(manda_analysis
    ${FLEX_MandaScanner_OUTPUTS}
    any_type.cpp any_type.hpp
    array_type.cpp array_type.hpp
    ast.cpp ast.hpp
    ast_printer.cpp ast_printer.hpp
    base_resolver.cpp base_resolver.hpp
    bool_type.cpp bool_type.hpp
    char_type.cpp char_type.hpp
    core_library.cpp core_library.hpp
    expr_ctx.cpp expr_ctx.hpp
    function_type.cpp function_type.hpp
    indenting_printer.cpp indenting_printer.hpp
    location.cpp location.hpp
    manda_exception.cpp manda_exception.hpp
    module.cpp module.hpp
    module_compiler.cpp module_compiler.hpp
    module.cpp module.hpp
    number_type.cpp number_type.hpp
    parser.cpp parser.hpp
    scanner.cpp scanner.hpp
    scope.cpp scope.hpp
    string_type.cpp string_type.hpp
    symbol.cpp symbol.hpp
    token.cpp token.hpp
    type.cpp type.hpp
    type_ctx.cpp type_ctx.hpp
    type_resolver.cpp type_resolver.hpp
    void_type.cpp void_type.hpp)

target_include_directories(manda_analysis PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
