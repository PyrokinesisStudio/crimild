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

#include "ConvexHullCollider.hpp"

using namespace crimild;
using namespace crimild::physics;

ConvexHullCollider::ConvexHullCollider( void )
{

}

ConvexHullCollider::~ConvexHullCollider( void )
{

}

SharedPointer< btCollisionShape > ConvexHullCollider::generateShape( void ) 
{
    Log::debug( "Generating shape for convex hull collider" );

	auto shape = crimild::alloc< btConvexHullShape >();

	getNode()->perform( ApplyToGeometries( [shape]( Geometry *geometry ) {
		geometry->forEachPrimitive( [shape]( Primitive *primitive ) {
			auto vbo = primitive->getVertexBuffer();
			for ( int i = 0; i < vbo->getVertexCount(); i++ ) {
				Vector3f v = vbo->getPositionAt( i );
				btVector3 btv = btVector3( v[ 0 ], v[ 1 ], v[ 2 ] );
        		shape->addPoint( btv );
			}
		});
	}));

	return shape;
}

void ConvexHullCollider::renderDebugInfo( Renderer *renderer, Camera *camera )
{
	DebugRenderHelper::renderSphere( 
		renderer, 
		camera, 
		getNode()->getWorldBound()->getCenter(), 
		getNode()->getWorldBound()->getRadius(), 
		RGBAColorf( 1.0f, 0.0f, 0.0f, 0.5f ) );
}

