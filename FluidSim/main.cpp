#include "pch.h"
#include "core/Application.hpp"
#include "render/Renderer.hpp"

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
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
	};

	std::vector<unsigned int> indices = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	TextureData textureData;
	textureData.width = 400;
	textureData.height = 300;
	textureData.pixels = std::vector<uint8_t>(textureData.width * textureData.height * 3, 128);

	Renderer renderer(vertices, indices, textureData);

	app.run(shader, renderer);

	return 0;  
}
