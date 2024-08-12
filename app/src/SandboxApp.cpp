///////////////////////////////////////////////////////////
/// THIS IS A RAY TRACING DEMO APP FOR THE LAURA ENGINE ///
///////////////////////////////////////////////////////////

#include <Laura.h>
#include <LauraEntrypoint.h>
#include <SandboxLayer.h>

class Sandbox : public Laura::Application
{
public:
	Sandbox()
		: Laura::Application()
	{

	}

	void init() override
	{
		Laura::Application::init();
		m_LayerStack->PushLayer(new SandboxLayer(m_Renderer));
	}

	~Sandbox()
	{

	}
};

Laura::Application* Laura::CreateApplication()
{
	return new Sandbox();
}