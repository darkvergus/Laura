#include <Laura.h>
#include <LauraEntrypoint.h>
#include "EditorLayer.h"
#include "ImGuiContext.h"  

namespace Laura
{

	class LauraEditor : public Application {
	public:
		LauraEditor()
			: Application() {
		}

		void init() override {
			Application::init();

			m_ImGuiContext = std::make_shared<ImGuiContext>(_Window);
			m_ImGuiContext->Init();

			_LayerStack->PushLayer(std::make_shared<EditorLayer>(_Profiler, _LayerStack, _ProjectManager, m_ImGuiContext));
		}

		~LauraEditor() {
		}

	private:
		std::shared_ptr<ImGuiContext> m_ImGuiContext;
	};

	Application* CreateApplication() {
		return new LauraEditor();
	}
}