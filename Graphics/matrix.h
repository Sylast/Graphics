#pragma once

#include <iostream>
#include <math.h>
#include "vector.h"
#define xaxis vector(1,0,0)
#define yaxis vector(0,1,0)
#define zaxis vector(0,0,1)


#define PI 3.14159265f

class matrix
{
	vector rows[3];
	
public:
	matrix();

	//Creates a matrix from 3 row vectors
	matrix( vector, vector, vector );

	//Creates a rotation matrix around any vector
	matrix( vector, float );

	friend ostream& operator<<( ostream&, matrix );
	friend istream& operator>>( istream&, matrix );

	vector& operator[]( int );	//Row Vector operation
	vector operator()( int );	//Column Vector operation

	matrix operator*( matrix );
	vector operator*( vector );
	matrix operator/( float );
	matrix operator*( float );

	void normalize();
	vector getCol(int i);
	void setCol(int i, vector ci);
	
};
	matrix cofactor( matrix );
	float det( matrix );
	matrix transpose( matrix );
	matrix invert( matrix );

