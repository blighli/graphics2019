#include "Vector.h"

/***********/
/*vec2*/
/***********/
vec2::vec2() {
	x = 0;
	y = 0;
}

vec2::vec2(float posX, float posY) {
	x = posX;
	y = posY;
}

vec2 vec2::add(const vec2 &v) {
	x = x + v.x;
	y = y + v.y;
	return *this;
}

vec2 vec2::subtract(const vec2 &v) {
	x = x - v.x;
	y = y - v.y;
	return *this;
}
vec2 vec2::multiply(const vec2 &v) {
	x = x * v.x;
	y = y * v.y;
	return *this;
}
vec2 vec2::divide(const vec2 &v) {
	x = x / v.x;
	y = y / v.y;
	return *this;
}

vec2 operator+(vec2 left, const vec2 &right) {
	return left.add(right);
}
vec2 operator-(vec2 left, const vec2 &right) {
	return left.subtract(right);
}
vec2 operator*(vec2 left, const vec2 &right) {
	return left.multiply(right);
}
vec2 operator/(vec2 left, const vec2 &right) {
	return left.divide(right);
}

void vec2::operator+=(const vec2 &right) {
	this->add(right);
}
void vec2::operator-=(const vec2 &right) {
	this->subtract(right);
}
void vec2::operator*=(const vec2 &right) {
	this->multiply(right);
}
void vec2::operator/=(const vec2 &right) {
	this->divide(right);
}
bool vec2::operator==(const vec2 &right) {
	if (this->x == right.x && this->y == right.y)
		return true;
	else
		return false;
}
bool vec2::operator!=(const vec2 &right) {
	if (*this == right)
		return false;
	else
		return true;
}

ostream &operator<<(ostream &out, vec2 &v) {
	out << "x: " << v.x << " y: " << v.y;
	return out;
}
istream &operator>>(istream &in, vec2 &v) {
	in >> v.x >> v.y;
	return in;
}


/***********/
/*vec3*/
/***********/

vec3::vec3() {
	x = 0;
	y = 0;
	z = 0;
}


vec3::vec3(float posX, float posY, float posZ) {
	x = posX;
	y = posY;
	z = posZ;
}


vec3 vec3::add(const vec3 &v) {
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
	return *this;
}

vec3 vec3::subtract(const vec3 &v) {
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
	return *this;
}
vec3 vec3::multiply(const vec3 &v) {
	x = x * v.x;
	y = y * v.y;
	z = z * v.z;
	return *this;
}
vec3 vec3::divide(const vec3 &v) {
	x = x / v.x;
	y = y / v.y;
	z = z / v.z;
	return *this;
}

vec3 vec3::normalize(const vec3 &v) {
	vec3 result;

	float a;
	a = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (a > 0) {
		result.x = v.x / a;
		result.y = v.y / a;
		result.z = v.z / a;
	}
	else {
		result.x = v.x;
		result.y = v.y;
		result.z = v.z;
	}
	return result;
}

//²æ³Ë
vec3 vec3::cross(const vec3 &left, const vec3 &right) {
	float u1 = left.x;
	float u2 = left.y;
	float u3 = left.z;
	float v1 = right.x;
	float v2 = right.y;
	float v3 = right.z;

	vec3 result;
	result.x = (u2 * v3 - u3 * v2);
	result.y = (u3 * v1 - u1 * v3);
	result.z = (u1 * v2 - u2 * v1);

	return result;
}


vec3 operator+(vec3 left, const vec3 &right) {
	return left.add(right);
}
vec3 operator-(vec3 left, const vec3 &right) {
	return left.subtract(right);
}
vec3 operator*(vec3 left, const vec3 &right) {
	return left.multiply(right);
}
vec3 operator/(vec3 left, const vec3 &right) {
	return left.divide(right);
}

vec3 operator*(float factor, const vec3 &right) {
	vec3 result;
	result.x = factor * right.x;
	result.y = factor * right.y;
	result.z = factor * right.z;

	return result;
}

void vec3::operator+=(const vec3 &right) {
	this->add(right);
}
void vec3::operator-=(const vec3 &right) {
	this->subtract(right);
}
void vec3::operator*=(const vec3 &right) {
	this->multiply(right);
}
void vec3::operator/=(const vec3 &right) {
	this->divide(right);
}
bool vec3::operator==(const vec3 &right) {
	if (this->x == right.x && this->y == right.y && this->z == right.z)
		return true;
	else
		return false;
}
bool vec3::operator!=(const vec3 &right) {
	if (*this == right)
		return false;
	else
		return true;
}

ostream &operator<<(ostream &out, vec3 &v) {
	out << "x: " << v.x << " y: " << v.y << " z: " << v.z;
	return out;
}
istream &operator>>(istream &in, vec3 &v) {
	in >> v.x >> v.y >> v.z;
	return in;
}

/***********/
/*vec4*/
/***********/

vec4::vec4() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vec4::vec4(float posX, float posY, float posZ, float posW) {
	x = posX;
	y = posY;
	z = posZ;
	w = posW;
}


vec4 vec4::add(const vec4 &v) {
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
	w = w + v.w;
	return *this;
}

vec4 vec4::subtract(const vec4 &v) {
	x = x - v.x;
	y = y - v.y;
	z = z - v.z;
	w = w - v.w;
	return *this;
}
vec4 vec4::multiply(const vec4 &v) {
	x = x * v.x;
	y = y * v.y;
	z = z * v.z;
	w = w * v.w;
	return *this;
}
vec4 vec4::divide(const vec4 &v) {
	x = x / v.x;
	y = y / v.y;
	z = z / v.z;
	w = w / v.w;
	return *this;
}

