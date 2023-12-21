cmake_minimum_required(VERSION 3.24)

project(Pub 
VERSION 1.0 
LANGUAGES CXX
)

#set your Mosquitto dir
set(Mosquitto D:/mosquitto/devel)

#set Unicode
add_definitions(-DUNICODE -D_UNICODE)

add_executable(Pub main.cpp pub.hpp pub.cpp)

target_include_directories(Pub PRIVATE
    ${Mosquitto}
)

target_link_directories(Pub PRIVATE
    ${Mosquitto}
)


