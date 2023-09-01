list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(project)

# 指定Unicode字符集
add_definitions(-DUNICODE -D_UNICODE)

# ---------------------------------------------------------------------------- #
# Define global variables 定义变量
# ---------------------------------------------------------------------------- #

# 设置目标架构
set(TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
        set(TARGET_ARCH x64)
        set(INSTALL_ARCH x64)
    else()
        set(TARGET_ARCH x86)
        set(INSTALL_ARCH x86)
    endif()
elseif(APPLE)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_ARCH x86_64)
        set(INSTALL_ARCH x64)
    endif()
    set(CMAKE_OSX_ARCHITECTURES ${TARGET_ARCH})
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
        set(TARGET_ARCH x64)
        set(INSTALL_ARCH x64)
    else()
        set(TARGET_ARCH x86)
        set(INSTALL_ARCH x86)
    endif()
endif()

# 设置目标名称
set(APP_TARGET_NAME ${PROJECT_NAME}-mgr)

set(APP_TARGET_DIR ${CODE_DIR}/${APP_TARGET_NAME})

# ---------------------------------------------------------------------------- #
# Project add runnable target 添加可执行目标
# ---------------------------------------------------------------------------- #

unset(app_code_files)
file(GLOB_RECURSE app_code_files ${APP_TARGET_DIR}/*.cpp ${APP_TARGET_DIR}/*.h ${APP_TARGET_DIR}/*.hpp)

# 对源码进行分组
source_group(src/ FILES ${app_code_files})
source_group(version/ FILES ${APP_VERSION_RC_FILE})

# 添加可执行目标
add_executable(${APP_TARGET_NAME})
target_sources(${APP_TARGET_NAME}
    PRIVATE
    ${app_code_files}
    ${VERSION_RC_FILE}
)
target_include_directories(${APP_TARGET_NAME}
    PRIVATE
    ${APP_TARGET_DIR}
    ${INTERFACE_DIR}
)

target_compile_definitions(${APP_TARGET_NAME} PUBLIC cxx_std_17)

if(WIN32)
    set_target_properties(${APP_TARGET_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif(WIN32)

# ---------------------------------------------------------------------------- #
# Target link 3th library 链接第三方库
# ---------------------------------------------------------------------------- #

target_add_third_party_from_config(${APP_TARGET_NAME} ${CPACKAGE_FILE} ${3TH_DIR} ${TARGET_PLATFORM} ${INSTALL_BIN_NAME})