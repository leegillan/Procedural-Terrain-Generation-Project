// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework

#include "TerrainShader.h"
#include "TerrainMesh.h"
#include "WaterShader.h"
#include "WaterMesh.h"

class App1 : public BaseApplication
{
public:
	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();
 
protected:
	bool render();
	void gui();

private:
	//Shaders
	TerrainShader* terrainShader;
	WaterShader* waterShader;

	//Meshes
	TerrainMesh* m_Terrain;
	WaterMesh* m_Water;

	//Light Variables (only XMFLOATS are supported)
	Light* light;
	Light* multiLight[4];

	float light1Diffuse[3] = { 0.0f, 0.0f, 0.0f };
	float light1Pos[3] = { 20.0f, -10.0f, 20.0f };

	float light2Diffuse[3] = { 0.0f, 0.0f, 0.0f };
	float light2Pos[3] = { -20.0f, -10.0f, 20.0f };

	float light3Diffuse[3] = { 0.0f, 0.0f, 0.0f };
	float light3Pos[3] = { -20.0f, -10.0f, -20.0f };

	float light4Diffuse[3] = { 0.0f, 0.0f, 0.0f };
	float light4Pos[3] = { 20.0f, -10.0f, -20.0f };

	//Terrain and Water var
	int terrainResolution = 128;
	float tim;
	int faultCount = 10, smoothCount = 1;
	bool terrainType = false;

	//
	float waveAmp = 0.7f;
	float waveSpeed = 2.2f;
	float waveSteep = 0.4f;
	float waveHeight = 2.0f;
	float waveXFreq = 0.5f;
	float waveYFreq = 0.4f;
	float waveZFreq = 0.1f;
	float blendAmount = 0.333f;

	//Perlin
	double pNoise;
	bool perlinNoise = false;


	//Input
	Input in;
};

#endif