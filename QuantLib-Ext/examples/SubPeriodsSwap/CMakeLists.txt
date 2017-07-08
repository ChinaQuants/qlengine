add_executable(SubPeriodsSwap SubPeriodsSwap.cpp)
target_link_libraries(SubPeriodsSwap QuantLibExt)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_link_libraries(SubPeriodsSwap QuantLib-vc140-x64-mt-gd)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_link_libraries(SubPeriodsSwap QuantLib-vc140-x64-mt)
endif()