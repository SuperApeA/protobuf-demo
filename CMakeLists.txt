cmake_minimum_required(VERSION 3.0)

project(myProtocolBuf)
set(CMAKE_CXX_STANDARD 11)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
link_libraries(${Protobuf_LIBRARY})

aux_source_directory(./src DIR_SRC)

add_executable(client client.cpp ${DIR_SRC})
add_executable(server server.cpp ${DIR_SRC})

add_executable(clientTest ./Test/clientTest.cpp ${DIR_SRC})
add_executable(serverTest ./Test/serverTest.cpp ${DIR_SRC})

add_executable(c ./Test/c.cpp ${DIR_SRC})
add_executable(s ./Test/s.cpp ${DIR_SRC})