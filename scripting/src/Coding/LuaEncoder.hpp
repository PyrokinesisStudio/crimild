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

#ifndef CRIMILD_SCRIPTING_CODING_LUA_ENCODER_
#define CRIMILD_SCRIPTING_CODING_LUA_ENCODER_

#include <Coding/Encoder.hpp>
#include <Foundation/Containers/Stack.hpp>

namespace crimild {

	namespace coding {

        class LuaEncoder : public coding::Encoder {
        public:
            LuaEncoder( void );
            virtual ~LuaEncoder( void );
            
            virtual void encode( SharedPointer< coding::Codable > const &codable ) override;
            virtual void encode( std::string key, SharedPointer< coding::Codable > const &codable ) override;

            virtual void encode( std::string key, std::string value ) override;
            virtual void encode( std::string key, crimild::Size value ) override;
            virtual void encode( std::string key, crimild::UInt16 value ) override;
            virtual void encode( std::string key, crimild::Int32 value ) override;
            virtual void encode( std::string key, crimild::UInt32 value ) override;
            virtual void encode( std::string key, crimild::Bool value ) override;
            virtual void encode( std::string key, crimild::Real32 value ) override;
            virtual void encode( std::string key, crimild::Real64 value ) override;
            virtual void encode( std::string key, const Vector3f &value ) override;
            virtual void encode( std::string key, const Transformation &value ) override;
            virtual void encode( std::string key, const VertexFormat &value ) override;

			inline std::string getEncodedString( void ) const
			{
                return _ss.str();
			}
            
        protected:
            virtual void encodeArrayBegin( std::string key, crimild::Size count ) override;
            virtual void encodeArrayEnd( std::string key ) override;
            
        private:
            void encodeKey( std::string key );
            std::string getIndentSpaces( void );
            
        private:
            std::stringstream _ss;
            crimild::Size _indentLevel = 0;
            containers::Stack< std::string > _arrayKeys;

        public:
            virtual void dump( void ) override;
        };

	}

}

#endif

