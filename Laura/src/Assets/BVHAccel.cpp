#include "BVHAccel.h"

namespace Laura::Asset
{
	BVHAccel::BVHAccel(const std::vector<Triangle>& meshBuffer, const uint32_t firstTriIdx, const uint32_t triCount) 
	: m_MeshBuff(meshBuffer), m_FirstTriIdx(firstTriIdx), m_TriCount(triCount) {
		m_Centroids = PrecomputeCentroids();
	}


	void BVHAccel::Build(std::vector<Node>& nodeBuffer, std::vector<uint32_t>& indexBuffer, uint32_t& firstNodeIdx, uint32_t& nodeCount)
	{
		const size_t N = m_TriCount; // for convenience

		size_t oldSize = nodeBuffer.size();

		firstNodeIdx = oldSize;
		// firstIndexBuffIdx == m_FirstTriIdx;
		
		// make space for new data
		nodeBuffer.resize(nodeBuffer.size() + 2 * N - 1);
		indexBuffer.resize(indexBuffer.size() + N);

		m_NodeBuff = &nodeBuffer[firstNodeIdx];
		m_IdxBuff = &indexBuffer[m_FirstTriIdx];
		for (int i = 0; i < N; i++) 
			m_IdxBuff[i] = i;

		m_NodesUsed = 0;
		Node& root = m_NodeBuff[m_NodesUsed++];
		root.leftChild_Or_FirstTri() = 0;
		root.triCount() = N;

		UpdateAABB(root);
		SubDivide(root);

		nodeCount = m_NodesUsed;
		nodeBuffer.resize(oldSize + m_NodesUsed); // Erase the unused space ( if we used less than 2N-1 nodes)
	}

	void BVHAccel::UpdateAABB(Node& node)
	{
		glm::vec3 minTmp = glm::vec3( FLT_MAX );
		glm::vec3 maxTmp = glm::vec3( -FLT_MAX );

		// iterate over primitives contained by the Node
		for (size_t i = 0; i < node.triCount_int(); i++) // every 3rd vertex is new triangle
		{
			const Triangle& t = m_MeshBuff[m_FirstTriIdx + m_IdxBuff[node.leftChild_Or_FirstTri_int() + i]];
			// find minimum & maximum coordinates
			minTmp = glm::min(minTmp, glm::vec3(t.v0));
			minTmp = glm::min(minTmp, glm::vec3(t.v1));
			minTmp = glm::min(minTmp, glm::vec3(t.v2));
			maxTmp = glm::max(maxTmp, glm::vec3(t.v0));
			maxTmp = glm::max(maxTmp, glm::vec3(t.v1));
			maxTmp = glm::max(maxTmp, glm::vec3(t.v2));
		}

		node.min = glm::vec4(minTmp, node.min.w); // copying leftChild_Or_FirstTri as w
		node.max = glm::vec4(maxTmp, node.max.w); // copying triCount as w
	}

	void BVHAccel::SubDivide(Node& node)
	{
		// Found Leaf Node
		if (node.triCount_int() <= 2)
			return;
		
		// SPLIT METHOD
		glm::vec3 AABB = node.max - node.min;
		int splitAxis = 0;
		if (AABB.y > AABB.x) splitAxis = 1;
		if (AABB.z > AABB.y) splitAxis = 2;
		double splitPoint = node.min[splitAxis] + AABB[splitAxis] * 0.5;
		// SPLIT METHOD

		uint32_t leftPtr = node.leftChild_Or_FirstTri_int(); // points to the firstTri in node's triangles
		uint32_t rightPtr = node.leftChild_Or_FirstTri_int() + node.triCount_int() - 1; // points to the lastTri

		// partition/sort the triangles (quicksort partition)
		while (leftPtr <= rightPtr)
		{
			if (m_Centroids[m_IdxBuff[leftPtr]][splitAxis] < splitPoint) 
				leftPtr++;
			else 
				Swap(leftPtr, rightPtr--); // swap and decrement right
		}

		int leftTriCount = leftPtr - node.leftChild_Or_FirstTri_int(); // distance between firstTri and partition point
		
		// couldn't partition
		if (leftTriCount == 0 || leftTriCount == node.triCount_int()) 
			return;

		// find indices for the new child nodes
		int leftChildIdx = m_NodesUsed++;
		int rightChildIdx = m_NodesUsed++;

		// populate children
		m_NodeBuff[leftChildIdx].leftChild_Or_FirstTri() = node.leftChild_Or_FirstTri_int();
		m_NodeBuff[leftChildIdx].triCount() = leftTriCount;
		m_NodeBuff[rightChildIdx].leftChild_Or_FirstTri() = node.leftChild_Or_FirstTri_int() + leftTriCount;
		m_NodeBuff[rightChildIdx].triCount() = node.triCount_int() - leftTriCount;
		
		// update current node
		node.triCount() = 0; // Important! mark the node as non-leaf node
		node.leftChild_Or_FirstTri() = leftChildIdx; // now points to the leftChild node

		UpdateAABB(m_NodeBuff[leftChildIdx]);
		UpdateAABB(m_NodeBuff[rightChildIdx]);
		
		SubDivide(m_NodeBuff[leftChildIdx]);
		SubDivide(m_NodeBuff[rightChildIdx]);
	}
}