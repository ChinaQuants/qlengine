get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

add_executable(AutoCallable AutoCallable.cpp)
target_link_libraries(AutoCallable ql_ext_library)
target_link_libraries(AutoCallable ${QL_LINK_LIBRARY})
target_include_directories(AutoCallable SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})

