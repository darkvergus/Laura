#include "Scene.h"

namespace Laura
{

	EntityHandle Scene::CreateEntity() {
		entt::entity entityID = m_Registry->create();
		EntityHandle entity(entityID, m_Registry);
		entity.AddComponent<TagComponent>("Empty Entity");
		entity.AddComponent<GUIDComponent>();
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

	bool Laura::Scene::Serialize(const std::filesystem::path& filepath) {
		LOG_ENGINE_INFO("Serializing: {0}", filepath.string());
		if (filepath.extension() != SCENE_FILE_EXTENSION) {
			return false;
		}

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "SceneGUID" << YAML::Value << (uint64_t)m_SceneGUID;
		out << YAML::Key << "SceneName" << YAML::Value << m_SceneName;
		out << YAML::Key << "SkyboxGUID" << YAML::Value << (uint64_t)m_SkyboxGUID;

		// Write entities
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto& e : m_Registry->view<entt::entity>()) {
			EntityHandle entity{ e, m_Registry };
			out << YAML::BeginMap;

			// TAG COMPONENT
			if (entity.HasComponent<TagComponent>()) {
				out << YAML::Key << "TagComponent" << YAML::Value << entity.GetComponent<TagComponent>().Tag;
			}

			// TRANSFORM COMPONENT
			if (entity.HasComponent<TransformComponent>()) {
				auto& tc = entity.GetComponent<TransformComponent>();
				out << YAML::Key << "TransformComponent" << YAML::Value << YAML::BeginMap;

				glm::vec3 translation = tc.GetTranslation();
				out << YAML::Key << "Translation" << YAML::Value << YAML::Flow
					<< YAML::BeginSeq << translation.x << translation.y << translation.z << YAML::EndSeq;

				glm::vec3 rotation = tc.GetRotation();
				out << YAML::Key << "Rotation" << YAML::Value << YAML::Flow
					<< YAML::BeginSeq << rotation.x << rotation.y << rotation.z << YAML::EndSeq;

				glm::vec3 scale = tc.GetScale();
				out << YAML::Key << "Scale" << YAML::Value << YAML::Flow
					<< YAML::BeginSeq << scale.x << scale.y << scale.z << YAML::EndSeq;

				out << YAML::EndMap;
			}

			// CAMERA COMPONENT
			if (entity.HasComponent<CameraComponent>()) {
				auto& cc = entity.GetComponent<CameraComponent>();
				out << YAML::Key << "CameraComponent" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "IsMain" << YAML::Value << cc.isMain;
				out << YAML::Key << "FOV" << YAML::Value << cc.fov;

				out << YAML::EndMap;
			}

			// MESH COMPONENT
			if (entity.HasComponent<MeshComponent>()) {
				auto& mc = entity.GetComponent<MeshComponent>();
				out << YAML::Key << "MeshComponent" << YAML::Value << YAML::BeginMap;
				
				out << YAML::Key << "SourceName" << YAML::Value << mc.sourceName;
				out << YAML::Key << "MeshGUID" << YAML::Value << (uint64_t)mc.guid;

				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		if (!fout) {
			return false;
		}

		fout << out.c_str();
		return true;
	}

	bool Laura::Scene::Deserialize(const std::filesystem::path& filepath) {
		LOG_ENGINE_INFO("Deserializing: {0}", filepath.string());

		if (filepath.extension() != SCENE_FILE_EXTENSION) {
			return false;
		}

		YAML::Node root;
		try {
			root = YAML::LoadFile(filepath.string());
		}
		catch (const YAML::ParserException& e) {
			LOG_ENGINE_ERROR("YAML parse error: {}", e.what());
			return false;
		}

		m_Registry->clear();
		if (root["SceneGUID"]) {
			m_SceneGUID = (LR_GUID)root["SceneGUID"].as<uint64_t>();
		}
		if (root["SceneName"]) {
			m_SceneGUID = root["SceneName"].as<std::string>();
		}
		if (root["SkyboxGUID"]) {
			m_SkyboxGUID = (LR_GUID)root["SkyboxGUID"].as<uint64_t>();
		}

		auto entitiesNode = root["Entities"];
		if (entitiesNode && entitiesNode.IsSequence()) {
			for (auto entityNode : entitiesNode) {
				EntityHandle entity = CreateEntity();

				if (entityNode["TagComponent"]) {
					auto tag = entityNode["TagComponent"].as<std::string>();
					entity.GetComponent<TagComponent>().Tag = tag;
				}

				if (entityNode["TransformComponent"]) {
					auto& tc = entity.HasComponent<TransformComponent>()
									 ? entity.GetComponent<TransformComponent>()
									 : entity.AddComponent<TransformComponent>();

					auto tnode = entityNode["TransformComponent"];
					auto pos = tnode["Translation"];
					tc.SetTranslation({ pos[0].as<float>(), pos[1].as<float>(), pos[2].as<float>() });

					auto rot = tnode["Rotation"];
					tc.SetRotation({ rot[0].as<float>(), rot[1].as<float>(), rot[2].as<float>() });

					auto scl = tnode["Scale"];
					tc.SetScale({ scl[0].as<float>(), scl[1].as<float>(), scl[2].as<float>() });
				}

				// CameraComponent
				if (entityNode["CameraComponent"]) {
					auto& cc = entity.HasComponent<CameraComponent>()
									? entity.GetComponent<CameraComponent>()
									: entity.AddComponent<CameraComponent>();

					auto cnode = entityNode["CameraComponent"];
					cc.isMain      = cnode["IsMain"].as<bool>();
					cc.fov         = cnode["FOV"].as<float>();
				}

				// MeshComponent
				if (entityNode["MeshComponent"]) {
					auto& mc = entity.HasComponent<MeshComponent>()
									? entity.GetComponent<MeshComponent>()
									: entity.AddComponent<MeshComponent>();

					auto mnode = entityNode["MeshComponent"];
					mc.sourceName = mnode["SourceName"].as<std::string>();
					mc.guid       = (LR_GUID)mnode["MeshGUID"].as<uint64_t>();
				}

			}
		}
		return true;
	}
}
