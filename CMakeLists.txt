# CMakeList.txt : CMake project for Triangles, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Triangles")

if(WIN32)
	add_executable (${CMAKE_PROJECT_NAME} WIN32 "main.cpp" "Triangles.h" "Triangles.cpp")
else()
	add_executable (${CMAKE_PROJECT_NAME} "main.cpp")
endif()

option(SDL_STATIC "Build SDL statically, so you don't have to install" ON)

add_subdirectory(SDL-Hpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL-Hpp)

add_subdirectory(glm)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm::glm)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE stdafx.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

add_subdirectory(shaders)
add_dependencies(${CMAKE_PROJECT_NAME} shaders)

# TODO: Add tests and install targets if needed.
