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

#include "Stream.h"

bool rw::Stream::open(const char *filename)
{
	m_file.open(filename, std::ios::in | std::ios::binary);
	return m_file != 0;
}


void rw::Stream::close()
{
    m_file.close();
}


bool rw::Stream::findChunk(ObjectID type)
{
    ChunkHeaderInfo chunkHeaderInfo;
    
    while (read(&chunkHeaderInfo))
    {
        if (chunkHeaderInfo.type == type)
            return true;
        skip(chunkHeaderInfo.length);
    }
    return false;
}


bool rw::Stream::read(ChunkHeaderInfo *chunkHeaderInfo)
{
	m_file.read(reinterpret_cast<char *>(chunkHeaderInfo), sizeof(ChunkHeaderInfo));
    return m_file.good();
}


void rw::Stream::read(short *ints, unsigned int numBytes)
{
	m_file.read(reinterpret_cast<char *>(ints), numBytes);
}


void rw::Stream::read(int *ints, unsigned int numBytes)
{
	m_file.read(reinterpret_cast<char *>(ints), numBytes);
}


void rw::Stream::read(float *floats, unsigned int numBytes)
{
	m_file.read(reinterpret_cast<char *>(floats), numBytes);
}


void rw::Stream::read(void *buffer, unsigned int length)
{
	m_file.read(static_cast<char *>(buffer), length);
}


void rw::Stream::skip(unsigned int offset)
{
	m_file.seekg(offset, std::ios::cur);
}
