get_quantlib_library_name(QL_LINK_LIBRARY)
get_quantlib_ext_library_name(QL_EXT_LINK_LIBRARY)

add_executable(BermudanSwaption BermudanSwaption.cpp)
target_link_libraries(BermudanSwaption ${QL_EXT_LINK_LIBRARY})
target_link_libraries(BermudanSwaption ${QL_LINK_LIBRARY})
target_include_directories(BermudanSwaption SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})

install(TARGETS BermudanSwaption
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )