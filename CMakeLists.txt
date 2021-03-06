cmake_minimum_required(VERSION 3.10.0)

PROJECT(SkyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/functions.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/bin)

if (APPLE)
set(PLATFORM_EXT_LIBS
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation")
endif()

configure_file(ProjectRoot.h.in ${CMAKE_BINARY_DIR}/Gen/ProjectRoot.h)
include_directories(${CMAKE_BINARY_DIR}/Gen)

enable_testing()

add_subdirectory(launcher)
add_subdirectory(engine)
add_subdirectory(framework)
add_subdirectory(thirdparty)
add_subdirectory(core)
add_subdirectory(vulkan)
add_subdirectory(sample)
add_subdirectory(tools)
add_subdirectory(test)

if (QT_PATH)
    add_subdirectory(editor)
endif()