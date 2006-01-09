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

#ifndef RW_CORE_H
#define RW_CORE_H

#include "Stream.h"

namespace rw
{
	template <typename T> void MemZero(T *m) { memset(m, 0, sizeof(T)); }

	struct Color
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	};

	struct Vector
	{
		float x, y, z;
	};

	class Matrix
	{
	public:
		void read(Stream &stream);

		Vector right;	/**< X-axis		*/
		Vector up;		/**< Y-axis		*/
		Vector at;		/**< Z-axis		*/
		Vector pos;		/**< Position	*/
	};

	struct Quat
	{
		float x, y, z, w;
	};

	struct Sphere
	{
		Vector	center;
		float	radius;
	};

	struct TexCoords
	{
		float u;
		float v;
	};

	struct Triangle
	{
		unsigned short vertIndex[3];
		unsigned short matIndex;
	};
}

#endif // RW_CORE_H
