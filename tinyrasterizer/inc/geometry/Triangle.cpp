#include "Triangle.h"

namespace core {
	Triangle::Triangle()
	{
		memset(this, 0, sizeof(Triangle));
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
		color[index] = Vector3f(r / 255.f, g / 255.f, b / 255.f);
	}

	void Triangle::setTexCoord(int index, float s, float t)
	{
		tex_coords[index] = Vector2f(s, t);
	}

}
