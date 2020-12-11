cmake_minimum_required(VERSION 3.12)

project(SOP VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Do shared compiles
set(BUILD_SHARED_LIBS ON)

#Add relevant packages
include(cmake/CPM.cmake)
CPMAddPackage(
  NAME SFML
  URL "https://github.com/SFML/SFML/archive/2.5.1.zip"
  VERSION 2.5.1
)

 
CPMAddPackage(
  NAME imgui
  URL "https://github.com/ocornut/imgui/archive/v1.68.zip"
  VERSION 1.68
)
 
set(IMGUI_ROOT ${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)
 
CPMAddPackage(
  NAME imgui-sfml
  GITHUB_REPOSITORY eliasdaler/imgui-sfml
  GIT_TAG        5f54b69b6aee18db846c81633f86f78c2586dded
)

CPMAddPackage(
  NAME glew
  URL "https://github.com/Perlmint/glew-cmake/archive/glew-cmake-2.1.0.zip"
  VERSION 2.1.0
)

CPMAddPackage(
  NAME glm
  URL "https://github.com/g-truc/glm/archive/0.9.9.8.zip"
  VERSION 0.9.9.8
)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HDR_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)

add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    ${CMAKE_BINARY_DIR}/res
)


include_directories(${PROJECT_SOURCE_DIR}/src/tinyfiledialogs ${glew_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES} ${HDR_FILES} "${CMAKE_SOURCE_DIR}/src/tinyfiledialogs/tinyfiledialogs.c" "${CMAKE_SOURCE_DIR}/src/tinyfiledialogs/tinyfiledialogs.h")
add_dependencies(${CMAKE_PROJECT_NAME} copy-files)
target_link_libraries(${CMAKE_PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system ImGui-SFML::ImGui-SFML libglew_static glm)