vec4 operator+(vec4 left, const vec4 &right) {
	return left.add(right);
}
vec4 operator-(vec4 left, const vec4 &right) {
	return left.subtract(right);
}
vec4 operator*(vec4 left, const vec4 &right) {
	return left.multiply(right);
}
vec4 operator/(vec4 left, const vec4 &right) {
	return left.divide(right);
}

void vec4::operator+=(const vec4 &right) {
	this->add(right);
}
void vec4::operator-=(const vec4 &right) {
	this->subtract(right);
}
void vec4::operator*=(const vec4 &right) {
	this->multiply(right);
}
void vec4::operator/=(const vec4 &right) {
	this->divide(right);
}
bool vec4::operator==(const vec4 &right) {
	if (this->x == right.x && this->y == right.y && this->z == right.z && this->w == right.w)
		return true;
	else
		return false;
}
bool vec4::operator!=(const vec4 &right) {
	if (*this == right)
		return false;
	else
		return true;
}

ostream &operator<<(ostream &out, vec4 &v) {
	out << "x: " << v.x << " y: " << v.y << " z: " << v.z << " w: " << v.w;
	return out;
}
istream &operator>>(istream &in, vec4 &v) {
	in >> v.x >> v.y >> v.z >> v.w;
	return in;
}


/***********/
/*mat4*/
/***********/

mat4::mat4(){
	this->colunms[0] = { 0, 0, 0, 0 };
	this->colunms[1] = { 0, 0, 0, 0 };
	this->colunms[2] = { 0, 0, 0, 0 };
	this->colunms[3] = { 0, 0, 0, 0 };
}

mat4::mat4(float c) {
	this->colunms[0] = { c, 0, 0, 0 };
	this->colunms[1] = { 0, c, 0, 0 };
	this->colunms[2] = { 0, 0, c, 0 };
	this->colunms[3] = { 0, 0, 0, c };
}

mat4::mat4(float m00, float m01, float m02, float m03,  //1st column
	float m04, float m05, float m06, float m07,  //2st column
	float m08, float m09, float m10, float m11,  //3st column
	float m12, float m13, float m14, float m15  //4st column
) {
	elements[0] = m00; elements[1] = m01; elements[2] = m02; elements[3] = m03;
	elements[4] = m04; elements[5] = m05; elements[6] = m06; elements[7] = m07;
	elements[8] = m08; elements[9] = m09; elements[10]= m10; elements[11]= m11;
	elements[12]= m12; elements[13]= m13; elements[14]= m14; elements[15]= m15;
}

mat4 mat4::translation(const vec3 &translation) {
	mat4 matrix;
	float x = translation.x;
	float y = translation.y;
	float z = translation.z;

	matrix.colunms[0] = { 1, 0, 0, 0 };
	matrix.colunms[1] = { 0, 1, 0, 0 };
	matrix.colunms[2] = { 0, 0, 1, 0 };
	matrix.colunms[3] = { x, y, z, 1 };

	return matrix;
}
mat4 mat4::scale(const vec3 &scale) {
	mat4 matrix;
	float x = scale.x;
	float y = scale.y;
	float z = scale.z;

	matrix.colunms[0] = { x, 0, 0, 0 };
	matrix.colunms[1] = { 0, y, 0, 0 };
	matrix.colunms[2] = { 0, 0, z, 0 };
	matrix.colunms[3] = { 0, 0, 0, 1 };

	return matrix;
}
mat4 mat4::rotation(float angle, const vec3 &axis) {
	mat4 matrix;
	float c = cos(toRadians(angle));
	float s = sin(toRadians(angle));

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	matrix.colunms[0] = { x * x *(1 - c) + c, x * y *(1 - c) + z * s, x * z*(1 - c) - y * s, 0 };
	matrix.colunms[1] = { x * y*(1 - c) - z * s, y * y*(1 - c) + c, y * z*(1 - c) + x * s, 0 };
	matrix.colunms[2] = { x * z*(1 - c) + y * s, y * z*(1 - c) - x * s, z * z*(1 - c) + c, 0 };
	matrix.colunms[3] = { 0, 0, 0, 1 };

	return matrix;
}
mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
	mat4 matrix;
	
	matrix.colunms[0] = { 2 / (right - left), 0, 0, 0 };
	matrix.colunms[1] = { 0, 2 / (top - bottom), 0, 0 };
	matrix.colunms[2] = { 0, 0, -2 / (far - near), 0 };
	matrix.colunms[3] = { -(right + left) / (right - left), -(top + bottom) / top - bottom, -(far + near) / (far - near), 1 };
	
	return matrix;
}
mat4 mat4::perspective(float fov, float aspectRatio, float near, float far) {
	mat4 matrix;

	matrix.colunms[0] = { 1 / (aspectRatio * tan(toRadians(fov) / 2)), 0, 0, 0 };
	matrix.colunms[1] = { 0, 1 / tan(toRadians(fov) / 2), 0, 0 };
	matrix.colunms[2] = { 0, 0, -(far + near) / (far - near), -1 };
	matrix.colunms[3] = { 0, 0, -(2 * far * near) / (far - near), 0 };

	return matrix;
}

#define I(_i, _j) ((_j) + 4*(_i))
mat4 mat4::multiply(const mat4 &m) {
	mat4 tempm;
	float temp = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp = 0;
			for (int k = 0; k < 4; k++) {
				temp += m.elements[I(i, k)] * this->elements[I(k, j)];
			}
			tempm.elements[I(i, j)] = temp;
		}
	}
	for (int i = 0; i < 4 * 4; i++) {
		this->elements[i] = tempm.elements[i];
	}
	return *this;
}
mat4 operator*(mat4 left, mat4 right) {
	return right.multiply(left);
}
void mat4::operator*=(const mat4 &right) {
	this->multiply(right);
}
