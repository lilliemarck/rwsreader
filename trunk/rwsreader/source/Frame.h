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

#ifndef RW_FRAME_H
#define RW_FRAME_H

#include "Core.h"

namespace rw
{
	class HAnimNodeInfo
	{
	public:
		enum Flag
		{
			NF_PUSH	= 0x00000001,	/**< Is leaf node				*/
			NF_POP	= 0x00000002	/**< Set if not last sibling	*/
		};

		int		m_nodeID;		/**< Unique ID that the artist can set				*/
		int		m_nodeIndex;	/**< Probably the index in HAnimHierarchy's array	*/
		Flag	m_flags;		/**< The flags are used to construct a tree			*/
	};

	/**
	* The HAnimHierarchy adds one frame extra to a Clump. This frame sits 
	* between the Clumps frame and the Atomics' frames. Only this extra
	* frame has a pointer to the HAnimHierarchy.
	*/
	class HAnimHierarchy
	{
	public:
				 HAnimHierarchy	();
				~HAnimHierarchy	();
		void	read			(Stream &stream);

		int				m_numNodes;	/** Number of nodes		*/
		HAnimNodeInfo*	m_nodeInfo;	/** Array of node info	*/
	};

	/**
	* Allows objects to be positioned in space.
	* Frames can be liked into a hierarchy.
	*/
	class Frame
	{
	public:
		 Frame();
		~Frame();

		Matrix			m_matrix;			/**< Modeling matrix						*/
		int				m_parent;			/**< Index of parent frame					*/

		// Hierarchical animation extension
		int				m_id;				/**< Connect to HAnimNodeInfo with this ID	*/
		HAnimHierarchy*	m_HAnimHierarchy;	/**< Animation Hierarchy, if any			*/
	};

	
	/**
	* A list of Frames.
	*/
	class FrameList {
	public:
				 FrameList	();
				~FrameList	();
		void	read		(Stream &stream);
		
		int		m_numFrames;
		Frame *	m_frames;
	};
}

#endif // RW_FRAME_H
