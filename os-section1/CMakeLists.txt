project(os1)

find_package(Threads)
add_executable(os1 main.c philosopher.h philosopher.c)
target_link_libraries(os1 ${CMAKE_THREAD_LIBS_INIT})
