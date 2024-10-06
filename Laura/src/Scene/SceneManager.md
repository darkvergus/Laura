# Scene Manager Design Plan

<i>Future plans for the Scene Manager in the Laura Renderer.</i>

### Overview:
The **Scene Manager** will take in a scene and output the correct data structure for the renderer to process and render. Its responsibilities include iterating over the scene, finding the main camera, determining visible objects, and optimizing the rendering pipeline by organizing and submitting necessary data to the renderer.

### Responsibilities:
1. **Camera and Object Visibility**:
   - The scene manager will iterate over the scene, find the main camera, and determine which objects are visible within the camera’s frustum.
   - It will ensure that only visible objects are passed to the renderer, improving performance.

2. **Handling Mesh Data**:
   - If there are multiple objects sharing the same mesh, the scene manager will only pass the mesh triangles **once** to the renderer to avoid redundancy.
   - The scene manager will ask the **Asset Manager** for a pointer to each mesh's unique BVH (Bounding Volume Hierarchy) to speed up rendering.

3. **BVH for Each Mesh**:
   - The **Asset Manager** will be responsible for generating a BVH for each mesh during asset import. This BVH will be written to a file, allowing quicker loading for future sessions.

4. **Scene-Level BVH (Level 2 BVH)**:
   - The scene manager will construct a **Level 2 BVH** (scene-level BVH) that organizes all objects in the scene based on their bounding volumes.
   - This structure will help accelerate rendering by allowing efficient frustum culling and object traversal.

5. **Submission to Renderer**:
   - The scene manager will submit the following to the renderer:
     - **Non-repeating triangles**: A list of triangles for all visible objects, ensuring that shared meshes are only submitted once.
     - **Mesh BVHs**: A BVH for each unique mesh.
     - **Level 2 BVH**: The scene BVH that organizes the objects spatially.
     - **Materials**: A list of unique materials used by the visible objects. Currently, each object has one material, but this will be expanded to support multiple materials and textures per object.
     - **Main Camera**: The camera’s data for view and projection transformations.
     - **Skybox Texture**: The texture used for rendering the skybox.

### Summary:
The **Scene Manager** serves as the intermediate system that optimizes and organizes the scene's data before submitting it to the renderer. By leveraging **BVH structures**, frustum culling, and material deduplication, it ensures efficient rendering and data management. The **Asset Manager** will handle per-mesh BVH generation and caching for fast asset loading. Future plans include expanding material and texture support for more complex rendering scenarios.
