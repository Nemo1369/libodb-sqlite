cmake_minimum_required(VERSION 2.8.12)

cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0048 NEW)

project(odb-sqlite CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(TargetArchitecture)

target_architecture(CMAKE_TARGET_ARCHITECTURE)

if(MSVC AND NOT EXISTS "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
    file(GENERATE
        OUTPUT "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user"
        INPUT "${CMAKE_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.in"
        )
endif()

option(BUILD_UNIT_TESTS "Build unit tests" FALSE)
option(BUILD_SHARED_LIBRARIES "Build shared library version" TRUE)

set(ODB_LIBODB_SOURCES_DIR "" CACHE STRING "libodb source directory path")
set(APPLE_SIGN_IDENTITY "iPhone Developer: nemo1369@gmail.com (S686GL5RJ6)" CACHE STRING "Sign identity name")

set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)" CACHE string "Build architecture")

if(${CMAKE_C_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_COMPILER_IS_CLANG TRUE)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
    option(USE_LIBC++ "Use libc++ instead of libstdc++" ${APPLE})
endif()

find_package(ODB)

if(NOT ${CMAKE_TARGET_ARCHITECTURE} EQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR} OR NOT ODB_LIBODB_FOUND)
    if (NOT TARGET odb)
        get_filename_component(ABSOLUTE_LIBODB_SOURCES_DIR ${ODB_LIBODB_SOURCES_DIR} ABSOLUTE)
        get_filename_component(LIBODB_SOURCES_DIR_NAME ${ODB_LIBODB_SOURCES_DIR}
            DIRECTORY)

        add_subdirectory(${ABSOLUTE_LIBODB_SOURCES_DIR}
            ${CMAKE_BINARY_DIR}/${LIBODB_SOURCES_DIR_NAME})
    endif()
endif()

configure_file(${PROJECT_SOURCE_DIR}/odb/sqlite/details/config.h.in
    ${PROJECT_SOURCE_DIR}/odb/sqlite/details/config.h)

if (NOT TARGET sqlite3)
    add_subdirectory(${PROJECT_SOURCE_DIR}/frameworks/sqlite3)
endif()

if(BUILD_UNIT_TESTS)
    enable_testing()
endif()

add_subdirectory(odb)
