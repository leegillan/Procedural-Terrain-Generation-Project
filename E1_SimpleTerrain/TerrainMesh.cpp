#include "TerrainMesh.h"

TerrainMesh::TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, Timer* timer, int lresolution) :
	PlaneMesh( device, deviceContext, lresolution ) 
{
	Resize( resolution );
	Regenerate( device, deviceContext, timer);
}

//Cleanup the heightMap
TerrainMesh::~TerrainMesh() 
{
	delete[] heightMap;
	heightMap = 0;
}

//Fill an array of floats that represent the height values at each grid point.
//Here we are producing a Sine wave along the X-axis
void TerrainMesh::BuildHeightMap(Timer* timer) 
{
	float height = 0.0f;
	
	//Scale everything so that the look is consistent across terrain resolutions
	const float scale =  terrainSize / (float)resolution;

	//TODO: Give some meaning to these magic numbers! What effect does changing them have on terrain?
	for (int j = 0; j < (resolution); j++)
	{
		for (int i = 0; i < (resolution); i++)
		{
			height = (sin((float)i * xF * scale)) * xA;
			height += (cos((float)j * zF * scale)) * zA;
			height -= (sin((float)i * zF + xA * scale)) * zA;
			height += (-cos((float)j * xA + scale)) * xA;

			if (start == false || generation == true)
			{
				heightMap[(j * resolution) + i] = height;
			}
			else
			{
				heightMap[(j * resolution) + i] = lerp(0.5 * timer->getTime(), heightMap[(j * resolution) + i], height);
			}
		}
	}

	start = true;

	if (pNoise == true)
	{
		PerNoise();
	}

	generation = false;
}

//Run PerlinNoise on the terrain
void TerrainMesh::PerNoise()
{
	float height = 0.0f;

	//Scale everything so that the look is consistent across terrain resolutions
	const float scale = terrainSize / (float)resolution;

	for (int j = 0; j < (resolution); j++)
	{
		for (int i = 0; i < (resolution); i++)
		{
			height = perlinNoise->noise(j, heightMap[(j * resolution) + i], i);

			heightMap[(j * resolution) + i] += height;
		}
	}
}


//Faulting algorithm found after a bit of researching
//http://www.lighthouse3d.com/opengl/terrain/index.php?fault
//Faulting algorithm

void TerrainMesh::faultTerrain(int faults)
{
	int maxH = 50;
	int minH = -10;

	float difference = 1.0f;

	for (int t = 0; t < faults; t++) 
	{
		//Checks if circular faulting is toggles or not
		if (!circleFault)
		{
			//Assigns random values to be used in calculations
			float v = (float)rand();
			float a = sin(v);
			float b = cos(v);

			float d = sqrt((resolution * resolution) + (resolution * resolution));
			  
			//finds random number between -d/2 and d/2
 			float c = ((float)rand() / RAND_MAX) * d - d / 2.0f; 

 			for (int j = 0; j < resolution; j++)
			{
				for (int i = 0; i < resolution; i++)
				{
					// if on one side of the line equation
					if (a * j + b * i - c > 0) 
					{
						heightMap[(j * resolution) + i] += (heightMap[(j * resolution) + i] + difference > maxH) ? 0 : difference;
					}
					else
					{
						heightMap[(j * resolution) + i] -= (heightMap[(j * resolution) + i] + difference < minH) ? 0 : difference;

					}
				}
			}

			//Smooths out the terrain after n amount of faults
			count++;

			if (count >= 100)
			{
				Smoothing(1);
				count = 0;
			}
		}
		else 
		{	
			//Gets random circle location
			float randX = rand() % (resolution + 1);
			float randZ = rand() % (resolution + 1);
			
			//Circle size
			float randCircleSize = (float)(25.0 * (rand() / (RAND_MAX + 1.0))); 

			//Loops through the terrain
			for (int j = 0; j < resolution; j++) 
			{
				for (int i = 0; i < resolution; i++) 
				{
					// pd = distanceFromCircle*2/size - from lighthouse3D
					float pointDisplaced = sqrt((randX - i) * (randX - i) + (randZ - j) * (randZ - j)) * 2.0f / randCircleSize; 

					//fabs returns absolute value to make it positive
					//if the vertex is within the circle, displace it upwards
					if (fabs(pointDisplaced) <= 1.0f)
					{
						//gets 
						float displacement = (0.5f / 2.0f + sin(pointDisplaced * 3.14f) * 0.5f / 2.0f);

						// displaces the terrain as long as the heightMap + displacement is not greater than 0
						//else it adds displacement onto the heightmap
						heightMap[(j * resolution) + i] += (heightMap[(j * resolution) + i] + displacement > maxH) ? 0 : displacement;
			
					}
				}
			}

			count++;
		}
	}
}

