// EDITOR

#include <Laura.h>
#include <LauraEntrypoint.h>
#include <EditorLayer.h>

namespace Laura
{

	class LauraEditor : public Application {
	public:
		LauraEditor()
			: Application() {
		}

		void init() override {
			IRendererAPI::SetAPI(IRendererAPI::API::OpenGL);
			Application::init();
			_LayerStack->PushLayer(std::make_shared<EditorLayer>(_LayerStack, _ResourcePool, _AssetManager, _Profiler));
		}

		~LauraEditor() {
		}
	};

	Application* CreateApplication() {
		return new LauraEditor();
	}
}