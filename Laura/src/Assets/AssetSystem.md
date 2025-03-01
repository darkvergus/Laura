# Laura::Asset\::Manager

The `Laura::Asset::Manager` is responsible for loading meshes, textures, BVHs, or any other assets directly (in place) into a `Laura::Asset::ResourcePool` and assigning each asset a unique GUID.

## Resource Pool

The resource pool might look something like this:

    struct ResourcePool { 
        std::vector<Vertex> meshVertices;
        std::vector<BvhNode> bvhNodes;
        std::vector<uint32_t> bvhIndexBuffer;
        std::vector<unsigned char> textures;
    
        bool updateMeshBuffer; // used to reduce GPU buffer calls
        bool updateBvhNodes; 
        bool updateTextures;
        // etc...
    };

## Asset Metadata

Within the `Laura::Asset` namespace, asset metadata structs are defined to hold information about the assets.

### Mesh & Texture Metadata
    struct MeshMetadata { 
        int meshStartIdx;
        int meshSize;
        // etc ...
    };

    struct TextureMetadata {
        // etc ...
    };
Other metadata structs can be defined similarly.

The `Asset::Loader` will maintain a hashmap that maps GUIDs to these metadata structs.

> **Note:** Loading a mesh should automatically trigger a `BVHAccel::Build()` with the same GUID since both Mesh & BVH will be treated as a single entity component.

Certain components in the ECS (e.g., `meshComponent`, `textureComponent`, etc.) will store a constant reference to the metadata structs from the internal `Asset::Loader` hashmap along with their corresponding GUIDs.

# Laura::Renderer

The `Laura::Renderer` provides a `Render(Scene, ResourcePool, RenderSettings)` method which internally performs the following steps:

1. **Parse:** Searches for the main camera and prepares rendering data.
2. **Draw:** Creates the necessary Rendering API objects and dispatches rendering commands.

## Renderer::Parse()

The `Parse()` method will:

- **Search for the main camera:** If none is found, abort rendering.
- **Reference the skybox texture:** A constant reference is maintained.
- **Create lookup structures:**
  - `EntityHandle* entityLookupTable;`
  - `uint32_t entityLookupTableSize;`
  
These structures are used for all renderable entities (i.e., those with the necessary components). Additionally, GUID checks against the `Asset::Loader` are performed to validate the asset metadata within each component.

### EntityHandle Structure

    struct EntityHandle {
        int meshStartIdx; // same as bvhIndexBufferStartIdx
        int meshSize;     // same as bvhIndexBufferSize
        int bvhStartIdx;
        int bvhSize;
        int transformIdx;
        // Perhaps in the future:
        // int materialIdx;
        // int UVtextureStartIdx;
        // int UVtextureSize;
    };

## Renderer::Draw()

The `Draw()` method will:

- Create the necessary Rendering API objects (e.g., SSBOs, UBOs, etc.).
- Pass the `RenderSettings`, camera, skybox, `EntityHandles`, and `ResourcePool` to the GPU.