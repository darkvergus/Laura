#include "Project/Scene/SceneManager.h"

namespace Laura 
{

    LR_GUID SceneManager::NewScene(const std::string& name) {
        auto scene = std::make_shared<Scene>(name);
        m_Scenes[scene->GetGUID()] = scene;
        return scene->GetGUID();
    }

    void SceneManager::RemoveScene(LR_GUID guid) {
        if (m_ActiveSceneGUID == guid)  { m_ActiveSceneGUID = LR_GUID::INVALID; }
        if (m_BootSceneGUID == guid)    { m_BootSceneGUID   = LR_GUID::INVALID; }
        size_t removed = m_Scenes.erase(guid);
        if (removed == 0) {
            LOG_ENGINE_WARN("Scene to be erased Not Found");
        }
    }

    bool SceneManager::SetActiveScene(LR_GUID guid) {
        if (guid == LR_GUID::INVALID) {
            LOG_ENGINE_WARN("Invalid GUID passed")
            return false;
        }
        auto it = m_Scenes.find(guid); 
        if (it == m_Scenes.end()) {
            LOG_ENGINE_WARN("Scene Not found");
            return false;
        }
        m_ActiveSceneGUID = guid; 
        return true;
    }

    std::shared_ptr<Scene> SceneManager::GetActiveScene() const {
        if (m_ActiveSceneGUID == LR_GUID::INVALID){
            LOG_ENGINE_WARN("Invalid GUID passed")
            return nullptr;
        }
        if (auto it = m_Scenes.find(m_ActiveSceneGUID); it != m_Scenes.end()) {
            return it->second;
        }
        LOG_ENGINE_WARN("Scene Not found");
        return nullptr;
    }

    bool SceneManager::SaveScenes(const std::filesystem::path& projectRoot) const {
       bool successAll = true;

		// Remove orphaned scene files
		for (const auto& filepath : FindFilesInFolder(projectRoot, SCENE_FILE_EXTENSION)) {
			LR_GUID guid = GuidFromFilename(filepath.filename());
            if (guid == LR_GUID::INVALID) {
                LOG_ENGINE_WARN("Unable to delete scene file: {0}", filepath);
                continue;
            }

			if (m_Scenes.find(guid) == m_Scenes.end()) {
				std::error_code ec;
				std::filesystem::remove(filepath, ec);
				if (ec) {
					successAll = false;
                    LOG_ENGINE_WARN("Unable to delete scene file: {0}", filepath);
				}
			}
		}

		// serialize all scenes
		for (const auto& [guid, scene] : m_Scenes) {
            auto filepath = SceneFilepathFromGuid(projectRoot, scene->GetGUID());
			if (!scene->Serialize(filepath)) {
				successAll = false;
                LOG_ENGINE_WARN("Unable to serialize scene file: {0}", filepath);
			}
		}

		return successAll; 
    }

    bool SceneManager::LoadScenes(const std::filesystem::path& projectRoot) {
        bool successAll = true;
        for (const auto& scenepath : FindFilesInFolder(projectRoot, SCENE_FILE_EXTENSION)) {
            auto scene = std::make_shared<Scene>();
            bool success = scene->Deserialize(scenepath);
            if (!success) { 
                successAll = false; 
                LOG_ENGINE_WARN("Unable to Deserialize scene file: {0}", filepath);
                continue;
            }
            m_Scenes[scene->GetGUID()] = scene;
        }
        return successAll;
    }

    static std::vector<std::filesystem::path> SceneManager::FindFilesInFolder(
            const std::filesystem::path& folder, 
            const std::string& extension) {
        std::vector<std::filesystem::path> result;
		if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
            LOG_ENGINE_WARN("Folder does not exists or is not a directory {0}", folder);
			return result;
		}
		for (const auto& entry : std::filesystem::directory_iterator(folder)) {
			if (entry.is_regular_file() && entry.path().extension() == extension) {
				result.push_back(entry.path());
			}
		}
		return result; 
    }

    std::filesystem::path SceneManager::SceneFilepathFromGuid(const std::filesystem::path& projectRoot, LR_GUID guid) {
        return projectRoot / (std::to_string(static_cast<uint64_t>(guid)) + SCENE_FILE_EXTENSION);
    }

    LR_GUID SceneManager::GuidFromSceneFilepath(const std::filesystem::path& filepath) {
        std::string stem = filepath.filename().stem().string(); // remove extension
        try {
            uint64_t value = std::stoull(stem); // convert String TO Unsigned Long Long - STOULL 
            return LR_GUID(value);
        }
        catch (const std::exception&) {
            LOG_ENGINE_WARN("Unable to extract GUID from filename: {0}", filepath);
            return LR_GUID::INVALID;
        }
    }
}
