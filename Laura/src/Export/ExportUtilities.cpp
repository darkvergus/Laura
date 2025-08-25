#include "Export/ExportUtilities.h"
#include "Project/ProjectManager.h"
#include "Project/Assets/AssetManager.h"
#include "EngineCfg.h"

namespace Laura
{

    bool ExportProject(
			const std::string& projectName,
			const std::filesystem::path dstfolderpath, // folder into which the exported project folder is created
			const std::filesystem::path projectFolderPath,
			const ExportSettings& exportSettings) {

		auto copyInto = [](const std::filesystem::path& dst, const std::filesystem::path& src) {
			for (const auto& entry : std::filesystem::recursive_directory_iterator(src)) {
				// e.g. if src = "C:/data" and entry = "C:/data/img/cat.png",
				//      then relPath = "img/cat.png"
				std::filesystem::path relPath = std::filesystem::relative(entry.path(), src);
				// e.g. if dst = "D:/backup", then dest = "D:/backup/img/cat.png"
				std::filesystem::path dest = dst / relPath;
				if (entry.is_directory()) {
					std::filesystem::create_directories(dest);
				} else {
					std::filesystem::create_directories(dest.parent_path());
					std::filesystem::copy_file(entry.path(), dest, std::filesystem::copy_options::overwrite_existing);
				}
			}
		};

		// ensure projectFolderPath contains .lrproj file
		std::filesystem::path srcProjectFilePath = projectFolderPath / (projectFolderPath.filename().string() + PROJECT_FILE_EXTENSION);
		if (!std::filesystem::exists(srcProjectFilePath)) {
			LOG_ENGINE_WARN("ExportProject: {0} does not contain project file {1}", projectFolderPath.string(), srcProjectFilePath.string());
			return false;
		}

		std::filesystem::path exportProjectFolderPath = dstfolderpath / projectName;
		if (std::filesystem::exists(exportProjectFolderPath)) {
			LOG_ENGINE_WARN("ExportProject: export folder already exists {0}", exportProjectFolderPath.string());
			return false;
		}

        try {
			// create the export folder
            std::filesystem::create_directories(exportProjectFolderPath);

            // Copy source folder (containing .lrproj and assets)
            copyInto(exportProjectFolderPath, projectFolderPath);

			// Copy all assets and update .lrmeta files to point to the new paths
			for (const auto& entry : std::filesystem::recursive_directory_iterator(projectFolderPath)) {
				if (entry.path().extension() == ASSET_META_FILE_EXTENSION) {
					std::optional<AssetMetaFile> metaFileOpt = LoadMetaFile(entry.path());
					if (!metaFileOpt.has_value())
						continue;

					AssetMetaFile& metaFile = metaFileOpt.value();
					// Destination path for the asset file
					std::filesystem::path assetFilename = metaFile.sourcePath.filename();
					std::filesystem::path exportAssetPath = exportProjectFolderPath / assetFilename;
					// Copy the asset file
					std::filesystem::copy_file(metaFile.sourcePath, exportAssetPath,
											   std::filesystem::copy_options::overwrite_existing);
					// Update the meta file to point to the new asset path
					metaFile.sourcePath = exportAssetPath;
					// Save the updated meta file in the export folder
					// Destination path for the metafile
					std::filesystem::path metaFileFilename = entry.path().filename();
					std::filesystem::path exportMetaFilePath = exportProjectFolderPath / metaFileFilename;
					SaveMetaFile(exportMetaFilePath, metaFile);
				}
			}

            // Rename the .lrproj
            std::filesystem::path oldProjFile = exportProjectFolderPath / srcProjectFilePath.filename();
            std::filesystem::path newProjFile = exportProjectFolderPath / (projectName + PROJECT_FILE_EXTENSION);
            std::filesystem::rename(oldProjFile, newProjFile);

            // Serialize export settings (creates ExportSettings.yaml)
            SerializeExportSettingsYaml(exportProjectFolderPath, exportSettings);

            // Copy runtime from a known location
            std::filesystem::path runtimePath = EngineCfg::EXECUTABLE_DIR / "runtime";
            copyInto(exportProjectFolderPath, runtimePath);

            // Rename the runtime .exe file
            //std::filesystem::path runtimeExe = exportProjectFolderPath / "runtime.exe_test";
            //std::filesystem::path gameExe = exportProjectFolderPath / (projectName + ".exe_text");
            //std::filesystem::rename(runtimeExe, gameExe);
            return true;
        }
        catch (const std::exception& e) {
			// clean up export folder on failiure
			std::filesystem::remove(exportProjectFolderPath);
			LOG_ENGINE_ERROR("ExportProject: {0}", e.what());
            return false;
        }
    }
}