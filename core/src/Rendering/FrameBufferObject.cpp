/*
 * Copyright (c) 2013-2018, H. Hernan Saez
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

#include "FrameBufferObject.hpp"
#include "RenderTarget.hpp"

using namespace crimild;

FrameBufferObject::FrameBufferObject( int width, int height )
	: _width( width ),
	  _height( height ),
	  _clearColor( 0.0f, 0.0f, 0.0f, 0.0f )
{

}

FrameBufferObject::~FrameBufferObject( void )
{
    _renderTargets.clear();
}

void FrameBufferObject::resize( int width, int height )
{
    _width = width;
    _height = height;
}

StandardFrameBufferObject::StandardFrameBufferObject( int width, int height )
	: FrameBufferObject( width, height )
{
#ifdef CRIMILD_PLATFORM_DESKTOP
    getRenderTargets().insert( RenderTarget::RENDER_TARGET_NAME_DEPTH, crimild::alloc< RenderTarget >( RenderTarget::Type::DEPTH_32, RenderTarget::Output::RENDER_AND_TEXTURE, width, height, true ) );
#else
    getRenderTargets().insert( RenderTarget::RENDER_TARGET_NAME_DEPTH, crimild::alloc< RenderTarget >( RenderTarget::Type::DEPTH_24, RenderTarget::Output::RENDER, width, height ) );
#endif
    getRenderTargets().insert( RenderTarget::RENDER_TARGET_NAME_COLOR, crimild::alloc< RenderTarget >( RenderTarget::Type::COLOR_RGBA, RenderTarget::Output::RENDER_AND_TEXTURE, width, height ) );
}

StandardFrameBufferObject::~StandardFrameBufferObject( void )
{

}

