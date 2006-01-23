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
#include "Core.h"
#include "Texture.h"

#define RW_TEXTURE_FILTER_MODE_MASK		0x000000ff
#define RW_TEXTURE_ADDRESS_MODE_U_MASK	0x00000f00
#define RW_TEXTURE_ADDRESS_MODE_V_MASK	0x0000f000

rw::Texture::Texture()
{
	MemZero(this);
}


rw::Texture::~Texture()
{
	delete [] m_maskName;
	delete [] m_name;
}


void rw::Texture::read(Stream &stream)
{
	int		temp;

	// Struct
   	ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRUCT) {
		std::cerr << "Unknown format of Texture";
		return;
	}

	// Texture modes
	stream.read(&temp);
	m_filterMode	= static_cast<FilterMode>(temp & RW_TEXTURE_FILTER_MODE_MASK);
	m_addressModeU	= static_cast<AddressMode>(temp & RW_TEXTURE_ADDRESS_MODE_U_MASK >> 8);
	m_addressModeV	= static_cast<AddressMode>(temp & RW_TEXTURE_ADDRESS_MODE_V_MASK >> 12);

	// Texture name
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRING) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	m_name = new char[chunkHeaderInfo.length];
	stream.read(m_name, chunkHeaderInfo.length);

	// Texture mask name
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRING) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	m_maskName = new char[chunkHeaderInfo.length];
	stream.read(m_maskName, chunkHeaderInfo.length);

	// Extension chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}
