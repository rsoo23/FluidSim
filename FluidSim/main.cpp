#include "pch.h"
#include "core/Application.hpp"
#include "render/Renderer.hpp"
#include "input/InputHandler.hpp"
#include "simulation/FluidSim.hpp"

int main()  
{  
	constexpr unsigned int versionMajor{ 4 };
	constexpr unsigned int versionMinor{ 6 };
	constexpr unsigned int width{ 800 };
	constexpr unsigned int height{ 600 };

	std::filesystem::path vertexRelPath{ R"(shaders\shader.vert)" };
	std::filesystem::path fragRelPath{ R"(shaders\shader.frag)" };
	std::filesystem::path cwd{ std::filesystem::current_path() };

	std::vector<float> vertices = {
		// positions        // texture coords
		 1.0f,  1.0f, 0.0f, 1.0f, 0.0f,   // top right
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,   // bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f    // top left 
	};

	std::vector<unsigned int> indices = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	const glm::vec4 bgColor(0.f, 0.f, 0.f, 1.f);

	constexpr unsigned int jacobiIterations{ 40u };
	constexpr float diffusionCoeff{ 2.f };
	constexpr float viscosityCoeff{ 0.25f };
	constexpr float densityIncrement{ 0.2f };
	constexpr float cursorRadius{ 10.f };
	constexpr float forceMultiplier{ 10000.f };

	constexpr float vorticityCoeff{ 40.f };

	Application app(versionMajor, versionMinor, width, height, "FluidSim");

	Shader shader(cwd / vertexRelPath, cwd / fragRelPath);

	Renderer renderer(bgColor, vertices, indices);

	FluidSim fluidSim(width, height, jacobiIterations, diffusionCoeff, viscosityCoeff, cursorRadius, vorticityCoeff, forceMultiplier);

	app.run(shader, renderer, fluidSim, densityIncrement);

	return 0;  
}