void TerrainMesh::Smoothing(int times)
{
	int count = 0;
	float height;

	while (count < times)
	{
		for (int j = 0; j < resolution; j++)
		{
			for (int i = 0; i < resolution; i++)
			{
				int adjacentTiles = 0;
				float tilesHeightTotal = 0.0f;

				//check below
				if (((j * resolution) - 1) > 0)
				{
					tilesHeightTotal = heightMap[((j * resolution) - 1) + i];
					adjacentTiles++;

					//checks below and to the left
					if ((i - 1) > 0)
					{
						tilesHeightTotal = heightMap[((j * resolution) - 1) + (i - 1)];
						adjacentTiles++;
					}

					//checks below and to the right
					if ((i + 1) < resolution)
					{
						tilesHeightTotal = heightMap[((j * resolution) - 1) + (i + 1)];
						adjacentTiles++;
					}
				}

				//check above
				if (((j * resolution) + 1) < resolution)
				{
					tilesHeightTotal = heightMap[((j * resolution) + 1) + i];
					adjacentTiles++;

					//checks up and to the left
					if ((i - 1) > 0)
					{
						tilesHeightTotal = heightMap[((j * resolution) - 1) + (i - 1)];
						adjacentTiles++;
					}

					//checks up and to the right
					if ((i + 1) < resolution)
					{
						tilesHeightTotal = heightMap[((j * resolution) - 1) + (i + 1)];
						adjacentTiles++;
					}
				}

				if ((i - 1) > 0) // Check left
				{
					tilesHeightTotal += heightMap[(j * resolution) + (i - 1)];
					adjacentTiles++;
				}

				if ((i + 1) < resolution) // Check right
				{
					tilesHeightTotal += heightMap[(j * resolution) + (i + 1)];
					adjacentTiles++;
				}

				height = heightMap[(j * resolution) + i] + (tilesHeightTotal / adjacentTiles);

				//sets new smoothed values to the heightMap
				heightMap[(j * resolution) + i] = height * 0.56;
			}
		}

		count++;
	}

}

void TerrainMesh::Resize( int newResolution ) {
	resolution = newResolution;
	heightMap = new float[resolution * resolution];
	if( vertexBuffer != NULL ) {
		vertexBuffer->Release();
	}
	vertexBuffer = NULL;
}

