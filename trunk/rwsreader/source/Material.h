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

#ifndef RW_MATERIAL_H
#define RW_MATERIAL_H

#include "Core.h"
#include "Stream.h"
#include "Texture.h"

namespace rw
{
	/**
	* A Material defines how the surface of a Geometry looks.
	*/
	class Material
	{
	public:
				 Material	();
				~Material	();
		void	read		(Stream &stream);

		Color		mColor;
		Texture*	mTexture;	/**< 0 if the material has no texture	*/
		float		mAmbient;
		float		mSpecular;	/**< Not used by RenderWare				*/
		float		mDiffuse;
	};

	
	/**
	* A list of Materials.
	*/
	class MaterialList
	{
	public:
				 MaterialList	();
				~MaterialList	();
		void	read			(Stream &stream);

		int			mNumMaterials;
		Material*	mMaterials;
	};
}

#endif // RW_MATERIAL_H
