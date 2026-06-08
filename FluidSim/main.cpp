#include "pch.h"
#include "core/Application.hpp"
#include "render/Renderer.hpp"
#include "input/InputHandler.hpp"

int main()  
{  
	// application setup
	constexpr int versionMajor{ 3 };
	constexpr int versionMinor{ 3 };
	constexpr int width{ 800 };
	constexpr int height{ 600 };
	constexpr std::string_view title{ "FluidSim" };

	Application app(versionMajor, versionMinor, width, height, title);

	// shader setup
	std::filesystem::path vertexRelPath{ R"(shaders\shader.vert)" };
	std::filesystem::path fragRelPath{ R"(shaders\shader.frag)" };
	std::filesystem::path cwd{ std::filesystem::current_path() };

	Shader shader(cwd / vertexRelPath, cwd / fragRelPath);

	// renderer setup
	std::vector<float> vertices = {
		// positions         // texture coords
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left 
	};

	std::vector<unsigned int> indices = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	const glm::vec4 bgColor(0.f, 0.f, 0.f, 1.f);
	TextureData textureData;
	textureData.width = 400;
	textureData.height = 400;
	textureData.pixels = std::vector<uint8_t>(textureData.width * textureData.height * 3, 0);

	Renderer renderer(bgColor, vertices, indices, textureData);

	// input handler setup
	InputHandler inputHandler;

	// run application
	app.run(shader, renderer, inputHandler);

	return 0;  
}
