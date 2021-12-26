add_executable(ShiborSwap ShiborSwap.cpp)
target_link_libraries(ShiborSwap ql_ext_library)
target_link_libraries(ShiborSwap ${QL_LINK_LIBRARY})