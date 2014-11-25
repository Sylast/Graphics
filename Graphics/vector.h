#pragma once
/*
*
* Vector is an array of 3 floats.
* Vectors can represent Points, Vectors, and Colors
*
*/
#include <iostream>

#define _X 0
#define _Y 1
#define _Z 2
#define ORIGIN	vector(0.0f,0.0f,0.0f)
#define XAXIS	vector(1.0f,0.0f,0.0f)
#define YAXIS	vector(0.0f,1.0f,0.0f)
#define ZAXIS	vector(0.0f,0.0f,1.0f)

using namespace std;
class vector
{
public:
	float xyz[3];

	vector() {};
	vector( float, float, float );
	friend ostream& operator<<( ostream&, vector );
	friend istream& operator>>( istream&, vector );

	float& operator[]( int );
	float length();

	float operator*( vector );		//Dot Product
	vector operator*( float );		//Scalar Multiplication
	vector operator/( float );		//Scalar Division
	vector operator/( vector );		//Vector Division (just divids each float)
	vector operator+( vector );		//Vector Addition
	vector operator+( float );
	vector operator-( vector );		//Vector Subtraction
	vector operator-( float );
	vector operator^( vector );		//Cross product

	vector operator-();

	//Assignments
	vector operator*=( float );	
	vector operator/=( float );	
	vector operator+=( vector );	
	vector operator+=( float );
	vector operator-=( vector );	

	vector norm( );					//Vector Normilization

	//Rotations
	void rotatePoint(vector, vector, float );
	void rotateVector(vector,  float );

	//Color
	void SetFromColor( unsigned int );
	unsigned int GetColor();

	bool IntersectPlaneWithRay(vector rayDirection, 
    vector planeNormal, vector planePoint, vector &intersectionPoint);

};

class AABB {
public:
	vector corners[2];
	AABB(vector firstPoint);
	void AddPoint(vector pt);
	bool Clip(float t, float b, float l, float r);
};
