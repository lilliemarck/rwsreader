/* Copyright (c) 2005-2006  Jonathan Lilliemarck Jansson
*
* This software is provided 'as-is', without any express or implied warranty.
* In no event will the authors be held liable for any damages arising from
* the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software in
*    a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <assert.h>
#include <iostream>
#include "World.h"

rw::Sector::Sector()
{
	MemZero(this);
}


rw::Sector::~Sector()
{
	delete [] m_triangles;
	delete [] m_texCoords;
	delete [] m_normals;
	delete [] m_vertices;
}


void rw::Sector::read(Stream &stream, bool textured)
{
	// Struct chunk
	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Sector";
		return;
	}

	// Attributes
	int unknown;
	stream.read(&unknown);
	assert(unknown == 0);
	stream.read(&m_numTriangles);
	stream.read(&m_numVertices);
	stream.read((float*)&m_boundingBox.lower, sizeof(Vector));
	stream.read((float*)&m_boundingBox.upper, sizeof(Vector));
	stream.read(&unknown); // Weird value
	stream.read(&unknown);
	assert(unknown == 0);

	// Vertices
	m_vertices = new Vector[m_numVertices];
	stream.read((float*)m_vertices, sizeof(Vector) * m_numVertices);

	// Normals
	m_normals = new Normal[m_numVertices];
	stream.read((void*)m_normals, sizeof(Normal) * m_numVertices);

	// Texture coordinates
	if (textured) {
		m_texCoords = new TexCoords[m_numVertices];
		stream.read((float*)m_texCoords, sizeof(TexCoords) * m_numVertices);
	}

	// Triangles
	m_triangles = new Triangle[m_numTriangles];
	for (int i=0; i<m_numTriangles; i++) {
		Triangle &triangle = m_triangles[i];
		stream.read((short*)&triangle.vertIndex[0]);
		stream.read((short*)&triangle.vertIndex[1]);
		stream.read((short*)&triangle.vertIndex[2]);
		stream.read((short*)(&triangle.matIndex));
		
		assert(triangle.vertIndex[0] < m_numVertices);
		assert(triangle.vertIndex[1] < m_numVertices);
		assert(triangle.vertIndex[2] < m_numVertices);
	}

	// Extension
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of World";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}


rw::World::World()
{
	MemZero(this);
}


rw::World::~World()
{
}


void rw::World::read(Stream &stream)
{
	// Struct chunk
	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of World";
		return;
	}

	// Attributes
	int unknown;
	stream.read(&unknown);
	assert(unknown == 1);
	stream.read((float*)&m_origin, sizeof(Vector));
	m_origin.x = -m_origin.x;
	m_origin.y = -m_origin.y;
	m_origin.z = -m_origin.z;
	stream.read(&m_numTriangles);
	stream.read(&m_numVertices);
	stream.read(&unknown);
	assert(unknown == 0);
	stream.read(&m_numSectors);
	stream.read(&unknown);
	assert(unknown == 0);
	stream.read((int*)&m_flags);
	stream.read((float*)&m_boundingBox.upper, sizeof(Vector));
	stream.read((float*)&m_boundingBox.lower, sizeof(Vector));

	// Material list
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_MATLIST) {
		std::cerr << "Unknown format of World";
		return;
	}
	m_materialList.read(stream);

	m_sectors = new Sector[m_numSectors];
	for (int i=0; i<m_numSectors; i++)
	{
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_ATOMICSECT) {
			std::cerr << "Unknown format of World";
			return;
		}
		m_sectors[i].read(stream, m_flags & F_TEXTURED);
	}

	// Extension
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of World";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}
