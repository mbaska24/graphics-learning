# CMakeList.txt : CMake project for graphics-learning, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("graphics-learning")

# Add source to this project's executable.
file(GLOB UTIL_FILES "util/*.h")
file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB HEADER_FILES "src/*.h" "src/*.hpp")
add_executable (graphics-learning "src/graphics-learning.cpp" "src/graphics-learning.h" ${UTIL_FILES} ${SOURCE_FILES} ${HEADER_FILES})

add_subdirectory(include/glad)
target_link_libraries(graphics-learning glad)

add_subdirectory(include/glfw)
target_link_libraries(graphics-learning glfw)

add_subdirectory(include/glm)
target_link_libraries(graphics-learning glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET graphics-learning PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
