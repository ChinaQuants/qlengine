cmake_minimum_required()

project(QuantLibExt)

# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QuantLib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QuantLib/lib)
link_directories($ENV{BOOST_ROOT}/stage/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)

find_package(Boost)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif (Boost_FOUND)

add_subdirectory(qlext)
add_subdirectory(examples)

option(USE_BOOST_DYNAMIC_LIBRARIES
       "Use the shared version of Boost.Test" OFF)
add_subdirectory(test-suite)
