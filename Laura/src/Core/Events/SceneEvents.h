#pragma once

#include "Core/Events/IEvent.h"
#include "Scene/Scene.h"

namespace Laura 
{
	struct SceneCreateEvent : public IEvent {
		SceneCreateEvent() = default;
		inline EventType GetType() const override { return EventType::SCENE_CREATE_EVENT; }
	};

	struct SceneCloseEvent : public IEvent {
		SceneCloseEvent() = default;
		inline EventType GetType() const override { return EventType::SCENE_CLOSE_EVENT; }
	};

	struct SceneLoadedEvent : public IEvent {
		std::weak_ptr<Scene> scene; // only the scene layer owns the scene
		SceneLoadedEvent(std::weak_ptr<Scene> scene) : scene(std::move(scene)){}
		inline EventType GetType() const override { return EventType::SCENE_LOADED_EVENT; }
	};
}
