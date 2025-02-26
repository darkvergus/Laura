// Thanks to: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics
#pragma once

#include "lrpch.h"

#include "Assets/AssetTypes.h"

namespace Laura::Asset 
{
	class BVHAccel
	{
	public:

		// according to std430 - 32 bytes
		struct Node {
			glm::vec4 min; // w component leftChild_Or_FirstTri
			glm::vec4 max; // w component triCount
			/*	if primCount == 0: leftChild_Or_FirstTri == leftChild
				else leftChild_Or_FirstTri == firstTri */

			inline float& triCount() { return max.w; }
			inline float& leftChild_Or_FirstTri() { return min.w; }

			inline const uint32_t triCount_int() const { return static_cast<uint32_t>(max.w); }
			inline const uint32_t leftChild_Or_FirstTri_int() const { return static_cast<uint32_t>(min.w); }
		};

		BVHAccel(const std::vector<Triangle>& meshBuffer, const uint32_t firstTriIdx, const uint32_t triCount);
		~BVHAccel() = default;

		// Builds the Bounding Volume Hierarchy for a given Mesh using the UpdateAABB() & SubDivide() helper methods
		void Build(std::vector<Node>& nodeBuffer, std::vector<uint32_t>& indexBuffer, uint32_t& firstNodeIdx, uint32_t& nodeCount);

	private:
		// Computes the Axis Aligned Bounding Box for a Node passed in using its triangles
		void UpdateAABB(Node& node);
		// Recursively splits the node using a split method, and sorts the triangle index array
		void SubDivide(Node& node);

		inline const std::vector<glm::vec3> PrecomputeCentroids() const {
			std::vector<glm::vec3> centroids;
			centroids.resize(m_TriCount);
			for (int i = 0; i < m_TriCount; i++) {
				const Triangle& t = m_MeshBuff[m_FirstTriIdx + i];
				centroids[i] = (t.v0 + t.v1 + t.v2) * 0.333333333333f;
			}
			return centroids;
		}

		inline void Swap(int idx1, int idx2) {
			uint32_t tmp = m_IdxBuff[idx1];
			m_IdxBuff[idx1] = m_IdxBuff[idx2];
			m_IdxBuff[idx2] = tmp;
		}
		
		// passed into the constructor
		const std::vector<Triangle>& m_MeshBuff;
		const uint32_t m_FirstTriIdx;
		const uint32_t m_TriCount;

		std::vector<glm::vec3> m_Centroids;

		uint32_t m_NodesUsed = 0;
		Node* m_NodeBuff = nullptr;
		uint32_t* m_IdxBuff = nullptr;
	};
}