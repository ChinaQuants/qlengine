cmake_minimum_required(VERSION 3.15.0)

# For MSVC RUNTIME LIBRARY, need CMP0091=NEW and cmake 3.15+
cmake_policy(SET CMP0091 NEW)

project(Examples)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib/cmake/Platform.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib-Ext/cmake/quantlibext.cmake)

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

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE
        STRING "Build type for single-config generators" FORCE)
endif()
# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib-Ext)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib-Ext/lib)
get_quantlib_library_name(QL_OUTPUT_NAME)
get_quantlib_ext_library_name(QL_EXT_OUTPUT_NAME)

add_subdirectory(BermudanSwaption)
add_subdirectory(CallableBond)
add_subdirectory(Interpolations)
