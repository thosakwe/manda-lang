#find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
#bison_target(MandaParser parser.y
#  ${CMAKE_CURRENT_BINARY_DIR}/manda_bison_parser.cpp
#  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_bison_parser.hpp)
flex_target(MandaScanner scanner.l
  ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.cpp
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_flex_scanner.hpp)
#add_flex_bison_dependency(MandaScanner MandaParser)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR})
add_library(manda_analysis
#  ${BISON_MandaParser_OUTPUTS}
  ${FLEX_MandaScanner_OUTPUTS}
  type_ctx.cpp type_ctx.hpp
  object.cpp object.hpp
  ast.cpp ast.hpp
  location.cpp location.hpp
  expr_ctx.cpp expr_ctx.hpp
  token.cpp token.hpp
  parser.cpp parser.hpp
  scanner.cpp scanner.hpp
  ast_printer.cpp ast_printer.hpp)
target_include_directories(manda_analysis PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")