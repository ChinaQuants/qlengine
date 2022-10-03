get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

add_executable(SubPeriodsSwap SubPeriodsSwap.cpp)
target_link_libraries(SubPeriodsSwap ql_ext_library)
target_link_libraries(SubPeriodsSwap ${QL_LINK_LIBRARY})
target_include_directories(SubPeriodsSwap SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
