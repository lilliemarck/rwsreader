/*
* Copyright (c) 2005, Jonathan Lilliemarck Jansson
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote products
*    derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <assert.h>
#include <iostream>
#include "Core.h"
#include "Material.h"

rw::Material::Material()
{
	memset(this, 0, sizeof(Material));
}


rw::Material::~Material()
{
	SAFE_DELETE(mTexture);
}


void rw::Material::read(Stream &stream)
{
	memset(this, 0, sizeof(Material));

	ChunkHeaderInfo chunkHeaderInfo;

	// Struct chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Material";
		return;
	}

	int unknown;
	int textured;
	stream.read(&unknown, sizeof(int));
	assert(unknown == 0 && "Unknown was value different than ususal");
	stream.read((void *)&mColor, sizeof(Color));
	stream.read(&unknown, sizeof(int));
	assert(unknown == 0xE28E70C && "Unknown was value different than ususal");
	stream.read(&textured, sizeof(int));
	stream.read(&mAmbient);
	stream.read(&mSpecular);
	stream.read(&mDiffuse);

	if (textured) {
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_TEXTURE) {
			std::cerr << "Should have found a Texture";
			return;
		}

		mTexture = new Texture;
		mTexture->read(stream);
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
	memset(this, 0, sizeof(MaterialList));
}


rw::MaterialList::~MaterialList()
{
	SAFE_DELETE_ARRAY(mMaterials);
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

	stream.read(&mNumMaterials);

	if (mNumMaterials > 0) {
		int unknown;
		stream.read(&unknown);
		assert(unknown == -1 && "Unknown was value different than ususal");

		mMaterials = new Material[mNumMaterials];

		for (int i=0; i<mNumMaterials; ++i)
		{
			// Material chunk
			stream.read(&chunkHeaderInfo);
			if (chunkHeaderInfo.type != ID_MATERIAL) {
				std::cerr << "Unknown format of Material";
				return;
			}

			// Create new material
			mMaterials[i].read(stream);
		}
	}
}
