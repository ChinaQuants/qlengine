cmake_minimum_required(VERSION 2.8)

macro(get_quantlib_ext_library_name QL_EXT_OUTPUT_NAME)
    message(STATUS "QuantLibExt library name tokens:")

    # MSVC: Give QuantLib built library different names following code in 'ql/autolink.hpp'
    if(MSVC)
        
        # - toolset
        # ...taken from FindBoost.cmake
        if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.10)
            set(QL_LIB_TOOLSET "-vc141")
        elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19)
            set(QL_LIB_TOOLSET "-vc140")
        elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18)
            set(QL_LIB_TOOLSET "-vc120")
        elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17)
            set(QL_LIB_TOOLSET "-vc110")
        elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
            set(QL_LIB_TOOLSET "-vc100")
        elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15)
            set(QL_LIB_TOOLSET "-vc90")
        else()
            message(FATAL_ERROR "Compiler below vc90 is not supported")
        endif()
        message(STATUS " - Toolset: ${QL_LIB_TOOLSET}")
        
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
        
        set(${QL_EXT_OUTPUT_NAME} "QuantLibExt${QL_LIB_TOOLSET}${QL_LIB_PLATFORM}${QL_LIB_THREAD_OPT}${QL_LIB_RT_OPT}")
    else()
        set(${QL_EXT_OUTPUT_NAME} "QuantLibExt")
    endif()
    message(STATUS "QuantLibExt library name: ${${QL_EXT_OUTPUT_NAME}}[${CMAKE_DEBUG_POSTFIX}]")
endmacro(get_quantlib_ext_library_name)