get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

file (GLOB QL_EXT_TEST_SUITE_FILES "*.hpp" "*.cpp" 
list(REMOVE_ITEM QL_EXT_TEST_SUITE_FILES main.cpp))

if (QL_EXT_BUILD_TEST_SUITE)
    add_library(ql_ext_unit_test_main STATIC main.cpp)
    target_include_directories(ql_ext_unit_test_main PRIVATE
                                ${PROJECT_BINARY_DIR}
                                ${PROJECT_SOURCE_DIR})
    target_include_directories(ql_ext_unit_test_main SYSTEM PRIVATE
                                ${Boost_INCLUDE_DIRS})
    if (NOT Boost_USE_STATIC_LIBS)
        target_compile_definitions(ql_ext_unit_test_main PRIVATE BOOST_TEST_DYN_LINK)
    endif()
endif()
    
if (QL_EXT_BUILD_TEST_SUITE)
    add_executable(ql_ext_test_suite ${QL_EXT_TEST_SUITE_FILES})
    set_target_properties(ql_ext_test_suite PROPERTIES OUTPUT_NAME "quantlibext-test-suite")
    if (NOT Boost_USE_STATIC_LIBS)
        target_compile_definitions(ql_ext_test_suite PRIVATE BOOST_ALL_DYN_LINK BOOST_TEST_DYN_LINK)
    endif()
    target_link_libraries(ql_ext_test_suite PRIVATE
        ql_ext_library
        ${QL_LINK_LIBRARY}
        ql_ext_unit_test_main
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})        
endif()

install(TARGETS ql_ext_test_suite
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

add_test(NAME quantlib_ext_test_suite COMMAND ql_ext_test_suite --log_level=message)
