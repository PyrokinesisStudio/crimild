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

#ifndef CRIMILD_CORE_RENDERING_RENDER_QUEUE_
#define CRIMILD_CORE_RENDERING_RENDER_QUEUE_

#include "Foundation/SharedObject.hpp"
#include "Foundation/SharedObjectList.hpp"
#include "Foundation/Pointer.hpp"

#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/Camera.hpp"
#include "SceneGraph/Light.hpp"

#include "Material.hpp"

#include <functional>
#include <vector>
#include <chrono>

namespace crimild {
    
    class RenderQueue;

    using RenderQueuePtr = SharedPointer< RenderQueue >;
    
    namespace messaging {
        
        struct RenderQueueAvailable {
            RenderQueuePtr renderQueue;
        };
        
    }
    
    class RenderQueue : public SharedObject {
    public:
        using GeometryContext = std::pair< GeometryPtr, Matrix4f >;
        using PrimitiveMap = std::map< PrimitivePtr, std::vector< GeometryContext >>;
        using MaterialMap = std::map< MaterialPtr, PrimitiveMap >;

    public:
        RenderQueue( void );
        virtual ~RenderQueue( void );
        
        void reset( void );
        
        void setCamera( CameraPtr const &camera );
        CameraPtr getCamera( void ) { return _camera; }
        const Matrix4f &getViewMatrix( void ) const { return _viewMatrix; }
        const Matrix4f &getProjectionMatrix( void ) const { return _projectionMatrix; }
        
        void push( MaterialPtr const &material, PrimitivePtr const &primitive, GeometryPtr const &geometry, const TransformationImpl &world, bool renderOnScreen = false );
        void push( LightPtr const &light );

        void each( MaterialMap const &objects, std::function< void( MaterialPtr const &, PrimitiveMap const & ) > callback );
        void each( std::function< void( LightPtr const &, int ) > callback );

        MaterialMap &getShadowCasters( void ) { return _shadowCasters; }
        MaterialMap &getShadedObjects( void ) { return _shadedObjects; }
        MaterialMap &getOpaqueObjects( void ) { return _opaqueObjects; }
        MaterialMap &getTranslucentObjects( void ) { return _translucentObjects; }
        MaterialMap &getScreenObjects( void ) { return _screenObjects; }
        
    private:
        CameraPtr _camera;
        Matrix4f _viewMatrix;
        Matrix4f _projectionMatrix;
        
        std::vector< LightPtr > _lights;

        MaterialMap _shadowCasters;
        MaterialMap _shadedObjects;
        MaterialMap _opaqueObjects;
        MaterialMap _translucentObjects;
        MaterialMap _screenObjects;
        
    public:
        unsigned long getTimestamp( void ) const { return _timestamp; }
        void setTimestamp( unsigned long value ) { _timestamp = value; }
        
    private:
        std::chrono::microseconds::rep _timestamp;
    };
    
}

#endif

