#pragma once

#include <filesystem>
#include "BaseShader.hpp"

class VertFragShader : public BaseShader
{
public:
    VertFragShader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath);

    ~VertFragShader()									= default;

	VertFragShader()							        = delete;
	VertFragShader(const VertFragShader&)		        = delete;
	VertFragShader& operator=(VertFragShader&&)			= delete;
	VertFragShader& operator=(const VertFragShader&)    = delete;
	VertFragShader(VertFragShader&&)					= delete;
};
