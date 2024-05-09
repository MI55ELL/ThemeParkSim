# CMakeList.txt : CMake project for ThemeParkSim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ThemeParkSim")

include(FetchContent)
## --------------------------------------------------------------------------------------- Adding Glm ------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# ---------------------------------------------------------------------------------------- Adding glad -----------------------------------------------------------------------------------------------------------
set(glad_SOURCE_DIR glad)
add_library(
        glad
        ${glad_SOURCE_DIR}/src/glad.c
        ${glad_SOURCE_DIR}/include/glad/glad.h
        ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
)
target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)

# ------------------------------------------------------------------------------------- Adding GLFW ----------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579 #refs/tags/3.4
)
FetchContent_MakeAvailable(glfw)

# Add source to this project's executable.
add_executable (
    ThemeParkSim
    "ThemeParkSim.cpp"
    "Mesh.h" "Mesh.cpp" 
    "VAO.h" "VAO.cpp" 
    "EBO.h" "EBO.cpp" 
    "Camera.h" "Camera.cpp" 
    "Texture.h" "Texture.cpp"
    "VBO.h" "VBO.cpp"
    "shaderClass.h" "shaderClass.cpp"
    "stb_image.h"
    "stb.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ThemeParkSim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# ---------------------------------------------------------------------------------------- Linking libraries together --------------------------------------------------------------------------------------------------
target_link_libraries(ThemeParkSim glm::glm glfw glad)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
