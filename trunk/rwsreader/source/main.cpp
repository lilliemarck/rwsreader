#include <assert.h>
#include <iostream>
#include "Clump.h"
#include "Stream.h"

int main(int argc, const char *argv[])
{
	rw::Stream stream;
    stream.open(argv[1]);

	rw::ChunkHeaderInfo chunkHeaderInfo;
	stream.read(&chunkHeaderInfo);

	if (chunkHeaderInfo.type == rw::ID_CLUMP)
    {
		rw::Clump clump;
		clump.read(stream);

		int pos = stream.mFile.tellg();
		stream.mFile.seekg(0, std::ios::end);
		int end = stream.mFile.tellg();
		assert(pos == end);
	}

	return 0;
}
