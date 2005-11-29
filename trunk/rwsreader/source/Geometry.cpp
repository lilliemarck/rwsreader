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

#include <iostream>
#include "Geometry.h"


rw::MorphTarget::MorphTarget()
{
	memset(this, 0, sizeof(MorphTarget));
}


rw::MorphInterpolator::MorphInterpolator()
{
	memset(this, 0, sizeof(MorphInterpolator));
}


rw::Geometry::Geometry(void)
{
	memset(this, 0, sizeof(Geometry));
}


rw::Geometry::~Geometry()
{
	SAFE_DELETE(mMorphInterpolators);

	for (int i=0; i<mNumMorphTargets; ++i)
	{
		SAFE_DELETE(mMorphTargets[i].normals);
		SAFE_DELETE(mMorphTargets[i].vertices);
	}
	SAFE_DELETE(mMorphTargets);

	SAFE_DELETE(mTriangles);

	for (int i=0; i<mNumTexCoordSets; ++i)
		SAFE_DELETE(mTexCoords[i]);

	SAFE_DELETE(mPreLitLum);
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
	stream.read(reinterpret_cast<int*>(&mFlags));
	stream.read(&mNumTriangles);
	stream.read(&mNumVertices);
	stream.read(&mNumMorphTargets);
	mNumTexCoordSets = (mFlags & 0x00ff0000) >> 16;

	// Prelit colors
	if (mFlags & GF_PRELIT) {
		mPreLitLum = new Color[mNumVertices];
		stream.read(reinterpret_cast<float *>(mPreLitLum), sizeof(Color)*mNumVertices);
	}

	// Texture Coordinates, redundant if
	if ((mFlags & GF_TEXTURED) || (mFlags & GF_TEXTURED2)) {
		for (int i=0; i<mNumTexCoordSets; ++i)
		{		
			mTexCoords[i] = new TexCoords[mNumVertices];
			stream.read(reinterpret_cast<float *>(mTexCoords[i]), sizeof(TexCoords)*mNumVertices);
		}
	}

	// Triangle information
	mTriangles = new Triangle[mNumTriangles];
	for (int i=0; i<mNumTriangles; ++i) {
		Triangle &triangle = mTriangles[i];
		stream.read(reinterpret_cast<short *>(&triangle.vertIndex[1]));
		stream.read(reinterpret_cast<short *>(&triangle.vertIndex[0]));
		stream.read(reinterpret_cast<short *>(&triangle.matIndex));
		stream.read(reinterpret_cast<short *>(&triangle.vertIndex[2]));
	}

	// Bounding sphere

	// Morph targets
	mMorphTargets = new MorphTarget[mNumMorphTargets];
	for (int i=0; i<mNumMorphTargets; ++i)
	{
		MorphTarget &morphTarget = mMorphTargets[i];

		int hasPositions;
		int hasNormals;
		stream.read(reinterpret_cast<float *>(&morphTarget.boundingSphere), sizeof(Sphere));
		stream.read(&hasPositions);
		stream.read(&hasNormals);

		if (hasPositions) {
			morphTarget.vertices = new Vector[mNumVertices];
			stream.read(reinterpret_cast<float *>(morphTarget.vertices), sizeof(Vector)*mNumVertices);
		}

		if (hasNormals) {
			morphTarget.normals = new Vector[mNumVertices];
			stream.read(reinterpret_cast<float *>(morphTarget.normals), sizeof(Vector)*mNumVertices);
		}
	}

	// Material list chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_MATLIST) {
		std::cerr << "Unknown format of Clump";
		return;
	}
	mMaterialList.read(stream);

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
				stream.read(&mNumMorphInterpolators);
				mMorphInterpolators = new MorphInterpolator[mNumMorphInterpolators];
				for (int i=0; i<mNumMorphInterpolators; ++i) {
					int pos = stream.mFile.tellg();
					int flags; // unused
					stream.read(&flags);
					stream.read(&mMorphInterpolators[i].startMorphTarget);
					stream.read(&mMorphInterpolators[i].endMorphTarget);
					stream.read(&mMorphInterpolators[i].time);
					stream.read(&mMorphInterpolators[i].next);
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
	memset(this, 0, sizeof(GeometryList));
}


rw::GeometryList::~GeometryList()
{
	SAFE_DELETE_ARRAY(mGeometries);
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
	stream.read(&mNumGeometries);

	mGeometries = new Geometry[mNumGeometries];
	for (int i=0; i < mNumGeometries; ++i)
	{
		// Geometry
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_GEOMETRY) {
			std::cerr << "Unknown format of Clump";
			return;
		}
		mGeometries[i].read(stream);
	}

	// No extensions
}
