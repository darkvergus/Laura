#include "BVHAccel.h"

namespace Laura
{

	BVHAccel::BVHAccel(const std::vector<Triangle>& meshBuffer, const uint32_t firstTriIdx, const uint32_t triCount) 
	: m_TriBuff(meshBuffer), m_FirstTriIdx(firstTriIdx), m_TriCount(triCount) {
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
		Aabb aabb;
		// iterate over primitives contained by the Node
		for (size_t i = 0; i < node.triCount; i++) { // every 3rd vertex is new triangle
			const Triangle& t = m_TriBuff[m_FirstTriIdx + m_IdxBuff[node.leftChild_Or_FirstTri + i]];
			aabb.grow(glm::vec3(t.v0));
			aabb.grow(glm::vec3(t.v1));
			aabb.grow(glm::vec3(t.v2));
		}
		node.min = aabb.boxMin;
		node.max = aabb.boxMax;
	}

	float BVHAccel::EvaluateSAH(Node& node, int axis, float candidatePos) {
		Aabb leftBox, rightBox;
		int leftCount = 0, rightCount = 0;
		for(uint32_t i = 0; i < node.triCount; i++ ) {
			uint32_t triIdx = m_IdxBuff[node.leftChild_Or_FirstTri + i]; // relative index [0..m_TriCount)
			const Triangle& tri = m_TriBuff[m_FirstTriIdx + triIdx];
			if (m_Centroids[triIdx][axis] < candidatePos) {
				leftCount++;
				leftBox.grow( tri.v0 );
				leftBox.grow( tri.v1 );
				leftBox.grow( tri.v2 );
			}
			else {
				rightCount++;
				rightBox.grow( tri.v0 );
				rightBox.grow( tri.v1 );
				rightBox.grow( tri.v2 );
			}
		}
		float cost = leftCount * leftBox.area() + rightCount * rightBox.area();
		return (cost > 0) ? cost : FLT_MAX;
	}

	void BVHAccel::SubDivide(Node& node) {
		int bestAxis = -1;
		float bestPos = 0, bestCost = FLT_MAX;
		for (int axis = 0; axis < 3; axis++) for (int i = 0; i < node.triCount; i++) {
			float candidatePos = m_Centroids[m_IdxBuff[node.leftChild_Or_FirstTri + i]][axis];
			float cost = EvaluateSAH(node, axis, candidatePos);
			if (cost < bestCost) {
				bestAxis = axis;
				bestPos	 = candidatePos;
				bestCost = cost;
			}
		}
		Aabb parentAabb{ node.min, node.max };
		float parentCost = node.triCount * parentAabb.area();
		if (bestCost >= parentCost) {
			return;
		}

		uint32_t leftPtr = node.leftChild_Or_FirstTri; // points to the firstTri in node's triangles
		uint32_t rightPtr = node.leftChild_Or_FirstTri + node.triCount - 1; // points to the lastTri

		// partition/sort the triangles (quicksort partition)
		while (leftPtr <= rightPtr) {
			if (m_Centroids[m_IdxBuff[leftPtr]][bestAxis] < bestPos) {
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
		node.triCount = 0; // ! mark the node as non-leaf node
		node.leftChild_Or_FirstTri = leftChildIdx; // now points to the leftChild node

		UpdateAABB(m_NodeBuff[leftChildIdx]); // figure out bounds based on the recently added triangle indices and counts
		UpdateAABB(m_NodeBuff[rightChildIdx]);
		
		SubDivide(m_NodeBuff[leftChildIdx]);
		SubDivide(m_NodeBuff[rightChildIdx]);
	}
}