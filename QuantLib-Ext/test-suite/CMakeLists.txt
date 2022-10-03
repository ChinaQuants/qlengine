<<<<<<< HEAD
get_quantlib_library_name(QL_LINK_LIBRARY)
message(STATUS "QL_LINK_LIBRARY: ${QL_LINK_LIBRARY}")

file (GLOB TEST_SUITE_FILES "*.hpp" "*.cpp")
=======
get_quantlib_library_name(QL_OUTPUT_NAME)
file(GLOB QL_EXT_TEST_SUITE_FILES "*.hpp" "*.cpp")
list(FILTER QL_EXT_TEST_SUITE_FILES EXCLUDE REGEX "main.cpp")
>>>>>>> 5232d3fe1fa50a865fc6e630c075cd382160beaf

if (USE_BOOST_DYNAMIC_LIBRARIES)
   add_definitions(-DBOOST_TEST_DYN_LINK)
endif (USE_BOOST_DYNAMIC_LIBRARIES)

add_library(ql_ext_unit_test_main STATIC main.cpp)
target_include_directories(ql_ext_unit_test_main PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS})

add_executable(ql_ext_test_suite ${QL_EXT_TEST_SUITE_FILES})
set_target_properties(ql_ext_test_suite PROPERTIES OUTPUT_NAME "quantlibext-test-suite")
if (NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(ql_ext_unit_test_main PRIVATE BOOST_TEST_DYN_LINK)
    target_compile_definitions(ql_ext_test_suite PRIVATE BOOST_ALL_DYN_LINK BOOST_TEST_DYN_LINK)
endif()
message(STATUS " - Starting Link: ql_ext_library")
message(STATUS " - Starting Link: ${QL_OUTPUT_NAME}")

<<<<<<< HEAD
target_link_libraries (${TEST} ${QL_LINK_LIBRARY}) 
target_link_libraries (${TEST} ql_ext_library)
target_link_libraries (${TEST} ${Boost_LIBRARIES})
target_include_directories(${TEST} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
=======
target_link_libraries(ql_ext_test_suite PRIVATE
        ${QL_OUTPUT_NAME}
        ql_ext_library
        ql_ext_unit_test_main
        Boost::unit_test_framework)
>>>>>>> 5232d3fe1fa50a865fc6e630c075cd382160beaf


install(TARGETS ql_ext_test_suite
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

add_test(NAME test_suite COMMAND ql_ext_test_suite --log_level=message)
