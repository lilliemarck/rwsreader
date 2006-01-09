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
#include "Core.h"
#include "Animation.h"

rw::Animation::Animation()
{
	MemZero(this);
}


rw::Animation::~Animation()
{
	delete [] m_frames;
}


void rw::Animation::read(Stream &stream)
{
	// Read info
	int unknown;
	stream.read(&unknown); // Maybe this is the criterion toolkit identifier, not important
	assert(unknown == 0x00000100);
	stream.read(&unknown); // Probably HAnim's type id which is 1, should check and return if not 1
	assert(unknown == 0x00000001);
	stream.read(&m_numFrames);
	stream.read(&unknown); // Flags?
	stream.read(&m_duration);

	// Read keyframes
	m_frames = new KeyFrame[m_numFrames];
	for (int i=0; i<m_numFrames; i++)
	{
		rw::KeyFrame &frame = m_frames[i];
		stream.read(&frame.time);
		stream.read(reinterpret_cast<float *>(&frame.orientation), sizeof(Quat));
		stream.read(reinterpret_cast<float *>(&frame.translation), sizeof(Vector));
		stream.read(&frame.prevFrame);
	}
}
