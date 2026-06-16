#include "pch.h"
#include "core/Application.hpp"
#include "simulation/FluidSim.hpp"

int main()  
{  
	constexpr unsigned int versionMajor{ 4 };
	constexpr unsigned int versionMinor{ 6 };
	constexpr unsigned int width{ 800 };
	constexpr unsigned int height{ 800 };

	const std::filesystem::path vertexRelPath{ R"(shaders\shader.vert)" };
	const std::filesystem::path fragRelPath{ R"(shaders\fire.frag)" };

	constexpr unsigned int jacobiIterations{ 40u };
	constexpr float diffusionCoeff{ 1.f };
	constexpr float viscosityCoeff{ 0.1f };
	constexpr float densityIncrement{ 0.1f };
	constexpr float cursorRadius{ 20.f };
	constexpr float forceMultiplier{ 100.f };
	constexpr float densityIncrementMultiplier{ 100.f };

	constexpr float vorticityCoeff{ 1.f };

	static_assert(jacobiIterations > 0,           			"jacobiIterations must be > 0");
	static_assert(jacobiIterations <= 40,          			"jacobiIterations must be <= 40");
	static_assert(diffusionCoeff > 0.f,           			"diffusionCoeff must be > 0.0");
	static_assert(diffusionCoeff <= 1.f,          			"diffusionCoeff must be <= 1.0");
	static_assert(viscosityCoeff > 0.f,           			"viscosityCoeff must be > 0.0");
	static_assert(viscosityCoeff <= 1.f,          			"viscosityCoeff must be <= 1.0");
	static_assert(densityIncrement > 0.f,         			"densityIncrement must be > 0.0");
	static_assert(densityIncrement <= 1.f,					"densityIncrement must be <= 1.0");
	static_assert(cursorRadius > 0.f,             	    	"cursorRadius must be > 0.0");
	static_assert(cursorRadius < float(width) / 10.f,    	"cursorRadius must be < width / 10");
	static_assert(cursorRadius < float(height) / 10.f,   	"cursorRadius must be < height / 10");
	static_assert(forceMultiplier > 0.f,          	    	"forceMultiplier must be > 0.0");
	static_assert(forceMultiplier <= 10000.f,           	"force_multiplier must be <= 10000.0");
	static_assert(densityIncrementMultiplier > 0.f,     	"densityIncrementMultiplier must be > 0.0");
	static_assert(densityIncrementMultiplier <= 10000.f,	"densityIncrementMultiplier must be <= 10000.0");
	static_assert(vorticityCoeff >= 0.f,          			"vorticityCoeff must be > 0.0");
	static_assert(vorticityCoeff <= 1.f,         			"vorticityCoeff must be <= 1.0");

	try
	{
		Application app{ versionMajor, versionMinor, width, height, "FluidSim" };

		FluidSim fluidSim{ width, height, jacobiIterations, diffusionCoeff, viscosityCoeff, cursorRadius, vorticityCoeff, forceMultiplier, densityIncrement, densityIncrementMultiplier };

		app.run(vertexRelPath, fragRelPath, fluidSim);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;  
}
