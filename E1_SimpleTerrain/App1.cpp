// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"
#include <ctime>

App1::App1()
{
	m_Terrain = nullptr;
	m_Water = nullptr;

	textureMgr = 0;

	terrainShader = nullptr;
	waterShader = nullptr;

	light = 0;
	multiLight[0] = 0;
	multiLight[1] = 0;
	multiLight[2] = 0;
	multiLight[3] = 0;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//Load in Terrain Texture
	textureMgr->loadTexture(L"grass", L"res/grass.jpg");
	textureMgr->loadTexture(L"snow", L"res/snow.jpg");
	textureMgr->loadTexture(L"rock", L"res/rock.jpg");
	textureMgr->loadTexture(L"sand", L"res/sand.jpg");

	//Load in Water Texture
	textureMgr->loadTexture(L"water", L"res/water.jpg");

	// Create Mesh objects and shader objects
	m_Terrain = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), timer);
	m_Water = new WaterMesh(renderer->getDevice(), renderer->getDeviceContext());

	waterShader = new WaterShader(renderer->getDevice(), hwnd);
	terrainShader = new TerrainShader(renderer->getDevice(), hwnd);

	//Setup lights for the scene
	light = new Light;
	light->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light->setDiffuseColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDirection(0.7f, 0.0f, 0.7f);

	multiLight[0] = new Light;
	multiLight[0]->setPosition(light1Pos[0], light1Pos[1], light1Pos[2]);
	multiLight[0]->setDiffuseColour(light1Diffuse[0], light1Diffuse[1], light1Diffuse[2], 1.0f);

	multiLight[1] = new Light;
	multiLight[1]->setPosition(light2Pos[0], light2Pos[1], light2Pos[2]);
	multiLight[1]->setDiffuseColour(light2Diffuse[0], light2Diffuse[1], light2Diffuse[2], 1.0f);

	multiLight[2] = new Light;
	multiLight[2]->setPosition(light3Pos[0], light3Pos[1], light3Pos[2]);
	multiLight[2]->setDiffuseColour(light3Diffuse[0], light3Diffuse[1], light3Diffuse[2], 1.0f);

	multiLight[3] = new Light;
	multiLight[3]->setPosition(light4Pos[0], light4Pos[1], light4Pos[2]);
	multiLight[3]->setDiffuseColour(light4Diffuse[0], light4Diffuse[1], light4Diffuse[2], 1.0f);

	camera->setPosition(0.0f, 45.0f, -30.0f);
	camera->setRotation( 0.0f, 30.0f,30.0f );

	terrainResolution = m_Terrain->GetResolution();
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if ( m_Terrain )
	{
		delete m_Terrain;
		m_Terrain = 0;
	}

	if (m_Water)
	{
		delete m_Water;
		m_Water = 0;
	}
	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = 0;
	}
	if (waterShader)
	{
		delete waterShader;
		waterShader = 0;
	}

	if (light)
	{
		delete light;
		light = 0;
	}

	if (multiLight)
	{
		delete multiLight;

		for (int i = 0; i < 4; i++)
		{
			multiLight[i] = 0;
		}
	}
}

bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	//Regenerates terrain every couple seconds if the boolean is true
	if (tim < 3 && m_Terrain->GetGen())
	{
		tim += timer->getTime();
	}
	else
	{
		if (terrainResolution != m_Terrain->GetResolution()) 
		{
			m_Terrain->Resize(terrainResolution);
		}

		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), timer);
		tim = 0;
	}

	//Updates lights when changed using ImGui
	multiLight[0]->setPosition(light1Pos[0], light1Pos[1], light1Pos[2]);
	multiLight[0]->setDiffuseColour(light1Diffuse[0], light1Diffuse[1], light1Diffuse[2], 1.0f);

	multiLight[1]->setPosition(light2Pos[0], light2Pos[1], light2Pos[2]);
	multiLight[1]->setDiffuseColour(light2Diffuse[0], light2Diffuse[1], light2Diffuse[2], 1.0f);

	multiLight[2]->setPosition(light3Pos[0], light3Pos[1], light3Pos[2]);
	multiLight[2]->setDiffuseColour(light3Diffuse[0], light3Diffuse[1], light3Diffuse[2], 1.0f);

	multiLight[3]->setPosition(light4Pos[0], light4Pos[1], light4Pos[2]);
	multiLight[3]->setDiffuseColour(light4Diffuse[0], light4Diffuse[1], light4Diffuse[2], 1.0f);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	//Setup XM lights to be passed
	XMFLOAT4 multiPos[4];
	XMFLOAT4 multiDiffuse[4];

	for (int i = 0; i < 4; i++)
	{
		multiPos[i] = XMFLOAT4(multiLight[i]->getPosition().x, multiLight[i]->getPosition().y, multiLight[i]->getPosition().z, 1.0f);
		multiDiffuse[i] = multiLight[i]->getDiffuseColour();
	}

	//Move the scene over to the left a bit
	XMMATRIX translation = XMMatrixTranslation(-40.0f, -5.0f, -40.0f);
	worldMatrix *= translation;

	// Send geometry data, set shader parameters, render object with shader
	m_Terrain->sendData(renderer->getDeviceContext());
	terrainShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), textureMgr->getTexture(L"snow"), textureMgr->getTexture(L"rock"), textureMgr->getTexture(L"sand"), light, timer, multiPos, multiDiffuse, waveHeight);
	terrainShader->render(renderer->getDeviceContext(), m_Terrain->getIndexCount());

	//Make the water transparent by turning off alpha buffer
	renderer->setAlphaBlending(true);

	// Send geometry data, set shader parameters, render object with shader
	m_Water->sendData(renderer->getDeviceContext());
	waterShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"water"), light, timer, blendAmount, waveAmp, waveSpeed, waveSteep, waveXFreq, waveYFreq, waveZFreq, waveHeight, multiPos, multiDiffuse);
	waterShader->render(renderer->getDeviceContext(), m_Water->getIndexCount());

	//Turn buffer back on
	renderer->setAlphaBlending(false);

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Text( "Camera Pos: (%.2f, %.2f, %.2f)", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z );
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::SliderInt( "Terrain Resolution", &terrainResolution, 2, 1024 );

	ImGui::Checkbox("If toggled will change to time-based terrain generation", &terrainType);

	if (terrainType == true)
	{
		//Changes seed of rand based on time
		srand(std::time(nullptr));
	}

	if (ImGui::Button("Generate Procedural Terrain"))
	{
		m_Terrain->SetGen(true);

		//Sets values to a random number between 0 and 1
		m_Terrain->SetXAmp((float)rand() / RAND_MAX);
		m_Terrain->SetZAmp((float)rand() / RAND_MAX);
		m_Terrain->SetXFreq((float)rand() / RAND_MAX);
		m_Terrain->SetZFreq((float)rand() / RAND_MAX);

		//regenerates terrain using random numbers
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), timer);
		m_Terrain->PerNoise();

		//Faults terrain using both methods
		m_Terrain->SetCircleFault(false);
		m_Terrain->faultTerrain(300);
		m_Terrain->PerNoise();
		m_Terrain->SetCircleFault(true);
		m_Terrain->faultTerrain(200);
		m_Terrain->PerNoise();
		m_Terrain->SetCircleFault(false);
		m_Terrain->faultTerrain(300);
		m_Terrain->PerNoise();

		//Give some texture and then smooth it out
		m_Terrain->PerNoise();
		m_Terrain->Smoothing(2);
		m_Terrain->PerNoise();

		//Puts wave to a random height between -2 and 15
		waveHeight = -2 + (float)(15.0 * (rand() / (RAND_MAX + 1.0)));
	}

	ImGui::Text("Allow for some time to smooth out after generation - 1-2 seconds");

	if( ImGui::Button( "Smooth Terrain" ) ) 
	{
		m_Terrain->Smoothing(smoothCount);
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), timer);
	}

	if (ImGui::Button("Fault Terrain"))
	{
		m_Terrain->faultTerrain(faultCount);
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), timer);
	}

	ImGui::Checkbox("Circle Fault Toggle", m_Terrain->GetCircleFault());

	ImGui::SliderInt("Smooth Amount", &smoothCount, 1.0f, 5.0f);
	ImGui::SliderInt("Fault Amount", &faultCount, 1.0f, 500.0f);

	ImGui::Text("Toggle to then regenerate the terrain after a few seconds");

	ImGui::Checkbox("Re-Generation Toggle", m_Terrain->GetGen());

	ImGui::Text("Toggle to then regenerate with noise already on terrain");

	if (ImGui::Button("Perlin Noise"))
	{
		m_Terrain->PerNoise();
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), timer);
	}

	//Amp
	ImGui::SliderFloat("X Amplitude", m_Terrain->GetXAmp(), -5.0f, 5.0f);
	ImGui::SliderFloat("Z Amplitude", m_Terrain->GetZAmp(), -5.0f, 5.0f);

	//Frequency
	ImGui::SliderFloat("X Frequency", m_Terrain->GetXFreq(), -1.0f, 1.0f);
	ImGui::SliderFloat("Z Frequency", m_Terrain->GetZFreq(), -1.0f, 1.0f);

	ImGui::Text("  ");

	ImGui::Text("Water Options");
	
	ImGui::Text("Wave Settings");

	ImGui::SliderFloat("Wave Positional Height", &waveHeight, -5, 20);
	ImGui::SliderFloat("Wave Amplitude", &waveAmp, -1, 1);
	ImGui::SliderFloat("Wave Speed", &waveSpeed, -5,10);
	ImGui::SliderFloat("Steepness", &waveSteep, -1, 1);

	ImGui::SliderFloat("Wave X Frequency", &waveXFreq, -1.0f, 1.0f);
	ImGui::SliderFloat("Wave Y Frequency", &waveYFreq, -1.0f, 1.0f);
	ImGui::SliderFloat("Wave Z Frequency", &waveZFreq, -1.0f, 1.0f);

	ImGui::Text("Transparency of water");
	ImGui::SliderFloat("Blend Amount", &blendAmount, -1.0f, 1.0f);

	ImGui::Text("  ");

	//Lights
	ImGui::Text("Light Settings - To turn off a light put all colours to 0 -");

	ImGui::Text("Light 1 Settings");

	//Light 1 Settings
	ImGui::SliderFloat3("Light 1 Position", light1Pos, -50.0f, 50.0f);
	ImGui::SliderFloat3("Light 1 Diffuse", light1Diffuse, 0.0f, 1.0f);
	ImGui::Text("     ");

	ImGui::Text("Light 2 Settings");

	//Light 2 Settings
	ImGui::SliderFloat3("Light 2 Position", light2Pos, -50.0f, 50.0f);
	ImGui::SliderFloat3("Light 2 Diffuse", light2Diffuse, 0.0f, 1.0f);
	ImGui::Text("     ");

	ImGui::Text("Light 3 Settings");

	//Light 3 Settings
	ImGui::SliderFloat3("Light 3 Position", light3Pos, -50.0f, 50.0f);
	ImGui::SliderFloat3("Light 3 Diffuse", light3Diffuse, 0.0f, 1.0f);
	ImGui::Text("     ");

	ImGui::Text("Light 4 Settings");

	//Light 4 Settings
	ImGui::SliderFloat3("Light 4 Position", light4Pos, -50.0f, 50.0f);
	ImGui::SliderFloat3("Light 4 Diffuse", light4Diffuse, 0.0f, 1.0f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

