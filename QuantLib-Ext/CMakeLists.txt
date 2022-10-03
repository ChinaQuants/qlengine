cmake_minimum_required(VERSION 3.15.0)

# For MSVC RUNTIME LIBRARY, need CMP0091=NEW and cmake 3.15+
cmake_policy(SET CMP0091 NEW)

# Version info
set(QUANTLIBEXT_VERSION_MAJOR 1)
set(QUANTLIBEXT_VERSION_MINOR 28)
set(QUANTLIBEXT_VERSION_PATCH 0)
set(QUANTLIBEXT_VERSION ${QUANTLIBEXT_VERSION_MAJOR}.${QUANTLIBEXT_VERSION_MINOR}.${QUANTLIBEXT_VERSION_PATCH})

# Project Info
set(PACKAGE_NAME          "QuantLibExt")
set(PACKAGE_VERSION       "${QUANTLIBEXT_VERSION}-dev")
set(PACKAGE_VERSION_HEX   "0x01280000")
set(PACKAGE_STRING        "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME       "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT     "https://github.com/lballabio/QuantLib/issues/")

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE
        STRING "Build type for single-config generators" FORCE)
endif()

project(${PACKAGE_NAME} LANGUAGES CXX DESCRIPTION "The QuantLibExt C++ Library")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(${CMAKE_CURRENT_LIST_DIR}/../QuantLib/cmake/Platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/quantlibext.cmake)

set(QLEXT_INSTALL_BINDIR "bin" CACHE STRING "Installation directory for executables")
set(QLEXT_INSTALL_LIBDIR "lib" CACHE STRING "Installation directory for libraries")
set(QLEXT_INSTALL_INCLUDEDIR "include" CACHE STRING "Installation directory for headers")

option(BUILD_SHARED_LIBS "Build shared libraries" ${UNIX})
option(QLEXT_TAGGED_LAYOUT "Library names use layout tags" ${MSVC})

# Boost static libs ON for MSVC
if (NOT DEFINED Boost_USE_STATIC_LIBS)
    set(Boost_USE_STATIC_LIBS ${MSVC})
endif()

# Boost static runtime ON for MSVC
if (NOT DEFINED Boost_USE_STATIC_RUNTIME)
    set(Boost_USE_STATIC_RUNTIME ${MSVC})
endif()

if (NOT DEFINED QL_BOOST_VERSION)
    # Boost 1.75.0 or greater required for compiling with C++20
    if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
        set(QL_BOOST_VERSION 1.75.0)
    else()
        set(QL_BOOST_VERSION 1.58.0)
    endif()
endif()

find_package(Boost ${QL_BOOST_VERSION} REQUIRED)

set(Boost_NO_WARN_NEW_VERSIONS ON)
add_compile_definitions(BOOST_ALL_NO_LIB)

# Require C++14 or higher
if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
elseif(CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR "Please specify CMAKE_CXX_STANDARD of 14 or higher")
endif()
if (NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# Avoid use of compiler language extensions, i.e. -std=c++14 not -std=gnu++14
if (NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS FALSE)
endif()

# If available, use PIC for shared libs and PIE for executables
if (NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Check for library name layout tagging
if (QLEXT_TAGGED_LAYOUT)
    if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(DEBUG_POSTFIX "-x64")
        set(RELEASE_POSTFIX "-x64")
    endif()
    set(DEBUG_POSTFIX ${DEBUG_POSTFIX}-mt)
    set(RELEASE_POSTFIX ${RELEASE_POSTFIX}-mt)
    if (NOT BUILD_SHARED_LIBS)
        set(DEBUG_POSTFIX ${DEBUG_POSTFIX}-sgd)
        set(RELEASE_POSTFIX ${RELEASE_POSTFIX}-s)
    else()
        set(DEBUG_POSTFIX ${DEBUG_POSTFIX}-gd)
    endif()
    set(CMAKE_DEBUG_POSTFIX ${DEBUG_POSTFIX})
    set(CMAKE_RELEASE_POSTFIX ${RELEASE_POSTFIX})
    set(CMAKE_RELWITHDEBINFO_POSTFIX ${RELEASE_POSTFIX})
    set(CMAKE_MINSIZEREL_POSTFIX ${RELEASE_POSTFIX})
endif()

# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QuantLib/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QuantLib/lib)

add_subdirectory(qlext)
add_subdirectory(examples)
add_subdirectory(test-suite)
