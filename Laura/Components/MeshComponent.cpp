#include "Components/MeshComponent.h"

namespace Laura
{
	MeshComponent::MeshComponent(const std::vector<Triangle> mesh)
		: m_Mesh(mesh)
	{
	}
}