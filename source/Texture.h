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

#ifndef RW_TEXTURE_H
#define RW_TEXTURE_H

#include "Stream.h"

namespace rw
{
	/**
	* Represents a texture map.
	*/
	class Texture
	{
	public:
		enum FilterMode {
			FM_NA_FILTER_MODE		= 0,
			FM_NEAREST				= 1,
			FM_LINEAR				= 2,
			FM_MIP_NEAREST			= 3,
			FM_MIP_LINEAR			= 4,
			FM_LINEAR_MIP_NEAREST	= 5,
			FM_LINEAR_MIP_LINEAR	= 6,
		};

		enum AddressMode {
			AM_NA_ADDRESS_MODE		= 0,
			AM_WRAP					= 1,
			AM_MIRROR				= 2,
			AM_CLAMP				= 3,
			AM_BORDER				= 4,
		};

				 Texture();
				~Texture();
		void	read	(Stream &stream);

		FilterMode	m_filterMode;
		AddressMode	m_addressModeU;
		AddressMode	m_addressModeV;
		char*		m_name;
		char*		m_maskName;
	};
}

#endif // RW_TEXTURE_H
