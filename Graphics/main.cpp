#include "vector.h"
#include "matrix.h"
#define DEBUG 0

int main() {
#if DEBUG>0
	vector v1(1.0f,2.0f,3.0f);
	vector v2(2.0f,4.0f,8.0f);
	vector v3(1.0f,5.0f,7.0f);
	vector v4(0,0,0);
	vector v5(3.0f,2.0f,1.0f);
	vector v6(2.0f,8.0f,3.0f);

	cout << "Enter Vector ";
	cin >> v4;
	
	cout << "Vector 1: " << v1 << endl;
	cout << "Vector 2: " << v2 << endl;
	cout << "Vector 3: " << v3 << endl;
	cout << "Vector 4: " << v4 << endl;

	cout << "v1 * v2 " << v1*v2 << endl;
	cout << "v2 - v2 " << v2-v1 << endl;
	cout << "v1 X v2 " << v1.cross(v2) << endl;

	cout << "Length of v3: " << v3.length() << endl;
	cout << "norm of v3: " << v3.norm() << endl;

	matrix m1(v1,v3,v5);
	matrix m2(v2,v4,v6);

	cout << "Matrix 1: " << endl << m1 << endl;
	cout << "Matrix 2: " << endl << m2 << endl;

	cout << "Transpose 1:" << endl << transpose(m1) << endl;
	cout << "Cofactor 1:" << endl << cofactor(m1) << endl;
	cout << "Det 1: " << det(m1) << endl;
	cout << "Inverse 1:" << endl << invert(m1) << endl;

	cout << endl << "---TEST---" << endl;
	matrix xRot( vector(1,0,0), 15 );
	matrix yRot( vector(0,1,0), 15 );
	matrix zRot( vector(0,0,1), 15 );
	cout << "xRot:" << endl << xRot2 << endl;
	cout << "yRot:" << yRot << endl;
	cout << "zRot:" << zRot << endl;
#endif

	vector p(2,3,1);
	vector point1(1,2,0);
	vector point2(0,1,4);
	float degree = 10;
	cout << "P:Before: " << p << endl;
	for( int i = 0; i < 36; i++ ) {
		p.rotatePoint( point1, point2, degree );
		cout << "Rotate" << i+3 << ": " << p << endl;
	}
	vector v(1,5,0);
	vector point3(0,0,0);
	vector point4(1,1,1);
	cout << "V:Before: " << v << endl;
	for( int i = 0; i < 36; i++ ) {
		v.rotateVector( point3, point4, degree );
		cout << "Rotate" << degree << ": " << v << endl;
	}


	cout << "End." << endl;
	int nothing;
	cin >> nothing;
	return 0;
}