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