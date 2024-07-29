#ifndef MATERIAL_H
#define MATERIAL_H

namespace Laura {

    struct Material
    {
        glm::vec3 color;              // offset 0   // alignment 16 // size 12 // total 12 bytes
        float emissionStrength;       // offset 12  // alignment 4  // size 4  // total 16 bytes
        glm::vec3 emissionColor;      // offset 16  // alignment 16 // size 12 // total 28 bytes
        float std140padding;          // offset 28  // alignment 4  // size 4  // total 32 bytes
    };
}

#endif // MATERIAL_H