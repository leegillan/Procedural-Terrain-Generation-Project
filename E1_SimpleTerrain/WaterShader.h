#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class WaterShader : public BaseShader
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
		float blendVal;
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

	struct WaveBufferType
	{
		float amplitude;
		float speed;
		float steepness;
		bool wavePad;
		//
		XMFLOAT3 freq;
		float waveHeight;
	};


public:
	WaterShader(ID3D11Device* device, HWND hwnd);
	~WaterShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, Light* light, Timer* timer, float blendAmount, float amp, float speed, float steep, float xfreq, float yfreq, float zfreq, float height, XMFLOAT4 multiPosition[], XMFLOAT4 multiDiffuse[]);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* waveBuffer;

	ID3D11Buffer* lightBuffer;

	ID3D11SamplerState* sampleState;

};

