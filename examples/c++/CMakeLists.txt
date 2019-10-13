cmake_minimum_required()

project(Examples)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib/cmake/quantlib.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib-Ext/cmake/quantlibext.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" ${UNIX})
option(USE_BOOST_DYNAMIC_LIBRARIES "Use the shared version of Boost libraries" ${UNIX})
if (USE_BOOST_DYNAMIC_LIBRARIES)
    add_definitions(-DBOOST_ALL_DYN_LINK)
else()
    set(Boost_USE_STATIC_LIBS ON)
endif()

if (MSVC)
    set(MSVC_RUNTIME "dynamic" CACHE STRING "MSVC runtime to link")
    set_property(CACHE MSVC_RUNTIME PROPERTY STRINGS static dynamic)
    configure_msvc_runtime()

    if(${MSVC_RUNTIME} STREQUAL "static")
        if(USE_BOOST_DYNAMIC_LIBRARIES)
            message(FATAL_ERROR "Use of shared Boost libraries while compiling with static runtime seems not be a good idea.")
        endif()
        set(Boost_USE_STATIC_RUNTIME ON)
    endif()

    # Remove warnings
    add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
endif()

# On single-configuration builds, select a default build type that
# gives the same compilation flags as a default autotools build.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib-Ext)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../QuantLib-Ext/lib)

find_package(Boost)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif (Boost_FOUND)

get_quantlib_library_name(QL_OUTPUT_NAME)
get_quantlib_ext_library_name(QL_EXT_OUTPUT_NAME)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(QL_LINK_LIBRARY "${QL_OUTPUT_NAME}${CMAKE_DEBUG_POSTFIX}")
else()
    set(QL_LINK_LIBRARY ${QL_OUTPUT_NAME})
endif()
set(QL_EXT_LINK_LIBRARY ${QL_EXT_OUTPUT_NAME})

add_subdirectory(BermudanSwaption)
add_subdirectory(Interpolations)
