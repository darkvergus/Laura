#include "BVHAccel.h"

namespace Laura
{

	BVHAccel::BVHAccel(const std::vector<Triangle>& meshBuffer, const uint32_t firstTriIdx, const uint32_t triCount) 
	: m_MeshBuff(meshBuffer), m_FirstTriIdx(firstTriIdx), m_TriCount(triCount) {
		m_Centroids = PrecomputeCentroids();
	}

	void BVHAccel::Build(std::vector<Node>& nodeBuffer, std::vector<uint32_t>& indexBuffer, uint32_t& firstNodeIdx, uint32_t& nodeCount) {
		const size_t N = m_TriCount; // for convenience

		size_t oldSize = nodeBuffer.size();

		firstNodeIdx = oldSize;
		// firstIndexBuffIdx == m_FirstTriIdx;
		
		// make space for new data
		nodeBuffer.resize(nodeBuffer.size() + 2 * N - 1);
		indexBuffer.resize(indexBuffer.size() + N);

		m_NodeBuff = &nodeBuffer[firstNodeIdx];
		m_IdxBuff = &indexBuffer[m_FirstTriIdx];
		for (int i = 0; i < N; i++) {
			m_IdxBuff[i] = i;
		}

		m_NodesUsed = 0;
		Node& root = m_NodeBuff[m_NodesUsed++];
		root.leftChild_Or_FirstTri = 0;
		root.triCount = N;

		UpdateAABB(root);
		SubDivide(root);

		nodeCount = m_NodesUsed;
		nodeBuffer.resize(oldSize + m_NodesUsed); // Erase the unused space ( if we used less than 2N-1 nodes)
	}

	void BVHAccel::UpdateAABB(Node& node) {
		node.min = glm::vec3( FLT_MAX );
		node.max = glm::vec3( -FLT_MAX );

		// iterate over primitives contained by the Node
		for (size_t i = 0; i < node.triCount; i++) { // every 3rd vertex is new triangle
			const Triangle& t = m_MeshBuff[m_FirstTriIdx + m_IdxBuff[node.leftChild_Or_FirstTri + i]];
			// find minimum & maximum coordinates
			node.min = glm::min(node.min, glm::vec3(t.v0));
			node.min = glm::min(node.min, glm::vec3(t.v1));
			node.min = glm::min(node.min, glm::vec3(t.v2));
			node.max = glm::max(node.max, glm::vec3(t.v0));
			node.max = glm::max(node.max, glm::vec3(t.v1));
			node.max = glm::max(node.max, glm::vec3(t.v2));
		}
	}

	void BVHAccel::SubDivide(Node& node) {
		// Found Leaf Node
		if (node.triCount <= 2) {
			return;
		}

		// naive splitting
		glm::vec3 AABB = node.max - node.min;
		int splitAxis = 0;
		if (AABB.y > AABB.x) { splitAxis = 1; }
		if (AABB.z > AABB.y) { splitAxis = 2; }
		double splitPoint = node.min[splitAxis] + AABB[splitAxis] * 0.5;

		uint32_t leftPtr = node.leftChild_Or_FirstTri; // points to the firstTri in node's triangles
		uint32_t rightPtr = node.leftChild_Or_FirstTri + node.triCount - 1; // points to the lastTri

		// partition/sort the triangles (quicksort partition)
		while (leftPtr <= rightPtr) {
			if (m_Centroids[m_IdxBuff[leftPtr]][splitAxis] < splitPoint) {
				leftPtr++;
			}
			else {
				Swap(leftPtr, rightPtr--); // swap and decrement right
			}
		}

		int leftTriCount = leftPtr - node.leftChild_Or_FirstTri; // distance between firstTri and partition point
		
		// couldn't partition
		if (leftTriCount == 0 || leftTriCount == node.triCount) {
			return;
		}

		// find indices for the new child nodes
		int leftChildIdx = m_NodesUsed++;
		int rightChildIdx = m_NodesUsed++;

		// populate children
		m_NodeBuff[leftChildIdx].leftChild_Or_FirstTri = node.leftChild_Or_FirstTri;
		m_NodeBuff[leftChildIdx].triCount = leftTriCount;
		m_NodeBuff[rightChildIdx].leftChild_Or_FirstTri = node.leftChild_Or_FirstTri + leftTriCount;
		m_NodeBuff[rightChildIdx].triCount = node.triCount - leftTriCount;
		
		// update current node
		node.triCount = 0; // ! mark the node as non-leaf node
		node.leftChild_Or_FirstTri = leftChildIdx; // now points to the leftChild node

		UpdateAABB(m_NodeBuff[leftChildIdx]);
		UpdateAABB(m_NodeBuff[rightChildIdx]);
		
		SubDivide(m_NodeBuff[leftChildIdx]);
		SubDivide(m_NodeBuff[rightChildIdx]);
	}
}