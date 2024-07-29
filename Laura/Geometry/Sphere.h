#ifndef SPHERE_H
#define SPHERE_H

namespace Laura
{
	struct Sphere {
		Material material;  // offset 0   // alignment 16 // size 32 // total 32 bytes
		glm::vec3 position;           // offset 32  // alignment 16 // size 12 // total 44 bytes
		float radius;                 // offset 44  // alignment 4  // size 4  // total 48 bytes
	};
}

#endif //SPHERE_H