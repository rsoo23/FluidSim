# CMakeList.txt : CMake project for FluidSim, include source and define
# project specific logic here.
#

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add source to this project's executable.
add_executable(FluidSim
	"main.cpp"
	"pch.h"
	"src/core/Application.cpp"
	"src/core/GLTexture.cpp"
	"src/render/Renderer.cpp"
	"src/input/InputHandler.cpp"
	"src/shader/BaseShader.cpp"
	"src/shader/ComputeShader.cpp"
	"src/shader/VertFragShader.cpp"
	"src/simulation/FluidSim.cpp"
)

target_link_libraries(FluidSim PRIVATE
    OpenGL::GL
	glfw
	glad::glad
	glm::glm
)

target_include_directories(FluidSim PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(TARGET FluidSim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/
		${CMAKE_CURRENT_BINARY_DIR}/shaders/
	COMMENT "Copying shaders into build output directory"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FluidSim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
