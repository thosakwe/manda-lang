find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

flex_target(WLexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/w_lexer.cpp)
bison_target(WParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/w_parser.cpp)
add_flex_bison_dependency(WLexer WParser)

add_library(w
  ${BISON_WParser_OUTPUTS}
  ${FLEX_WLexer_OUTPUTS}
)
target_compile_options(w PUBLIC -Wno-deprecated-register)
