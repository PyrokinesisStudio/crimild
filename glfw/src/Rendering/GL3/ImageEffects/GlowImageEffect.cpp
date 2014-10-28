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

#include "GlowImageEffect.hpp"
#include "Rendering/GL3/Programs/BlurShaderProgram.hpp"

using namespace crimild;
using namespace crimild::gl3;

GlowImageEffect::GlowImageEffect( void )
{
    _alphaState.set( new AlphaState( true ) );
    _depthState.set( new DepthState( false ) );
    
    _amount = 10;
    _glowMapSize = 128;
}

GlowImageEffect::~GlowImageEffect( void )
{
    
}

void GlowImageEffect::apply( crimild::Renderer *renderer, int inputCount, Texture **inputs, Primitive *primitive, FrameBufferObject *output )
{
    if ( _glowMapBuffer == nullptr ) {
        buildGlowBuffer( 512, 512 );
    }
    
    if ( _blurBuffer == nullptr ) {
        int width = renderer->getScreenBuffer()->getWidth();
        int height = renderer->getScreenBuffer()->getHeight();
        buildBlurBuffer( width, height );
    }
    
    Texture *source = inputCount >= 5 ? inputs[ 4 ] : inputs[ 0 ];
    computeGlow( renderer, source, primitive );
    computeBlur( renderer, _glowMap.get(), primitive );
    applyResult( renderer, inputs[ 0 ], _blurMap.get(), primitive, output );
}

void GlowImageEffect::buildGlowBuffer( int width, int height )
{
    _glowMapBuffer.set( new FrameBufferObject( width, height ) );
    _glowMapBuffer->setClearColor( RGBAColorf( 0.0f, 0.0f, 0.0f, 0.0f ) );
    _glowMapBuffer->getRenderTargets().add( new RenderTarget( RenderTarget::Type::DEPTH_16, RenderTarget::Output::RENDER, width, height ) );
    
    RenderTarget *glowTarget = new RenderTarget( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height );
    _glowMap = glowTarget->getTexture();
    _glowMapBuffer->getRenderTargets().add( glowTarget );
}

void GlowImageEffect::buildBlurBuffer( int width, int height )
{
    _blurBuffer.set( new FrameBufferObject( width, height ) );
    _blurBuffer->setClearColor( RGBAColorf( 0.0f, 0.0f, 0.0f, 0.0f ) );
    _blurBuffer->getRenderTargets().add( new RenderTarget( RenderTarget::Type::DEPTH_16, RenderTarget::Output::RENDER, width, height ) );
    
    RenderTarget *blurTarget = new RenderTarget( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::TEXTURE, width, height );
    _blurMap = blurTarget->getTexture();
    _blurBuffer->getRenderTargets().add( blurTarget );
}

void GlowImageEffect::computeGlow( crimild::Renderer *renderer, Texture *srcImage, Primitive *primitive )
{
    renderer->bindFrameBuffer( _glowMapBuffer.get() );
    
    ShaderProgram *program = renderer->getShaderProgram( "screen" );
    if ( program == nullptr ) {
        Log::Error << "Cannot find shader program for screen rendering" << Log::End;
        exit( 1 );
        return;
    }
    
    renderer->setAlphaState( _alphaState.get() );
    renderer->setDepthState( _depthState.get() );
    
    renderer->bindProgram( program );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_COLOR_MAP_UNIFORM ), srcImage );
    
    renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
    renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );
    
    renderer->drawPrimitive( program, primitive );
    
    renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
    renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );
    
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_COLOR_MAP_UNIFORM ), srcImage );
    renderer->unbindProgram( program );
    
    renderer->unbindFrameBuffer( _glowMapBuffer.get() );
}

void GlowImageEffect::computeBlur( crimild::Renderer *renderer, Texture *srcImage, Primitive *primitive )
{
    renderer->bindFrameBuffer( _blurBuffer.get() );
    
    ShaderProgram *program = renderer->getShaderProgram( "gaussianBlur" );
    if ( program == nullptr ) {
        Log::Error << "Cannot find shader program for blur effect" << Log::End;
        exit( 1 );
        return;
    }
    
//    renderer->setAlphaState( _alphaState.get() );
//    renderer->setDepthState( _depthState.get() );
    
    renderer->bindProgram( program );
//    renderer->bindTexture( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_INPUT ), srcImage );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_COLOR_MAP_UNIFORM ), srcImage );
    renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_USE_COLOR_MAP_UNIFORM ), true );
    
//    renderer->bindUniform( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_AMOUNT ), getAmount() );
//    renderer->bindUniform( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_SCALE ), 0.5f );
//    renderer->bindUniform( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_STRENTH ), 1.0f );
    
    renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
    renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );
    
//    for ( int i = 0; i < 2; i++ ) {
//        if ( i == 0 ) {
//            renderer->bindTexture( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_INPUT ), srcImage );
//        }
//        else {
//            renderer->bindTexture( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_INPUT ), _blurMap.get() );
//        }
    
//        renderer->bindUniform( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_ORIENTATION ), i );
    
        renderer->drawPrimitive( program, primitive );
        
//        if ( i == 0 ) {
//            renderer->unbindTexture( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_INPUT ), srcImage );
//        }
//        else {
//            renderer->unbindTexture( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_INPUT ), _blurMap.get() );
//        }
//    }
    
    renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
    renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );
    
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::MATERIAL_COLOR_MAP_UNIFORM ), srcImage );
//    renderer->unbindTexture( program->getLocation( BlurShaderProgram::UNIFORM_BLUR_INPUT ), srcImage );
    renderer->unbindProgram( program );
    
    renderer->unbindFrameBuffer( _blurBuffer.get() );
}

void GlowImageEffect::applyResult( Renderer *renderer, Texture *srcImage, Texture *dstImage, Primitive *primitive, FrameBufferObject *output )
{
    // bind buffer for ssao output
    renderer->bindFrameBuffer( output );
    
    ShaderProgram *program = renderer->getShaderProgram( "blend" );
    if ( program == nullptr ) {
        Log::Error << "Cannot find shader program for blending" << Log::End;
        exit( 1 );
        return;
    }
    
    // bind shader program first
    renderer->bindProgram( program );
    
    // bind framebuffer texture
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::BLEND_SRC_MAP_UNIFORM ), srcImage );
    renderer->bindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::BLEND_DST_MAP_UNIFORM ), dstImage );
    renderer->bindUniform( program->getStandardLocation( ShaderProgram::StandardLocation::BLEND_MODE_UNIFORM ), 0 );
    
    // bind vertex and index buffers
    renderer->bindVertexBuffer( program, primitive->getVertexBuffer() );
    renderer->bindIndexBuffer( program, primitive->getIndexBuffer() );
    
    // draw primitive
    renderer->drawPrimitive( program, primitive );
    
    // unbind primitive buffers
    renderer->unbindVertexBuffer( program, primitive->getVertexBuffer() );
    renderer->unbindIndexBuffer( program, primitive->getIndexBuffer() );
    
    // unbind framebuffer texture
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::BLEND_SRC_MAP_UNIFORM ), srcImage );
    renderer->unbindTexture( program->getStandardLocation( ShaderProgram::StandardLocation::BLEND_DST_MAP_UNIFORM ), dstImage );
    
    // lastly, unbind the shader program
    renderer->unbindProgram( program );
    
    // unbind buffer for ssao output
    renderer->unbindFrameBuffer( output );
}

