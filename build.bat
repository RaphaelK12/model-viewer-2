clang -O2 -Wall -Wextra src/main.cpp src/AssetManagement/asset_loader.cpp thirdparty/glad/src/glad.c -I thirdparty/glad/include -I thirdparty/GLFW-Clang/include -I thirdparty/glm -D_CRT_SECURE_NO_WARNINGS -L thirdparty/GLFW-Clang/lib -lglfw3-clang -lgdi32 -luser32 -lshell32 -lmsvcrt -fuse-ld=lld