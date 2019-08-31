# find_package(FLEX REQUIRED)

# flex_target(MandaLexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/manda_lexer.cpp
#   DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/manda_lexer.hpp)
# include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(manda
  # ${FLEX_MandaLexer_OUTPUTS}
  type.hpp type.cpp
  type_reference.hpp type_reference.cpp
  object.cpp object.hpp)
target_compile_options(manda PUBLIC -Wno-deprecated-register)
