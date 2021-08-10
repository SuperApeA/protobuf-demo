cmake_minimum_required(VERSION 3.0)

project(myProtocolBuf)
set(CMAKE_CXX_STANDARD 11)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
link_libraries(${Protobuf_LIBRARY})

FILE(GLOB SRC ./src/*.cc ./src/*.h)

add_executable(client client.cpp ${SRC})
add_executable(server server.cpp ${SRC})

add_executable(clientTest ./Test/clientTest.cpp ${SRC})
add_executable(serverTest ./Test/serverTest.cpp ${SRC})

add_executable(c ./Test/c.cpp ${SRC})
add_executable(s ./Test/s.cpp ${SRC})