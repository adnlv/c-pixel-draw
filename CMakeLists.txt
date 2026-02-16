cmake_minimum_required(VERSION 3.28)
project(pixeldraw LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

add_executable(pixeldraw src/main.c)

target_link_libraries(pixeldraw PRIVATE SDL3::SDL3)
