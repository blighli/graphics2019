#pragma once
#include <iostream>
#include "Util.h"
using namespace std;

struct vec2 {
public:
	float x;
	float y;

	vec2();
	vec2(float posX, float posY);

	vec2 add(const vec2 &v);
	vec2 subtract(const vec2 &v);
	vec2 multiply(const vec2 &v);
	vec2 divide(const vec2 &v);

	friend vec2 operator+(vec2 left, const vec2 &right);
	friend vec2 operator-(vec2 left, const vec2 &right);
	friend vec2 operator*(vec2 left, const vec2 &right);
	friend vec2 operator/(vec2 left, const vec2 &right);

	void operator+=(const vec2 &right);
	void operator-=(const vec2 &right);
	void operator*=(const vec2 &right);
	void operator/=(const vec2 &right);
	bool operator==(const vec2 &right);
	bool operator!=(const vec2 &right);

	friend ostream &operator<<(ostream &out, vec2 &v);
	friend istream &operator>>(istream &in, vec2 &v);
};

struct vec3 {
public:
	float x;
	float y;
	float z;

	vec3();
	vec3(float posX, float posY, float posZ);

	vec3 add(const vec3 &v);
	vec3 subtract(const vec3 &v);
	vec3 multiply(const vec3 &v);
	vec3 divide(const vec3 &v);
	static vec3 cross(const vec3 &v1, const vec3 &v2);
	static vec3 normalize(const vec3 &v);

	friend vec3 operator+(vec3 left, const vec3 &right);
	friend vec3 operator-(vec3 left, const vec3 &right);
	friend vec3 operator*(vec3 left, const vec3 &right);
	friend vec3 operator/(vec3 left, const vec3 &right);

	friend vec3 operator*(float factor, const vec3 &right);


	void operator+=(const vec3 &right);
	void operator-=(const vec3 &right);
	void operator*=(const vec3 &right);
	void operator/=(const vec3 &right);
	bool operator==(const vec3 &right);
	bool operator!=(const vec3 &right);

	friend ostream &operator<<(ostream &out, vec3 &v);
	friend istream &operator>>(istream &in, vec3 &v);
};

//储存index buffer
struct vec3u {
	unsigned short x;
	unsigned short y;
	unsigned short z;

	vec3u() {
		x = 0; y = 0; z = 0;
	}

	vec3u(unsigned short vx, unsigned short vy, unsigned short vz) {
		x = vx; y = vy; z = vz;
	}
};

struct vec3i {
	unsigned int x;
	unsigned int y;
	unsigned int z;

	vec3i() {
		x = 0; y = 0; z = 0;
	}

	vec3i(unsigned int vx, unsigned int vy, unsigned int vz) {
		x = vx; y = vy; z = vz;
	}
};

struct vec4 {
public:
	float x;
	float y;
	float z;
	float w;

	vec4();
	vec4(float posX, float posY, float posZ, float posW);

	vec4 add(const vec4 &v);
	vec4 subtract(const vec4 &v);
	vec4 multiply(const vec4 &v);
	vec4 divide(const vec4 &v);

	friend vec4 operator+(vec4 left, const vec4 &right);
	friend vec4 operator-(vec4 left, const vec4 &right);
	friend vec4 operator*(vec4 left, const vec4 &right);
	friend vec4 operator/(vec4 left, const vec4 &right);

	void operator+=(const vec4 &right);
	void operator-=(const vec4 &right);
	void operator*=(const vec4 &right);
	void operator/=(const vec4 &right);
	bool operator==(const vec4 &right);
	bool operator!=(const vec4 &right);

	friend ostream &operator<<(ostream &out, vec4 &v);
	friend istream &operator>>(istream &in, vec4 &v);
};

struct mat4{
	union {
		float elements[4 * 4];
		vec4 colunms[4];
	};

	mat4();
	mat4(float c);
	mat4(float m00, float m01, float m02, float m03,  //1st column
		 float m04, float m05, float m06, float m07,  //2st column
		 float m08, float m09, float m10, float m11,  //3st column
		 float m12, float m13, float m14, float m15  //4st column
		);

	//矩阵构造函数
	static mat4 translation(const vec3 &translation); //平移矩阵
	static mat4 scale(const vec3 &scale); //放缩矩阵
	static mat4 rotation(float angle, const vec3 &axis); //任意轴旋转矩阵
	static mat4 orthographic(float left, float right, float bottom, float top, float near, float far); //正投影矩阵
	static mat4 perspective(float fov, float aspectRatio, float near, float far); //透视投影矩阵

	mat4 multiply(const mat4 &m);
	friend mat4 operator*(mat4 left, mat4 right);
	void operator*=(const mat4 &right);
};