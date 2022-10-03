get_quantlib_library_name(QL_OUTPUT_NAME)
file(GLOB_RECURSE QUANTLIB_EXT_FILES "*.hpp" "*.cpp")

if(MSVC)
    if (${BUILD_SHARED_LIBS})
        # Windows needs to link static library (nothing is declared to export in QuantLib)
        message(FATAL_ERROR "You should build QuantLibExt in STATIC mode as nothing is exported from the library. Use BUILD_SHARED_LIBS=OFF")
    endif()
    add_library(ql_ext_library ${QUANTLIB_EXT_FILES})
    set_property(TARGET ql_ext_library PROPERTY PROJECT_LABEL "QuantLibExt")
else()
    add_library(ql_ext_library ${QUANTLIB_EXT_FILES})
endif()
target_include_directories(ql_ext_library SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(${QL_LINK_LIBRARY} "${QL_OUTPUT_NAME}${CMAKE_DEBUG_POSTFIX}" PARENT_SCOPE)
else()
    set(${QL_LINK_LIBRARY} ${QL_OUTPUT_NAME} PARENT_SCOPE)
endif()

set_target_properties(ql_ext_library PROPERTIES
    EXPORT_NAME ${PACKAGE_NAME}
    OUTPUT_NAME ${PACKAGE_NAME}
    VERSION ${QUANTLIBEXT_VERSION}
    SOVERSION ${QUANTLIBEXT_VERSION_MAJOR})

install(DIRECTORY . DESTINATION include/qlext
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

install(TARGETS ql_ext_library
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )
