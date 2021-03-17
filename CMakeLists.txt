cmake_minimum_required(VERSION 3.17)
project(diffie_hellman_tls)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include)
add_subdirectory(other)

add_library(diffie_hellman_tls STATIC
        include/session.h
        include/errors.h
        include/control.h
        include/protocol.h
        include/diffie_hellman.h
        src/diffie_hellman.cpp
        include/encryption.h
        src/encryption.cpp
        include/hash_validator.h
        src/hash_validator.cpp
        src/session.cpp
        src/protocol.cpp)

target_link_libraries(diffie_hellman_tls PRIVATE cryptopp pthread)

add_executable(dhm_cli
        main.cpp
        menu.h
        menu.cpp)
target_link_libraries(dhm_cli PUBLIC diffie_hellman_tls argparse)