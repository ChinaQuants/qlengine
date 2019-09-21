add_executable(BermudanSwaption BermudanSwaption.cpp)
target_link_libraries(BermudanSwaption ${QL_EXT_LINK_LIBRARY})
target_link_libraries(BermudanSwaption ${QL_LINK_LIBRARY})