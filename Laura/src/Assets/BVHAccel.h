// inspired by https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics
#pragma once

#include "lrpch.h"

namespace Laura 
{
	struct Tri{
		glm::vec3 v0, v1, v2, centroid;
	};

	struct BVHNode{
		glm::vec3 min, max;
		int leftfirst, primCount; 
		// if primCount = 0, leftfirst points to the left child node
		// else first primitive
	};

	class BVHAccel
	{
	public:
		BVHAccel();
		// takes in a mesh and an inout parameter returning the ordered triangles
		std::vector<BVHNode>& Build(const std::vector<Tri>& mesh, std::vector<int>* triIdxs);
	private:
		void UpdateAABB(BVHNode& node);
		void SubDivide(BVHNode& node);
		inline void SwapTris(int idx1, int idx2) { m_TriIdxs[idx1] = idx2; m_TriIdxs[idx2] = idx1; }
		
		std::vector<BVHNode> m_Nodes;
		std::vector<int> m_TriIdxs;
		std::vector<Tri> m_Mesh;
		int m_NodesUsed;
	};
}