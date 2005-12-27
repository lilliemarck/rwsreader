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

#ifndef RW_CORE_H
#define RW_CORE_H

#include "Stream.h"

#define SAFE_DELETE(p)			\
{								\
    if (p)						\
    {							\
        delete (p);				\
        (p) = 0;				\
    }							\
}

#define SAFE_DELETE_ARRAY(p)	\
{								\
    if (p)						\
    {							\
        delete [] (p);		    \
        (p) = 0;			    \
    }						    \
}

namespace rw
{
	struct Color
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	};

	struct Vector
	{
		float x;
		float y;
		float z;
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
