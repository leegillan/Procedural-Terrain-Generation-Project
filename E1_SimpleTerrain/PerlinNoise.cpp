#include "PerlinNoise.h"

bool start = true;
int p[B + B + 2];
float g3[B + B + 2][3];

void PerlinNoise::init()
{
	//This sets up a table of numbers which we use for generating a pseudo-random number later
	//and then generates arrays of pseudo-random gradients that we can access using the pseudo-random numbers
	//You don't need to worry too much about this
	int i, j, k;

	for (i = 0; i < B; i++)
	{
		p[i] = i;

		for (j = 0; j < 3; j++)
		{
			g3[i][j] = (float)((rand() % (B + B)) - B) / B;
		}

		normalise(g3[i]);
	}

	while (--i)
	{
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0; i < B + 2; i++)
	{
		p[B + i] = p[i];

		for (j = 0; j < 3; j++)
		{
			g3[B + i][j] = g3[i][j];
		}
	}

}

double PerlinNoise::noise(float x, float y, float z) 
{
	//The integer left & right x values and the bottom & top y values. These define what "cell" we are in
	int bx0 = 0, bx1 = 0, by0 = 0, by1 = 0, bz0 = 0, bz1 = 0;

	//We'll store pseudo-random numbers for each corner here, that we'll then use to get a pseudo-random gradient
	int b00 = 0, b10 = 0, b01 = 0, b11 = 0;

	//How far into the current "cell" we are. This is the fractional part of our point.
	float rx0 = 0.f, rx1 = 0.f, ry0 = 0.f, ry1 = 0.f, rz0 = 0.f, rz1 = 0.f;

	//The easing value along the x and y for our point
	float sx = 0.f, sy = 0.f, sz = 0.0f;

	//Some variables used to store values during calculations
	float* q;
	float a = 0.f, b = 0.f, c = 0.0f, d = 0.0f, u = 0.f, v = 0.f;
	int i, j;

	if (start == true)
	{
		start = false;
		init();
	}

	//Initialise all our variables on the X & Y & Z
	setup(x, 0, bx0, bx1, rx0, rx1);
	setup(y, 1, by0, by1, ry0, ry1);
	setup(z, 2, bz0, bz1, rz0, rz1);

	//Get a pseudo-random number for the x boundaries
	i = p[bx0];
	j = p[bx1];

	//Use this pseudo-random number to get a pseudo-random number for the four corners
	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];

	//Work out our position on the blend curve (3t^2 - 2t^3 here)
	sx = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

	//q is set to the pseudo-random gradient
	//Solve for the bottom two points
	q = g3[b00 + bz0];
	u = dotProduct(rx0, ry0, rz0, q[0], q[1], q[2]);	//U is the weighting of the gradient based on the distance of the point from this corner (0,0)
	q = g3[b10 + bz0];
	v = dotProduct(rx1, ry0, rz0, q[0], q[1], q[2]);	//V is the weighting of the gradient based on the distance of the point from this corner (1,0)
	a = lerp(sx, u, v);						//a is the the value of U and V blended together using the horizontal easing curve

	//Solve for the top two points
	q = g3[b01 + bz0];
	u = dotProduct(rx0, ry1, rz0, q[0], q[1], q[2]);	//U is the weighting of the gradient based on the distance of the point from this corner (0,1)
	q = g3[b11 + bz0];
	v = dotProduct(rx1, ry1, rz0, q[0], q[1], q[2]);	//V is the weighting of the gradient based on the distance of the point from this corner (1,1)
	b = lerp(sx, u, v);						//b is the the value of U and V blended together using the horizontal easing curve

	c = lerp(sy, a, b);

	//Solve for the front two points
	q = g3[b00 + bz1];
	u = dotProduct(rx0, ry0, rz1, q[0], q[1], q[2]);	//U is the weighting of the gradient based on the distance of the point from this corner (0,0)
	q = g3[b10 + bz1];
	v = dotProduct(rx1, ry0, rz1, q[0], q[1], q[2]);	//V is the weighting of the gradient based on the distance of the point from this corner (1,0)
	a = lerp(sx, u, v);						//a is the the value of U and V blended together using the horizontal easing curve

	//Solve for the back two points
	q = g3[b01 + bz1];
	u = dotProduct(rx0, ry1, rz1, q[0], q[1], q[2]);	//U is the weighting of the gradient based on the distance of the point from this corner (0,1)
	q = g3[b11 + bz1];
	v = dotProduct(rx1, ry1, rz1, q[0], q[1], q[2]);	//V is the weighting of the gradient based on the distance of the point from this corner (1,1)
	b = lerp(sx, u, v);						//b is the the value of U and V blended together using the horizontal easing curve

	d = lerp(sy, a, b);

	return lerp(sz, c, d);

}

//Setup the function variables
//b0 and b1 are the integer values before and after our point
//r0 and r1 are the fractional distances from the integer boundaries
//This is an old school way of doing it, which I've kept in to be as close to the original implementation as possible
void PerlinNoise::setup(float ax, int i, int& b0, int& b1, float& r0, float& r1) 
{
	float t = ax + N;

	b0 = ((int)t) & BM;
	b1 = (b0 + 1) & BM;

	r0 = t - (int)t;
	r1 = r0 - 1.0f;
}

//Function for normalising a 3D vector
void PerlinNoise::normalise(float v[]) 
{
	float s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}