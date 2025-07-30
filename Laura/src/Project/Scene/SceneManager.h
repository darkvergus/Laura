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
        void RemoveScene(LR_GUID guid);

        bool SetActiveScene(LR_GUID guid);
        std::shared_ptr<Scene> GetActiveScene() const;

        bool SaveScenes(const std::filesystem::path& projectRoot) const;
        bool LoadScenes(const std::filesystem::path& projectRoot);

    private:
        std::unordered_map<LR_GUID, std::shared_ptr<Scene>> m_Scenes;
        LR_GUID m_ActiveSceneGUID = LR_GUID::INVALID;
        LR_GUID m_BootSceneGUID = LR_GUID::INVALID;
        
        // helper funcs
        static std::vector<std::filesystem::path> FindFilesInFolder(
            const std::filesystem::path& folder, const std::string& extension);

        static LR_GUID GuidFromSceneFilepath(const std::filesystem::path& filepath);
        static std::filesystem::path SceneFilepathFromGuid(const std::filesystem::path& projectRoot, LR_GUID guid);
    };
}
