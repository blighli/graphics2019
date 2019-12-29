#ifndef SPHERE_H
#define SPHERE_H

#ifndef PI
#define PI 3.1415926
#endif
#ifndef PI2
#define PI2 6.2831853
#endif

#include <glad/glad.h>
#include <cmath>

class Sphere {
public:
	GLuint* vboId;
	GLuint vert, texcoord;
	GLfloat* verts;//保存顶点与法向量的指针
	GLfloat* texcoords;//保存纹理坐标的指针
	GLfloat vertNum;

	Sphere(int m, int n) {
		vertNum = m*n * 4*1.5;//顶点总数
		verts = new GLfloat[int(vertNum) * 5];//每个顶点有xyz三个分量,因此*3
		float stepAngZ = PI / m;//纵向角度每次增加的值
		float stepAngXY = PI2 / n;//横向角度每次增加的值
		float angZ = 0.0;//初始的纵向角度
		float angXY = 0.0;//初始的横向角度

		int index = 0;
		for (int i = 0; i<m; i++) {
			for (int j = 0; j<n; j++) {
				//构造一个顶点
				float x1 = sin(angZ)*cos(angXY);
				float y1 = sin(angZ)*sin(angXY);
				float z1 = cos(angZ);
				verts[index] = x1; index++;
				verts[index] = y1; index++;
				verts[index] = z1; index++;
				float v1 = angZ / PI;
				float u1 = angXY / PI2;
				verts[index] = u1; index++;
				verts[index] = v1; index++;

				float x2 = sin(angZ + stepAngZ)*cos(angXY);
				float y2 = sin(angZ + stepAngZ)*sin(angXY);
				float z2 = cos(angZ + stepAngZ);
				verts[index] = x2; index++;
				verts[index] = y2; index++;
				verts[index] = z2; index++;
				float v2 = (angZ + stepAngZ) / PI;
				float u2 = angXY / PI2;
				verts[index] = u2; index++;
				verts[index] = v2; index++;


				float x3 = sin(angZ + stepAngZ)*cos(angXY + stepAngXY);
				float y3 = sin(angZ + stepAngZ)*sin(angXY + stepAngXY);
				float z3 = cos(angZ + stepAngZ);
				verts[index] = x3; index++;
				verts[index] = y3; index++;
				verts[index] = z3; index++;
				float v3 = (angZ + stepAngZ) / PI;
				float u3 = (angXY + stepAngXY) / PI2;
				verts[index] = u3; index++;
				verts[index] = v3; index++;

			    verts[index] = x1; index++;
				verts[index] = y1; index++;
				verts[index] = z1; index++;
				verts[index] = u1; index++;
				verts[index] = v1; index++;

				verts[index] = x3; index++;
				verts[index] = y3; index++;
				verts[index] = z3; index++;
				verts[index] = u3; index++;
				verts[index] = v3; index++;
				
				float x4 = sin(angZ)*cos(angXY + stepAngXY);
				float y4 = sin(angZ)*sin(angXY + stepAngXY);
				float z4 = cos(angZ);
				verts[index] = x4; index++;
				verts[index] = y4; index++;
				verts[index] = z4; index++;
				float v4 = angZ / PI;
				float u4 = (angXY + stepAngXY) / PI2;
				verts[index] = u4; index++;
				verts[index] = v4; index++;

				angXY += stepAngXY;
			}
			angXY = 0.0;//每次横向到达2PI角度则横向角度归0
			angZ += stepAngZ;
		}
	}//m是纵向细分程度,n是横向细分程度
	~Sphere() {}
};

#endif