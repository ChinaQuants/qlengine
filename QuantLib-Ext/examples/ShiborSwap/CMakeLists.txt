get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

add_executable(ShiborSwap ShiborSwap.cpp)
target_link_libraries(ShiborSwap ql_ext_library)
target_link_libraries(ShiborSwap ${QL_LINK_LIBRARY})
target_include_directories(ShiborSwap SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})