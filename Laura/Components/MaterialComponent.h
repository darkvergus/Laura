#ifndef MATERIAL_COMPONENT_H
#define MATERIAL_COMPONENT_H

#include "lrpch.h"
#include "renderer/Material.h"

namespace Laura
{
	class MaterialComponent
	{
		MaterialComponent(const Material& material);

		inline void updateMaterial(const Material& material) { m_Material = material; }
		inline const Material& getMaterial() { return m_Material; }

	private:
		Material m_Material;
	};
}

#endif // MATERIAL_COMPONENT_H