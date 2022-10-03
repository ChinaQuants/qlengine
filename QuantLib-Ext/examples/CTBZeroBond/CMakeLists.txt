get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

add_executable(CTBZeroBond CTBZeroBond.cpp)
target_link_libraries(CTBZeroBond ql_ext_library)
target_link_libraries(CTBZeroBond ${QL_LINK_LIBRARY})
target_include_directories(CTBZeroBond SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})

