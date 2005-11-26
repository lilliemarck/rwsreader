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

#include "Stream.h"

bool rw::Stream::open(const char *filename)
{
	mFile.open(filename, std::ios::in | std::ios::binary);
	return mFile != 0;
}


void rw::Stream::read(ChunkHeaderInfo *chunkHeaderInfo)
{
	mFile.read(reinterpret_cast<char *>(chunkHeaderInfo), sizeof(ChunkHeaderInfo));
}


void rw::Stream::read(short *ints, unsigned int numBytes)
{
	mFile.read(reinterpret_cast<char *>(ints), numBytes);
}


void rw::Stream::read(int *ints, unsigned int numBytes)
{
	mFile.read(reinterpret_cast<char *>(ints), numBytes);
}


void rw::Stream::read(float *floats, unsigned int numBytes)
{
	mFile.read(reinterpret_cast<char *>(floats), numBytes);
}


void rw::Stream::read(void *buffer, unsigned int length)
{
	mFile.read(static_cast<char *>(buffer), length);
}


void rw::Stream::skip(unsigned int offset)
{
	mFile.seekg(offset, std::ios::cur);
}
