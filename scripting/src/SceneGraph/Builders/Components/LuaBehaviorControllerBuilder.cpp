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

#include "LuaBehaviorControllerBuilder.hpp"

#include "SceneGraph/LuaSceneBuilder.hpp"

#include <Behaviors/Behavior.hpp>
#include <Behaviors/BehaviorTree.hpp>

using namespace crimild;
using namespace crimild::behaviors;
using namespace crimild::scripting;

SharedPointer< BehaviorController > LuaBehaviorControllerBuilder::build( ScriptEvaluator &eval )
{
	auto cmp = crimild::alloc< BehaviorController >();
	
	eval.foreach( "events", [cmp]( ScriptEvaluator &eventEval, int ) {
		std::string eventName;
		eventEval.getPropValue( "name", eventName );

		Log::debug( CRIMILD_CURRENT_CLASS_NAME, "Building behaviors for event ", eventName );

		ScriptEvaluator rootBehaviorEval = eventEval.getChildEvaluator( "root" );
		std::string rootBehaviorType;
		if ( rootBehaviorEval.getPropValue( "type", rootBehaviorType ) ) {
			auto behaviorBuilder = LuaObjectBuilderRegistry::getInstance()->getBuilder( rootBehaviorType );
			if ( behaviorBuilder != nullptr ) {
				auto behavior = crimild::cast_ptr< Behavior >( behaviorBuilder( rootBehaviorEval ) );
				if ( behavior != nullptr ) {
					cmp->attachBehaviorTree( crimild::alloc< BehaviorTree >( eventName, behavior ) );
				}
				else {
					Log::error( CRIMILD_CURRENT_CLASS_NAME, "Failed to build behavior wit type ", rootBehaviorType );
				}
			}
			else {
				Log::error( CRIMILD_CURRENT_CLASS_NAME, "No builder found for behavior with type ", rootBehaviorType );
			}
		}
		else {
			Log::error( CRIMILD_CURRENT_CLASS_NAME, "No 'type' field found for root behavior" );
		}
	});

	eval.foreach( "context", [cmp]( ScriptEvaluator &contextEval, int ) {
		std::string key;
		if ( !contextEval.getPropValue( "key", key ) ) {
			return;
		}

		std::string value;
		if ( !contextEval.getPropValue( "value", value ) ) {
			return;
		}

		cmp->getContext()->setValue( key, value );
	});
	
	return cmp;
}

