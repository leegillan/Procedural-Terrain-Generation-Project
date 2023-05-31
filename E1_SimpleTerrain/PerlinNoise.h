/**
* \class C Perlin Noise
*
* \brief Implementation of the original Perlin Noise algorithm
*
* This class takes most of the original C implementation of Perlin Noise from https://mrl.nyu.edu/~perlin/doc/oscar.html
* and builds it in a class for C++.
* As much as possible, the code has been kept the same as Ken's implementation, except where it would be overly confusing.
* There is much scope to improve this implementation, and this makes none of the suggested improvements from Ken's "Improved Perlin Noise"
* nor does it have the code for 3D Noise
*
* As such, this class is mostly for educational purposes.
*
* \author Gaz Robinson, from Ken Perlin

* Upgraded partially to improved noise using https://mrl.nyu.edu/~perlin/paper445.pdf
*/
#pragma once
#include <cstdlib>

//These should be constants, but I'm sticking to the original implementation for clarity
const int B = 256;	//256
const int BM = 255; 	//255
const int N = 4096;	//4096

class PerlinNoise 
{
public:
	//Prevent instances being created
	PerlinNoise() {};
	~PerlinNoise() {};

	//noise function
	double noise(float x, float y, float z);

private:

	void init();

	void setup(float ax, int i, int& b0, int& b1, float& r0, float& r1);

	///Helper math functions///
	void normalise(float vec[]);

	//Easing interpolation
	//old (3t^2 - 2t^3)
	double	s_curve(float t) { return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f); }

	//Linear interpolation
	double lerp(float t, float a, float b) { return (a + t * (b - a)); }

	double dotProduct(float x1, float y1, float z1, float x2, float y2, float z2) { return  x1 * x2 + y1 * y2 + z1 * z2; }
};

