cmake_minimum_required(VERSION 3.17)
project(diffie_hellman_tls)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_library(diffie_hellman STATIC include/diffie_hellman.h src/diffie_hellman.cpp)

add_executable(diffie_hellman_tls main.cpp)