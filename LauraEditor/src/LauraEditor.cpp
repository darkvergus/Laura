/////////////////////////////
/// OFFICIAL LAURA EDITOR ///
/////////////////////////////

#include <Laura.h>
#include <LauraEntrypoint.h>
#include <EditorLayer.h>

namespace Laura
{

	class LauraEditor : public Application
	{
	public:
		LauraEditor()
			: Application() // call the default application constructor
		{
		}

		void init() override
		{
			IRendererAPI::SetAPI(IRendererAPI::API::OpenGL);
			Application::init(); // call the default init method first
			_LayerStack->PushLayer(std::make_shared<EditorLayer>(_Renderer));
		}

		~LauraEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new LauraEditor();
	}

}