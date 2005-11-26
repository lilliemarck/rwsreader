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
#include "Core.h"
#include "Texture.h"

#define TEXTURE_FILTER_MODE_MASK	0x000000ff
#define TEXTURE_ADDRESS_MODE_U_MASK 0x00000f00
#define TEXTURE_ADDRESS_MODE_V_MASK 0x0000f000

rw::Texture::Texture()
{
	memset(this, 0, sizeof(Texture));
}


rw::Texture::~Texture()
{
	SAFE_DELETE(mMaskName);
	SAFE_DELETE(mName);
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
	mFilterMode		= static_cast<FilterMode>(temp & TEXTURE_FILTER_MODE_MASK);
	mAddressModeU	= static_cast<AddressMode>(temp & TEXTURE_ADDRESS_MODE_U_MASK >> 8);
	mAddressModeV	= static_cast<AddressMode>(temp & TEXTURE_ADDRESS_MODE_V_MASK >> 12);

	// Texture name
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRING) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	mName = new char[chunkHeaderInfo.length];
	stream.read(mName, chunkHeaderInfo.length);

	// Texture mask name
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_STRING) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	mMaskName = new char[chunkHeaderInfo.length];
	stream.read(mMaskName, chunkHeaderInfo.length);

	// Extension chunk
	stream.read(&chunkHeaderInfo);
	if (chunkHeaderInfo.type != ID_EXTENSION) {
		std::cerr << "Unknown format of Texture";
		return;
	}
	stream.skip(chunkHeaderInfo.length);
}