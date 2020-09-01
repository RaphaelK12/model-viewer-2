@echo off

if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)

set "compiler_flags=/W4 /O2"

if not exist "build" mkdir build

set "source_files=src/main.cpp src/Mesh/mesh.cpp src/Display/display.cpp src/AssetManagement/*.cpp"
set "tp_source_files=thirdparty/glad/src/glad.c thirdparty/imgui-1.78/*.cpp"

set "include_dirs=/Ithirdparty /Ithirdparty/glad/include /Ithirdparty/GLFW/include /Ithirdparty/imgui-1.78 /Ithirdparty/glm"
set "lib_dirs=/LIBPATH:thirdparty/GLFW/lib"
set "libs=glfw3.lib opengl32.lib shell32.lib gdi32.lib msvcrt.lib"

cl.exe /Fo.\build\ /EHsc /MD /D_CRT_SECURE_NO_WARNINGS %compiler_flags% %source_files% %tp_source_files% %include_dirs% /link %lib_dirs% %libs% /out:model-viewer.exe