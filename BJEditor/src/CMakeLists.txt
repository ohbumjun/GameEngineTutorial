# CONFIGURE_DEPENDS : when file is added or removed, cmake will be reconfigured
# set(ENGINE_SOURCES "my_lib.cc")

SET(TOTAL_EDITOR_FILES) # lv1engine 은 그냥 그대로 사용함 ;

SET(FILE_HEADERS 
    public/File/FileManager.h
    public/File/PathManager.h
)
SOURCE_GROUP("file/public" FILES ${FILE_HEADERS})
LIST(APPEND TOTAL_ENGINE_FILES ${FILE_HEADERS})

SET(FILE_SOURCES
    private/File/FileManager.cpp
    private/File/PathManager.cpp
)
SOURCE_GROUP("file/private" FILES ${FILE_SOURCES})
LIST(APPEND TOTAL_ENGINE_FILES ${FILE_SOURCES})

# Create filters for Visual Studio
# foreach(FILE ${EDITOR_SOURCES} ${EDITOR_HEADERS}) 
#     # Get the directory of the source file
#     get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
# 
#     # Remove common directory prefix to make the group
#     string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
# 
#     # Make sure we are using windows slashes
#     string(REPLACE "/" "\\" GROUP "${GROUP}")
# 
#     # Group into "Source Files" and "Header Files"
#     if ("${FILE}" MATCHES ".*\\.cpp")
#         set(GROUP "Source Files${GROUP}")
#     elseif("${FILE}" MATCHES ".*\\.h")
#         set(GROUP "Header Files${GROUP}")
#     endif()
# 
#     source_group("${GROUP}" FILES "${FILE}")
# endforeach()

SET(PANEL_HEADERS 
    public/Panel/ComponentPanel/CameraPanel.h
    public/Panel/ContentBrowserPanel.h
    public/Panel/SceneHierarchyPanel.h
)
SOURCE_GROUP("panel/public" FILES ${PANEL_HEADERS})
LIST(APPEND TOTAL_ENGINE_FILES ${PANEL_HEADERS})

SET(PANEL_SOURCES
    private/Panel/ComponentPanel/CameraPanel.cpp
    private/Panel/ContentBrowserPanel.cpp
    private/Panel/SceneHierarchyPanel.cpp
)
SOURCE_GROUP("panel/private" FILES ${PANEL_SOURCES})
LIST(APPEND TOTAL_ENGINE_FILES ${PANEL_SOURCES})


SET(EDITOR_HEADERS
    public/EditorLayer.h
    public/ParticleSystem.h
)
SOURCE_GROUP("panel/private" FILES ${EDITOR_HEADERS})
LIST(APPEND TOTAL_ENGINE_FILES ${EDITOR_HEADERS})


SET(EDITOR_SOURCES
    private/EditorLayer.cpp
    private/ParticleSystem.cpp
    private/HazelEditorApp.cpp
)
SOURCE_GROUP("panel/private" FILES ${EDITOR_SOURCES})
LIST(APPEND TOTAL_ENGINE_FILES ${EDITOR_SOURCES})

set(EDITOR_INCLUDES 
"${CMAKE_SOURCE_DIR}/BJEditor/src/public"
)

# executable : main 함수가 반드시 존재해야 한다.
add_executable(${EDITOR_NAME} 
${TOTAL_ENGINE_FILES}
)

target_compile_definitions(
    ${EDITOR_NAME} 
    PUBLIC 
    # DLL (Dynamic Link Library) toward engine 
    HZ_DYNAMIC_LINK
)

target_include_directories(
    ${EDITOR_NAME} 
    PUBLIC 
    ${EDITOR_INCLUDES}
)

# 특정 target 에 library 를 종속 걸어주는 함수
# PUBLIC   : 현재 target 뿐만 아니라, 이 녀석을 link 건 다른 target 까지도 link
# PRIVATE : 현재 target only
# INTERFACE : 이 녀석을 link 건 다른 target "만"
# 참고 : library 안에 include 가 private 이 아니라, public 으로 추가된 경우
# target_include_directories 필요 없이 target_link_libraries 만으로도 include 가 가능하다.
target_link_libraries(
    ${EDITOR_NAME}
    # 참고 : 되도록 PRIVATE. 아니면 종속성이 꼬이는 문제가 있다.
    PRIVATE
    ${ENGINE_NAME})

# if(${ENABLE_LTO})
#    target_enable_lto(
#        TARGET ${EDITOR_NAME} 
#        ENABLE
#        ON)
#endif()

# apply clang-tidy
if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${EDITOR_NAME})
endif()

# Dependency Tree 를 만들어 준다.
# - 어떤 target 을 build 할 때, 그 이전에 다른 target 을 build 해줘
add_dependencies(
    ${EDITOR_NAME} 
    ${ENGINE_NAME}
)

# 아래 두 줄을 통해서 해당 경로에 있는 모든 cpp 파일들을 pch 를 사용하지 않을 것이라고 하는 것이다
# 그러면 hazel engine 은 현재 hzpch.cpp 를 무조건 모든 cpp 파일에서 include 하게 해놨는데
# 이것을 imgui 코드들은 하지 않도록 하는 것이다.
set_source_files_properties(
    ${IMGUIZMO_SOURCES} 
    PROPERTIES SKIP_PRECOMPILE_HEADERS 
    ON
    )
    
# Set the output directory of the dll, exe
set_target_properties(${EDITOR_NAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY
    ${RUNTIME_OUTPUT_DIR}
)

if(MSVC)
    message(STATUS "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")

    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        
    endif()
endif()
