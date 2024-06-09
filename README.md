# Ray Tracing from scratch in C++
* Version 1.1.0

![alt text](images/ray-tracer-preview.gif "Optional title")
![alt text](images/knight.png "Optional title")


## Project Structure

* **core:**  Write common, reusable rendering code here.
* **app:**  Create code for specific scenes or applications.
* **thirdparty:** Houses all third-party libraries.

This separation keeps your project organized and promotes code reusability.

## Building The Project

### Windows (Visual Studio)

1. Clone this repository:
```
git clone --recursive https://github.com/jakubg05/ray-tracing.git your-project-name
cd your-project-name
git submodule update --init --recursive 
```
2. Right-click the cloned folder and select "Open with Visual Studio," or open Visual Studio and navigate to File -> Open -> CMake... and choose the root CMakeLists.txt.
3. Run the Debug build.
4. Create a Release configuration by clicking the x64-Debug dropdown and adding x64-Release.
5. Choose raytracing.exe from the box on the right and hit the run button.

## Project Structure and CMake

This project uses a slightly more advanced CMake setup to promote code organization and reusability:

* **Core as a Static Library:** The `core` directory contains its own CMakeLists.txt file and is configured to be built as a static library. This means the compiled code from the `core` directory is packaged into a single `.lib` (Windows) or `.a` (Linux) file.

* **Application Linking:** The `app` directory doesn't have its own CMakeLists.txt. Instead, the root CMakeLists.txt handles:
* Building the executable for the application
* Linking the application's executable against the `core` static library
* Linking the application's executable against the third-party libraries (GLFW, GLEW, etc.)

![alt text](images/dependency-graph-version-1_0_0.png "Optional title")
