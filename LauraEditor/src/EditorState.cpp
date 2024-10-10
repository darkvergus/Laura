#include "EditorState.h"

namespace Laura
{
    bool serializeState(const std::shared_ptr<const EditorState>& state)
    {
        std::string filepath = EDITOR_STATE_FILE_PATH;
        std::cout << "Serializing: " << filepath << std::endl;
        if (!filepath.ends_with(".yaml"))
        {
            LR_EDITOR_CRITICAL("Invalid file extension for theme file: {0}", filepath);
            return false;
        }

        // Create a file and Check for permission issues, non-existent directory, or another I/O error
        std::ofstream fout(filepath);
        if (!fout.is_open())
        {
            LR_EDITOR_CRITICAL("Could not open file for writing: {0}", filepath);
            return false;
        }

        try 
        {
            YAML::Node node;
            node["doubleConfirmation"] = state->persistent.doubleConfirmation;
            node["viewportMode"] = state->persistent.viewportMode;
            node["ThemeFilePath"] = state->persistent.ThemeFilePath;

            std::ofstream fout(EDITOR_STATE_FILE_PATH);
            fout << node;
            return true;
        }
        catch (const YAML::RepresentationException& e)
        {
            LR_EDITOR_CRITICAL("YAML representation error (invalid syntax?): {0}, error: {1}", EDITOR_STATE_FILE_PATH, e.what());
            return false;
        }
        catch (const std::exception& e)
        {
            LR_EDITOR_CRITICAL("Unknown error occurred while saving file: {0}, error: {1}", EDITOR_STATE_FILE_PATH, e.what());
            return false;
        }
    }

	bool deserializeState(const std::shared_ptr<EditorState>& state)
	{
        std::string filepath = EDITOR_STATE_FILE_PATH;
        std::cout << "Deserializing: " << filepath << std::endl;
        // Check for the correct file extension
        if (!filepath.ends_with(".yaml"))
        {
            LR_EDITOR_CRITICAL("Invalid file extension for theme file: {0}", filepath);
            return false;
        }

        // Check if the file exists and can be opened
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            LR_EDITOR_WARN("Could not open file (Does it exit?): {0}", filepath);
            return false;
        }
        file.close();

        try
		{
			YAML::Node node = YAML::LoadFile(filepath);
			state->persistent.doubleConfirmation = node["doubleConfirmation"].as<bool>();
			state->persistent.viewportMode = node["viewportMode"].as<ViewportMode>();
			state->persistent.ThemeFilePath = node["ThemeFilePath"].as<std::string>();
			return true;
		}
		catch (const YAML::RepresentationException& e)
		{
			LR_EDITOR_CRITICAL("YAML representation error (make sure the file is valid): {0}, error: {1}", filepath, e.what());
			return false;
		}
		catch (const std::exception& e)
		{
			LR_EDITOR_CRITICAL("Unknown error occurred while saving file: {0}, error: {1}", filepath, e.what());
			return false;
		}
	}
}