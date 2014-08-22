cmake_minimum_required(VERSION 2.6)
project(Whois)
set(EXECUTABLE_NAME "whois")
ADD_DEFINITIONS(
    -std=c++11
    -Wall
    -Werror
    )
list(APPEND SRCS whois.c++ query.c++)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
if (Boost_FOUND AND Threads_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${EXECUTABLE_NAME} ${SRCS})
    target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES} ${Threads_LIBRARIES})
endif()



