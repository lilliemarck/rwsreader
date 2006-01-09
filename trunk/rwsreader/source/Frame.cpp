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
#include "Frame.h"

rw::HAnimHierarchy::HAnimHierarchy()
{
	MemZero(this);
}

rw::HAnimHierarchy::~HAnimHierarchy()
{
	delete [] m_nodeInfo;
}


void rw::HAnimHierarchy::read(Stream &stream)
{
	int unknown;
	stream.read(&unknown);
	assert(unknown == 0x00000100);					// Always this?
	stream.read(&unknown);

	stream.read(&m_numNodes);
	stream.read(&unknown);
	assert(unknown == 0);
	stream.read(&unknown); // Is 36 when 6 nodes

	m_nodeInfo = new HAnimNodeInfo[m_numNodes];
	for (int i = 0; i < m_numNodes; i++)
	{
		stream.read(&m_nodeInfo->m_nodeID);		// Same as NodeIndex if not changed by artist
		stream.read(&m_nodeInfo->m_nodeIndex);
		stream.read(reinterpret_cast<int*>(&m_nodeInfo->m_flags));
	}
}


rw::Frame::Frame()
{
	MemZero(this);
}


rw::Frame::~Frame()
{
	delete m_HAnimHierarchy;
}


rw::FrameList::FrameList()
{
	MemZero(this);
}


rw::FrameList::~FrameList()
{
	delete [] m_frames;
}


void rw::FrameList::read(Stream &stream)
{
	// Struct
	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of FrameList";
		return;
	}

	// Attributes
	stream.read(&m_numFrames);
	m_frames = new Frame[m_numFrames];
	for (int i=0; i<m_numFrames; i++) {
		int unknown;
		m_frames[i].m_matrix.read(stream);
		stream.read(&m_frames[i].m_parent);
		stream.read(&unknown);
		assert(unknown == 0 && "Unknown was value different than usual");
	}

	// Extensions, one for each Frame
	for (int i=0; i<m_numFrames; i++)
	{
		stream.read(&chunkHeaderInfo);
		if (chunkHeaderInfo.type != ID_EXTENSION) {
			std::cerr << "Unknown format of FrameList";
			return;
		}

		if (chunkHeaderInfo.length > 0)
		{
			stream.read(&chunkHeaderInfo);
			// Do we know this plugin?
			if (chunkHeaderInfo.type == ID_HANIMPLUGIN)
			{
				if (chunkHeaderInfo.length > 12) {
					m_frames[i].m_HAnimHierarchy = new HAnimHierarchy;
					m_frames[i].m_HAnimHierarchy->read(stream);
					m_frames[i].m_id = m_frames[i].m_HAnimHierarchy->m_numNodes;
				}
				else {
					int unknown;
					stream.read(&unknown);
					stream.read(&m_frames[i].m_id);
					stream.read(&unknown);
				}
			}
			// If not, then skip it
			else {
				stream.skip(chunkHeaderInfo.length);
			}

		}
	}
}
