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

#ifndef RW_ANIMATION_H
#define RW_ANIMATION_H

#include "Core.h"
#include "Stream.h"

namespace rw
{
	struct HAnimKeyFrame
	{
		float	time;			/**< Time of keyframe				*/
		Quat	orientation;	/**< Orientation					*/
		Vector	translation;	/**< Position						*/
		int		prevFrame;		/**< Byte offset to	previous frame	*/
	};

	/**
	* This specifically describes an hierarchical animation and
	* not the general animation in RenderWare.
	*
	* The KeyFrame list must contain at least two frames for each node.
	* There must be a KeyFrames for the  the beginning and the end of the
	* animation.
	*
	* Frames are ordered like this. First each node has a KeyFrame with time
	* 0 that defines the initial transform for each node. Then the next
	* KeyFrame for each node is stored so that each node has at least one
	* pair of data. After this the frames are stored in such an order that
	* feeds new KeyFrames to the nodes...
	*
	* A real example for an animation with 24 KeyFrames used to animate a
	* six node hierarchy. Times for the nodes are not printed but their
	* position on the row gives it away. Here node 0 and 1 has only two keys
	* because they are not animated (but they could be). The other nodes have
	* been keyed all at the same times at regular intervals during the animation.
	*
	* Node, KeyFrame
	*		|  |  |  |  |
	* 0     0           6
	* 1     1           7
	* 2     2  8 12 16 20
	* 3     3  9 13 17 21
	* 4     4 10 14 18 22
	* 5     5 11 15 19 23
	*/
	class HAnimAnimation
	{
	public:
		HAnimAnimation	();
		~HAnimAnimation	();
		void read(Stream &stream);
		
		int				m_numFrames;
		float			m_duration;
        HAnimKeyFrame*	m_frames;
	};
}

#endif // RW_ANIMATION_H
