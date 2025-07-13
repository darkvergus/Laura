#include "EditorState.h"

namespace Laura
{
    bool serializeState(const std::shared_ptr<const EditorState>& state)
    {
        std::string filepath = EDITOR_STATE_FILE_PATH;
        if (!filepath.ends_with(".yaml"))
        {
            LOG_EDITOR_CRITICAL("Invalid file extension for theme file: {0}", filepath);
            return false;
        }

        // Create a file and Check for permission issues, non-existent directory, or another I/O error
        std::ofstream fout(filepath);
        if (!fout.is_open())
        {
            LOG_EDITOR_CRITICAL("Could not open file for writing: {0}", filepath);
            return false;
        }

        try 
        {
            YAML::Node node;
            node["doubleConfirmation"] = state->persistent.doubleConfirmEnabled;
            node["viewportMode"] = state->persistent.viewportMode;
            node["ThemeFilePath"] = state->persistent.editorThemeFilepath;

            std::ofstream fout(EDITOR_STATE_FILE_PATH);
            fout << node;
            return true;
        }
        catch (const YAML::RepresentationException& e)
        {
            LOG_EDITOR_CRITICAL("YAML representation error (invalid syntax?): {0}, error: {1}", EDITOR_STATE_FILE_PATH, e.what());
            return false;
        }
        catch (const std::exception& e)
        {
            LOG_EDITOR_CRITICAL("Unknown error occurred while saving file: {0}, error: {1}", EDITOR_STATE_FILE_PATH, e.what());
            return false;
        }
    }

	bool deserializeState(const std::shared_ptr<EditorState>& state)
	{
        std::string filepath = EDITOR_STATE_FILE_PATH;
        // Check for the correct file extension
        if (!filepath.ends_with(".yaml"))
        {
            LOG_EDITOR_CRITICAL("Invalid file extension for theme file: {0}", filepath);
            return false;
        }

        // Check if the file exists and can be opened
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            LOG_EDITOR_WARN("Could not open file (Does it exit?): {0}", filepath);
            return false;
        }
        file.close();

        try
		{
			YAML::Node node = YAML::LoadFile(filepath);
			state->persistent.doubleConfirmEnabled = node["doubleConfirmation"].as<bool>();
			state->persistent.viewportMode = node["viewportMode"].as<ViewportMode>();
			state->persistent.editorThemeFilepath = node["ThemeFilePath"].as<std::string>();
		}
		catch (const YAML::RepresentationException& e)
		{
			LOG_EDITOR_CRITICAL("YAML representation error (make sure the file is valid): {0}, error: {1}", filepath, e.what());
			return false;
		}
		catch (const std::exception& e)
		{
			LOG_EDITOR_CRITICAL("Unknown error occurred while saving file: {0}, error: {1}", filepath, e.what());
			return false;
		}
        
        auto [status, errMsg] = state->temp.editorTheme.LoadFromFile(state->persistent.editorThemeFilepath); // deserialize derived state
        if (state->persistent.editorThemeFilepath == "") {
            LOG_EDITOR_INFO("Using default theme");
        }
        else if (!status) {
            LOG_EDITOR_WARN("Unable to deserialize theme: {0} [Using Default Theme instead]", state->persistent.editorThemeFilepath);
        }
        else {
            LOG_EDITOR_INFO("Successfully loaded theme {0}", state->persistent.editorThemeFilepath);
        }
		return true;
	}
}