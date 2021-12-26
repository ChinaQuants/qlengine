add_executable(BermudanSwaption BermudanSwaption.cpp)
target_link_libraries(BermudanSwaption ${QL_OUTPUT_NAME})
target_link_libraries(BermudanSwaption ${QL_EXT_OUTPUT_NAME})

install(TARGETS BermudanSwaption
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )