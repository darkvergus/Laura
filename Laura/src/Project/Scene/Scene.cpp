#include "Scene.h"

namespace Laura
{

	
	EntityHandle Scene::CreateEntity() {
		entt::entity entityID = m_Registry->create();
		EntityHandle entity(entityID, m_Registry);
		entity.GetOrAddComponent<TagComponent>("Empty Entity");
		entity.GetOrAddComponent<GUIDComponent>();
		return entity;
	}


	void Scene::DestroyEntity(EntityHandle entity) {
		m_Registry->destroy(entity.GetID());
	}


	void Scene::OnStart() {
	}


	void Scene::OnUpdate() {
	}


	void Scene::OnShutdown() {
	}


	bool SaveSceneFile(const std::filesystem::path& scenepath, std::shared_ptr<const Scene> scene) {
		if (!(scenepath.has_extension() && scenepath.extension() == SCENE_FILE_EXTENSION)) {
			LOG_ENGINE_WARN("SaveSceneFile: invalid file extension '{}'.", scenepath.string());
			return false;
		}

		if (!std::filesystem::exists(scenepath.parent_path())) {
			LOG_ENGINE_WARN("SaveSceneFile: parent directory '{}' does not exist.", scenepath.parent_path().string());
			return false;
		}

		YAML::Emitter out;
		out << YAML::BeginMap
		<< YAML::Key << "SceneGuid"  << YAML::Value << static_cast<uint64_t>(scene->guid)
		<< YAML::Key << "SceneName"  << YAML::Value << scene->name
		<< YAML::Key << "SkyboxGuid" << YAML::Value << static_cast<uint64_t>(scene->skyboxGuid)
		<< YAML::Key << "SkyboxName" << YAML::Value << scene->skyboxName
		<< YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		// iterate over all entities
		for (auto& e : scene->GetRegistry()->view<entt::entity>()) {
			EntityHandle entity{ e, scene->GetRegistry() };
			out << YAML::BeginMap;

			// Tag component 
			if (entity.HasComponent<TagComponent>()) {
				out << YAML::Key << "TagComponent" << YAML::Value << entity.GetComponent<TagComponent>().Tag;
			}

			// Transform component 
			if (entity.HasComponent<TransformComponent>()) {
				auto& tc = entity.GetComponent<TransformComponent>();
				glm::vec3 translation	= tc.GetTranslation();
				glm::vec3 rotation		= tc.GetRotation();
				glm::vec3 scale			= tc.GetScale();

				out << YAML::Key << "TransformComponent" << YAML::Value 
				<< YAML::BeginMap
					<< YAML::Key << "Translation" << YAML::Value << YAML::Flow
					<< YAML::BeginSeq << translation.x << translation.y << translation.z << YAML::EndSeq

					<< YAML::Key << "Rotation" << YAML::Value << YAML::Flow
					<< YAML::BeginSeq << rotation.x << rotation.y << rotation.z << YAML::EndSeq

					<< YAML::Key << "Scale" << YAML::Value << YAML::Flow
					<< YAML::BeginSeq << scale.x << scale.y << scale.z << YAML::EndSeq
				<< YAML::EndMap;
			}

			// Camera component 
			if (entity.HasComponent<CameraComponent>()) {
				auto& cc = entity.GetComponent<CameraComponent>();
				out << YAML::Key << "CameraComponent" << YAML::Value 
				<< YAML::BeginMap
					<< YAML::Key << "IsMain" << YAML::Value << cc.isMain
					<< YAML::Key << "Fov"    << YAML::Value << cc.fov 
				<< YAML::EndMap;
			}

			// Mesh component 
			if (entity.HasComponent<MeshComponent>()) {
				auto& mc = entity.GetComponent<MeshComponent>();
				out << YAML::Key << "MeshComponent" << YAML::Value 
				<< YAML::BeginMap
					<< YAML::Key << "SourceName" << YAML::Value << mc.sourceName
					<< YAML::Key << "MeshGuid"   << YAML::Value << static_cast<uint64_t>(mc.guid)
				<< YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		// write to scenefile
		std::ofstream fout(scenepath);
		if (!fout) {
			LOG_ENGINE_WARN("SaveSceneFile: failed to open file '{}'.", scenepath.string());
			return false;
		}
		fout << out.c_str();
		LOG_ENGINE_INFO("SaveSceneFile: successfully saved scene to {0}", scenepath.string());
		return true;
	}


	std::shared_ptr<Scene> LoadSceneFile(const std::filesystem::path& scenepath){
		auto deserializeVec3 = [](const YAML::Node& node) -> glm::vec3 {
			return { node[0].as<float>(), node[1].as<float>(), node[2].as<float>() };
		};

		LOG_ENGINE_INFO("Deserializing: {0}", scenepath.string());

		if (!(std::filesystem::exists(scenepath) && std::filesystem::is_regular_file(scenepath) && 
			scenepath.has_extension() && scenepath.extension() == SCENE_FILE_EXTENSION))
		{
			LOG_ENGINE_WARN("LoadSceneFile: invalid or missing scene file: {0}", scenepath.string());
			return nullptr;
		}

		YAML::Node root;
		try {
			root = YAML::LoadFile(scenepath.string());
			auto scene = std::make_shared<Scene>();
			scene->guid	= static_cast<LR_GUID>(root["SceneGuid"].as<uint64_t>());
			scene->name = root["SceneName"].as<std::string>();
			scene->skyboxGuid = static_cast<LR_GUID>(root["SkyboxGuid"].as<uint64_t>());
			scene->skyboxName = root["SkyboxName"].as<std::string>();

			auto entitiesNode = root["Entities"];
			if (entitiesNode && entitiesNode.IsSequence()) {
				for (auto entityNode : entitiesNode) {
					EntityHandle entity = scene->CreateEntity();
					
					auto tag = entityNode["TagComponent"].as<std::string>();
					entity.GetOrAddComponent<TagComponent>().Tag = tag;

					if (entityNode["TransformComponent"]) {
						auto& tc = entity.GetOrAddComponent<TransformComponent>();
						auto tnode = entityNode["TransformComponent"];
						tc.SetTranslation(deserializeVec3(tnode["Translation"]));
						tc.SetRotation	 (deserializeVec3(tnode["Rotation"]));
						tc.SetScale		 (deserializeVec3(tnode["Scale"]));
					}

					if (entityNode["CameraComponent"]) {
						auto& cc = entity.GetOrAddComponent<CameraComponent>();
						auto cnode = entityNode["CameraComponent"];
						cc.isMain      = cnode["IsMain"].as<bool>();
						cc.fov         = cnode["Fov"].as<float>();
					}

					if (entityNode["MeshComponent"]) {
						auto& mc = entity.GetOrAddComponent<MeshComponent>();
						auto mnode = entityNode["MeshComponent"];
						mc.sourceName = mnode["SourceName"].as<std::string>();
						mc.guid       = static_cast<LR_GUID>(mnode["MeshGuid"].as<uint64_t>());
					}
				}
			}
			LOG_ENGINE_INFO("LoadSceneFile: successfully loaded scene from {0}", scenepath.string());
			return scene;
		}
		catch (const YAML::ParserException& e) {
			LOG_ENGINE_ERROR("LoadSceneFile: YAML parse error while reading {0}: {1}", scenepath.string(), e.what());
			return nullptr;
		}
	}
}
