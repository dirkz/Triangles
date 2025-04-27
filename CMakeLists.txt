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

find_package(Boost REQUIRED)

add_library(triangles OBJECT
	"PositionColorVertex.h"
	"PositionColorVertex.cpp"
	"ShaderLoader.h"
	"ShaderLoader.cpp"
	"Uploader.h"
	"Uploader.cpp"
	"Common.h"
	"Common.cpp"
	"Surface.h"
	"Surface.cpp"
	"PositionColorTextureVertex.h"
	"PositionColorTextureVertex.cpp"
	"Noise.h"
	"Noise.cpp")

add_library(samples OBJECT
	"BasicTriangle.h"
	"BasicTriangle.cpp"
	"BasicUniform.h"
	"BasicUniform.cpp"
	"IndexedQuad.h"
	"IndexedQuad.cpp"
	"TexturedQuad.h"
	"TexturedQuad.cpp"
	"Cube.h"
	"Cube.cpp"
	"IndexedVertexBuffer.h")

if(WIN32)
	add_executable(Main WIN32 "main.cpp")
else()
	add_executable(Main "main.cpp")
endif()

target_link_libraries(Main PRIVATE triangles samples)

option(SDL_STATIC "Build SDL statically, so you don't have to install" ON)

add_subdirectory(SDL-Hpp EXCLUDE_FROM_ALL)
target_link_libraries(triangles PUBLIC SDL-Hpp)
target_link_libraries(samples PUBLIC SDL-Hpp)

add_subdirectory(glm EXCLUDE_FROM_ALL)
target_link_libraries(triangles PUBLIC glm::glm)
target_link_libraries(samples PUBLIC glm::glm)

target_include_directories(triangles PUBLIC ${Boost_INCLUDE_DIRS})

target_precompile_headers(triangles PRIVATE stdafx.h)
target_precompile_headers(samples PRIVATE stdafx.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET triangles PROPERTY CXX_STANDARD 20)
  set_property(TARGET samples PROPERTY CXX_STANDARD 20)
  set_property(TARGET Main PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(shaders)
add_dependencies(triangles shaders)

# TODO: Add tests and install targets if needed.
