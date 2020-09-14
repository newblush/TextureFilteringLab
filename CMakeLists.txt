cmake_minimum_required(VERSION 3.1)
project(cg_lab1)

set(CMAKE_CXX_STANDARD 11)

message(${CMAKE_GENERATOR})
message(${CMAKE_CXX_COMPILER_ID})
message(${CMAKE_SIZEOF_VOID_P})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # mingw-w64
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(GLFW_DIRECTORY ext/mingw-w64)
    # mingw
    elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(GLFW_DIRECTORY ext/mingw)
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # msvc 64bit
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(GLFW_DIRECTORY ext/msvc-x64)
    # msvc 32bit
    elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(GLFW_DIRECTORY ext/msvc-x86)
    endif()
endif()

include_directories(
        ${GLFW_DIRECTORY}
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/gl
        ${CMAKE_CURRENT_SOURCE_DIR}/ext/gl/glm
)
link_directories(${GLFW_DIRECTORY})
file(GLOB RESOURCE_DIR resource *.*)
add_executable(cg_lab1
        ${RESOURCE_DIR}
        # shader files
        shader/vertex.glsl
        shader/fragment.glsl
        # header files
        src/TextureFilteringScene.h
        tools/SpriteRenderer.h
        tools/ResourceManager.h
        tools/Shader.h
        tools/stb_image.h
        tools/Texture2D.h
        # source files
        tools/SpriteRenderer.cpp
        tools/ResourceManager.cpp
        tools/Shader.cpp
        tools/stb_image.cpp
        tools/Texture2D.cpp
        src/TextureFilteringScene.cpp
        src/main.cpp
        glad.c
)
target_link_libraries(cg_lab1 glfw3dll opengl32)

add_custom_command(TARGET cg_lab1 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/${GLFW_DIRECTORY}/glfw3.dll" $<TARGET_FILE_DIR:cg_lab1>)