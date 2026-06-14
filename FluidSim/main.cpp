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

	const std::filesystem::path vertexRelPath{ R"(shaders\shader.vert)" };
	const std::filesystem::path fragRelPath{ R"(shaders\fire.frag)" };

	constexpr unsigned int jacobiIterations{ 40u };
	constexpr float diffusionCoeff{ 2.f };
	constexpr float viscosityCoeff{ 0.25f };
	constexpr float densityIncrement{ 0.2f };
	constexpr float cursorRadius{ 10.f };
	constexpr float forceMultiplier{ 10000.f };

	constexpr float vorticityCoeff{ 40.f };

	Application app{ versionMajor, versionMinor, width, height, "FluidSim" };

	BaseShader vertShader{ vertexRelPath, GL_VERTEX_SHADER };
	BaseShader fragShader{ fragRelPath, GL_FRAGMENT_SHADER };

	FluidSim fluidSim{ width, height, jacobiIterations, diffusionCoeff, viscosityCoeff, cursorRadius, vorticityCoeff, forceMultiplier, densityIncrement };

	app.run(vertShader, fragShader, fluidSim);

	return 0;  
}
