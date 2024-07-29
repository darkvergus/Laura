#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "lrpch.h"
#include "Geometry/Triangle.h"


namespace Laura 
{
	class MeshComponent
	{
	public:
		struct Info
		{
			uint32_t triangleCount;
		};

	public:
		MeshComponent(const std::vector<Triangle> mesh);
		inline void updateMesh(const std::vector<Triangle> mesh) { m_Mesh = mesh; }
		inline const std::vector<Triangle>& GetMeshData() { return m_Mesh; }
		Info info;
	private:
		std::vector<Triangle> m_Mesh;
	};
}

#endif // MESH_COMPONENT_H