project(pacman)
cmake_minimum_required(VERSION 3.25)

FILE(GLOB SRC *.cxx *.rc)
FILE(GLOB INC *.h *hpp)

add_executable(maze WIN32 ${SRC} ${INC})
target_link_libraries(maze d2d1.dll)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")
