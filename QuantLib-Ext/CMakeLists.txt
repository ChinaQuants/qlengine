cmake_minimum_required()

# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(BOOST_ROOT D:/dev/boost_1_64_0)

find_package(Boost)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif (Boost_FOUND)

add_subdirectory(qlext)
