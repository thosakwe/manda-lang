# find_package(FLEX REQUIRED)

# flex_target(WLexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/w_lexer.cpp
#   DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/w_lexer.hpp)
# include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(w
  # ${FLEX_WLexer_OUTPUTS}
  type.hpp type.cpp
  type_reference.hpp type_reference.cpp
  object.cpp object.hpp)
target_compile_options(w PUBLIC -Wno-deprecated-register)
