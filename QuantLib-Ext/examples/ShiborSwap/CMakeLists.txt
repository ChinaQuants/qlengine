add_executable(ShiborSwap ShiborSwap.cpp)
target_link_libraries(ShiborSwap ${QL_EXT_LINK_LIBRARY})
target_link_libraries(ShiborSwap ${QL_LINK_LIBRARY})