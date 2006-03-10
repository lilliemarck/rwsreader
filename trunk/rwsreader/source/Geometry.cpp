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

#include <iostream>
#include "Geometry.h"


rw::MorphTarget::MorphTarget()
{
	MemZero(this);
}


rw::MorphInterpolator::MorphInterpolator()
{
	MemZero(this);
}


rw::Geometry::Geometry()
{
	MemZero(this);
}


rw::Geometry::~Geometry()
{
	delete [] m_skinToBoneMatrices;
	delete [] m_vertexBoneWeights;
	delete [] m_vertexBoneIndices;
	delete [] m_morphInterpolators;

	for (int i=0; i<m_numMorphTargets; i++)
	{
		delete [] m_morphTargets[i].normals;
		delete [] m_morphTargets[i].vertices;
	}

	delete [] m_morphTargets;
	delete [] m_triangles;

	for (int i=0; i<m_numTexCoordSets; i++)
		delete [] m_texCoords[i];

	delete [] m_preLitLum;
}


void rw::Geometry::read(Stream &stream)
{
	// Struct chunk
	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Geometry";
		return;
	}

	// Attributes
	stream.read(reinterpret_cast<int*>(&m_flags));
	stream.read(&m_numTriangles);
	stream.read(&m_numVertices);
	stream.read(&m_numMorphTargets);
	m_numTexCoordSets = (m_flags & 0x00ff0000) >> 16;

	// Prelit colors
	if (m_flags & GF_PRELIT) {
		m_preLitLum = new Color[m_numVertices];
		stream.read(reinterpret_cast<float *>(m_preLitLum), sizeof(Color)*m_numVertices);
	}

	// Texture Coordinates, redundant if
	if ((m_flags & GF_TEXTURED) || (m_flags & GF_TEXTURED2)) {
		for (int i=0; i<m_numTexCoordSets; i++)
		{		
			m_texCoords[i] = new TexCoords[m_numVertices];
			stream.read(reinterpret_cast<float *>(m_texCoords[i]), sizeof(TexCoords)*m_numVertices);
		}
	}

	// Triangle information
	m_triangles = new Triangle[m_numTriangles];
	for (int i=0; i<m_numTriangles; i++) {
		Triangle &triangle = m_triangles[i];
		stream.read(reinterpret_cast<short *>(&triangle.vertIndex[1]));
		stream.read(reinterpret_cast<short *>(&triangle.vertIndex[0]));
		stream.read(reinterpret_cast<short *>(&triangle.matIndex));
		stream.read(reinterpret_cast<short *>(&triangle.vertIndex[2]));
	}

	// Morph targets
	m_morphTargets = new MorphTarget[m_numMorphTargets];
	for (int i=0; i<m_numMorphTargets; i++)
	{
		MorphTarget &morphTarget = m_morphTargets[i];

		int hasPositions;
		int hasNormals;
		stream.read(reinterpret_cast<float *>(&morphTarget.boundingSphere), sizeof(Sphere));
		stream.read(&hasPositions);
		stream.read(&hasNormals);

		if (hasPositions) {
			morphTarget.vertices = new Vector[m_numVertices];
			stream.read(reinterpret_cast<float *>(morphTarget.vertices), sizeof(Vector)*m_numVertices);
		}

		if (hasNormals) {
			morphTarget.normals = new Vector[m_numVertices];
			stream.read(reinterpret_cast<float *>(morphTarget.normals), sizeof(Vector)*m_numVertices);
		}
	}

	// Material list chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_MATLIST) {
		std::cerr << "Unknown format of Geometry";
		return;
	}
	m_materialList.read(stream);

	// Extensions
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of Geometry";
		return;
	}
	int length = chunkHeaderInfo.length;
	while (length > 0)
	{
		stream.read(&chunkHeaderInfo);
		length -= 12 + chunkHeaderInfo.length;

		switch (chunkHeaderInfo.type)
		{
		case ID_MORPHPLUGIN:
			std::cout << "Morph plugin" << std::endl;
			stream.read(&m_numMorphInterpolators);
			m_morphInterpolators = new MorphInterpolator[m_numMorphInterpolators];
			for (int i=0; i<m_numMorphInterpolators; i++) {
				int pos = stream.m_file.tellg();
				int flags; // unused
				stream.read(&flags);
				stream.read(&m_morphInterpolators[i].startMorphTarget);
				stream.read(&m_morphInterpolators[i].endMorphTarget);
				stream.read(&m_morphInterpolators[i].time);
				stream.read(&m_morphInterpolators[i].next);
			}
			break;

		case ID_SKINPLUGIN:
			{
				std::cout << "Skin plugin";
				unsigned int header;
				stream.read((int*)&header);
				m_numBones = header & 0x000000FF;

				// Some unknown data
				unsigned int numSomething = (header & 0x0000FF00) >> 8;
				for (unsigned int i=0; i < numSomething; i++) {
					unsigned char unknown;
					stream.read((void*)&unknown, 1);
				}

				// Vertex indices
				m_vertexBoneIndices = new unsigned int[m_numVertices];
				for (int i=0; i < m_numVertices; i++)
					stream.read((int*)&m_vertexBoneIndices[i]);

				// Vertex weights
				m_vertexBoneWeights = new MatrixWeights[m_numVertices];
				for (int i=0; i < m_numVertices; i++) {
					for (int j=0; j<4; j++) 
						stream.read(&m_vertexBoneWeights[i].weights[j]);
				}

				// Bone matrices, Note that they have to be post processed in the app.
				m_skinToBoneMatrices = new Matrix44[m_numVertices];
				for (int i=0; i < m_numBones; i++) {
					for (int j=0; j<16; j++)
						stream.read(&m_skinToBoneMatrices[i].m[j]);
				}

				// Always 12 zero bytes at end...
				stream.skip(12);
			}
			break;

		case ID_USERDATAPLUGIN:
			std::cout << "UserData plugin";
			stream.skip(chunkHeaderInfo.length);
			break;

		case ID_BINMESHPLUGIN:
			std::cout << "Bin mesh plugin";
			stream.skip(chunkHeaderInfo.length);
			break;

		default:
			std::cout << "Unknown geometry extension " << chunkHeaderInfo.type;
			stream.skip(chunkHeaderInfo.length);
			break;
		}
	}
}


rw::GeometryList::GeometryList()
{
	MemZero(this);
}


rw::GeometryList::~GeometryList()
{
	delete [] m_geometries;
}


void rw::GeometryList::read(Stream &stream)
{
	// Struct
	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Geometry";
		return;
	}

	// Attributes
	stream.read(&m_numGeometries);

	m_geometries = new Geometry[m_numGeometries];
	for (int i=0; i < m_numGeometries; i++)
	{
		// Geometry
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_GEOMETRY) {
			std::cerr << "Unknown format of Clump";
			return;
		}
		m_geometries[i].read(stream);
	}

	// No extensions
}
