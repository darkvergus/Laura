#include "core/application.h"

#include <iostream>
#include "core/gl_util/OpenGLdebugFuncs.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GUI/CameraComponentGUI.h"
#include "GUI/PerformanceCounterGUI.h"
#include "GUI/SkyBoxGUI.h"
#include "GUI/BVHsettingsGUI.h"

// new includes
#include "core/Log.h"


Application::Application() :
	camera						(90, 16.0 / 9.0, deltaTime),
	cameraHandler				(camera),
	sceneData					(emptyScene()),
	active_heuristic			(BVH::Heuristic::SURFACE_AREA_HEURISTIC_BUCKETS),
	//scene_BVH					(BVH::construct(APP_RESOURCES_PATH "models/suzanne_high_poly_rotated.glb", active_heuristic)),
	scene_BVH					(BVH::construct(APP_RESOURCES_PATH "models/stanford_dragon_pbr.glb", active_heuristic)),
	//scene_BVH					(BVH::construct(APP_RESOURCES_PATH "models/tiny_house.glb", active_heuristic)),
	//scene_BVH					(BVH::construct(APP_RESOURCES_PATH "models/sponza_scene.glb", active_heuristic)),
	//scene_BVH					(BVH::construct(APP_RESOURCES_PATH "models/stanford_bunny.obj", active_heuristic)),
	//scene_BVH					(BVH::construct(APP_RESOURCES_PATH "models/stanford_dragon_pbr.glb", active_heuristic)),
	shouldPostProcess			(false),
	shouldAccumulate			(true),
	heatmap_color_limit			(350),
	raysPerPixel				(1),
	bouncesPerRay				(5),
	show_skybox					(true),
	reloadSkybox				(false),
	SkyGroundColor				(glm::vec3(0.63f, 0.58f, 0.63f)),
	SkyColorHorizon				(glm::vec3(1.00f, 1.00f, 1.00f)),
	SkyColorZenith				(glm::vec3(0.48f, 0.71f, 234.0f / 255.0f)),
	show_demo_window			(true),
	totalFrames					(0),
	prevCamAspect				(0),
	prevViewportWindowSize		(ImVec2(0.0f, 0.0f)),
	prevViewportWindowPos		(ImVec2(0.0f, 0.0f)),
	display_BVH					(false),
	showPixelData				(true),
	displayed_layer				(1),
	display_multiple			(true),
	BVH_height					(30),
	prev_viewport_mouseX		(0),
	prev_viewport_mouseY		(0),
	inverted_viewport_mouseY	(0),
	AABB_intersect_count_sum	(0),
	TRI_intersect_count_sum		(0),
	same_mouse_pos_count		(0),
	wasGlobalInput				(false), // input from the camera, ImGui or any other source (which should reset accumulation)
	m_NumAccumulatedFrames		(0)
{}

Application::~Application(){}

void Application::run()
{
	init();
	while (!m_Window->shouldClose())
	{
		
		update();
		m_ImGuiLayer->Begin();
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		
		

		/// need a refactoring
		deltaTime.update();

		totalFrames += 1;
		was_ImGui_Input = false;
		
		
		// Widget windows
		
		component_cameraGUI		(camera, was_ImGui_Input, cameraHandler.CameraControllMode, shouldAccumulate, shouldPostProcess, raysPerPixel, bouncesPerRay);
		genSkyboxGUI			(SkyGroundColor, SkyColorHorizon, SkyColorZenith, show_skybox, was_ImGui_Input, skyboxFilePath, reloadSkybox, cameraHandler.CameraControllMode);
		BVH_settings_GUI		(display_BVH, active_heuristic, scene_BVH.BVH_tree_depth, heatmap_color_limit, showPixelData, was_ImGui_Input, cameraHandler.CameraControllMode);
		genPerformanceCounter	();
		ImGui::ShowDemoWindow(&show_demo_window);
		if (camera.cameraKeybinds.camera_keybind_window_active) { genCameraChangeKeybindSplashScreen(); }
		
		
		/// VIEWPORT WINDOW
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
		
		ImVec2 viewportWindowSize = ImGui::GetContentRegionAvail();
		
		// check if the viewport size or aspect ratio changed
		if (viewportWindowSize.x != prevViewportWindowSize.x || viewportWindowSize.y != prevViewportWindowSize.y || camera.getAspect() != prevCamAspect)
		{
			prevViewportWindowSize = viewportWindowSize;
			prevCamAspect = camera.getAspect();
		
			double parentAspect = double(viewportWindowSize.x) / double(viewportWindowSize.y);
			// check if the parent aspect is smaller or equal to camera aspect (width is the limiting factor so the window will span the entire width)
			if (parentAspect <= camera.getAspect())
			{
				viewportSize.x = viewportWindowSize.x; // width stays the same
				viewportSize.y = ceil(viewportWindowSize.x / camera.getAspect()); // calculate the height based on the aspect ratio
			}
			// (height is the limiting factor so the window will span the entire height)
			else if (parentAspect > camera.getAspect())
			{
				viewportSize.y = viewportWindowSize.y; // height stays the same
				viewportSize.x = ceil(viewportWindowSize.y * camera.getAspect()); // calculate the width based on the aspect ratio
			}
		
			renderer->setViewportSize(glm::vec2(viewportSize.x, viewportSize.y)); // sets the texture size to the viewport size
		
			camera.setScreenDimentions(int(viewportSize.x), int(viewportSize.y));
			was_ImGui_Input = true;
		
			topLeftTextureCoords.x = (viewportWindowSize.x - viewportSize.x) / 2.0f;
			topLeftTextureCoords.y = (viewportWindowSize.y - viewportSize.y) / 2.0f;
			// viewport offset
			topLeftTextureCoords.x += ImGui::GetWindowPos().x;
			topLeftTextureCoords.y += ImGui::GetWindowPos().y;
		
			bottomLeftTextureCoords.x = topLeftTextureCoords.x + viewportSize.x;
			bottomLeftTextureCoords.y = topLeftTextureCoords.y + viewportSize.y;
		}
		
		
		// check if the viewport window position changed
		ImVec2 viewportWindowPos = ImGui::GetWindowPos();
		if (viewportWindowPos.x != prevViewportWindowPos.x || viewportWindowPos.y != prevViewportWindowPos.y)
		{
			prevViewportWindowPos = viewportWindowPos;
		
			topLeftTextureCoords.x = (viewportWindowSize.x - viewportSize.x) / 2.0f;
			topLeftTextureCoords.y = (viewportWindowSize.y - viewportSize.y) / 2.0f;
			// viewport offset
			topLeftTextureCoords.x += viewportWindowPos.x;
			topLeftTextureCoords.y += viewportWindowPos.y;
		
			bottomLeftTextureCoords.x = topLeftTextureCoords.x + viewportSize.x;
			bottomLeftTextureCoords.y = topLeftTextureCoords.y + viewportSize.y;
		}
		
		camera.Update();
		
		wasGlobalInput = (camera.wasCameraInput || was_ImGui_Input || !shouldAccumulate);
		if (wasGlobalInput) { m_NumAccumulatedFrames = 0; }
		else { m_NumAccumulatedFrames++; }
		
		viewport_mouseX = ImGui::GetMousePos().x - topLeftTextureCoords.x;
		viewport_mouseY = ImGui::GetMousePos().y - topLeftTextureCoords.y;
		inverted_viewport_mouseY = viewportSize.y - viewport_mouseY;
		
		if (reloadSkybox) {
			renderer->setSkyboxFilePath(skyboxFilePath);
			reloadSkybox = false;
		}
		
		
		renderer->BeginComputeRtxStage();
		renderer->rtx_uniform_parameters.numAccumulatedFrames = m_NumAccumulatedFrames;
		renderer->rtx_uniform_parameters.raysPerPixel = raysPerPixel;
		renderer->rtx_uniform_parameters.bouncesPerRay = bouncesPerRay;
		renderer->rtx_uniform_parameters.FocalLength = camera.focalLength;
		renderer->rtx_uniform_parameters.skyboxGroundColor = SkyGroundColor;
		renderer->rtx_uniform_parameters.skyboxHorizonColor = SkyColorHorizon;
		renderer->rtx_uniform_parameters.skyboxZenithColor = SkyColorZenith;
		renderer->rtx_uniform_parameters.CameraPos = camera.posVec;
		renderer->rtx_uniform_parameters.ModelMatrix = glm::mat4(camera.getModelMatrix());
		renderer->rtx_uniform_parameters.WasInput = wasGlobalInput;
			
		renderer->rtx_uniform_parameters.display_BVH = display_BVH;
		renderer->rtx_uniform_parameters.displayed_layer = displayed_layer;
		renderer->rtx_uniform_parameters.display_multiple = display_multiple;
		renderer->rtx_uniform_parameters.BVH_tree_depth = scene_BVH.BVH_tree_depth;
		renderer->rtx_uniform_parameters.show_skybox = show_skybox;
		renderer->rtx_uniform_parameters.heatmap_color_limit = heatmap_color_limit;
		renderer->rtx_uniform_parameters.pixelGlobalInvocationID = glm::vec3(viewport_mouseX, inverted_viewport_mouseY, 1.0f); // invocations start from bottom left
		
		
		/// ACTUALLY RENDERING THE FRAME IN THE SHADER
		ComputeTexture* outputTexture = renderer->RenderComputeRtxStage();
		
		/// Post Processing stage currently does NOTHING
		renderer->BeginComputePostProcStage();
		renderer->postProcessing_uniform_parameters.numAccumulatedFrames = m_NumAccumulatedFrames;
		ComputeTexture* postProcOutput = renderer->RenderComputePostProcStage();
		
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddImage((ImTextureID)postProcOutput->ID(), topLeftTextureCoords, bottomLeftTextureCoords, { 0, 1 }, { 1, 0 });
		
		// DEBUG pixeldata overlay
		if (ImGui::IsWindowHovered() && showPixelData && !cameraHandler.CameraControllMode) {
			if (viewport_mouseX >= 0 && viewport_mouseX < viewportSize.x && inverted_viewport_mouseY >= 0 && inverted_viewport_mouseY < viewportSize.y) {
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
				ImGui::BeginTooltip();
				ImGui::BeginGroup();
				ImGui::Spacing();
				ImGui::Text("Pixel Data");
				ImGui::Separator();
				ImGui::Text("X: %d, Y: %d", viewport_mouseX, inverted_viewport_mouseY);
				if (wasGlobalInput || viewport_mouseX != prev_viewport_mouseX || viewport_mouseY != prev_viewport_mouseY) {
					AABB_intersect_count_sum = 0;
					TRI_intersect_count_sum = 0;
					same_mouse_pos_count = 0;
				}
				prev_viewport_mouseX = viewport_mouseX;
				prev_viewport_mouseY = viewport_mouseY;
		
				AABB_intersect_count_sum += renderer->pixelData.AABB_intersect_count;
				TRI_intersect_count_sum += renderer->pixelData.pixelColor.w;
				same_mouse_pos_count++;
		
		
				glm::vec4 pclr = renderer->pixelData.pixelColor;
		
				ImVec4 pixelColor = ImVec4(pclr.r, pclr.g, pclr.b, 1.0f);
				ImGui::ColorButton("MyColor##3c", *(ImVec4*)&pclr, NULL, ImVec2(20, 20));
				ImGui::SameLine();
				ImGui::Text("RGB: (%f, %f, %f)", pclr.r, pclr.g, pclr.b);
				ImGui::Text("ray-AABB intersects: %u", AABB_intersect_count_sum / same_mouse_pos_count);
				ImGui::Text("ray-triangle intersects: %u", TRI_intersect_count_sum / same_mouse_pos_count);
				ImGui::EndGroup();
				ImGui::EndTooltip();
				ImGui::PopStyleVar();
			}
		}
		
		ImGui::PopStyleVar();
		ImGui::End();
		
		camera.ResetFlags();

		m_ImGuiLayer->End();


		render();
	}
	shutdown();
}


void Application::init()
{
	Log::Init();
	m_Window = IWindow::createWindow();
	m_LayerStack = new LayerStack();
	m_ImGuiLayer = new ImGuiLayer(m_Window);
	// make window forward events to the layer stack
	m_Window->setEventCallback([this](Event* event) { m_LayerStack->dispatchEvent(event); });
	m_LayerStack->PushLayer(m_ImGuiLayer);
	renderer = new Renderer(sceneData, scene_BVH, skyboxFilePath);
}

void Application::update()
{
	m_Window->onUpdate();
	m_LayerStack->onUpdate();
	//Logging test:
	//CORE_TRACE(m_Window->isKeyPressed(Key::W));
}

void Application::render()
{
}

void Application::shutdown()
{
	glfwTerminate();
	delete renderer;
	delete m_LayerStack;
	delete m_Window;
}

Application* createApplication()
{
	return new Application();
}
