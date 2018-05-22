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

#include "GridPositionParticleGenerator.hpp"

#include "Mathematics/Random.hpp"
#include "Coding/Encoder.hpp"
#include "Coding/Decoder.hpp"

#include "SceneGraph/Node.hpp"

using namespace crimild;

GridPositionParticleGenerator::GridPositionParticleGenerator( void )
	: _origin( Vector3f::ZERO ),
	  _size( Vector3f::ONE )
{

}

GridPositionParticleGenerator::~GridPositionParticleGenerator( void )
{

}

void GridPositionParticleGenerator::configure( Node *node, ParticleData *particles )
{
	_positions = particles->createAttribArray< Vector3f >( ParticleAttrib::POSITION );
	assert( _positions != nullptr );
}

void GridPositionParticleGenerator::generate( Node *node, crimild::Real64 dt, ParticleData *particles, ParticleId startId, ParticleId endId )
{
	auto ps = _positions->getData< Vector3f >();
	
    const auto posMin = _origin - _size;
    const auto posMax = _origin + _size;

    const auto halfSize = 0.5f * _size;

    auto i = startId;
    const auto pCount = endId - startId;
    if ( pCount == 0 ) {
    	return;
    }

    for ( auto z = -halfSize.z(); z <= halfSize.z(); z += 1.0f ) {
    	for ( auto x = -halfSize.x(); x <= halfSize.x(); x += 1.0f ) {
			auto p = Vector3f( 2 * x, 0.0f, 2 * z );
    		if ( particles->shouldComputeInWorldSpace() ) {
    			node->getWorld().applyToPoint( p, p );
    		}
    		ps[ i ] = p;
    		if ( ++i >= pCount ) {
    			return;
    		}
    	}
    }
}

void GridPositionParticleGenerator::encode( coding::Encoder &encoder ) 
{
	ParticleSystemComponent::ParticleGenerator::encode( encoder );

	encoder.encode( "origin", _origin );
	encoder.encode( "size", _size );
}

void GridPositionParticleGenerator::decode( coding::Decoder &decoder )
{
	ParticleSystemComponent::ParticleGenerator::decode( decoder );

	decoder.decode( "origin", _origin );
	decoder.decode( "size", _size );
}

