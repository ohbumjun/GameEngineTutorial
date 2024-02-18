# CONFIGURE_DEPENDS : when file is added or removed, cmake will be reconfigured
# set(ENGINE_SOURCES "my_lib.cc")
file(GLOB EDITOR_SOURCES 
"./*.cpp"
"./*.h"
)
add_executable(
    ${EDITOR_NAME} ${EDITOR_SOURCES})

target_link_libraries(
    ${EDITOR_NAME} 
    ${ENGINE_NAME})

# 아래 두 줄을 통해서 해당 경로에 있는 모든 cpp 파일들을 pch 를 사용하지 않을 것이라고 하는 것이다
# 그러면 hazel engine 은 현재 hzpch.cpp 를 무조건 모든 cpp 파일에서 include 하게 해놨는데
# 이것을 imgui 코드들은 하지 않도록 하는 것이다.
set_source_files_properties(
    ${IMGUIZMO_SOURCES} 
    PROPERTIES SKIP_PRECOMPILE_HEADERS 
    ON
    )

# Set the output directory for the binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${EDITOR_NAME})

# Set the output directory for the intermediate files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin-int/${CMAKE_BUILD_TYPE}/${EDITOR_NAME})

# Set platform-specific variables
# Windows specific settings
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # Set the system version to the latest
    set(CMAKE_SYSTEM_VERSION "latest")

    # Define Hazel specific preprocessor macros
    target_compile_definitions(
        ${ENGINE_NAME} 
        PRIVATE
        HZ_PLATFORM_WINDOWS
    )
endif()


# Debug configuration
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(${ENGINE_NAME} 
    PUBLIC 
    HZ_DEBUG)

    # game engine을 dll 로 만드는 경우 세팅
    # /MDd for MT runtime library in Debug
    target_compile_options(${ENGINE_NAME} 
    PUBLIC 
    /MDd)  # For MT runtime library

    # DEBUG_POSTFIX: Appends "d" 
    # to the library name in Debug configuration.
    set_target_properties(
        ${ENGINE_NAME} 
    PROPERTIES 
    DEBUG_POSTFIX "d")

    set_target_properties(
        ${ENGINE_NAME} 
    PROPERTIES 
    # RUNTIME_OUTPUT_DIRECTORY_DEBUG ?
    # = Specifies output directory for Debug binaries.
    RUNTIME_OUTPUT_DIRECTORY_DEBUG 
    "${CMAKE_BINARY_DIR}/bin/Debug")

endif()

# Release configuration
if(CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_definitions(
        ${ENGINE_NAME} 
        PUBLIC 
        HZ_RELEASE)
    set_target_properties(
        ${ENGINE_NAME} 
        PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY_RELEASE 
        "${CMAKE_BINARY_DIR}/bin/Release")
endif()

# Dist configuration
if(CMAKE_BUILD_TYPE MATCHES "Dist")
    target_compile_definitions(
        ${ENGINE_NAME}  
        PUBLIC 
        HZ_DIST)
    set_target_properties(
        ${ENGINE_NAME}  
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE 
        "${CMAKE_BINARY_DIR}/bin/Dist")
endif()