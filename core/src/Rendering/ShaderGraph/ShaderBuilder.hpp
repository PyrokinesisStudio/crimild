/*
 * Copyright (c) 2013-2018, Hernan Saez
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

#ifndef CRIMILD_RENDERING_SHADER_GRAPH_SHADER_BUILDER_
#define CRIMILD_RENDERING_SHADER_GRAPH_SHADER_BUILDER_

#include "Foundation/SharedObject.hpp"
#include "Foundation/Containers/Array.hpp"

namespace crimild {

	class Shader;
	class ShaderProgram;

	namespace shadergraph {

		class ShaderGraph;
		class Node;

		class ShaderBuilder : public SharedObject {
		private:
			using NodeArray = containers::Array< Node * >;
			
		protected:
			ShaderBuilder( void );
			
		public:
			virtual ~ShaderBuilder( void );

			SharedPointer< ShaderProgram > build( SharedPointer< ShaderGraph > const &vsGraph, SharedPointer< ShaderGraph > const &fsGraph );

		private:
			NodeArray compile( ShaderGraph *graph );

			NodeArray sortNodes( ShaderGraph *graph );

			void generateVertexShader( ShaderGraph *graph, ShaderProgram *program );
			void generateFragmentShader( ShaderGraph *graph, ShaderProgram *program );

			virtual std::string generateShaderSource( NodeArray const &nodes, ShaderGraph *graph, ShaderProgram *program ) = 0;
		};

	}

}

#endif

