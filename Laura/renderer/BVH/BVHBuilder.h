#ifndef BVH_BUILDER_H
#define BVH_BUILDER_H

#include "lrpch.h"

#include "renderer/Material.h"
#include "Geometry/Triangle.h"
#include "Components/MeshComponent.h"

namespace Laura 
{

    namespace BVH 
    {
        enum class Heuristic 
        {
            OBJECT_MEDIAN_SPLIT,
            SPATIAL_MIDDLE_SPLIT,
            SURFACE_AREA_HEURISTIC,
            SURFACE_AREA_HEURISTIC_BUCKETS
        };

        // The maximum number of primitives (triangles) a leaf node can contain
        const unsigned int AABB_primitives_limit = 2;

        // true stupidity
        struct Int128 
        {
            int data;
            char padding[12];  // Ensure 16-byte size
        };

        /**
            * @class Node
            * @brief A class representing a node in a Bounding Volume Hierarchy (BVH).
            *
            * Each node in the BVH represents a bounding box in 3D space. It contains the
            * indices of the primitives (triangles) it contains if it is a leaf node, and
            * the indices of its child nodes if it is an internal node.
            */
        struct Node 
        {
        public:
            Node() {};
            Node(glm::vec3 minVec, glm::vec3 maxVec);

            // overload the << operator to print the node
            friend std::ostream& operator<<(std::ostream& os, const Node& node);

            /**
                * @brief The indices of the primitives (triangles) contained in the leaf node.
                * Initially populated with -1 to indicate that the node is not a leaf node.
                * Size is determined by AABB_primitives_limit.
                */
            Int128 leaf_primitive_indices[AABB_primitives_limit];

            // AABB (Axis-Aligned Bounding Box) of the node
            glm::vec3 minVec;
            int child1_idx;
            glm::vec3 maxVec;
            int child2_idx;
        };

        /**
            * @struct BVH_data
            * @brief A structure containing the data of a Bounding Volume Hierarchy (BVH).
            *
            * This structure contains a vector of all nodes in the BVH, the size of the BVH,
            * a pointer to the array of triangles represented by the BVH, and the size of this array.
            */
        struct BVH_data 
        {
            std::vector<BVH::Node> BVH;
            std::vector<Triangle> TRIANGLES;

            unsigned int BVH_tree_depth;
            std::vector<glm::vec3> heatmapLayers;

            unsigned int BVH_size;
            unsigned int TRIANGLES_size;
        };

        // Helper functions for calculating the minimum and maximum vectors of an AABB
        glm::vec3 minCorner(const glm::vec3& current_min, const glm::vec3& vertex);
        glm::vec3 maxCorner(const glm::vec3& current_max, const glm::vec3& vertex);

        /**
            * @brief Computes the Axis-Aligned Bounding Box (AABB) for a set of triangles.
            *
            * This function iterates over a set of triangles, represented by their indices, and computes
            * the minimum and maximum corners of the AABB that encloses them. The minimum and maximum
            * corners are updated for each vertex of each triangle.
            *
            * @param triangle_indices The indices of the triangles for which to compute the AABB.
            * @param triangle_mesh The mesh containing the triangles.
            * @param minVec A reference to a vector that will be set to the minimum corner of the AABB. (return value)
            * @param maxVec A reference to a vector that will be set to the maximum corner of the AABB. (return value)
            */
        void computeAABB(const std::vector<unsigned int>& triangle_indices, const std::vector<Triangle>& triangle_mesh, glm::vec3& minVec, glm::vec3& maxVec);

        /**
            * @brief Initializes a BVH node with a set of triangles.
            *
            * This function creates the root BVH node and computes its bounding box based on the given set of triangles (mesh).
            * The triangles are specified by their indices in the triangle mesh.
            *
            * @param triangle_indices A vector of indices specifying which triangles are contained in the new node.
            * @param triangle_mesh The mesh containing all triangles.
            * @return A new BVH node containing the specified triangles and with a bounding box that encloses them.
            */
        BVH::Node init(const std::vector<unsigned int>& triangle_indices, const std::vector<Triangle> triangle_mesh);


        /**
            * @struct Partition_output
            * @brief A structure containing the output of a BVH node partition operation.
            *
            * This structure contains the indices of the triangles in the left and right
            * child nodes after partitioning, the minimum and maximum corners of the AABBs
            * of the left and right child nodes, and flags indicating whether the left and
            * right child nodes are leaf nodes.
            */
        struct Partition_output {
            std::vector<unsigned int> LTris;    ///< Indices of the triangles in the left child node
            glm::vec3 LAABBmin;                 ///< Minimum corner of the AABB of the left child node
            glm::vec3 LAABBmax;                 ///< Maximum corner of the AABB of the left child node
            bool LIsLeaf = false;               ///< Flag indicating whether the left child node is a leaf node

            std::vector<unsigned int> RTris;    ///< Indices of the triangles in the right child node
            glm::vec3 RAABBmin;                 ///< Minimum corner of the AABB of the right child node
            glm::vec3 RAABBmax;                 ///< Maximum corner of the AABB of the right child node
            bool RIsLeaf = false;               ///< Flag indicating whether the right child node is a leaf node
        };

        /**
            * @brief Partitions a BVH node into two child nodes.
            *
            * This function partitions the primitives (triangles) in a BVH node into two sets, each contained in a child node. The partitioning is done based on a heuristic.
            *
            * @param parent_node The BVH node to partition.
            * @param triangle_indices The indices of the triangles contained in the parent node.
            * @param triangles The mesh containing all triangles.
            * @param heuristic The heuristic to use for partitioning.
            * @return A Partition_output structure containing the output of the partition operation.
            */
        BVH::Partition_output PartitionNode(const BVH::Node parent_node, std::vector<unsigned int>& triangle_indices, const std::vector<Triangle>& triangles, const Heuristic& heuristic);

        BVH::Partition_output surface_area_heuristic(const BVH::Node parent_node, std::vector<unsigned int>& triangle_indices, const std::vector<Triangle>& triangles, const bool& split_buckets);

        /**
            * @brief Constructs a Bounding Volume Hierarchy (BVH) from a 3D mesh.
            *
            * This function reads a 3D mesh from a file, constructs a BVH from the mesh, and returns the BVH data.
            *
            * @param path The path to the file containing the 3D mesh.
            * @param heuristic The heuristic to use for partitioning the BVH nodes.
            * @return A BVH_data structure containing the data of the constructed BVH.
            */
        BVH::BVH_data construct(MeshComponent& mesh, const Heuristic heuristic);

        unsigned int getBVHTreeDepth(const std::vector<Node>& BVH, BVH::Node current_node, unsigned int height);
    }
}

#endif // BVH_BUILDER_H