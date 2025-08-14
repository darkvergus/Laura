#pragma once

#include "Laura.h"
#include <array>
#include "EditorState.h"
#include "EditorUI/IEditorPanel.h"

namespace Laura
{

	class RenderSettingsPanel : public IEditorPanel {
	public:
		RenderSettingsPanel(std::shared_ptr<EditorState> editorState, std::shared_ptr<IEventDispatcher> eventDispatcher, std::shared_ptr<ProjectManager> projectManager)
			: m_EditorState(editorState), m_EventDispatcher(eventDispatcher), m_ProjectManager(projectManager) {
        }

		~RenderSettingsPanel() = default;

        virtual void init() override;
		virtual void OnImGuiRender() override;
		virtual inline void onEvent(std::shared_ptr<IEvent> event) override {}

	private:
		struct ResolutionOption {
			glm::vec2 resolution;      // {width, height}
			const char* label;   // friendly name or description
		};

        const std::array<ResolutionOption, 17> m_ResolutionOptions = {{
            // 4:3
            {{640, 480},    "VGA 640x480 (4:3)"},
            {{800, 600},    "SVGA 800x600 (4:3)"},
            {{1024, 768},   "XGA 1024x768 (4:3)"},

            // 16:10
            {{1280, 800},   "WXGA 1280x800 (16:10)"},
            {{1440, 900},   "WXGA+ 1440x900 (16:10)"},
            {{1680, 1050},  "WSXGA+ 1680x1050 (16:10)"},
            {{1920, 1200},  "WUXGA 1920x1200 (16:10)"},

            // 16:9
            {{1280, 720},   "HD 1280x720 (16:9)"},
            {{1920, 1080},  "Full HD 1920x1080 (16:9)"},
            {{2560, 1440},  "QHD 2560x1440 (16:9)"},
            {{3840, 2160},  "4K UHD 3840x2160 (16:9)"},

            // Vertical (9:16)
            {{1080, 1920},  "Vertical Full HD 1080x1920 (9:16)"},
            {{720, 1280},   "Vertical HD 720x1280 (9:16)"},
            {{540, 960},    "Vertical qHD 540x960 (9:16)"},
            {{480, 854},    "Vertical FWVGA 480x854 (9:16)"},
            {{1080, 2340},  "Vertical FHD+ 1080x2340 (9:19.5)"},
            {{1440, 3200},  "Vertical WQHD+ 1440x3200 (9:20)"}
        }};

		std::shared_ptr<EditorState> m_EditorState;
        std::shared_ptr<IEventDispatcher> m_EventDispatcher;
        std::shared_ptr<ProjectManager> m_ProjectManager;
	};
}