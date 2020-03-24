find_package(FLEX REQUIRED)
flex_target(MandaScanner scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.cpp
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.hpp)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR})
add_library(manda_analysis
  ${FLEX_MandaScanner_OUTPUTS}
  type_ctx.cpp type_ctx.hpp
  ast.cpp ast.hpp
  location.cpp location.hpp
  expr_ctx.cpp expr_ctx.hpp
  token.cpp token.hpp
  parser.cpp parser.hpp
  scanner.cpp scanner.hpp
  ast_printer.cpp ast_printer.hpp
  manda_exception.cpp manda_exception.hpp
  indenting_printer.cpp indenting_printer.hpp)
target_include_directories(manda_analysis PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")