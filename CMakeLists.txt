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
	"BasicUniform.h"
	"BasicUniform.cpp"
	"PositionColorVertex.h"
	"PositionColorVertex.cpp"
	"ShaderLoader.h"
	"ShaderLoader.cpp"
	"Uploader.h"
	"Uploader.cpp"
	"Common.h"
	"Common.cpp"
	"IndexedQuad.h"
	"IndexedQuad.cpp"
	"TexturedQuad.h"
	"TexturedQuad.cpp"
	"Surface.h"
	"Surface.cpp"
	"PositionColorTextureVertex.h"
	"PositionColorTextureVertex.cpp"
	"Noise.h"
	"Noise.cpp")

if(WIN32)
	add_executable(MainBasicTriangle WIN32 "MainBasicTriangle.cpp")
	add_executable(MainBasicUniform WIN32 "MainBasicUniform.cpp")
	add_executable(MainIndexedQuad WIN32 "MainIndexedQuad.cpp")
	add_executable(MainTexturedQuad WIN32 "MainTexturedQuad.cpp")
else()
	add_executable(MainBasicTriangle "MainBasicTriangle.cpp")
	add_executable(MainBasicUniform "MainBasicUniform.cpp")
	add_executable(MainIndexedQuad "MainIndexedQuad.cpp")
	add_executable(MainTexturedQuad "MainTexturedQuad.cpp")
endif()

target_link_libraries(MainBasicTriangle PRIVATE triangles)
target_link_libraries(MainBasicUniform PRIVATE triangles)
target_link_libraries(MainIndexedQuad PRIVATE triangles)
target_link_libraries(MainTexturedQuad PRIVATE triangles)

option(SDL_STATIC "Build SDL statically, so you don't have to install" ON)

add_subdirectory(SDL-Hpp EXCLUDE_FROM_ALL)
target_link_libraries(triangles PUBLIC SDL-Hpp)

add_subdirectory(glm EXCLUDE_FROM_ALL)
target_link_libraries(triangles PUBLIC glm::glm)

target_precompile_headers(triangles PRIVATE stdafx.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET triangles PROPERTY CXX_STANDARD 20)
  set_property(TARGET MainBasicTriangle PROPERTY CXX_STANDARD 20)
  set_property(TARGET MainBasicUniform PROPERTY CXX_STANDARD 20)
  set_property(TARGET MainIndexedQuad PROPERTY CXX_STANDARD 20)
  set_property(TARGET MainTexturedQuad PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(shaders)
add_dependencies(triangles shaders)

# TODO: Add tests and install targets if needed.
