file(GLOB_RECURSE QUANTLIB_EXT_FILES "*.hpp" "*.cpp")
add_library(QuantLibExt SHARED ${QUANTLIB_EXT_FILES})
