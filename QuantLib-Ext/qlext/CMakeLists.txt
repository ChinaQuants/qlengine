file(GLOB_RECURSE QUANTLIB_EXT_FILES "*.hpp" "*.cpp")
add_library(QuantLibExt STATIC ${QUANTLIB_EXT_FILES})
set_target_properties(QuantLibExt PROPERTIES OUTPUT_NAME "QuantLibExt-vc140-x64-mt")
