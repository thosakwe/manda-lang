add_subdirectory(analysis)
add_subdirectory(runtime)

add_executable(manda bin/main.cpp bin/defs.hpp)
target_link_libraries(manda manda_analysis manda_runtime)