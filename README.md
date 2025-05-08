# TetrisSFML

If you just want to play it, download prebuilt

if you want to build it yourself, you are going to need:
-g++ or any other c++ compiler
-make and cmake respectively for building

To build it you go into powershell and:
cd path\to\Raw Files

1. Create & enter a build directory
mkdir build
cd build

2. Configure for VS 2022, 64-bit, Release (but you can also configure it for any IDE you want)
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

3. Build the Release configuration
cmake --build . --config Release
