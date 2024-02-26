cmake_minimum_required(VERSION 3.27)
project(algo2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address,undefined -fno-sanitize-recover=all"
        CACHE STRING "Compiler flags in asan build"
        FORCE)

file(GLOB_RECURSE SOURCES "contest_*/*.cpp" "review/*.cpp")
foreach (SOURCE ${SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE})
endforeach ()


#   manual variant:
#   add_executable([name] [path_to_file])

#    variant with sanitizers without profiles
#    add_executable(${TARGET_NAME} ${SOURCE})
#    target_compile_options(${TARGET_NAME} PRIVATE -std=c++17 -O2 -Wall -Werror -Wsign-compare)
#    set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")
#
#    # Sanitizers
#    add_executable(${TARGET_NAME}_san ${SOURCE})
#    target_compile_options(${TARGET_NAME}_san PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all -std=c++17 -O2 -Wall -Werror -Wsign-compare)
#    set_target_properties(${TARGET_NAME}_san PROPERTIES OUTPUT_NAME "san_${TARGET_NAME}")