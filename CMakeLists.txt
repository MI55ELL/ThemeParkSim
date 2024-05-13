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

# ------------------------------------------------------------------------------------- Adding assimp --------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG 8b9ed34eaa3e6ad24254cb7e058fb9150f66b865 #refs/tag/v5.4.0
)
FetchContent_MakeAvailable(assimp)

#-------------------------------------------------------------------------------------Adding IMGUI ----------------------------------------------------------------------------------------------------------
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 231cbee0fc4f59dbe5b8b853a11b08dc84e57c65 #refs/tags/v1.90.5
)
FetchContent_MakeAvailable(imgui)
file(GLOB imgui_SRC ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)
add_library(
        imgui
        ${imgui_SRC}
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
)
target_link_libraries(imgui PUBLIC glad glfw)
target_include_directories(
        imgui
        PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# Add source to this project's executable.
add_executable (
    ThemeParkSim
    "ThemeParkSim.cpp"
    "ModelMesh.h"  
    "mesh.h" "mesh.cpp"
    "VAO.h" "VAO.cpp" 
    "EBO.h" "EBO.cpp" 
    "Camera.h" "Camera.cpp" 
    "Texture.h" "Texture.cpp"
    "VBO.h" "VBO.cpp"
    "shaderClass.h" "shaderClass.cpp"
    "stb_image.h"
    "stb.cpp"
    "Model.h"
 "Duck.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ThemeParkSim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# ---------------------------------------------------------------------------------------- Linking libraries together --------------------------------------------------------------------------------------------------
target_link_libraries(ThemeParkSim glm::glm glfw glad assimp imgui)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