// Set up the heightmap and create or update the appropriate buffers
void TerrainMesh::Regenerate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Timer* timer) 
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, height, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Calculate and store the height values
	if (generation == true)
	{
		BuildHeightMap(timer);
	}

	// Calculate the number of vertices in the terrain mesh.
	// We share vertices in this mesh, so the vertex count is simply the terrain 'resolution'
	// and the index count is the number of resulting triangles * 3 OR the number of quads * 6
	vertexCount = resolution * resolution;

	indexCount = ( ( resolution - 1 ) * ( resolution - 1 ) ) * 6;
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	index = 0;

	// UV coords.
	u = 0;
	v = 0;
	increment = m_UVscale / resolution;

	//Scale everything so that the look is consistent across terrain resolutions
	const float scale = terrainSize / (float)resolution;

	//Set up vertices
	for( j = 0; j < ( resolution ); j++ ) 
	{
		for( i = 0; i < ( resolution ); i++ ) 
		{
			positionX = (float)i * scale;
			positionZ = (float)( j ) * scale;

			height = heightMap[index];

			vertices[index].position = XMFLOAT3(positionX, height, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);

			u += increment;
			index++;
		}

		u = 0;
		v += increment;
	}

	//Set up index list
	index = 0;
	for( j = 0; j < ( resolution - 1 ); j++ ) 
	{
		for( i = 0; i < ( resolution - 1 ); i++ ) 
		{
			//Build index array
			indices[index] = (j*resolution) + i;
			indices[index + 1] = ( ( j + 1 ) * resolution ) + ( i + 1 );
			indices[index + 2] = ( ( j + 1 ) * resolution ) + i;

			indices[index + 3] = ( j * resolution ) + i;
			indices[index + 4] = ( j * resolution ) + ( i + 1 );
			indices[index + 5] = ( ( j + 1 ) * resolution ) + ( i + 1 );
			index += 6;
		}
	}

	//Set up normals
	for( j = 0; j < ( resolution - 1 ); j++ ) 
	{
		for( i = 0; i < ( resolution - 1 ); i++ ) 
		{
			//Calculate the plane normals
			XMFLOAT3 a, b, c;	//Three corner vertices
			a = vertices[j * resolution + i].position;
			b = vertices[j * resolution + i + 1].position;
			c = vertices[( j + 1 ) * resolution + i].position;

			//Two edges
			XMFLOAT3 ab( c.x - a.x, c.y - a.y, c.z - a.z );
			XMFLOAT3 ac( b.x - a.x, b.y - a.y, b.z - a.z );
			
			//Calculate the cross product
			XMFLOAT3 cross;
			cross.x = ab.y * ac.z - ab.z * ac.y;
			cross.y = ab.z * ac.x - ab.x * ac.z;
			cross.z = ab.x * ac.y - ab.y * ac.x;
			float mag = ( cross.x * cross.x ) + ( cross.y * cross.y ) + ( cross.z * cross.z );
			mag = sqrtf( mag );
			cross.x/= mag;
			cross.y /= mag;
			cross.z /= mag;
			vertices[j * resolution + i].normal = cross;
		}
	}

	//Smooth the normals by averaging the normals from the surrounding planes
	XMFLOAT3 smoothedNormal( 0, 1, 0 );
	for( j = 0; j < resolution; j++ ) {
		for( i = 0; i < resolution; i++ ) {
			smoothedNormal.x = 0;
			smoothedNormal.y = 0;
			smoothedNormal.z = 0;
			float count = 0;
			//Left planes
			if( ( i - 1 ) >= 0 ) {
				//Top planes
				if( ( j ) < ( resolution - 1 ) ) {
					smoothedNormal.x += vertices[j * resolution + ( i - 1 )].normal.x;
					smoothedNormal.y += vertices[j * resolution + ( i - 1 )].normal.y;
					smoothedNormal.z += vertices[j * resolution + ( i - 1 )].normal.z;
					count++;
				}
				//Bottom planes
				if( ( j - 1 ) >= 0 ) {
					smoothedNormal.x += vertices[( j - 1 ) * resolution + ( i - 1 )].normal.x;
					smoothedNormal.y += vertices[( j - 1 ) * resolution + ( i - 1 )].normal.y;
					smoothedNormal.z += vertices[( j - 1 ) * resolution + ( i - 1 )].normal.z;
					count++;
				}
			}
			//right planes
			if( ( i ) <( resolution - 1 ) ) {

				//Top planes
				if( ( j ) < ( resolution - 1 ) ) {
					smoothedNormal.x += vertices[j * resolution + i].normal.x;
					smoothedNormal.y += vertices[j * resolution + i].normal.y;
					smoothedNormal.z += vertices[j * resolution + i].normal.z;
					count++;
				}
				//Bottom planes
				if( ( j - 1 ) >= 0 ) {
					smoothedNormal.x += vertices[( j - 1 ) * resolution + i].normal.x;
					smoothedNormal.y += vertices[( j - 1 ) * resolution + i].normal.y;
					smoothedNormal.z += vertices[( j - 1 ) * resolution + i].normal.z;
					count++;
				}
			}
			smoothedNormal.x /= count;
			smoothedNormal.y /= count;
			smoothedNormal.z /= count;

			float mag = sqrt( (smoothedNormal.x * smoothedNormal.x) + (smoothedNormal.y * smoothedNormal.y) + ( smoothedNormal.z * smoothedNormal.z ) );
			smoothedNormal.x /= mag;
			smoothedNormal.y /= mag;
			smoothedNormal.z /= mag;

			vertices[j * resolution + i].normal = smoothedNormal;
		}
	}
	//If we've not yet created our dyanmic Vertex and Index buffers, do that now
	if( vertexBuffer == NULL ) {
		CreateBuffers( device, vertices, indices );
	}
	else {
		//If we've already made our buffers, update the information
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory( &mappedResource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

		//  Disable GPU access to the vertex buffer data.
		deviceContext->Map( vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
		//  Update the vertex buffer here.
		memcpy( mappedResource.pData, vertices, sizeof( VertexType ) * vertexCount );
		//  Reenable GPU access to the vertex buffer data.
		deviceContext->Unmap( vertexBuffer, 0 );
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

//Create the vertex and index buffers that will be passed along to the graphics card for rendering
//For CMP305, you don't need to worry so much about how or why yet, but notice the Vertex buffer is DYNAMIC here as we are changing the values often
void TerrainMesh::CreateBuffers( ID3D11Device* device, VertexType* vertices, unsigned long* indices ) {

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the dyanmic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof( VertexType ) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer( &vertexBufferDesc, &vertexData, &vertexBuffer );

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof( unsigned long ) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer( &indexBufferDesc, &indexData, &indexBuffer );
}