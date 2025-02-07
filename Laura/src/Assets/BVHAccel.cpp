#include "BVHAccel.h"

namespace Laura
{
	BVHAccel::BVHAccel(){}

	std::vector<BVHNode>& BVHAccel::Build(const std::vector<Tri>& mesh, std::vector<int>* triIdxs)
	{
		m_Mesh = mesh;
		m_NodesUsed = 0;
		const int N = mesh.size();

		m_Nodes.resize(2 * N - 1);
		m_TriIdxs.resize(N);
		for (int i = 0; i < N; i++) m_TriIdxs[i] = i;

		BVHNode& root = m_Nodes[m_NodesUsed++];
		root.leftfirst = 0;
		root.primCount = N;
		UpdateAABB(root);
		SubDivide(root);
		
		triIdxs = &m_TriIdxs;
		return m_Nodes;
	}

	void BVHAccel::UpdateAABB(BVHNode& node)
	{
		node.min = glm::vec3(FLT_MAX);
		node.max = glm::vec3(-FLT_MAX);
		for (int i = 0; i < node.primCount; i++)
		{
			const int triIdx = m_TriIdxs[node.leftfirst + i];
			const Tri& tri = m_Mesh[triIdx];
			node.min = glm::min(node.min, tri.v0);
			node.min = glm::min(node.min, tri.v1);
			node.min = glm::min(node.min, tri.v2);
			node.max = glm::max(node.max, tri.v0);
			node.max = glm::max(node.max, tri.v1);
			node.max = glm::max(node.max, tri.v2);
		}
	}

	void BVHAccel::SubDivide(BVHNode& node)
	{
		if (node.primCount <= 2) return;
		// SPLIT METHOD
		glm::vec3 AABB = node.max - node.min;
		int axis = 0;
		if (AABB.y > AABB.x) axis = 1;
		if (AABB.z > AABB.y) axis = 2;
		int splitPoint = node.min[axis] + AABB[axis] * 0.5;
		// SPLIT METHOD

		int left = node.leftfirst;
		int right = node.leftfirst + node.primCount - 1;

		while (left <= right)
		{
			if (m_Mesh[left].centroid[axis] < splitPoint) 
				left++;
			else 
				SwapTris(left, right);
		}

		int leftCount = node.leftfirst - left;
		if (leftCount == 0 || leftCount == node.primCount) return;

		int leftChildIdx = m_NodesUsed++;
		int rightChildIdx = m_NodesUsed++;

		m_Nodes[leftChildIdx].leftfirst = node.leftfirst;
		m_Nodes[leftChildIdx].primCount = leftCount;
		m_Nodes[rightChildIdx].leftfirst = node.leftfirst + leftCount;
		m_Nodes[rightChildIdx].primCount = node.primCount - leftCount;
		node.leftfirst = leftChildIdx;
		node.primCount = 0;

		UpdateAABB(m_Nodes[leftChildIdx]);
		UpdateAABB(m_Nodes[rightChildIdx]);
		
		SubDivide(m_Nodes[leftChildIdx]);
		SubDivide(m_Nodes[rightChildIdx]);
	}
}