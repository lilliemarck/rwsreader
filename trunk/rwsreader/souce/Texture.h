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

		FilterMode	mFilterMode;
		AddressMode	mAddressModeU;
		AddressMode	mAddressModeV;

		char *	mName;
		char *	mMaskName;
	};
}

#endif // RW_TEXTURE_H
