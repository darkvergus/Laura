#pragma once 

#include "lrpch.h"
#include <filesystem>
#include "Project/Scene/SceneManager.h"
#include "Project/Assets/AssetManager.h"

namespace Laura 
{

	class ProjectManager {
	public:
		ProjectManager() 
			: m_SceneManager(std::make_shared<SceneManager>()),
			  m_AssetManager(std::make_shared<AssetManager>()) {
		}
		~ProjectManager() = default;

		bool NewProject(std::filesystem::path projectRoot);
		bool OpenProject(std::filesystem::path projectRoot);
		bool SaveProject();
		void CloseProject();

		std::filesystem::path GetProjectRoot() const;
		std::filesystem::path GetProjectFilepath() const;
		std::string GetProjectFilename() const;

		bool isProjectOpen() const;
		
		inline std::weak_ptr<SceneManager> GetSceneManager() { return m_SceneManager; }
		inline std::weak_ptr<AssetManager> GetAssetManager() { return m_AssetManager; }

	private:
		std::filesystem::path m_ActiveProjectRoot;
		std::string m_ProjectFilename = "NewProject";

		LR_GUID m_BootScene = LR_GUID::INVALID;
		std::shared_ptr<SceneManager> m_SceneManager;
		std::shared_ptr<AssetManager> m_AssetManager;
	};
}