cmake_minimum_required(VERSION 3.16)
project(SFMLProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# 1) Find SFML via vcpkg toolchain (must pass -DCMAKE_TOOLCHAIN_FILE)
# --------------------------------------------------
find_package(SFML 3.0 CONFIG
    REQUIRED
    COMPONENTS Graphics Window System
)

# --------------------------------------------------
# 2) Collect your sources
# --------------------------------------------------
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# --------------------------------------------------
# 3) Define the executable **before** touching its properties
# --------------------------------------------------
add_executable(SFMLProject ${SOURCES})

# --------------------------------------------------
# 4) Include your own headers
# --------------------------------------------------
target_include_directories(SFMLProject
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# --------------------------------------------------
# 5) Link SFML’s imported targets
# --------------------------------------------------
target_link_libraries(SFMLProject
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

# --------------------------------------------------
# 6) Copy your External/resources folder next to the .exe
# --------------------------------------------------
add_custom_command(TARGET SFMLProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/External/resources
        $<TARGET_FILE_DIR:SFMLProject>/resources
)
