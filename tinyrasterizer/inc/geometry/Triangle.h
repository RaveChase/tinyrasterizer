#ifndef __TRIANGLE_H_
#define __TRIANGLE_H_

#include "Vector.h"
#include "Matrix.h"

namespace core {
	class Triangle {
	public:
		Triangle();

		Vector3f a() const { return v[0]; }
		Vector3f b() const { return v[1]; }
		Vector3f c() const { return v[2]; }

		void setVertex(int index, Vector3f vertex);			/*set i-th vertex coordinates */
		void setNormal(int index, Vector3f n);				/*set i-th vertex normal vector*/
		void setColor(int index, float r, float g, float b);	/*set i-th vertex color*/
		void setTexCoord(int index, float s, float t);		/*set i-th vertex texture coordinate*/

		//Matrix4f toHomoCoord() const;

	private:
		Vector3f v[3]; //coordinates of the triangle, v0, v1, v2 in counter clockwise order
		/* values for each vertex*/
		Vector3f color[3];				// color
		Vector3f normal[3];			// normal direction
		Vector2f tex_coords[3];	// texture u,v
	};
}
#endif // __TRIANGLE_H_
