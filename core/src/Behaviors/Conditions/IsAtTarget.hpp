#ifndef CRIMILD_CORE_BEHAVIORS_CONDITIONS_IS_AT_TARGET_
#define CRIMILD_CORE_BEHAVIORS_CONDITIONS_IS_AT_TARGET_

#include "Behaviors/Behavior.hpp"

namespace crimild {

	namespace behaviors {

		namespace conditions {

			class IsAtTarget : public Behavior {
				CRIMILD_IMPLEMENT_RTTI( crimild::behaviors::actions::IsAtTarget )
				
			public:
				IsAtTarget( void );
				explicit IsAtTarget( crimild::Real32 minDistance );
				virtual ~IsAtTarget( void );
				
				virtual crimild::behaviors::Behavior::State step( crimild::behaviors::BehaviorContext *context ) override;

			private:
				crimild::Real32 _minDistance = crimild::Numericf::ZERO_TOLERANCE;

				/**
				   \name Coding support
				*/
				//@{
				
			public:
				virtual void encode( coding::Encoder &encoder ) override;
				virtual void decode( coding::Decoder &decoder ) override;
				
				//@}
			};

		}

	}

}

#endif

