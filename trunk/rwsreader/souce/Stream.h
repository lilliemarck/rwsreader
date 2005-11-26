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

#ifndef RW_STREAM_H
#define RW_STREAM_H

#include <fstream>

namespace rw
{
	enum ObjectID
	{
		// Core
		ID_NA_OBJECT			= 0x00,
		ID_STRUCT				= 0x01,
		ID_STRING				= 0x02,
		ID_EXTENSION			= 0x03,
		ID_TEXTURE				= 0x06,
		ID_MATERIAL				= 0x07,
		ID_MATLIST				= 0x08,
		ID_FRAMELIST			= 0x0E,
		ID_GEOMETRY				= 0x0F,
		ID_CLUMP				= 0x10,
		ID_ATOMIC				= 0x14,
		ID_GEOMETRYLIST			= 0x1A,

		// Criterion Toolkit
		rwID_HANIMPLUGIN		= 0x011e,
	};

	struct ChunkHeaderInfo
	{
		ObjectID		type;			// Plugin ID
		unsigned int	length;			// Length of data
		unsigned short	buildNumber;
		unsigned short	version;
	};

	/**
	* Stream is the interface to a RenderWare streams.
	* When inmplemented correctly it should do endian conversions.
	*/
	class Stream
	{
	public:
		bool open(const char *filename);
		void read(ChunkHeaderInfo *chunkHeaderInfo);
		void read(short *ints, unsigned int numBytes = sizeof(short));
		void read(int *ints, unsigned int numBytes = sizeof(int));
		void read(float *floats, unsigned int numBytes = sizeof(float));
		void read(void *buffer, unsigned int length);
		void skip(unsigned int offset);

		std::ifstream mFile;
	};
}

#endif // RW_STREAM_H
