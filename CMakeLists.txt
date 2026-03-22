cmake_minimum_required(VERSION 3.28)
project(pixdraw LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

add_executable(pixdraw src/main.c)

target_link_libraries(pixdraw PRIVATE SDL3::SDL3)
