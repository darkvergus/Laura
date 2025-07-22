#include "Core\Layers\SceneLayer.h"

namespace Laura
{
	SceneLayer::SceneLayer()
		: m_Scene(std::make_shared<Scene>()) {
	}

	void SceneLayer::onAttach() {
		m_Scene->OnStart();
	}

	void SceneLayer::onDetach() {
		m_Scene->OnShutdown();
	}

	void SceneLayer::onUpdate() {
		m_Scene->OnUpdate();
	}

	void SceneLayer::onImGuiRender() {
		// no rendering in this layer
	}

	void SceneLayer::onEvent(Event* event) {
		// no events should get here 
	}
}