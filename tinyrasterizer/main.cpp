#include "inc/geometry/Vector.h"
#include "inc/geometry/Matrix.h"
#include <iostream>

using namespace Core;
using std::cout;
using std::cin;
using std::endl;

int main()
{
	Vector3f v3fa{ 1.0f, 2.0f, 3.0f };
	Vector3f v3fb{ 3.0f, 2.0f, 1.0f };
	Vector4f v4f{ 1.0f, 2.0f, 3.0f, 4.0f };
	Vector3f v3fc = v3fa * v3fb;
	cout << v3fa + v3fb << endl;
	Matrix3f m1;
	cout << m1 << endl;
	m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
	cout << m1 << endl;
	Matrix3f m2(m1);
	Matrix<float, 3, 4> tmp
	{ 1, 2, 3, 4,
	  5, 6, 7, 8,
	  9, 10, 11, 12 };
	cout << tmp << endl;
	//cout << tmp.transpose() << endl;
	cout << tmp.transpose_matrix() << endl;

	return 0;
}