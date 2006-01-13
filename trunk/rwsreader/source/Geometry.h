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

		Sphere	boundingSphere;	/**< Bounding sphere that encapulates the geometry	*/
		Vector*	vertices;		/**< Vertex list for geomtery						*/
		Vector*	normals;		/**< Normal list for geometry						*/
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
	* Skin plugin
	*/
	struct MatrixWeights
	{
		float weights[4]; /** Zero weights must come last */
	};
	/**
	* Strange matrix used for inverse bone transformation.
	* You need to put 1.0 into position m[15] manually.
	*/
	struct Matrix44
	{
		float m[16]; // Column major
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

		Flag				m_flags;
		int					m_numTriangles;
		int					m_numVertices;
		int					m_numMorphTargets;
		int					m_numTexCoordSets;
		Triangle *			m_triangles;
		Color *				m_preLitLum;
		TexCoords *			m_texCoords[RW_GEOMETRY_MAX_TEXTURE_COORDS];
		MorphTarget *		m_morphTargets;
		MaterialList		m_materialList;

		// Morph animation extension
		int					m_numMorphInterpolators;
		MorphInterpolator*	m_norphInterpolators;

		// Skin extension
		int					m_numBones;				/** Number of bones						*/
		unsigned int*		m_vertexBoneIndices;	/** 4 8-bit indices into HAnimHierarchy::m_nodeInfo. LSB is weight 0 */
		MatrixWeights*		m_vertexBoneWeights;	/** Weights for vertices				*/
		Matrix44*			m_skinToBoneMatrices;	/** Model-to-Bone/Inverse bone matrices	*/
	};


	/**
	* A list of Geometries.
	*/
	class GeometryList {
	public:
				 GeometryList	();
				~GeometryList	();
		void	read			(Stream &stream);
		
		int			m_numGeometries;	/**< Should match m_numAtomics in Clump	*/
		Geometry*	m_geometries;	/**< List of Geometries					*/
	};
}

#endif // RW_GEOMETRY_H
