# CMakeList.txt : CMake project for Triangles, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Triangles")

add_library(triangles OBJECT
	"BasicTriangle.h"
	"BasicTriangle.cpp"
	"PositionColorVertex.h"
	"PositionColorVertex.cpp"
)

if(WIN32)
	add_executable(BasicTriangleMain WIN32 "BasicTriangleMain.cpp")
else()
	add_executable(BasicTriangleMain "BasicTriangle.cpp")
endif()

target_link_libraries(BasicTriangleMain PRIVATE triangles)

option(SDL_STATIC "Build SDL statically, so you don't have to install" ON)

add_subdirectory(SDL-Hpp EXCLUDE_FROM_ALL)
target_link_libraries(triangles PUBLIC SDL-Hpp)

add_subdirectory(glm EXCLUDE_FROM_ALL)
target_link_libraries(triangles PUBLIC glm::glm)

target_precompile_headers(triangles PRIVATE stdafx.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BasicTriangleMain PROPERTY CXX_STANDARD 20)
  set_property(TARGET triangles PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(shaders)
add_dependencies(triangles shaders)

# TODO: Add tests and install targets if needed.
