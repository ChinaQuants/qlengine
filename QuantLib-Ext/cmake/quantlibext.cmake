cmake_minimum_required(VERSION 3.15.1)

macro(get_quantlib_library_name QL_OUTPUT_NAME)
    message(STATUS "QuantLib library name tokens:")

    # MSVC: Give QuantLib built library different names following code in 'ql/autolink.hpp'
    if(MSVC)
        # - platform
        if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            set(QL_LIB_PLATFORM "-x64")
        endif()
        message(STATUS " - Platform: ${QL_LIB_PLATFORM}")

        # - thread linkage
        set(QL_LIB_THREAD_OPT "-mt")  # _MT is defined for /MD and /MT runtimes (https://docs.microsoft.com/es-es/cpp/build/reference/md-mt-ld-use-run-time-library)
        message(STATUS " - Thread opt: ${QL_LIB_THREAD_OPT}")
        
        # - static/dynamic linkage
        if(${MSVC_RUNTIME} STREQUAL "static")
            set(QL_LIB_RT_OPT "-s")
            set(CMAKE_DEBUG_POSTFIX "gd")
        else()
            set(CMAKE_DEBUG_POSTFIX "-gd")
        endif()
        message(STATUS " - Linkage opt: ${QL_LIB_RT_OPT}")
        
        set(${QL_OUTPUT_NAME} "QuantLib${QL_LIB_PLATFORM}${QL_LIB_THREAD_OPT}${QL_LIB_RT_OPT}")
    else()
        set(${QL_OUTPUT_NAME} "QuantLib")
    endif()
    message(STATUS "QuantLib library name: ${${QL_OUTPUT_NAME}}[${CMAKE_DEBUG_POSTFIX}]")
endmacro(get_quantlib_library_name)

macro(get_quantlib_ext_library_name QL_EXT_OUTPUT_NAME)
    message(STATUS "QuantLibExt library name tokens:")

    # MSVC: Give QuantLib built library different names following code in 'ql/autolink.hpp'
    if(MSVC)
        # - platform
        if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            set(QL_EXT_LIB_PLATFORM "-x64")
        endif()
        message(STATUS " - Platform: ${QL_EXT_LIB_PLATFORM}")

        # - thread linkage
        set(QL_EXT_LIB_THREAD_OPT "-mt")  # _MT is defined for /MD and /MT runtimes (https://docs.microsoft.com/es-es/cpp/build/reference/md-mt-ld-use-run-time-library)
        message(STATUS " - Thread opt: ${QL_EXT_LIB_THREAD_OPT}")
        
        # - static/dynamic linkage
        if(${MSVC_RUNTIME} STREQUAL "static")
            set(QL_EXT_LIB_RT_OPT "-s")
            set(CMAKE_DEBUG_POSTFIX "gd")
        else()
            set(CMAKE_DEBUG_POSTFIX "-gd")
        endif()
        message(STATUS " - Linkage opt: ${QL_EXT_LIB_RT_OPT}")
        
        set(${QL_EXT_OUTPUT_NAME} "QuantLibExt${QL_EXT_LIB_PLATFORM}${QL_EXT_LIB_THREAD_OPT}${QL_EXT_LIB_RT_OPT}")
    else()
        set(${QL_EXT_OUTPUT_NAME} "QuantLibExt")
    endif()
    message(STATUS "QuantLibExt library name: ${${QL_EXT_OUTPUT_NAME}}[${CMAKE_DEBUG_POSTFIX}]")
endmacro(get_quantlib_ext_library_name)
