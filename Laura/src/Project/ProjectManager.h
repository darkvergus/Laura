#pragma once 

#include "lrpch.h"
#include <filesystem>
#include "Project/Scene/SceneManager.h"
#include "Project/Assets/AssetManager.h"

#define PROJECT_FILE_EXTENSION ".lrproj"

namespace Laura 
{

	class ProjectManager {
	public:
		ProjectManager() 
			: m_SceneManager(std::make_shared<SceneManager>()),
			  m_AssetManager(std::make_shared<AssetManager>()) {
		}
		~ProjectManager() = default;

		void NewProject(std::filesystem::path projectRoot);
		bool OpenProject(std::filesystem::path projectRoot);
		bool SaveProject();
		void CloseProject();


		inline std::filesystem::path GetProjectFolderpath() const { return m_ProjectFolderpath; }
		inline std::string GetProjectFoldername() const { return m_ProjectFolderpath.filename().string(); }

		inline bool isProjectOpen() const { return !m_ProjectFolderpath.empty() }
		
		inline std::weak_ptr<SceneManager> GetSceneManager() { return m_SceneManager; }
		inline std::weak_ptr<AssetManager> GetAssetManager() { return m_AssetManager; }

	private:
		bool SerializeProjFile() const;
		bool DeserializeProjFile();

		std::filesystem::path m_ProjectFolderpath;
		LR_GUID m_BootSceneGUID = LR_GUID::INVALID;
		std::shared_ptr<SceneManager> m_SceneManager = nullptr;
		std::shared_ptr<AssetManager> m_AssetManager = nullptr;
	};
}