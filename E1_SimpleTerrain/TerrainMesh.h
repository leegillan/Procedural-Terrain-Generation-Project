#pragma once
#include "PlaneMesh.h"
#include "PerlinNoise.h"
#include "Timer.h"

class TerrainMesh : public PlaneMesh 
{
public:
	TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Timer* timer, int resolution = 128);
	~TerrainMesh();

	void Resize(int newResolution);
	void Regenerate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Timer* timer);
	void Smoothing(int times);
	void faultTerrain(int faults);
	void PerNoise();

	const inline int GetResolution() { return resolution; }

	float* GetZAmp() { return &zA; }
	void SetZAmp(float z) { zA = z; }

	float* GetXAmp() { return &xA; }
	void SetXAmp(float x) { xA = x; }

	float* GetZFreq() { return &zF; }
	void SetZFreq(float z) { zF = z; }

	float* GetXFreq() { return &xF; }
	void SetXFreq(float x) { xF = x; }

	bool* GetGen() { return &generation; }
	void SetGen(bool b) { generation = b; }

	bool* GetCircleFault() { return &circleFault; }
	void SetCircleFault(bool c) { circleFault = c; }

	bool* GetPerlinNoise() { return &pNoise; }
	void SetPerlinNoise(bool pN) { pNoise = pN; }

	//Linear interpolation
	double lerp(float t, float a, float b) { return (a + t * (b - a)); }

private:
	void CreateBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	void BuildHeightMap(Timer* timer);

	const float m_UVscale = 50.0f;			//Tile the UV map 10 times across the plane
	const float terrainSize = 100.0f;		//What is the width and height of our terrain
	float* heightMap;

	float zA = 2.0f, xA = 1.0f;
	float zF = 0.2f, xF = 0.9f;

	bool start = false;
	bool generation = true;
	bool circleFault = false;
	bool pNoise = false;

	int count = 0;

	//Class instances
	PerlinNoise* perlinNoise;
};
