cmake_minimum_required(VERSION 3.5)
project(CPPCSALab2Client)

set(CMAKE_CXX_STANDARD 11)

include_directories(hv)

set(SOURCE_FILES
    src/CPPCSALab2Client.cpp
    src/MyClient.cpp
    src/Test.cpp
)

add_executable(CPPCSALab2Client ${SOURCE_FILES})

target_link_libraries(CPPCSALab2Client Catch2 hv)

set_target_properties(CPPCSALab2Client
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY build
)

install(TARGETS CPPCSALab2Client
    DESTINATION package/usr/bin
)

add_executable(Test src/Test.cpp)
target_link_libraries(Test Catch2 hv)
set_target_properties(Test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY test
)