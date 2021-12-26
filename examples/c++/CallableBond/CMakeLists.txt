add_executable(CallableBond CallableBond.cpp)
target_link_libraries(CallableBond ${QL_OUTPUT_NAME})
target_link_libraries(CallableBond ${QL_EXT_OUTPUT_NAME})

install(TARGETS CallableBond
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )