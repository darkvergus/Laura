#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <imgui.h>

#include "deltaLib/deltaLib.h"
#include "core/camera/CameraHandler.hpp" // also includes Camera.hpp
#include "scenes/Scene1.hpp"
#include "core/Renderer.h"


#include "core/IWindow.h"
#include "core/Events.h"
#include "core/LayerStack.h"
#include "core/ImGuiLayer.h"
#include "platform/windows/GLFWWindow.h"


class Application
{
private:
    // new stuff
    IWindow* m_Window;
    LayerStack* m_LayerStack;
    ImGuiLayer* m_ImGuiLayer;

    // stuff to refactor
    DeltaTime deltaTime;
    Camera camera;
    CameraHandler cameraHandler;
    SceneData sceneData;
    BVH::Heuristic active_heuristic;
    BVH::BVH_data scene_BVH;
    std::string skyboxFilePath;
    Renderer* renderer;    
    bool was_ImGui_Input;
    bool shouldPostProcess;
    bool shouldAccumulate;
    int heatmap_color_limit;
    int raysPerPixel;
    int bouncesPerRay;
    bool show_skybox;
    bool reloadSkybox;
    glm::vec3 SkyGroundColor;
    glm::vec3 SkyColorHorizon;
    glm::vec3 SkyColorZenith;
    bool show_demo_window;
    int totalFrames;
    double prevCamAspect;
    ImVec2 prevViewportWindowSize;
    ImVec2 prevViewportWindowPos;
    ImVec2 viewportSize;
    ImVec2 topLeftTextureCoords;
    ImVec2 bottomLeftTextureCoords;
    bool display_BVH;
    bool showPixelData;
    int displayed_layer;
    bool display_multiple;
    int BVH_height;
    unsigned int viewport_mouseX;
    unsigned int viewport_mouseY;
    unsigned int prev_viewport_mouseX;
    unsigned int prev_viewport_mouseY;
    unsigned int inverted_viewport_mouseY;
    unsigned int AABB_intersect_count_sum;
    unsigned int TRI_intersect_count_sum;
    unsigned int same_mouse_pos_count;
    bool wasGlobalInput; // input from the camera, ImGui or any other source (which should reset accumulation)
    unsigned int m_NumAccumulatedFrames;

public:
	Application();
	virtual ~Application();
	void run();

protected:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void shutdown();
};

Application* createApplication();

#endif // APPLICATION_H