cmake_minimum_required(VERSION 3.10)

project(GL-robots-db)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wno-deprecated-declarations")
set(CMAKE_EXE_LINKER_FLAGS "-s")
set_property(
    DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/offsets.db"
    "${CMAKE_CURRENT_BINARY_DIR}/robots.db"
)

include_directories("include/" "/usr/include/jsoncpp/")

file(GLOB server_sources "src/server/*.cpp")
file(GLOB client_sources "src/client/*.cpp")

add_executable(server ${server_sources})
add_executable(client ${client_sources})

target_link_libraries(server zmq jsoncpp)
target_link_libraries(client zmq jsoncpp)

