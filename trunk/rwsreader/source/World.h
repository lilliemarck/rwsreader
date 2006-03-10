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

#ifndef RW_WORLD
#define RW_WORLD

#include "Material.h"
#include "Core.h"
#include "Stream.h"

namespace rw
{
	typedef char Normal[4]; // 3 values plus 1 pad byte.

	class Sector
	{
	public:
				 Sector	();
				~Sector	();
		void	read	(Stream &stream, bool textured);

		int			m_numTriangles;
		int			m_numVertices;
		BoundingBox	m_boundingBox;
		Vector*		m_vertices;
		Normal*		m_normals;
		TexCoords*	m_texCoords;
		Triangle*	m_triangles;
	};

	/**
	*
	*/
	class World
	{
	public:
		enum Flag
		{
			F_TRISTRIP					= 0x01,
			F_POSITIONS					= 0x02,
			F_TEXTURED					= 0x04,
			F_PRELIT					= 0x08,
			F_NORMALS					= 0x10,
			F_LIGHT						= 0x20,
			F_MODULATE_MATERIAL_COLOR	= 0x40,
			F_TEXTURED2					= 0x80,
			F_NATIVE					= 0x01000000,
			F_NATIVE_INSTANCE			= 0x02000000,
			F_FLAGSMASK					= 0x000000FF,
			F_NATIVE_FLAGS_MASK			= 0x0F000000,
			F_SECTORS_OVERLAP			= 0x40000000
		};

				 World	();
				~World	();
		void	read	(Stream &stream);

		Vector			m_origin;
		int				m_numTriangles;
		int				m_numVertices;
		int				m_numSectors;
		Flag			m_flags;
		BoundingBox		m_boundingBox;
		MaterialList	m_materialList;
		Sector*			m_sectors;
	};
}

#endif // RW_WORLD
