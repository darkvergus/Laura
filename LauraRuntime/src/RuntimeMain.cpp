#include <Laura.h>
#include <LauraEntrypoint.h>
#include <filesystem>
#include "RuntimeLayer.h"
#include "RuntimeCfg.h"

namespace Laura
{

	class LauraRuntime : public Application {
	public:
		LauraRuntime(WindowProps windowProps)
			: Application(windowProps) {

			_LayerStack->PushLayer(std::make_shared<RuntimeLayer>(_Window, _Profiler, _LayerStack, _ProjectManager));
		}

		~LauraRuntime() {

		}
	};

	Application* CreateApplication(const std::filesystem::path& exeDir) {
		RuntimeCfg::Init(exeDir); // init EXECUTABLE_DIR
		WindowProps spec;
		spec.CustomTitlebar = false;
		return new LauraRuntime(spec);
	}
}
