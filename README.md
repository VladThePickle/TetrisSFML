# TetrisSFML

If you just want to play it, download prebuilt

if you want to build it yourself, you are going to need:
  1. g++ or any other c++ compiler
  2. make and cmake respectively for building

To build it you go into powershell and:

git clone https://github.com/VladThePickle/TetrisSFML.git
cd TetrisSFML\RawFiles

if you setup vckpg skip this step:
git clone https://github.com/microsoft/vcpkg.git C:/path/to/vcpkg
cd C:/path/to/vcpkg
.\bootstrap-vcpkg.bat        # Windows
# or
./bootstrap-vcpkg.sh         # macOS/Linux

C:/path/to/vcpkg/vcpkg install --triplet x64-windows

then to build you just:
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
