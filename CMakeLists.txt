cmake_minimum_required(VERSION 3.5)
project(CPPCSALab2Client)

set(CMAKE_CXX_STANDARD 11)

include_directories(hv)

# Исполняемый файл для CPPCSALab2Client
add_executable(CPPCSALab2Client
    src/CPPCSALab2Client.cpp
    src/MyClient.cpp
)

target_link_libraries(CPPCSALab2Client ${CMAKE_SOURCE_DIR}/lib/libhv.a)

set_target_properties(CPPCSALab2Client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY build
)

install(TARGETS CPPCSALab2Client
    DESTINATION package/usr/bin
)

# Исполняемый файл для Test
add_executable(Test
    src/Test.cpp
)

target_link_libraries(Test ${CMAKE_SOURCE_DIR}/lib/libCatch2.a ${CMAKE_SOURCE_DIR}/lib/libhv.a)

set_target_properties(Test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY test
)