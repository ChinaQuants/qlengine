add_executable(Interpolations Interpolations.cpp)
target_link_libraries(Interpolations ${QL_EXT_LINK_LIBRARY})
target_link_libraries(Interpolations ${QL_LINK_LIBRARY})

install(TARGETS Interpolations
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )