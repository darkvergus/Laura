#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(std::shared_ptr<Laura::Renderer> renderer)
	: m_Renderer(renderer)
{
	setLayerName("SandboxLayer");
}

void SandboxLayer::onAttach()
{
	m_Renderer->SetAPI(Laura::IRendererAPI::API::OpenGL);

	m_Camera.transform.setPosition({ 0.0f, 40.0f, -200.0f });
	m_Camera.transform.setRotation({ 0.0f, 0.0f, 0.0f });
	m_Camera.setFOV(90.0f);

	m_Environment.skybox.changeType(Laura::SkyboxType::SKYBOX_TEXTURE);
	std::string texturePath = LR_RESOURCES_PATH "Skyboxes/Metro_default.hdr";
	m_Environment.skybox.setTexturePath(texturePath);

	m_Renderer->renderSettings.viewportDimensions = glm::vec2(300, 100);
	m_Renderer->renderSettings.raysPerPixel = 1;
	m_Renderer->renderSettings.bouncesPerRay = 5;
	m_Renderer->renderSettings.maxAABBIntersections = 10000;
	m_Renderer->renderSettings.displayBVH = false;

	m_Renderer->BeginScene(m_Camera, m_Environment);

	Laura::MeshComponent model3D = Laura::MeshLoader::loadMesh(std::string(LR_RESOURCES_PATH "Models/stanford_dragon_pbr.glb"));
	//MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/suzanne_high_poly_rotated.glb"));
	//MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/tiny_house.glb"));
	//MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/sponza.obj"));
	//MeshComponent model3D = MeshLoader::loadMesh(std::string(APP_RESOURCES_PATH "models/stanford_bunny.obj"));
	m_Renderer->SubmitMesh(model3D);
}

void SandboxLayer::onDetach()
{
}

void SandboxLayer::onUpdate()
{
}

void SandboxLayer::onImGuiRender()
{
}

void SandboxLayer::onEvent(Laura::Event* event)
{
}
