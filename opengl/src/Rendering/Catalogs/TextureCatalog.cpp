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

#include "TextureCatalog.hpp"

#include "Rendering/OpenGLUtils.hpp"

// TODO: this will cause visual artifacts in some platforms. use with care
#ifndef GL_BGR
    #define GL_BGR GL_RGB
#endif

using namespace crimild;
using namespace crimild::opengl;

TextureCatalog::TextureCatalog( void )
	: _boundTextureCount( 0 )
{

}

TextureCatalog::~TextureCatalog( void )
{

}

int TextureCatalog::getNextResourceId( void )
{
	GLuint textureId = 0;
	glGenTextures( 1, &textureId );
    return textureId;
}

void TextureCatalog::bind( ShaderLocation *location, Texture *texture )
{
	if ( !texture ) {
		return;
	}

	CRIMILD_CHECK_GL_ERRORS_BEFORE_CURRENT_FUNCTION;

	Catalog< Texture >::bind( location, texture );

	if ( location && location->isValid() ) {
		glActiveTexture( GL_TEXTURE0 + _boundTextureCount );
		glBindTexture( GL_TEXTURE_2D, texture->getCatalogId() );
		glUniform1i( location->getLocation(), _boundTextureCount );

		++_boundTextureCount;
	} 

	CRIMILD_CHECK_GL_ERRORS_AFTER_CURRENT_FUNCTION;
}

void TextureCatalog::unbind( ShaderLocation *location, Texture *texture )
{
	if ( texture == nullptr ) {
		return;
	}
	
	CRIMILD_CHECK_GL_ERRORS_BEFORE_CURRENT_FUNCTION;

	if ( _boundTextureCount > 0 ) {
		--_boundTextureCount;
		glActiveTexture( GL_TEXTURE0 + _boundTextureCount );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	
	Catalog< Texture >::unbind( location, texture );

	CRIMILD_CHECK_GL_ERRORS_AFTER_CURRENT_FUNCTION;
}

void TextureCatalog::load( Texture *texture )
{
	Catalog< Texture >::load( texture );

	int textureId = texture->getCatalogId();
    glBindTexture( GL_TEXTURE_2D, textureId );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	GLint internalFormat = GL_RGBA;
	GLint format = GL_BGRA;
	if ( texture->getImage()->getBpp() == 4 ) {
		internalFormat = GL_RGBA;
		if ( texture->getImage()->getPixelFormat() == Image::PixelFormat::BGRA ) {
			format = GL_BGRA;
		}
		else {
			format = GL_RGBA;
		}
	}
	else if ( texture->getImage()->getBpp() == 3 ) {
		internalFormat = GL_RGB;
		if ( texture->getImage()->getPixelFormat() == Image::PixelFormat::BGR ) {
			format = GL_BGR;
		}
		else {
			format = GL_RGB;
		}
	}
	else if ( texture->getImage()->getBpp() == 1 ) {
		internalFormat = GL_RED;
		format = GL_RED;
	}

    glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, 
    	texture->getImage()->getWidth(), texture->getImage()->getHeight(), 0, 
    	format, 
    	GL_UNSIGNED_BYTE,
        ( GLvoid * ) texture->getImage()->getData() );
}

void TextureCatalog::unload( Texture *texture )
{
    if ( texture->getCatalogId() > 0 ) {
        _textureIdsToDelete.push_back( texture->getCatalogId() );
    }
    
    Catalog< Texture >::unload( texture );
}

void TextureCatalog::cleanup( void )
{
    for ( auto id : _textureIdsToDelete ) {
        GLuint textureId = id;
        glDeleteTextures( 1, &textureId );
    }
    
    _textureIdsToDelete.clear();
}
