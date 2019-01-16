/*
 * Copyright (c) 2002-present, H. Hernán Saez
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

#include "ShadowPass.hpp"

#include "Rendering/RenderGraph/RenderGraphAttachment.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderQueue.hpp"
#include "Rendering/FrameBufferObject.hpp"
#include "Rendering/ShaderGraph/ShaderGraph.hpp"
#include "Foundation/Profiler.hpp"
#include "Rendering/Programs/DepthShaderProgram.hpp"
#include "Rendering/ShaderGraph/Constants.hpp"
#include "Rendering/ShadowMap.hpp"

using namespace crimild;
using namespace crimild::rendergraph;
using namespace crimild::rendergraph::passes;
using namespace crimild::shadergraph;
using namespace crimild::shadergraph::locations;

ShadowPass::ShadowPass( RenderGraph *graph )
	: RenderGraphPass( graph, "Shadow Pass" )
{
	_shadowOutput = graph->createAttachment(
		getName() + " - Shadow",
		RenderGraphAttachment::Hint::FORMAT_DEPTH_HDR |
		RenderGraphAttachment::Hint::WRAP_REPEAT |
		RenderGraphAttachment::Hint::SIZE_1024 |
		RenderGraphAttachment::Hint::PERSISTENT );
}

ShadowPass::~ShadowPass( void )
{
	
}

void ShadowPass::setup( RenderGraph *graph )
{
	graph->write( this, { _shadowOutput } );
	
	_program = crimild::alloc< DepthShaderProgram >();
}

void ShadowPass::execute( RenderGraph *graph, Renderer *renderer, RenderQueue *renderQueue )
{
	CRIMILD_PROFILE( getName() )
	
	auto fbo = graph->createFBO( { _shadowOutput } );
	
	renderer->bindFrameBuffer( crimild::get_ptr( fbo ) );

	renderQueue->each( [ this, renderer, renderQueue ]( Light *light, int ) {
		if ( light != nullptr && light->castShadows() ) {
			renderShadowMap( renderer, renderQueue, light );
		}
	});
	
	renderer->unbindFrameBuffer( crimild::get_ptr( fbo ) );	
}

void ShadowPass::renderShadowMap( Renderer *renderer, RenderQueue *renderQueue, Light *light )
{
	auto renderables = renderQueue->getRenderables( RenderQueue::RenderableType::SHADOW_CASTER );
	if ( renderables->size() == 0 ) {
		return;
	}

	auto program = crimild::get_ptr( _program );
	
	Frustumf f( -2.0f, 2.0f, -2.0f, 2.0f, 0.01f, 10.0f );
	const auto pMatrix = f.computeOrthographicMatrix();
	program->bindUniform( PROJECTION_MATRIX_UNIFORM, pMatrix );

	const auto vMatrix = light->getWorld().computeModelMatrix().getInverse();
	program->bindUniform( VIEW_MATRIX_UNIFORM, vMatrix );

	if ( auto shadowMap = light->getShadowMap() ) {
		shadowMap->setLightProjectionMatrix( pMatrix );
		shadowMap->setLightViewMatrix( vMatrix );
	}
	
	renderQueue->each( renderables, [ this, renderer, program ]( RenderQueue::Renderable *renderable ) {

		const auto &mMatrix = renderable->modelTransform;
		program->bindUniform( MODEL_MATRIX_UNIFORM, mMatrix );

		renderer->bindProgram( program );
		
		renderable->geometry->forEachPrimitive( [ renderer ]( Primitive *primitive ) {
			renderer->bindPrimitive( nullptr, primitive );
			renderer->drawPrimitive( nullptr, primitive );
			renderer->unbindPrimitive( nullptr, primitive );
		});
		
		renderer->unbindProgram( program );
	});
	
}
