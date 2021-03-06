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
#include "Foundation/Containers/Array.hpp"
#include "Mathematics/Vector.hpp"

#include <string>

namespace crimild {

	namespace shadergraph {

		class Variable;

		namespace csl {

			Variable *scalar( crimild::Real32 value, std::string name = "" );
			Variable *scalar_uniform( std::string name );
			Variable *scalar_constant( crimild::Real32 value );

			Variable *vec2_in( std::string name );
			Variable *vec3( Variable *vector );
			Variable *vec3_in( std::string name );
			Variable *vec4( Variable *scalar );
			Variable *vec4( Variable *vector, Variable *scalar );
			Variable *vec4( Variable *x, Variable *y, Variable *z, Variable *w );
			Variable *vec4( const Vector4f &value );
			Variable *vec_x( Variable *vector );
			Variable *vec_y( Variable *vector );
			Variable *vec_z( Variable *vector );
			Variable *vec_w( Variable *vector );

			Variable *mat3( Variable *matrix );
			Variable *mat4_uniform( std::string name );

			Variable *red( Variable *color );
			Variable *green( Variable *color );
			Variable *blue( Variable *color );
			Variable *alpha( Variable *color );

			/**
			   \name Math ops
			*/
			//@{
			
			Variable *add( Variable *a, Variable *b );

			Variable *sub( Variable *a, Variable *b );

			Variable *mult( Variable *a, Variable *b );
			Variable *mult( containers::Array< Variable * > const &inputs );
			template< class ... Args >
			Variable *mult( Args &&... args ) { return mult( { args... } ); }

			Variable *div( Variable *a, Variable *b );

			Variable *pow( Variable *base, Variable *exp );

			Variable *max( Variable *a, Variable *b );
			Variable *neg( Variable *input );

			Variable *dot( Variable *a, Variable *b );
			Variable *normalize( Variable *input );

			//@}

			void vertexPosition( Variable *position );
			void vertexOutput( std::string name, Variable *value );

			void fragColor( Variable *color );

			/**
			   \name Position
			 */
			//@{

			Variable *modelPosition( void );
			Variable *worldPosition( void );
			Variable *viewPosition( void );
			Variable *projectedPosition( void );
			Variable *screenPosition( void );

			//@}

			/**
			   \name Texture Coordinates
			*/
			//@{

			Variable *modelTextureCoords( void );

			//@}

			/**
			   \name Normals
			*/
			//@{

			Variable *modelNormal( void );
			Variable *worldNormal( void );
			Variable *worldNormal( Variable *worldMatrix, Variable *normal );
			Variable *viewNormal( void );

			//@}
			
			Variable *viewVector( Variable *viewPosition );

			Variable *linearizeDepth( Variable *input, Variable *near, Variable *far );

			Variable *texture2D_uniform( std::string name );
			Variable *textureColor( Variable *texture, Variable *uvs );
		}

	}

}

#endif

