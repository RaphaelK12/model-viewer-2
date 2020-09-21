cmake_minimum_required(VERSION 3.15)

project(model-viewer)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE 
        STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(FetchContent)

# GLFW
message("Configuring GLFW3")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG "3.3.2"
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD
message("Configuring GLAD")
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG v0.1.33
)
FetchContent_MakeAvailable(glad)

# GLM
message("Configuring GLM")
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
)
FetchContent_MakeAvailable(glm)

# Dead ImGui
message("Configuring Dear ImGui")
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.78
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    add_library(lib_imgui ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/examples/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp)
    
    target_compile_definitions(lib_imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_include_directories(lib_imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/examples)
    target_link_libraries(lib_imgui PUBLIC glfw glad)
endif()

# STB libs
message("Configuring stb")
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
    add_library(lib_stb INTERFACE)
    target_include_directories(lib_stb SYSTEM INTERFACE ${stb_SOURCE_DIR})
endif()

file(GLOB SRC src/*.cpp src/*/*.cpp src/*/*.c src/*/*.h)
add_executable(${PROJECT_NAME} ${SRC})
if(WIN32)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
    set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm lib_stb lib_imgui)
