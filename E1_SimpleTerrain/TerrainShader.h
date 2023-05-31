#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class TerrainShader : public BaseShader
{
private:
	struct LightBufferType
	{
		// = 16 
		XMFLOAT4 multiLightPosition[4];
		//
		XMFLOAT4 ambient;
		//
		XMFLOAT4 diffuse;
		//
		XMFLOAT3 direction;
		float waveHeight;
		//
		XMFLOAT4 multiLightDiffuse[4];
		//
	};

	struct TimeBufferType
	{
		float time;
		float fps;
		XMFLOAT2 padding;
		//
	};


public:
	TerrainShader(ID3D11Device* device, HWND hwnd);
	~TerrainShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* grassTexture, ID3D11ShaderResourceView* snowTexture, ID3D11ShaderResourceView* rockTexture, ID3D11ShaderResourceView* sandTexture, Light* light, Timer* timer, XMFLOAT4 multiPosition[], XMFLOAT4 multiDiffuse[], float height);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

	//buffer
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* lightBuffer;

	//sampler
	ID3D11SamplerState* sampleState;

};

