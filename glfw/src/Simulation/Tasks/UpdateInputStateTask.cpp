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

#include "UpdateInputStateTask.hpp"

using namespace crimild;

UpdateInputStateTask::UpdateInputStateTask( int priority, GLFWwindow *window )
	: Task( priority ),
	  _window( window )
{
	glfwGetWindowSize( _window, &_windowWidth, &_windowHeight);
}

UpdateInputStateTask::~UpdateInputStateTask( void )
{

}

void UpdateInputStateTask::start( void )
{
	InputState::getCurrentState().reset( GLFW_KEY_LAST, GLFW_MOUSE_BUTTON_LAST );
}

void UpdateInputStateTask::stop( void )
{

}

void UpdateInputStateTask::update( void )
{
	glfwPollEvents();

	for ( int i = 0; i < GLFW_KEY_LAST; i++ ) {
		int keyState = glfwGetKey( _window, i );
		InputState::getCurrentState().setKeyState( i, keyState == GLFW_PRESS ? InputState::KeyState::PRESSED : InputState::KeyState::RELEASED );
	}

	double x, y;
	glfwGetCursorPos( _window, &x, &y );

	if ( x >= 0 && x < _windowWidth && y >= 0 && y < _windowHeight ) {
		InputState::getCurrentState().setMousePosition( Vector2i( x, y ) );
		InputState::getCurrentState().setNormalizedMousePosition( Vector2f( ( float ) x / float( _windowWidth - 1.0f ), ( float ) y / float( _windowHeight - 1.0f ) ) );
	}

	for ( int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST; i++ ) {
		int buttonState = glfwGetMouseButton( _window, i );
		InputState::getCurrentState().setMouseButtonState( i, buttonState == GLFW_PRESS ? InputState::MouseButtonState::PRESSED : InputState::MouseButtonState::RELEASED );
	}
}

