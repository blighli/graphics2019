#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

const float PI = glm::acos(-1.0f);

class Sphere {
public:
	std::vector<float> vertices; // positions(x,y,z), normals(nx,ny,nz), textureCoords(s,t)
	std::vector<unsigned int> indices;
	Sphere(int sectorCount = 32, int stackCount = 16, float radius = 1.0f) {
		float x, y, z, zx;
		float nx, ny, nz, lengthInv = 1.0f / radius;
		float s, t;

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;
		
		for (int i = 0; i <= stackCount; ++i) {
			stackAngle = PI / 2 - i * stackStep;	// [pi/2, -pi/2]
			zx = radius * glm::cos(stackAngle);		// r * cos(u)
			y = radius * glm::sin(stackAngle);		// r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j) {
				sectorAngle = j * sectorStep;		// [0, 2pi]

				// vertex position (x, y, z)
				z = zx * cosf(sectorAngle);			// r * cos(u) * cos(v)
				x = zx * sinf(sectorAngle);			// r * cos(u) * sin(v)
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				vertices.push_back(nx);
				vertices.push_back(ny);
				vertices.push_back(nz);

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				vertices.push_back(s);
				vertices.push_back(t);
			}
		}

		// generate CCW index list of sphere triangles
		int k1, k2;
		//  k1--k1+1
		//  |  / |
		//  | /  |
		//  k2--k2+1
		for (int i = 0; i < stackCount; ++i) {
			k1 = i * (sectorCount + 1);		// beginning of current stack
			k2 = k1 + sectorCount + 1;		// beginning of next stack
			
			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}
};

#endif // !SPHERE_H
