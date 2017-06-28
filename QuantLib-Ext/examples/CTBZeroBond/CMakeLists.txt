add_executable(CTBZeroBond CTBZeroBond.cpp)
target_link_libraries(CTBZeroBond QuantLibExt)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_link_libraries(CTBZeroBond QuantLib-vc140-x64-mt-gd)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_link_libraries(CTBZeroBond QuantLib-vc140-x64-mt)
endif()

