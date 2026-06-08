#pragma once

#include <glm/glm.hpp>
#include "shader/Shader.hpp"

struct TextureData {
	unsigned int width;
	unsigned int height;
	std::vector<uint8_t> pixels;
};

class Renderer {
public:
	Renderer(
		const glm::vec4& bgColor,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices,
		const TextureData& textureData
	);
	~Renderer();

	Renderer()								= delete;
	Renderer(const Renderer&)				= delete;
	Renderer& operator=(const Renderer&)	= delete;
	Renderer(Renderer&&)					= delete;
	Renderer& operator=(Renderer&&)			= delete;

	void render(Shader& shader);

private:
	unsigned int m_TextureId, m_VBO, m_VAO, m_EBO, m_PBO;
	glm::vec4 m_BgColor;
};