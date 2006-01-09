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

#ifndef RW_CLUMP_H
#define RW_CLUMP_H

#include "Frame.h"
#include "Geometry.h"
#include "Stream.h"

namespace rw
{
	/**
	* An Atomic contains a Geometry and a Frame.
	* They can be put in hierarchies and can be grouped together in a Clump.
	*/
	class Atomic
	{
	public:
				Atomic();
		void	read(Stream &stream);

		int m_frame;	/**< Index of frame		*/
		int m_geometry;	/**< Index of geometry	*/
	};


	/**
	* A Clump is a container for Atomics.
	*/
	class Clump
	{
	public:
				Clump	();
				~Clump	();
		void	read	(Stream &stream);

		int				m_numAtomics;	/**< Number of atomics				*/
		int				m_numLights;	/**< Not saved with clump			*/
		int				m_numCameras;	/**< Not saved with clump			*/
		FrameList		m_frameList;	/**< Frames for Clump and Atomics	*/
		GeometryList	m_geometryList;	/**< Contains all geometries		*/
		Atomic*			m_atomics;		/**< Array of atomics				*/
	};
}

#endif // RW_CLUMP_H
