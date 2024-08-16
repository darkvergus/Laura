#pragma once

#include <Laura.h>

namespace Laura
{

	class EditorLayer : public ILayer
	{
	public:
		EditorLayer(std::shared_ptr<Renderer> renderer);
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event* event) override;
	private:
		std::shared_ptr<Renderer> m_Renderer;
		Camera m_Camera;
		EnvironmentEntity m_Environment;
	private:
		glm::vec2 prevViewportWindowSize, prevViewportWindowPos, viewportSize;
		ImVec2 topLeftTextureCoords, bottomLeftTextureCoords;
		float aspectRatio;
	};

}