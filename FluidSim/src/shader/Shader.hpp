#pragma once

class Shader
{
public:
    Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    ~Shader()                           = default;

	Shader()                 			= delete;
	Shader(const Shader&)				= delete;
	Shader& operator=(const Shader&)	= delete;
	Shader(Shader&&)					= delete;
	Shader& operator=(Shader&&)		    = delete;

    void use();

private:
    GLuint programId;
    enum class ShaderType {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };
    void checkShaderError(GLuint shaderId, ShaderType type) const;
};
  