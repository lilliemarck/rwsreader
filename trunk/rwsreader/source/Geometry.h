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

#ifndef RW_GEOMETRY_H
#define RW_GEOMETRY_H

#include "Material.h"
#include "Core.h"
#include "Stream.h"

namespace rw
{
	#define RW_GEOMETRY_MAX_TEXTURE_COORDS 8

	/**
	* Stores a keyframe for vertex animation.
	*/
	class MorphTarget
	{
	public:
		MorphTarget();

		Sphere		boundingSphere;	/**< Bounding sphere that encapulates the geometry	*/
		Vector *	vertices;		/**< Vertex list for geomtery						*/
		Vector *	normals;		/**< Normal list for geometry						*/
	};


	/**
	* Morph animation extension
	*/
	class MorphInterpolator
	{
	public:
		MorphInterpolator();

		int		startMorphTarget;	/**< Index of starting MorphTarget		*/
		int		endMorphTarget;		/**< Index of end MorphTarget			*/
		float	time;				/**< Duration of interpolator			*/
		int		next;				/**< Index of next MorphInterpolator	*/
	};


	/**
	* Contains mesh data stored in Triangles and in
	* in one or more MorphTarget objects.
	*/
	class Geometry
	{
	public:
		enum Flag
		{
			GF_TRISTRIP					= 0x00000001,
			GF_POSITIONS				= 0x00000002,
			GF_TEXTURED					= 0x00000004,
			GF_PRELIT					= 0x00000008,
			GF_NORMALS					= 0x00000010,
			GF_LIGHT					= 0x00000020,
			GF_MODULATE_MATERIAL_COLOR	= 0x00000040,
			GF_TEXTURED2				= 0x00000080,
			GF_NATIVE					= 0x01000000,
			GF_NATIVE_INSTANCE			= 0x02000000,
			GF_FLAGS_MASK				= 0x000000FF,
			GF_NATIVE_FLAGS_MASK		= 0x0F000000,
		};

				 Geometry	();
				~Geometry	();
		void	read		(Stream &stream);

		Flag				mFlags;
		int					mNumTriangles;
		int					mNumVertices;
		int					mNumMorphTargets;
		int					mNumTexCoordSets;
		Triangle *			mTriangles;
		Color *				mPreLitLum;
		TexCoords *			mTexCoords[RW_GEOMETRY_MAX_TEXTURE_COORDS];
		MorphTarget *		mMorphTargets;
		MaterialList		mMaterialList;

		// Morph animation extension
		int					mNumMorphInterpolators;
		MorphInterpolator*	mMorphInterpolators;
	};


	/**
	* A list of Geometries.
	*/
	class GeometryList {
	public:
				 GeometryList	();
				~GeometryList	();
		void	read			(Stream &stream);
		
		int			mNumGeometries;	/**< Should match mNumAtomics in Clump	*/
		Geometry*	mGeometries;	/**< List of Geometries					*/
	};
}

#endif // RW_GEOMETRY_H
