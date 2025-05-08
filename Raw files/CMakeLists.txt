cmake_minimum_required(VERSION 3.16)
project(RawFilesProject LANGUAGES CXX)

# --- C++ version and build type defaults ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build (Debug or Release)" FORCE)
endif()

# --- SFML setup ---
# Adjust this path if your SFML CMake config lives elsewhere:
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS graphics system window REQUIRED)

# --- Source files ---
set(SOURCE_FILES
    Classes.cpp
    Randomness.cpp
    Main.cpp
)

# --- Target ---
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include your headers and SFML headers
target_include_directories(${PROJECT_NAME}
    PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}"
      "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include"
)

# Link SFML libs
target_link_libraries(${PROJECT_NAME}
    PRIVATE
      sfml-graphics
      sfml-system
      sfml-window
)

# --- Post-build: copy Resources folder next to the .exe ---
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/Resources"
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)

# --- Post-build: copy SFML DLLs ---
file(GLOB SFML_DLLS
    "${CMAKE_SOURCE_DIR}/external/SFML/bin/*.dll"
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${SFML_DLLS}
      $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
