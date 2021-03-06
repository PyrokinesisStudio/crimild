#include "Composite.hpp"
#include "Coding/Encoder.hpp"
#include "Coding/Decoder.hpp"

using namespace crimild;
using namespace crimild::behaviors;
using namespace crimild::behaviors::composites;

Composite::Composite( void )
{

}

Composite::~Composite( void )
{

}

void Composite::init( BehaviorContext *context )
{
	Behavior::init( context );
}

void Composite::attachBehavior( BehaviorPtr const &behavior )
{
	_behaviors.add( behavior );
}

size_t Composite::getBehaviorCount( void ) const
{
	return _behaviors.size();
}

Behavior *Composite::getBehaviorAt( size_t index )
{
	return crimild::get_ptr( _behaviors[ index ] );
}

void Composite::encode( coding::Encoder &encoder )
{
	Behavior::encode( encoder );

	encoder.encode( "behaviors", _behaviors );
}

void Composite::decode( coding::Decoder &decoder )
{
	Behavior::decode( decoder );

	decoder.decode( "behaviors", _behaviors );
}

