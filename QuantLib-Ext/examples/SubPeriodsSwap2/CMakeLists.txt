get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

add_executable(SubPeriodsSwap2 SubPeriodsSwap2.cpp)
target_link_libraries(SubPeriodsSwap2 ql_ext_library)
target_link_libraries(SubPeriodsSwap2 ${QL_LINK_LIBRARY})
target_include_directories(SubPeriodsSwap2 SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})