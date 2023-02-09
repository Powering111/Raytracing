#include "Renderer.h"

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color) {
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a= (uint8_t)(color.a * 255.0f);
		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

void Renderer::onResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage) {
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) {
			// no resizing
			return;
		}

		m_FinalImage->Resize(width, height);
	}
	else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];

	m_AspectRatio = (float)width / (float)height;
}

void Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
			
		}
	}
	m_FinalImage->SetData(m_ImageData);

}

glm::vec4 Renderer::TraceRay(const Ray& ray) {

	float radius = 0.5f;

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = glm::dot(ray.Origin, ray.Direction) * 2.0f;
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0) {
		// not hit to sphere
		return glm::vec4(0,0,0,1);
	}

	// hit point
	float t0 = (-b - glm::sqrt(discriminant)) / (a * 2.0f);
	float t1 = (-b + glm::sqrt(discriminant)) / (a * 2.0f);
	glm::vec3 hitPoint;
	
	if (t0 > 0) {
		hitPoint = ray.Origin + ray.Direction * t0;
	}
	else if (t1 > 0) {
		hitPoint = ray.Origin + ray.Direction * t1;
	}
	else {
		// not hit to sphere
		return glm::vec4(0, 0, 0, 1);
	}
	glm::vec3 normal=glm::normalize(hitPoint);

	glm::vec3 lightDirection = glm::normalize(lightDir);

	float deg = glm::max(glm::dot(normal, -lightDirection), 0.0f);

	glm::vec3 sphereColor(0, 1, 0);
	sphereColor = sphereColor * deg;

	//sphereColor = (normal+glm::vec3(1.0f))/2.0f;

	return glm::vec4(sphereColor,1.0f);
	
}