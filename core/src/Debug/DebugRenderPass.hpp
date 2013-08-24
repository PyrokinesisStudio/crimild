/*
 * Copyright (c) 2013, Hernan Saez
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

#ifndef CRIMILD_CORE_DEBUG_RENDER_PASS_
#define CRIMILD_CORE_DEBUG_RENDER_PASS_

#include "Rendering/RenderPass.hpp"
#include "Rendering/Material.hpp"

namespace crimild {

	class DebugRenderPass : public RenderPass {
	public:
		DebugRenderPass( RenderPassPtr actualRenderPass );
		virtual ~DebugRenderPass( void );

		void setRenderBoundings( bool value ) { _renderBoundings = value; }
		bool shouldRenderBoundings( void ) const { return _renderBoundings; }

		void setRenderNormals( bool value ) { _renderNormals = value; }
		bool shouldRenderNormals( void ) const { return _renderNormals; }

		virtual void render( Renderer *renderer, VisibilitySet *vs, Camera *camera );

	private:
		void renderNormalsAndTangents( Renderer *renderer, Geometry *geometry, Material *material, Camera *camera );
		void renderBoundings( Renderer *renderer, Geometry *geometry, Material *material, Camera *camera );

		bool _renderNormals;
		bool _renderBoundings;
		RenderPassPtr _actualRenderPass;
		MaterialPtr _debugMaterial;
	};

	typedef std::shared_ptr< DebugRenderPass > DebugRenderPassPtr;

}

#endif
