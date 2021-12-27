cmake_minimum_required(VERSION 3.15.0)

# For MSVC RUNTIME LIBRARY, need CMP0091=NEW and cmake 3.15+
cmake_policy(SET CMP0091 NEW)

# Version info
set(QUANTLIB_EXT_VERSION_MAJOR 1)
set(QUANTLIB_EXT_VERSION_MINOR 25)
set(QUANTLIB_EXT_VERSION_PATCH 0)
set(QUANTLIB_EXT_VERSION ${QUANTLIB_EXT_VERSION_MAJOR}.${QUANTLIB_EXT_VERSION_MINOR}.${QUANTLIB_EXT_VERSION_PATCH})

# Project Info
set(PACKAGE_NAME          "QuantLibExt")
set(PACKAGE_VERSION       "${QUANTLIB_EXT_VERSION}-dev")
set(PACKAGE_VERSION_HEX   "0x01250000")
set(PACKAGE_STRING        "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME       "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT     "https://github.com/ChinaQuants/qlengine/issues/")

# Default build type for single-config generators (set this before project() command)
# For multi-config generators, such as Visual Studio, use: cmake --build . --config=<CONFIG>
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE
        STRING "Build type for single-config generators" FORCE)
endif()

project(${PACKAGE_NAME} LANGUAGES CXX DESCRIPTION "The QuantLibExt C++ Library")

# Path for package-local cmake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Installation directories
set(QL_EXT_INSTALL_BINDIR "bin" CACHE STRING "Installation directory for executables")
set(QL_EXI_INSTALL_LIBDIR "lib" CACHE STRING "Installation directory for libraries")
set(QL_EXI_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation directory for headers")
set(QL_EXT_INSTALL_EXAMPLESDIR "lib/QuantLibExt/examples" CACHE STRING
    "Installation directory for examples")
set(QL_EXT_INSTALL_CMAKEDIR "lib/cmake/${PACKAGE_NAME}" CACHE STRING
    "Installation directory for CMake scripts")

# Options
option(QL_EXT_BUILD_EXAMPLES "Build examples" ON)
option(QL_EXT_BUILD_TEST_SUITE "Build test suite" ON)
option(QL_EXT_INSTALL_EXAMPLES "Install examples" ON)
option(QL_EXT_INSTALL_TEST_SUITE "Install test suite" ON)
option(QL_TAGGED_LAYOUT "Library names use layout tags" ${MSVC})
option(QL_EXT_TAGGED_LAYOUT "Library names use layout tags" ${MSVC})

# Project shared libs ON for UNIX
if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${UNIX})
endif()

# Boost static libs ON for MSVC
if (NOT DEFINED Boost_USE_STATIC_LIBS)
    set(Boost_USE_STATIC_LIBS ${MSVC})
endif()

# Boost static runtime ON for MSVC
if (NOT DEFINED Boost_USE_STATIC_RUNTIME)
    set(Boost_USE_STATIC_RUNTIME ${MSVC})
endif()

# Require C++11 or higher
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
elseif(CMAKE_CXX_STANDARD LESS 11)
    message(FATAL_ERROR "Please specify CMAKE_CXX_STANDARD of 11 or higher")
endif()
if (NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
# Avoid use of compiler language extensions, i.e. -std=c++11 not -std=gnu++11
if (NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS FALSE)
endif()

if (NOT DEFINED QL_EXT_BOOST_VERSION)
    # Boost 1.75.0 or greater required for compiling with C++20
    if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        set(QL_EXT_BOOST_VERSION 1.75.0)
    else()
        set(QL_EXT_BOOST_VERSION 1.58.0)
    endif()
endif()

if (NOT DEFINED QL_EXT_BOOST_COMPONENTS)
    # Boost thread and unit_test_framework needed for tests and benchmark
    if (QL_EXT_BUILD_TEST_SUITE)
        set(QL_EXT_BOOST_COMPONENTS unit_test_framework)
    else()
        set(QL_EXT_BOOST_COMPONENTS)
    endif()
endif()

find_package(Boost ${QL_EXT_BOOST_VERSION} REQUIRED COMPONENTS ${QL_EXT_BOOST_COMPONENTS})

# Do not warn about Boost versions higher than 1.58.0
set(Boost_NO_WARN_NEW_VERSIONS ON)

# Avoid using Boost auto-linking
add_compile_definitions(BOOST_ALL_NO_LIB)

# If available, use PIC for shared libs and PIE for executables
if (NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()
if (CMAKE_POSITION_INDEPENDENT_CODE)
    # cmake policy CMP0083: add PIE support if possible (need cmake 3.14)
    include(CheckPIESupported)
    check_pie_supported()
endif()

# Check for library name layout tagging
if (QL_EXT_TAGGED_LAYOUT)
    set(DEBUG_POSTFIX "-mt")
    set(RELEASE_POSTFIX "-mt")
    if (NOT BUILD_SHARED_LIBS)
        set(DEBUG_POSTFIX ${DEBUG_POSTFIX}-sgd)
        set(RELEASE_POSTFIX ${RELEASE_POSTFIX}-s)
    else()
        set(DEBUG_POSTFIX ${DEBUG_POSTFIX}-gd)
    endif()
    if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(DEBUG_POSTFIX ${DEBUG_POSTFIX}-x64)
        set(RELEASE_POSTFIX ${RELEASE_POSTFIX}-x64)
    endif()
    set(CMAKE_DEBUG_POSTFIX ${DEBUG_POSTFIX})
    set(CMAKE_RELEASE_POSTFIX ${RELEASE_POSTFIX})
    set(CMAKE_RELWITHDEBINFO_POSTFIX ${RELEASE_POSTFIX})
    set(CMAKE_MINSIZEREL_POSTFIX ${RELEASE_POSTFIX})
endif()

include(Platform)
include(quantlibext)

# On single-configuration builds, select a default build type that
# gives the same compilation flags as a default autotools build.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QuantLib/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QuantLib/lib)

# Add subdirectories
add_subdirectory(qlext)
if (QL_EXT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (QL_EXT_BUILD_TEST_SUITE)
    add_subdirectory(test-suite)
endif()
