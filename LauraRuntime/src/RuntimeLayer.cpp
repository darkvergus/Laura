#include "RuntimeLayer.h"
#include <GL/glew.h>
#include "RuntimeCfg.h"
#include <filesystem>
#include <algorithm>
#include "Core/Events/WindowEvents.h"

namespace Laura
{
	RuntimeLayer::RuntimeLayer(std::shared_ptr<IWindow> window,
							   std::shared_ptr<Profiler> profiler,
							   std::shared_ptr<IEventDispatcher> eventDispatcher,
							   std::shared_ptr<ProjectManager> projectManager
	)
		: m_Window(window)
		, m_Profiler(profiler)
		, m_EventDispatcher(eventDispatcher)
		, m_ProjectManager(projectManager)
		, m_Framebuffer(0)
		, m_ViewportCoords(0, 0, 0, 0)
		, m_CurrentWindowDimensions(0, 0)
		, m_PrevWindowDimensions(0, 0)
		, m_PrevImageDimensions(0, 0)
		, m_ForceUpdate(true)
		, m_NeedsDimensionUpdate(false)
	{}

	void RuntimeLayer::onAttach() {
		m_ExportSettings = DeserializeExportSettingsYaml(RuntimeCfg::EXECUTABLE_DIR).value_or(ExportSettings{});
		m_Window->setVSync(m_ExportSettings.vSync);
		m_Window->setFullscreen(m_ExportSettings.fullscreen);

		// Mark that we need to update dimensions after fullscreen change
		// The actual dimension update will happen in CalculateViewportCoordinates
		m_NeedsDimensionUpdate = true;
		m_CurrentWindowDimensions = glm::ivec2{ m_Window->getWidth(), m_Window->getHeight() };

		std::filesystem::path projectFilePath = "";
		for (const auto& entry : std::filesystem::directory_iterator(RuntimeCfg::EXECUTABLE_DIR)) {
			if (entry.is_regular_file()) {
				auto path = entry.path();
				if (path.extension() == PROJECT_FILE_EXTENSION) {
					projectFilePath = path;
				}
			}
		}
		m_ProjectManager->OpenProject(projectFilePath);
		m_Window->setTitle(m_ProjectManager->GetProjectName());

		m_EventDispatcher->dispatchEvent(std::make_shared<UpdateRenderSettingsEvent>(m_ProjectManager->GetMutableRuntimeRenderSettings()));

		glGenFramebuffers(1, &m_Framebuffer);
	}

	void RuntimeLayer::onDetach() {
		if (m_Framebuffer) {
			glDeleteFramebuffers(1, &m_Framebuffer);
		}
	}

	void RuntimeLayer::onUpdate() {
		if (m_CurrentFrame) {
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Framebuffer);
			glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_CurrentFrame->GetID(), 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Bind the default framebuffer (screen)
			CalculateViewportCoordinates();
			// Display the Texture
			glBlitFramebuffer(0, 0, m_CurrentFrame->GetDimensions().x, m_CurrentFrame->GetDimensions().y,
							 m_ViewportCoords.x, m_ViewportCoords.y, m_ViewportCoords.z, m_ViewportCoords.w,
							 GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); // Unbind
		}
	}

	void RuntimeLayer::onEvent(std::shared_ptr<IEvent> event) {
		if (event->GetType() == EventType::NEW_FRAME_RENDERED_EVENT) {
			m_CurrentFrame = std::dynamic_pointer_cast<NewFrameRenderedEvent>(event)->frame;
		}
		if (event->GetType() == EventType::KEY_PRESS_EVENT) {
			if (std::dynamic_pointer_cast<KeyPressEvent>(event)->key == Key::F11) {
				m_Window->setFullscreen(!m_Window->isFullscreen());
				// Mark that dimensions need to be updated after fullscreen toggle
				m_NeedsDimensionUpdate = true;
			}
		}
		if (event->GetType() == EventType::WINDOW_RESIZE_EVENT) {
			glm::vec2 dims = std::dynamic_pointer_cast<WindowResizeEvent>(event)->windowDims;
			m_CurrentWindowDimensions = glm::ivec2{ dims.x, dims.y };
			m_ForceUpdate = true;
		}
	}

	void RuntimeLayer::CalculateViewportCoordinates() {
		if (!m_CurrentFrame) { return; }

		// Update window dimensions if needed (e.g., after fullscreen changes)
		if (m_NeedsDimensionUpdate) {
			m_CurrentWindowDimensions = glm::ivec2{ m_Window->getWidth(), m_Window->getHeight() };
			m_NeedsDimensionUpdate = false;
		}

		glm::ivec2 windowSize = m_CurrentWindowDimensions;
		glm::ivec2 imageSize = m_CurrentFrame->GetDimensions();

		// Check if recalculation needed
		bool dimensionsChanged = (windowSize != m_PrevWindowDimensions || imageSize != m_PrevImageDimensions);
		if (!dimensionsChanged && !m_ForceUpdate) {
			return; // No changes, use existing coordinates
		}
		m_PrevWindowDimensions = windowSize;
		m_PrevImageDimensions = imageSize;
		m_ForceUpdate = false;

		switch (m_ExportSettings.screenFitMode) {
			case ScreenFitMode::OriginalCentered: {
				int offsetX = (windowSize.x - imageSize.x) / 2;
				int offsetY = (windowSize.y - imageSize.y) / 2;

				// Ensure positive (in case image is larger than window)
				offsetX = std::max(0, offsetX);
				offsetY = std::max(0, offsetY);

				m_ViewportCoords = glm::ivec4(
					offsetX,                    // x
					offsetY,                    // y
					offsetX + imageSize.x,      // width
					offsetY + imageSize.y       // height
				);
				break;
			}

			case ScreenFitMode::StretchFill: {
				m_ViewportCoords = glm::ivec4(
					0,              // x
					0,              // y
					windowSize.x,   // width
					windowSize.y    // height
				);
				break;
			}

			case ScreenFitMode::MaxAspectFit: {
				float windowAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
				float imageAspectRatio = static_cast<float>(imageSize.x) / static_cast<float>(imageSize.y);

				int targetWidth, targetHeight;
				if (windowAspectRatio <= imageAspectRatio) {
					// Width is the limiting factor
					targetWidth = windowSize.x;
					targetHeight = static_cast<int>(std::ceil(windowSize.x / imageAspectRatio));
				} else {
					// Height is the limiting factor
					targetWidth = static_cast<int>(std::ceil(windowSize.y * imageAspectRatio));
					targetHeight = windowSize.y;
				}

				// Center the scaled image
				int offsetX = (windowSize.x - targetWidth) / 2;
				int offsetY = (windowSize.y - targetHeight) / 2;

				m_ViewportCoords = glm::ivec4(
					offsetX,                // x
					offsetY,                // y
					offsetX + targetWidth,  // width
					offsetY + targetHeight  // height
				);
				break;
			}

			default:
				// Fallback to MaxAspectFit
				m_ExportSettings.screenFitMode = ScreenFitMode::MaxAspectFit;
				CalculateViewportCoordinates();
				break;
		}
	}
}