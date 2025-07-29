#include "Project/Scene/SceneManager.h"

namespace Laura 
{
    LR_GUID SceneManager::NewScene(const std::string& name) {
        return LR_GUID();
    }

    LR_GUID SceneManager::LoadScene(const std::string& name, const std::filesystem::path& filepath) {
        auto scene = std::make_shared<Scene>();
        LR_GUID guid = scene->Deserialize(filepath);

        if (guid == LR_GUID::INVALID) {
            return LR_GUID::INVALID;
        }

        m_Scenes[guid] = scene;
        return guid;
    }

    void SceneManager::RemoveScene(LR_GUID guid) {
        if (m_ActiveSceneGUID == guid)  { m_ActiveSceneGUID = LR_GUID::INVALID; }
        if (m_BootSceneGUID == guid)    { m_BootSceneGUID   = LR_GUID::INVALID; }
        m_Scenes.erase(guid);
    }

    bool SceneManager::SetActiveScene(const std::string& name) {
        if (m_Scenes.find(name) == m_Scenes.end()) {
            return false;
        }
        m_ActiveSceneName = name;
        m_Scenes[m_ActiveSceneName]->OnStart();
        return true;
    }

    std::shared_ptr<Scene> SceneManager::GetActiveScene() const {
        if (m_ActiveSceneName.empty()) {
            return nullptr;
        }

        if (auto it = m_Scenes.find(m_ActiveSceneName); it != m_Scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool SceneManager::SetBootScene(const std::string& name) {
        if (m_Scenes.find(name) == m_Scenes.end()) {
            return false;
        }
        m_BootSceneName = name;
        return true;
    }

    std::shared_ptr<Scene> SceneManager::GetBootScene() const {
        if (m_BootSceneName.empty()) {
            return nullptr;
        }
        if (auto it = m_Scenes.find(m_BootSceneName); it != m_Scenes.end()) {
            return it->second;
        }
        return nullptr;
    }

    void SceneManager::StartBootScene() {
        if (!m_BootSceneName.empty()) {
            SetActiveScene(m_BootSceneName);
        }
    }

    void SceneManager::UpdateActiveScene() {
        if (auto scene = GetActiveScene()) {
            scene->OnUpdate();
        }
    }

    void SceneManager::Shutdown() {
        for (auto& [name, scene] : m_Scenes) {
            scene->OnShutdown();
        }
        m_Scenes.clear();
        m_ActiveSceneName.clear();
        m_BootSceneName.clear();
    }
}
