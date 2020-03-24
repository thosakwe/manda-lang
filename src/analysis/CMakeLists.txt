find_package(FLEX REQUIRED)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR})

flex_target(MandaScanner scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.cpp
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.hpp)

add_library(manda_analysis
  ${FLEX_MandaScanner_OUTPUTS}
  ast.cpp ast.hpp
  ast_printer.cpp ast_printer.hpp
  expr_ctx.cpp expr_ctx.hpp
  indenting_printer.cpp indenting_printer.hpp
  location.cpp location.hpp
  manda_exception.cpp manda_exception.hpp
  parser.cpp parser.hpp
  scanner.cpp scanner.hpp
  token.cpp token.hpp
  type_ctx.cpp type_ctx.hpp)

target_include_directories(manda_analysis PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
