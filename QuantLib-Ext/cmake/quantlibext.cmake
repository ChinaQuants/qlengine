cmake_minimum_required(VERSION 3.15.0)

macro(get_quantlib_library_name QL_OUTPUT_NAME)
    message(STATUS "QuantLib library name tokens:")

    # MSVC: Give QuantLib built library different names following code in 'ql/autolink.hpp'
    if(QL_TAGGED_LAYOUT)
        if (DEBUG)
            set(${QL_OUTPUT_NAME} "QuantLib${DEBUG_POSTFIX}")
        else()
            set(${QL_OUTPUT_NAME} "QuantLib${RELEASE_POSTFIX}")
        endif()
    else()
        set(${QL_OUTPUT_NAME} "QuantLib")
    endif()
    message(STATUS "QuantLib library name: " ${QL_OUTPUT_NAME})
endmacro(get_quantlib_library_name)

macro(get_quantlib_ext_library_name QL_EXT_OUTPUT_NAME)
    message(STATUS "QuantLibExt library name tokens:")

    # MSVC: Give QuantLib built library different names following code in 'ql/autolink.hpp'
    if(QL_EXT_TAGGED_LAYOUT)
        if (DEBUG)
            set(${QL_EXT_OUTPUT_NAME} "QuantLib${DEBUG_POSTFIX}")
        else()
            set(${QL_EXT_OUTPUT_NAME} "QuantLib${RELEASE_POSTFIX}")
        endif()
    else()
        set(${QL_EXT_OUTPUT_NAME} "QuantLib")
    endif()
    message(STATUS "QuantLibExt library name: " ${QL_EXT_OUTPUT_NAME})
endmacro(get_quantlib_ext_library_name)