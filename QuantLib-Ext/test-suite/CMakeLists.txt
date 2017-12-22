file (GLOB TEST_SUITE_FILES "*.hpp" "*.cpp")

if (USE_BOOST_DYNAMIC_LIBRARIES)
   add_definitions(-DBOOST_TEST_DYN_LINK)
endif (USE_BOOST_DYNAMIC_LIBRARIES)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
set (TEST quantlibext-test-suite)
add_executable (${TEST} ${TEST_SUITE_FILES})

message(STATUS " - Starting Link: ${QL_EXT_OUTPUT_NAME}")
message(STATUS " - Starting Link: ${QL_OUTPUT_NAME}")

target_link_libraries (${TEST} ${QL_LINK_LIBRARY}) 
target_link_libraries (${TEST} ${QL_EXT_LINK_LIBRARY})
target_link_libraries (${TEST} ${Boost_LIBRARIES})

set_property(TARGET ${TEST} PROPERTY PROJECT_LABEL "testsuite")

install(TARGETS ${TEST}
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )

enable_testing ()
add_test (${TEST} ${TEST})
