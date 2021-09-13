#include "Triangle.h"

Triangle::Triangle()
{
	v[0] << 0, 0, 0, 1;
	v[1] << 0, 0, 0, 1;
	v[2] << 0, 0, 0, 1;

	color[0] << 0.0, 0.0, 0.0;
	color[1] << 0.0, 0.0, 0.0;
	color[2] << 0.0, 0.0, 0.0;

	tex_coords[0] << 0.0, 0.0;
	tex_coords[1] << 0.0, 0.0;
	tex_coords[2] << 0.0, 0.0;
}

inline void Triangle::setVertex(int index, Vector3f vertex)
{
	v[index] = vertex;
}

inline void Triangle::setNormal(int index, Vector3f n)
{
	normal[index] = n;
}

void Triangle::setColor(int index, float r, float g, float b)
{
	if ((r < 0.0f) || (r > 255.f) || (g < 0.0f) || (g > 255.f) || (b < 0.0f) || (b > 255.f))
		throw std::runtime_error("Invalid color values");
	color[index] = Vector3f({ r / 255.f, g / 255.f, b / 255.f });
}

void Triangle::setTexCoord(int index, float s, float t)
{
	tex_coords[index] = Vector2f({ s, t });
}