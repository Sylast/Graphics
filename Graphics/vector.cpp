#include "vector.h"
#include <cmath>
#include "matrix.h"
#define origin vector(0.0f,0.0f,0.0f)

vector::vector(float x, float y, float z) {
	xyz[_X] = x;
	xyz[_Y] = y;
	xyz[_Z] = z;
}


ostream& operator<<(ostream& os, vector v) {
	os
		<< v.xyz[_X] << " "
		<< v.xyz[_Y] << " "
		<< v.xyz[_Z];
	return os;
}

istream& operator>>(istream& in, vector v) {
	in >> v[_X] >> v[_Y] >> v[_Z];
	return in;
}

float& vector::operator[](int i) {
	return xyz[i];
}
float vector::length(){
	return
		sqrt( (float)(pow(xyz[_X], 2.0) + pow(xyz[_Y], 2.0) + pow(xyz[_Z], 2.0)) );
}

//Dot Product
float vector::operator*(vector v) {
	return
		xyz[_X]*v[_X] + xyz[_Y]*v[_Y] + xyz[_Z]*v[_Z];
}

//Scalar Multiplication
vector vector::operator*(float f) {
	vector ret(*this);
	ret[0] *= f;
	ret[1] *= f;
	ret[2] *= f;
	return ret;
}

//Scalar Division
vector vector::operator/(float f) {
	vector ret(*this);
	ret[0] /= f;
	ret[1] /= f;
	ret[2] /= f;
	return ret;
}

vector vector::operator/(vector v) {
	vector ret(*this);
	ret[0] /= v[0];
	ret[1] /= v[1];
	ret[2] /= v[2];
	return ret;
}

//Vector Addition
vector vector::operator+(vector v) {
	vector ret(*this);
	ret[0] += v[0];
	ret[1] += v[1];
	ret[2] += v[2];
	return ret;
}

//Adding a float to all parts of a vector
vector vector::operator+(float f) {
	vector ret(*this);
	ret[0] += f;
	ret[1] += f;
	ret[2] += f;
	return ret;
}

//Vector Subtrcation
vector vector::operator-(vector v) {
	vector ret(*this);
	ret[0] -= v[0];
	ret[1] -= v[1];
	ret[2] -= v[2];
	return ret;
}

vector vector::operator-(float f) {
	vector ret(*this);
	ret[0] -= f;
	ret[1] -= f;
	ret[2] -= f;
	return ret;
}

//Cross Product
vector vector::operator^(vector v) {
	float x = xyz[_Y]*v[_Z]-xyz[_Z]*v[_Y];
	float y = xyz[_Z]*v[_X]-xyz[_X]*v[_Z];
	float z = xyz[_X]*v[_Y]-xyz[_Y]*v[_X];
	return
		vector(x,y,z);
}

vector vector::operator-() {
	return (*this)*-1;
}

vector vector::operator*=( float f ) {
	return (*this)*f;
}	

vector vector::operator/=( float f ) {
	return (*this)*f;
}	
vector vector::operator+=( vector v ) {
	return (*this)+v;
}	
vector vector::operator+=( float f ) {
	return (*this)+f;
}	
vector vector::operator-=( vector v ) {
	return (*this)-v;
}

//Vector Normlization
vector vector::norm() {
	vector v(*this);
	return v/v.length();
}

bool vector::IntersectPlaneWithRay(vector rayDirection, 
    vector planeNormal, vector planePoint, vector &intersectionPoint) {

  vector &P = *this;
  float t = ((planePoint - P)*planeNormal) / (rayDirection * planeNormal);
  if (t < 0.0f)
    return false;
  intersectionPoint = P + rayDirection * t;
  return true;

}

//Rotations

//Rotation of a point, using two points to define the axis of rotation
void vector::rotatePoint( vector aO, vector aD, float thetad ) {
 
  float adx = fabsf(xaxis*aD);
  float ady = fabsf(yaxis*aD);
  vector aux;
  if (adx < ady) {
    aux = xaxis;
  }
  else {
    aux = yaxis;
  }
 
  matrix lm;
  lm[0] = (aux^aD).norm();
  lm[1] = aD.norm();
  lm[2] = (lm[0] ^ lm[1]).norm();
 
  matrix ilm = invert(lm);
 
  matrix rotY(yaxis, thetad);
 
  vector pt(*this);
  vector lpt = lm*(pt-aO);
  vector rlpt = rotY*lpt;
  vector ret = aO + ilm*rlpt;
  (*this) = ret;
}

//Rotation of a vector around an axis.
void vector::rotateVector( vector aD, float thetad ) {

  float adx = fabsf(xaxis*aD);
  float ady = fabsf(yaxis*aD);
  vector aux;
  if (adx < ady) {
    aux = xaxis;
  }
  else {
    aux = yaxis;
  }
 
  matrix lm;
  lm[0] = (aux^aD).norm();
  lm[1] = aD.norm();
  lm[2] = (lm[0] ^ lm[1]).norm();
 
  matrix ilm = invert(lm);
 
  matrix rotY(yaxis, thetad);
 
  vector pt(*this);
  vector lpt = lm*pt;
  vector rlpt = rotY*lpt;
  vector ret = ilm*rlpt;
  (*this) = ret;

}

void vector::SetFromColor(unsigned int color) {

  vector &v = *this;
  v[0] = ((unsigned char*)&color)[0];
  v[1] = ((unsigned char*)&color)[1];
  v[2] = ((unsigned char*)&color)[2];
  v = v / 255.0f;

}

unsigned int vector::GetColor() {

  vector &v = *this;
  unsigned int red = (int) (v[0]*255.0f);
  unsigned int green = (int) (v[1]*255.0f);
  unsigned int blue = (int) (v[2]*255.0f);

  if (red < 0)
    red = 0;
  else if (red > 255)
    red = 255;
  if (green < 0)
    green = 0;
  else if (green > 255)
    green = 255;
  if (blue < 0)
    blue = 0;
  else if (blue > 255)
    blue = 255;

  unsigned int ret = 0xFF000000 + blue * 256*256 + green * 256 + red;

  return ret;

}

AABB::AABB(vector firstPoint) {

  corners[0] = corners[1] = firstPoint;

}

void AABB::AddPoint(vector pt) {

  if (corners[0][0] > pt[0]) {
    corners[0][0] = pt[0];
  }
  if (corners[0][1] > pt[1]) {
    corners[0][1] = pt[1];
  }
  if (corners[0][2] > pt[2]) {
    corners[0][2] = pt[2];
  }


  if (corners[1][0] < pt[0]) {
    corners[1][0] = pt[0];
  }
  if (corners[1][1] < pt[1]) {
    corners[1][1] = pt[1];
  }
  if (corners[1][2] < pt[2]) {
    corners[1][2] = pt[2];
  }

}

bool AABB::Clip(float l, float r, float t, float b) {
	
	if (corners[1][0] <= l)
		return false;
	if (corners[0][0] >= r)
		return false;
	if (corners[1][1] <= t)
		return false;
	if (corners[0][1] >= b)
		return false;

	if (corners[0][0] < l)
		corners[0][0] = l;
	if (corners[1][0] > r)
		corners[1][0] = r;
	if (corners[0][1] < t)
		corners[0][1] = t;
	if (corners[1][1] > b)
		corners[1][1] = b;

	return true;
}
