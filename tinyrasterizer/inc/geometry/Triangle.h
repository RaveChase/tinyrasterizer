#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vector.h"
#include "Matrix.h"

using namespace Core;
class Triangle {
public:
	Triangle();

	Vector3f a() const { return v[0]; }
	Vector3f b() const { return v[1]; }
	Vector3f c() const { return v[2]; }

	void setVertex(int index, Vector3f vertex);			    /*set i-th vertex coordinates */
	void setNormal(int index, Vector3f n);				    /*set i-th vertex normal vector*/
	void setColor(int index, float r, float g, float b);	/*set i-th vertex color*/
	void setTexCoord(int index, float s, float t);		    /*set i-th vertex texture coordinate*/

private:
	std::array<Vector3f, 3> v;          //coordinates of the triangle, v0, v1, v2 in counter clockwise order
	/* values for each vertex*/
	std::array<Vector3f, 3> color;		// color
	std::array<Vector3f, 3> normal;		// normal direction
	std::array<Vector2f, 3> tex_coords;	// texture u,v
};
#endif // __TRIANGLE_H__
