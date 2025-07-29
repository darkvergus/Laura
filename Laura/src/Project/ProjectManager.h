#pragma once 

#include "lrpch.h"
#include <filesystem>
//#include "Project/Scene/SceneManager.h"
#include "Project/Assets/AssetManager.h"

namespace Laura 
{
	
	class ProjectManager {
	public:
		ProjectManager() 
			: m_AssetManager(std::make_shared<AssetManager>()) {
		}
		~ProjectManager() = default;

		bool NewProject(std::filesystem::path projectRoot);
		bool OpenProject(std::filesystem::path projectRoot);

	private:
		std::filesystem::path m_ActiveProjectRoot;
		std::shared_ptr<AssetManager> m_AssetManager;
	};
}