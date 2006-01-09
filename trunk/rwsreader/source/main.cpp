#include <assert.h>
#include <iostream>
#include "Animation.h"
#include "Clump.h"
#include "Stream.h"

int main(int argc, const char *argv[])
{
	// Open a RenderWare stream
	rw::Stream stream;
    stream.open(argv[1]);

	// Read the header
	rw::ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);
	
	// Usually you want to find the object
	// for example
	// stream.findChunk(rw::ID_CLUMP);
	// and then then
	// rw::Clump clump;
	// clump.read(stream);

	if (chunkHeaderInfo.type == rw::ID_CLUMP)
    {
		rw::Clump clump;
		clump.read(stream);

		// Don't need this check
		int pos = stream.m_file.tellg();
		stream.m_file.seekg(0, std::ios::end);
		int end = stream.m_file.tellg();
		assert(pos == end);
	}
	else if (chunkHeaderInfo.type == rw::ID_HANIMANIMATION)
	{
		rw::Animation clump;
		clump.read(stream);

		// Don't need this check
		int pos = stream.m_file.tellg();
		stream.m_file.seekg(0, std::ios::end);
		int end = stream.m_file.tellg();
		assert(pos == end);
	}

	return 0;
}
