#pragma once

#include <Laura.h>

class SandboxLayer : public Laura::ILayer
{
public:
	SandboxLayer(std::shared_ptr<Laura::Renderer> renderer);
	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(Laura::Event* event) override;
private:
	std::shared_ptr<Laura::Renderer> m_Renderer;

	Laura::Camera m_Camera;
	Laura::EnvironmentEntity m_Environment;
};