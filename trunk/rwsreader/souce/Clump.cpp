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
#include "Clump.h"

rw::Atomic::Atomic()
{
	memset(this, 0, sizeof(Atomic));
}


void rw::Atomic::read(Stream &stream)
{
	ChunkHeaderInfo chunkHeaderInfo;

	// Struct chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Atomic";
		return;
	}

	int unknown;
	stream.read(&mFrame);
	stream.read(&mGeometry);
	stream.read(&unknown);
	assert(unknown == 5 && "Unknown was value different than ususal");
	stream.read(&unknown);
	assert(unknown == 0 && "Unknown was value different than ususal");

	// Extension chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of Atomic";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}


rw::Clump::Clump()
{
	memset(this, 0, sizeof(Clump));
}


void rw::Clump::read(Stream &stream)
{
	// Struct
	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Clump";
		return;
	}

	// Atrributes
	stream.read(&mNumAtomics);
	stream.read(&mNumLights);
	stream.read(&mNumCameras);

	// FrameList
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_FRAMELIST) {
		std::cerr << "Unknown format of Clump";
		return;
	}
	mFrameList.read(stream);

	// GeometryList	
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_GEOMETRYLIST) {
		std::cerr << "Unknown format of Clump";
		return;
	}
	mGeometryList.read(stream);

	// Atomics
	mAtomics = new Atomic[mNumAtomics];
	for (int i=0; i < mNumAtomics; ++i)
	{
		// Atomic chunk
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_ATOMIC) {
			std::cerr << "Unknown format of Clump";
			return;
		}

		// Create new atomic
		mAtomics[i].read(stream);
	}

	// Extension
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of FrameList";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}


rw::Clump::~Clump()
{
	SAFE_DELETE_ARRAY(mAtomics);
}
