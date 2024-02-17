cmake_minimum_required(VERSION 3.27)
project(algo2)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCES "contest_*/*.cpp")
foreach(SOURCE ${SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE}
            contest_1/Prefix_function.cpp)
endforeach()

#add_executable(name
#        [path_to_file] contest_1/Z_function.cpp)

