#pragma once

#include "Core/application.h"

#include "Core/ILayer.h"
#include "Core/LayerStack.h"

#include "Core/Events/Events.h"
#include "Core/Events/KeyCodes.h"
#include "Core/Events/MouseCodes.h"

#include "Renderer/Renderer.h"
#include "Renderer/IRendererAPI.h"

#include "Renderer/IImage2D.h"
#include "Renderer/IComputeShader.h"
#include "Renderer/IShaderStorageBuffer.h"
#include "Renderer/IUniformBuffer.h"
#include "Renderer/ITexture2D.h"
#include "Renderer/Material.h"

//// ECS within scene ////
#include "Scene/Scene.h" 
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Scene/Skybox.h" 
//// ---------------- ////

#include "Assets/MeshLoader.h"
#include "Assets/TextureLoader.h"
