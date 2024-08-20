cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(pico_wireguard C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/crypto)

pico_sdk_init()
add_library(pico_wireguard
  src/crypto.c
  src/wireguard.c
  src/wireguardif.c
  src/wireguard-platform.c
)

# use μNaCl
target_compile_definitions(pico_wireguard PUBLIC USE_mNaCl)

target_include_directories(pico_wireguard PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(pico_wireguard
  pico_lwip_nosys
  pico_wireguard_crypto
  pico_stdlib
  pico_lwip_arch_headers
  pico_lwip_arch
  pico_lwip_api
  pico_lwip
)

target_link_libraries(pico_stdlib)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
