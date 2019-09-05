find_package(FLEX REQUIRED)
flex_target(MandaScanner scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.cpp
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.hpp)
add_library(manda_analysis
    type_ctx.cpp type_ctx.hpp
    object.cpp object.hpp
    ast.cpp ast.hpp
    location.cpp location.hpp
    expr_ctx.cpp expr_ctx.hpp
    token.cpp token.hpp
    parser.cpp parser.hpp)
