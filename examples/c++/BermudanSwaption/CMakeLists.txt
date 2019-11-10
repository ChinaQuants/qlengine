add_executable(BermudanSwaption BermudanSwaption.cpp)
target_link_libraries(BermudanSwaption ${QL_EXT_LINK_LIBRARY})
target_link_libraries(BermudanSwaption ${QL_LINK_LIBRARY})

install(TARGETS BermudanSwaption
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )