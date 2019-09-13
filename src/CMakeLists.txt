find_package(Readline REQUIRED)
find_library(JIT_LIBRARY jit)
find_library(JIT_PLUS_LIBRARY jitplus)
add_subdirectory(analysis)
add_subdirectory(runtime)
add_executable(manda main.cpp defs.hpp)
target_include_directories(manda PUBLIC ${READLINE_INCLUDE_DIRS})
target_link_libraries(manda
    manda_analysis manda_runtime
    ${READLINE_LIBRARIES}
    ${JIT_LIBRARY} ${JIT_PLUS_LIBRARY})

add_executable(manda_dump_tree dump_tree.cpp)
target_include_directories(manda_dump_tree PUBLIC ${READLINE_INCLUDE_DIRS})
target_link_libraries(manda_dump_tree
    manda_analysis
    ${READLINE_LIBRARIES}
    ${JIT_LIBRARY} ${JIT_PLUS_LIBRARY})

add_executable(manda_dump_tokens dump_tokens.cpp)
target_include_directories(manda_dump_tokens PUBLIC ${READLINE_INCLUDE_DIRS})
target_link_libraries(manda_dump_tokens
    manda_analysis
    ${READLINE_LIBRARIES}
    ${JIT_LIBRARY} ${JIT_PLUS_LIBRARY})