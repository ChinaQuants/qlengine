add_executable(Interpolations Interpolations.cpp)
target_link_libraries(Interpolations ${QL_OUTPUT_NAME})
target_link_libraries(Interpolations ${QL_EXT_OUTPUT_NAME})

install(TARGETS Interpolations
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )