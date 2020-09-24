# model-viewer

A simple obj model viewer with camera, light and model transform controls.

**How to build without an IDE:**
Make sure you have Ninja and CMake installed and available in your PATH
1) `mkdir build`
2) `cmake -B build -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Release`
3) `ninja -C build`
4) Launch the model-viewer(.exe) executable that has appeared in the source directory
**NOTE**: The program has relative paths so do not move it from this directory.

If you have an IDE, it should have support for opening a CMakeLists.txt file and go from there.

**NOTE**: On first CMake configure, the dependenices will download, slowing down the configuration time. On subsequent CMake runs in the same build directory it will be faster.

# Images
![1](https://raw.githubusercontent.com/limepixl/model-viewer/master/img/1.png)
![2](https://raw.githubusercontent.com/limepixl/model-viewer/master/img/2.png)
![3](https://raw.githubusercontent.com/limepixl/model-viewer/master/img/3.png)
