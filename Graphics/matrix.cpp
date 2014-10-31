#include "matrix.h"

using namespace std;

matrix::matrix() {
	rows[0] = vector( 1.0f, 0.0f, 0.0f );
	rows[1] = vector( 0.0f, 1.0f, 0.0f );
	rows[2] = vector( 0.0f, 0.0f, 1.0f );
}

matrix::matrix(vector v1, vector v2, vector v3) {
	rows[0] = v1;
	rows[1] = v2;
	rows[2] = v3;
}


matrix::matrix( vector v, float a) {
	v = v.length() != 1 ? v/v.length() : v;
	a = a*PI/180.0f;
	float s = sin(a);
	float c = cos(a);
	float x = v[_X], y = v[_Y], z = v[_Z];
	float x2 = pow(v[_X],2), y2 = pow(v[_Y], 2), z2 = pow(v[_Z], 2);

	rows[0] = vector(	c + x2*(1-c), 
		x * y * (1-c) - z*s,
		x * z * (1-c) + y*s);

	rows[1] = vector(	y * x * (1-c) + z*s, 
		c + y2 * (1-c),
		y * z * (1-c) - x*s);

	rows[2] = vector(	z * x * (1-c) - y*s, 
		z * y * (1-c) + x*s,
		c + z2 * (1-c));
}

ostream& operator<<(ostream& os, matrix m) {
	os
		<< m.rows[0] << endl
		<< m.rows[1] << endl
		<< m.rows[2] << endl;
	return os;
}
istream& operator>>(istream& in, matrix m) {
	cin >> m.rows[0] >> m.rows[1] >> m.rows[2];
	return in;
}


//Row Vector operation
vector& matrix::operator[](int i) {
	return rows[i];
}

//Column Vector operation
vector matrix::operator()(int i) {
	vector ret(rows[_X][i], rows[_Y][i], rows[_Z][i]);
	return ret;
}

matrix matrix::operator*(matrix m) {
	vector x( rows[0]*m(0), rows[0]*m(1), rows[0]*m(2) );
	vector y( rows[1]*m(0), rows[1]*m(1), rows[1]*m(2) );
	vector z( rows[2]*m(0), rows[2]*m(1), rows[2]*m(2) );

	return matrix(x,y,z);
}

vector matrix::operator*(vector v) {
	vector u( rows[0]*v, rows[1]*v, rows[2]*v );
	return u;
}

matrix matrix::operator/(float f) {
	return matrix( rows[0]/f, rows[1]/f, rows[2]/f );
}

matrix matrix::operator*(float f) {
	return matrix( rows[0]*f, rows[1]*f, rows[2]*f );
}

void matrix::normalize() {
	void setCol(int i, vector ci);

	vector vs = rows[0] + rows[1] + rows[2];
	float sum = vs[0] + vs[1] + vs[2];

	rows[0] = rows[0] / sum;
	rows[1] = rows[1] / sum;
	rows[2] = rows[2] / sum;
}

float det(matrix m) {
	return(	(	m[0][0]*( m[1][1]*m[2][2] - m[1][2]*m[2][1] ))
		-	(	m[0][1]*( m[1][0]*m[2][2] - m[1][2]*m[2][0] ))
		+	(	m[0][2]*( m[1][0]*m[2][1] - m[1][1]*m[2][0] )) );
}

matrix transpose(matrix m) {
	return matrix( m(0), m(1), m(2) );
}

matrix cofactor(matrix m) {
	vector v1(	
		(m[1][1]*m[2][2]-m[1][2]*m[2][1]), 
		(m[1][0]*m[2][2]-m[1][2]*m[2][0])*-1,
		(m[1][0]*m[2][1]-m[1][1]*m[2][0]) 
		);

	vector v2(
		(m[0][1]*m[2][2]-m[0][2]*m[2][1])*-1, 
		(m[0][0]*m[2][2]-m[0][2]*m[2][0]),
		(m[0][0]*m[1][2]-m[0][1]*m[2][0])*-1 
		);

	vector v3(
		(m[0][1]*m[1][2]-m[0][2]*m[1][1]), 
		(m[0][0]*m[1][2]-m[0][2]*m[1][0])*-1,
		(m[0][0]*m[1][1]-m[0][1]*m[1][0])
		);

	return matrix( v1, v2, v3 );
}

matrix invert(matrix m) {

	matrix ret;
	vector a = m(0), b = m(1), c = m(2);
	vector _a = b ^ c; _a = _a / (a * _a);
	vector _b = c ^ a; _b = _b / (b * _b);
	vector _c = a ^ b; _c = _c / (c * _c);
	ret[0] = _a;
	ret[1] = _b;
	ret[2] = _c;

	return ret;
}

void matrix::setCol(int i, vector ci) {
	rows[0][i] = ci[0];
	rows[1][i] = ci[1];
	rows[2][i] = ci[2];
}

