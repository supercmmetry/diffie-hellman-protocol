cmake_minimum_required(VERSION 3.17)
project(diffie_hellman_tls)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)

add_library(diffie_hellman_tls STATIC
        include/diffie_hellman.h
        src/diffie_hellman.cpp
        include/encryption.h
        src/encryption.cpp
        include/hash_validator.h
        src/hash_validator.cpp src/session.cpp src/protocol.cpp)

add_executable(diffie_hellman_demo main.cpp include/session.h include/errors.h include/control.h include/protocol.h)
target_link_libraries(diffie_hellman_demo PUBLIC diffie_hellman_tls)