#include "Renderer.h"



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
}

void Renderer::Render()
{

	//for (int i = 0; i < m_ViewportWidth * m_ViewportHeight; i++) {
	//	//m_ImageData[i] = 0xff00ff00; // ABGR
	//	m_ImageData[i] = 0xff000000 | Random::UInt();
	//}
	for (int y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (int x = 0; x < m_FinalImage->GetWidth(); x++) {
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
			
		}
	}
	m_FinalImage->SetData(m_ImageData);

}

uint32_t Renderer::PerPixel(glm::vec2 coord) {
	uint8_t r = uint8_t(coord.x * 255.0f);
	uint8_t g = uint8_t(coord.y * 255.0f);
	uint8_t b = 0;
	return 0xff000000| (b<<16) | (g<<8)  | r;
}