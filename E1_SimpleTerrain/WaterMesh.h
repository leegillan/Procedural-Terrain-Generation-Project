#pragma once
#include "PlaneMesh.h"
#include "PerlinNoise.h"

class WaterMesh : public PlaneMesh 
{
public:
	WaterMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128);
	~WaterMesh();

	void Resize(int newResolution);
	void Regenerate(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	const inline int GetResolution() { return resolution; }

private:
	void CreateBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	void BuildHeightMap();

	const float m_UVscale = 50.0f;			//Tile the UV map 10 times across the plane
	const float terrainSize = 100.0f;		//What is the width and height of our terrain
	float* heightMap;

	float zA = 2, xA = 1;
	float zF, xF;

	bool generation = true;
	bool circleFault = false;
	bool pNoise = false;

	int count = 0;

	PerlinNoise* perlinNoise;
};
