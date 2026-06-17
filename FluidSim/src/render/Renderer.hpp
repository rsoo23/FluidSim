#pragma once

#include "shader/VertFragShader.hpp"

class Renderer {
public:
	Renderer(const std::filesystem::path& vertexRelPath, const std::filesystem::path& fragRelPath);
	~Renderer();

	Renderer(const Renderer&)				= delete;
	Renderer& operator=(const Renderer&)	= delete;
	Renderer(Renderer&&)					= delete;
	Renderer& operator=(Renderer&&)			= delete;

	void render(GLuint finalTexture);

private:
	VertFragShader m_VertFragShader;
	GLuint m_VBO{ 0 }, m_VAO{ 0 }, m_EBO{ 0 };
};