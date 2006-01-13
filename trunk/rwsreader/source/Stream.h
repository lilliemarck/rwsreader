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

#ifndef RW_STREAM_H
#define RW_STREAM_H

#include <fstream>

namespace rw
{
	enum ObjectID
	{
		// Corerw
        ID_NA_OBJECT		= 0x00,
		ID_STRUCT			= 0x01,
		ID_STRING			= 0x02,
		ID_EXTENSION		= 0x03,
		ID_TEXTURE			= 0x06,
		ID_MATERIAL			= 0x07,
		ID_MATLIST			= 0x08,
		ID_FRAMELIST		= 0x0E,
		ID_GEOMETRY			= 0x0F,
		ID_CLUMP			= 0x10,
		ID_ATOMIC			= 0x14,
		ID_GEOMETRYLIST		= 0x1A,
		ID_HANIMANIMATION	= 0x1B,

		// Criterion Toolkit
		ID_MORPHPLUGIN		= 0x0105,
		ID_SKINPLUGIN		= 0x0116,
		ID_HANIMPLUGIN		= 0x011e,
		ID_USERDATAPLUGIN	= 0x011f,

		// Criterion World
		ID_BINMESHPLUGIN	= 0x050e
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
		bool open       (const char *filename);
        void close      ();
        bool findChunk  (ObjectID type);
		bool read       (ChunkHeaderInfo *chunkHeaderInfo);
		void read       (short *ints, unsigned int numBytes = sizeof(short));
		void read       (int *ints, unsigned int numBytes = sizeof(int));
		void read       (float *floats, unsigned int numBytes = sizeof(float));
		void read       (void *buffer, unsigned int length);
		void skip       (unsigned int offset);

		std::ifstream m_file;
	};
}

#endif // RW_STREAM_H
