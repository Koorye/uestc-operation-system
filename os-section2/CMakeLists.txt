PROJECT (os2)

find_package(Threads)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

ADD_EXECUTABLE(os2 main.c producer.h producer.c consumer.h consumer.c buffer_pool.h buffer_pool.c)
target_link_libraries(os2 ${CMAKE_THREAD_LIBS_INIT})