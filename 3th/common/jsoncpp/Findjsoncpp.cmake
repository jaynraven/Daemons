set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

# ---------------------------------------------------------------------------------------
# Define variables 
# ---------------------------------------------------------------------------------------

# 设置目标平台
string(TOLOWER ${CMAKE_SYSTEM_NAME} TS_TARGET_PLATFORM_NAME)

# 设置目标架构名称
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TS_TARGET_ARCH_NAME x64)
    else()
        set(TS_TARGET_ARCH_NAME x86)
    endif()
elseif(APPLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TS_TARGET_ARCH_NAME x64)
    endif()
elseif(ANDROID)
endif()

# 设置路径
set(TS_CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(TS_INCLUDE_DIR ${TS_CURRENT_DIR}/include)
set(TS_LIB_DIR ${TS_CURRENT_DIR}/lib)
set(TS_LIB_PLATFORM_ARCH_DIR ${TS_LIB_DIR}/${TS_TARGET_PLATFORM_NAME}/${TS_TARGET_ARCH_NAME})

# 设置目标名称
set(TS_PACKAGE_NAME jsoncpp)
set(TS_INCLUDE_FILES json/json.h)
set(TS_LIB_NAMES jsoncpp)
set(TS_DLL_NAMES jsoncpp.dll)

# ---------------------------------------------------------------------------------------
# Find libraries
# ---------------------------------------------------------------------------------------

find_path(${TS_PACKAGE_NAME}_INCLUDE_DIR
    NAMES ${TS_INCLUDE_FILES}
    PATHS ${TS_INCLUDE_DIR}
    NO_CACHE
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_BUILDS_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
)

find_library(${TS_PACKAGE_NAME}_LIBRARY_RELEASE
    NAMES ${TS_LIB_NAMES}
    PATHS ${TS_LIB_PLATFORM_ARCH_DIR}/release
    NO_CACHE
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_BUILDS_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
)

find_file(${TS_PACKAGE_NAME}_DLL_RELEASE
    NAMES ${TS_DLL_NAMES} 
    PATHS ${TS_LIB_PLATFORM_ARCH_DIR}/release
    NO_CACHE
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_BUILDS_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
)

find_library(${TS_PACKAGE_NAME}_LIBRARY_DEBUG
    NAMES ${TS_LIB_NAMES}
    PATHS ${TS_LIB_PLATFORM_ARCH_DIR}/debug
    NO_CACHE
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_BUILDS_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
)

find_file(${TS_PACKAGE_NAME}_DLL_DEBUG
    NAMES ${TS_DLL_NAMES} 
    PATHS ${TS_LIB_PLATFORM_ARCH_DIR}/debug
    NO_CACHE
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_PACKAGE_REGISTRY
    NO_CMAKE_BUILDS_PATH
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
)

include(SelectLibraryConfigurations)
select_library_configurations(${TS_PACKAGE_NAME})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${TS_PACKAGE_NAME}
    FOUND_VAR ${TS_PACKAGE_NAME}_FOUND
    REQUIRED_VARS
    ${TS_PACKAGE_NAME}_LIBRARY
    ${TS_PACKAGE_NAME}_INCLUDE_DIR
)

if(${TS_PACKAGE_NAME}_FOUND)
    if(NOT TARGET ${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME})
        add_library(${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME} UNKNOWN IMPORTED)
    endif()

    if(${TS_PACKAGE_NAME}_LIBRARY_RELEASE)
        set_property(TARGET ${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME} APPEND PROPERTY
            IMPORTED_CONFIGURATIONS RELEASE
        )
        set_target_properties(${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME} PROPERTIES
            IMPORTED_LOCATION_RELEASE "${${TS_PACKAGE_NAME}_LIBRARY_RELEASE}"
        )
    endif()

    if(${TS_PACKAGE_NAME}_LIBRARY_DEBUG)
        set_property(TARGET ${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME} APPEND PROPERTY
            IMPORTED_CONFIGURATIONS DEBUG
        )
        set_target_properties(${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME} PROPERTIES
            IMPORTED_LOCATION_DEBUG "${${TS_PACKAGE_NAME}_LIBRARY_DEBUG}"
        )
    endif()

    set_target_properties(${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME} PROPERTIES
        INTERFACE_COMPILE_FEATURES "cxx_std_11;cxx_auto_type;cxx_decltype;cxx_default_function_template_args;cxx_defaulted_functions;cxx_delegating_constructors;cxx_enum_forward_declarations;cxx_explicit_conversions;cxx_extended_friend_declarations;cxx_extern_templates;cxx_final;cxx_lambdas;cxx_long_long_type;cxx_nullptr;cxx_override;cxx_range_for;cxx_raw_string_literals;cxx_right_angle_brackets;cxx_rvalue_references;cxx_static_assert;cxx_strong_enums;cxx_trailing_return_types;cxx_uniform_initialization;cxx_variadic_macros;cxx_variadic_templates"
        INTERFACE_INCLUDE_DIRECTORIES "${${TS_PACKAGE_NAME}_INCLUDE_DIR}/.."
    )
endif()

mark_as_advanced(
    ${TS_PACKAGE_NAME}_INCLUDE_DIR
    ${TS_PACKAGE_NAME}_LIBRARY
)

if(${TS_PACKAGE_NAME}_FOUND)
    set(${TS_PACKAGE_NAME}_TARGET ${TS_PACKAGE_NAME}::${TS_PACKAGE_NAME})
    set(${TS_PACKAGE_NAME}_INCLUDE_DIRS ${${TS_PACKAGE_NAME}_INCLUDE_DIR})
    set(${TS_PACKAGE_NAME}_LIBRARY_DIRS ${TS_LIB_PLATFORM_ARCH_DIR})
    set(${TS_PACKAGE_NAME}_LIBRARIES ${${TS_PACKAGE_NAME}_LIBRARY})
    set(${TS_PACKAGE_NAME}_SHARED_LIBRARIES_DEBUG ${${TS_PACKAGE_NAME}_DLL_DEBUG})
    set(${TS_PACKAGE_NAME}_SHARED_LIBRARIES_RELEASE ${${TS_PACKAGE_NAME}_DLL_RELEASE})
endif()