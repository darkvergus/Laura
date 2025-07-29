#pragma once

#include "lrpch.h"
#include <filesystem>
#include "Project/Scene/Scene.h"
#include "Core/GUID.h"

namespace Laura
{

    class SceneManager {
    public:
        SceneManager() = default;
        ~SceneManager() = default;

        LR_GUID NewScene(const std::string& name);
        LR_GUID LoadScene(const std::string& name, const std::filesystem::path& filepath);
        void RemoveScene(LR_GUID guid);

        bool SetActiveScene(LR_GUID guid);
        std::shared_ptr<Scene> GetActiveScene() const;

        bool SetBootScene(LR_GUID guid);
        std::shared_ptr<Scene> GetBootScene() const;

        void StartBootScene();
        void UpdateActiveScene();
        void Shutdown();

    private:
        std::unordered_map<LR_GUID, std::shared_ptr<Scene>> m_Scenes;
        LR_GUID m_ActiveSceneGUID;
        LR_GUID m_BootSceneGUID;
    };
}
