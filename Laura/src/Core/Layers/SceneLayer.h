#pragma once
#include "lrpch.h"

#include "Scene/Scene.h"
#include "Core/Layers/ILayer.h"
#include "Core/Events/Events.h"

namespace Laura
{

	class SceneLayer : public ILayer {
	public:
		SceneLayer();

		inline std::shared_ptr<Scene> GetScene() const { return m_Scene; }

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event* event) override;
	private:
		std::shared_ptr<Scene> m_Scene;
	};
}
