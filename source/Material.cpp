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
#include "Core.h"
#include "Material.h"

rw::Material::Material()
{
	MemZero(this);
}


rw::Material::~Material()
{
	delete m_texture;
}


void rw::Material::read(Stream &stream)
{
	ChunkHeaderInfo chunkHeaderInfo;

	// Struct chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Material";
		return;
	}

	int unknown;
	int textured;
	stream.read(&unknown);
	assert(unknown == 0 && "Unknown was value different than usual");
	stream.read((void *)&m_color, sizeof(Color));
	stream.read(&unknown);
//	assert(unknown == 0xE28E70C && "Unknown was value different than usual");
	stream.read(&textured);
	stream.read(&m_ambient);
	stream.read(&m_specular);
	stream.read(&m_diffuse);

	if (textured) {
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_TEXTURE) {
			std::cerr << "Should have found a Texture";
			return;
		}

		m_texture = new Texture;
		m_texture->read(stream);
	}

	// Extension chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}


rw::MaterialList::MaterialList()
{
	MemZero(this);
}


rw::MaterialList::~MaterialList()
{
	delete [] m_materials;
}


void rw::MaterialList::read(Stream &stream)
{
	ChunkHeaderInfo chunkHeaderInfo;

	// Struct chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Material";
		return;
	}

	stream.read(&m_numMaterials);

	if (m_numMaterials > 0) {
		for (int i=0; i<m_numMaterials; i++)
		{
			int unknown;
			stream.read(&unknown);
			assert(unknown == -1 && "Unknown was value different than usual");
		}

		m_materials = new Material[m_numMaterials];
		for (int i=0; i<m_numMaterials; i++)
		{
			// Material chunk
			stream.read(&chunkHeaderInfo);
			if (chunkHeaderInfo.type != ID_MATERIAL) {
				std::cerr << "Unknown format of Material";
				return;
			}

			// Create new material
			m_materials[i].read(stream);
		}
	}
}
