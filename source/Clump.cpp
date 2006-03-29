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
#include "Clump.h"

rw::Atomic::Atomic()
{
	MemZero(this);
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
	stream.read(&m_frame);
	stream.read(&m_geometry);
	stream.read(&unknown);
	assert(unknown == 5 && "Unknown was value different than usual");
	stream.read(&unknown);
	assert(unknown == 0 && "Unknown was value different than usual");

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
	MemZero(this);
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
	stream.read(&m_numAtomics);
	stream.read(&m_numLights);
	stream.read(&m_numCameras);

	// FrameList
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_FRAMELIST) {
		std::cerr << "Unknown format of Clump";
		return;
	}
	m_frameList.read(stream);

	// GeometryList	
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_GEOMETRYLIST) {
		std::cerr << "Unknown format of Clump";
		return;
	}
	m_geometryList.read(stream);

	// Atomics
	m_atomics = new Atomic[m_numAtomics];
	for (int i=0; i < m_numAtomics; i++)
	{
		// Atomic chunk
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_ATOMIC) {
			std::cerr << "Unknown format of Clump";
			return;
		}

		// Create new atomic
		m_atomics[i].read(stream);
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
	delete [] m_atomics;
}
