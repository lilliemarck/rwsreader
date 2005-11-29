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

		int mFrame;		/**< Index of frame		*/
		int mGeometry;	/**< Index of geometry	*/
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

		int				mNumAtomics;	/**< Number of atomics				*/
		int				mNumLights;		/**< Not saved with clump			*/
		int				mNumCameras;	/**< Not saved with clump			*/
		FrameList		mFrameList;		/**< Frames for Clump and Atomics	*/
		GeometryList	mGeometryList;	/**< Contains all geometries		*/
		Atomic*			mAtomics;		/**< Array of atomics				*/
	};
}

#endif // RW_CLUMP_H
