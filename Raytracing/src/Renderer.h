#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
class Renderer {
public:
	Renderer() = default;

	void onResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

	glm::vec3 cameraPos=glm::vec3(0.0f,0.0f,2.0f);
	glm::vec3 lightDir=glm::vec3(-1.0f,-1.0f,-1.0f);
private:
	glm::vec4 PerPixel(glm::vec2 coord);
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	float m_AspectRatio;
};