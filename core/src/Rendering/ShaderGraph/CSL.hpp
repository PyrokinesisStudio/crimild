/*
 * Copyright (c) 2002-present, H. Hernan Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CRIMILD_RENDERING_SHADER_GRAPH_LANGUAGE_
#define CRIMILD_RENDERING_SHADER_GRAPH_LANGUAGE_

#include "Foundation/Types.hpp"
#include "Mathematics/Vector.hpp"

#include <string>

namespace crimild {

	namespace shadergraph {

		class Variable;

		namespace csl {

			Variable *scalar( crimild::Real32 value, std::string name = "" );

			Variable *vec3( Variable *vector );
			Variable *vec3_in( std::string name );
			Variable *vec4( Variable *vector, Variable *scalar );
			Variable *vec4( const Vector4f &value );

			Variable *mat3( Variable *matrix );
			Variable *mat4_uniform( std::string name );
			
			Variable *sub( Variable *a, Variable *b );
			Variable *mult( Variable *a, Variable *b );
			Variable *pow( Variable *base, Variable *exp );
			Variable *max( Variable *a, Variable *b );
			Variable *neg( Variable *input );

			Variable *dot( Variable *a, Variable *b );
			Variable *normalize( Variable *input );

			void vertexPosition( Variable *position );
			void vertexOutput( std::string name, Variable *value );

			void fragColor( Variable *color );

			Variable *worldPosition( void );
			Variable *viewPosition( void );
			Variable *projectedPosition( void );

			Variable *worldNormal( void );
			Variable *worldNormal( Variable *worldMatrix, Variable *normal );
			Variable *viewNormal( void );
			
			Variable *viewVector( Variable *viewPosition );
		}

	}

}

#endif

