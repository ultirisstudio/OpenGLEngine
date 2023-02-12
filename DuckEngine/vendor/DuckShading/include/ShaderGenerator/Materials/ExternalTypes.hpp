#pragma once

class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Texture
{
public:
	unsigned int id;

	Texture(unsigned int id = 0) : id(id) {}
};

class Cubemap
{
public:
	unsigned int id;

	Cubemap(unsigned int id = 0) : id(id) {}
};