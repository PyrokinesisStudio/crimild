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

#ifndef CRIMILD_CORE_MATHEMATICS_QUATERNION_
#define CRIMILD_CORE_MATHEMATICS_QUATERNION_

#include "Vector.hpp"
#include "Numeric.hpp"
#include "Matrix.hpp"

namespace Crimild {

	/**
		 \brief Template-based implementation for Quaternions

		 A Quaternion, as invented by Sir William Rowan Hamilton, is an
		 extension to complex numbers.

		 In mathematics, a quaternion q can be defined in the following ways, all equivalent:

		 \code
		 q = (qv, qw) = iqx + jqy + kqz + qw = qv + qw,
		 qv = iqx + jqy + kqz = (qx, qy, qz),
		 i2 = j2 = k2 = -1, jk = -kj = i, ki = -ik = j, ij = -ji = k
		 \endcode

		 The variable qw is called the real part of a quaternion. The imaginary part
		 is qv, and i, j and k are called imaginary units

		 \section REFERENCES References

		 - "Real-Time Rendering", 3rd Edition
		 - "Mathematics for 3D Game Programming and Computer Graphics", 2nd Edition
		 - Wikipedia (http://en.wikipedia.org/wiki/Quaternion)
	 */
	template< typename PRECISION >
	class Quaternion {
	private:
		typedef Vector< 3, PRECISION > Vector3Impl;
		typedef Vector< 4, PRECISION > Vector4Impl;

	public:
		/**
		 	\brief Creates a new quaternion representing a rotation
		 */
		static Quaternion createFromAxisAngle( const Vector3Impl &axis, PRECISION angle )
		{
			Quaternion q;
			q.fromAxisAngle( axis, angle );
			return q;
		}
        
        /**
            \brief Computes the quaternion that rotates from a to b
         
            This method was taken from "The Shortest Arc Quaternion" by Stan Melax
            in "Game Programming Gems". It calculates a quaternion that 
            rotates from a to b, avoiding numerical instability.
         */
        static Quaternion createFromVectors( const Vector3Impl &v0, const Vector3Impl &v1 )
        {
            if ( v0 == -v1 ) {
                return Quaternion::createFromAxisAngle( Vector3Impl( 1, 0, 0 ), Numeric< PRECISION >::PI );
            }
            
            Vector3Impl c = v0 ^ v1;
            PRECISION d = v0 * v1;
            PRECISION s = std::sqrt( ( 1.0 + d ) * 2.0 );
            
            return Quaternion( s / 2.0, c[ 0 ] / s, c[ 1 ] / s, c[ 2 ] / s );
        }

	public:
		/**
			\brief Default constructor

			The default constructor does not nothing. The user is responsible for
			setting valid values for all quaternion components
		 */
		Quaternion( void )
		{
		}

		/**
			\brief Explicit constructor

			Constructs a quaternion by specifying both the real
			and the imaginary parts of it as a scalar and a vector
			respectively.
		 */
		Quaternion( PRECISION r, const Vector3Impl &i )
			: _data( i[ 0 ], i[ 1 ], i[ 2 ], r )
		{
		}

		/**
			\brief Explicit constructor
		 */
		Quaternion( PRECISION w, PRECISION x, PRECISION y, PRECISION z )
			: _data( x, y, z, w )
		{
		}

		explicit Quaternion( const Vector4Impl &data )
			: _data( data )
		{

		}

		/**
			\brief Copy constructor
		 */
		Quaternion( const Quaternion &q )
			: _data( q._data )
		{
		}

		/**
			\brief Destructor
		 */
		~Quaternion( void )
		{
		}

		/**
			\brief Assignment operator
		 */
		Quaternion operator=( const Quaternion &q )
		{
			_data = q._data;
			return *this;
		}

		bool operator==( const Quaternion &q ) const
		{
			return ( _data == q._data );
		}

		bool operator!=( const Quaternion &q ) const
		{
			return ( _data != q._data );
		}

		Vector3Impl getImaginary( void ) const
		{
			return _data.xyz();
		}

		void setImaginary( const Vector3Impl &value )
		{
			_data[ 0 ] = value[ 0 ];
			_data[ 1 ] = value[ 1 ];
			_data[ 2 ] = value[ 2 ];
		}

		PRECISION getReal( void ) const
		{
			return _data[ 3 ];
		}

		void setReal( PRECISION value )
		{
			_data[ 3 ] = value;
		}

		template< typename U >
		friend Quaternion< U > operator+( const Quaternion< U > &q, const Quaternion< U > &r );

		template< typename U >
		friend Quaternion< U > operator-( const Quaternion< U > &q, const Quaternion< U > &r );

		template< typename U >
		friend Quaternion< U > operator*( const Quaternion< U > &q, const Quaternion< U > &r );

		template< typename U, typename V >
		friend Quaternion< U > operator*( const Quaternion< U > &q, V s );

		template< typename U, typename V >
		friend Quaternion< U > operator*( V s, const Quaternion< U > &q );

		template< typename U, typename V >
		friend Quaternion< U > operator/( const Quaternion< U > &q, V s );

		template< typename U >
		friend std::ostream &operator<<( std::ostream &output, const Quaternion &input );

		/**
			\breif Calculates the conjugate for this quaternion

			The conjugate is calculated as:
			\code
			q* = (qv, qw)* = (-qv, qw)
			\endcode
		 */
		Quaternion getConjugate( void ) const
		{
			return Quaternion( _data[ 3 ], -_data[ 0 ], -_data[ 1 ], -_data[ 2 ] );
		}

		Quaternion &makeIdentity( void )
		{
			_data[ 0 ] = 0;
			_data[ 1 ] = 0;
			_data[ 2 ] = 0;
			_data[ 3 ] = 1;
			return *this;
		}

		double getSquaredNorm( void ) const
		{
			return _data * _data;
		}

		double getNorm( void ) const
		{
			return std::sqrt( _data * _data );
		}

		Quaternion getInverse( void ) const
		{
			double squaredN = getSquaredNorm();
			Quaternion conjugate = getConjugate();
			if ( Numeric< PRECISION >::equals( squaredN, 1 ) ) {
				// avoid a division if the norm is 1
				return conjugate;
			}

			return conjugate / squaredN;
		}

		Quaternion &normalize( void )
		{
			double n = getNorm();
			if ( n == 0 ) {
				return *this;
			}
			*this = *this / n;
			return *this;
		}

		void fromAxisAngle( const Vector3Impl &axis, PRECISION angle )
		{
			double sinTheta = std::sin( 0.5 * angle );
			double cosTheta = std::cos( 0.5 * angle );

			_data[ 0 ] = axis[ 0 ] * sinTheta;
			_data[ 1 ] = axis[ 1 ] * sinTheta;
			_data[ 2 ] = axis[ 2 ] * sinTheta;
			_data[ 3 ] = cosTheta;
		}

		/**
		 	\brief Computes the rotation from the compositions of two quaternions
		 */
		Quaternion rotate( Quaternion q1 ) const
		{
			PRECISION w0 = getReal();
			PRECISION x0 = getImaginary()[ 0 ];
			PRECISION y0 = getImaginary()[ 1 ];
			PRECISION z0 = getImaginary()[ 2 ];

			PRECISION w1 = q1.getReal();
			PRECISION x1 = q1.getImaginary()[ 0 ];
			PRECISION y1 = q1.getImaginary()[ 1 ];
			PRECISION z1 = q1.getImaginary()[ 2 ];
            
            Quaternion q( w0 * w1 - x0 * x1 - y0 * y1 - z0 * z1,
						  w0 * x1 + x0 * w1 + y0 * z1 - z0 * y1,
						  w0 * y1 + y0 * w1 + z0 * x1 - x0 * z1,
						  w0 * z1 + z0 * w1 + x0 * y1 - y0 * x1 );
            return q.normalize();
		}

		void getRotationMatrix( Matrix< 3, PRECISION > &output )
		{
			float x = _data[ 0 ];
			float y = _data[ 1 ];
			float z = _data[ 2 ];
			float w = _data[ 3 ];

			float twoXX = 2 * x * x;
			float twoXY = 2 * x * y;
			float twoXZ = 2 * x * z;
			float twoXW = 2 * x * w;
			float twoYY = 2 * y * y;
			float twoYZ = 2 * y * z;
			float twoYW = 2 * y * w;
			float twoZZ = 2 * z * z;
			float twoZW = 2 * z * w;

			output[ 0 ] = 1 - twoYY - twoZZ;
			output[ 1 ] = twoXY - twoZW;
			output[ 2 ] = twoXZ + twoYW;
			output[ 3 ] = twoXY + twoZW;
			output[ 4 ] = 1 - twoXX - twoZZ;
			output[ 5 ] = twoYZ - twoXW;
			output[ 6 ] = twoXZ - twoYW;
			output[ 7 ] = twoYZ + twoXW;
			output[ 8 ] = 1 - twoXX - twoYY;
		}

	private:
		Vector4Impl _data;
	};

	template< typename U >
	Quaternion< U > operator+( const Quaternion< U > &q, const Quaternion< U > &r )
	{
		return Quaternion< U >( q._data + r._data );
	}

	template< typename U >
	Quaternion< U > operator-( const Quaternion< U > &q, const Quaternion< U > &r )
	{
		return Quaternion< U >( q._data - r._data );
	}

	template< typename U >
	Quaternion< U > operator*( const Quaternion< U > &q, const Quaternion< U > &r )
	{
		Vector< 3, U > qImaginary = q.getImaginary();
		U qReal = q.getReal();
		Vector< 3, U > rImaginary = r.getImaginary();
		U rReal = r.getReal();


		// TODO: this should be replaced by a faster method
		return Quaternion< U >( qReal * rReal - qImaginary * rImaginary,
								( qReal * rImaginary ) + ( qImaginary * rReal ) + ( qImaginary ^ rImaginary ) );
	}

	template< typename U, typename V >
	Quaternion< U > operator*( const Quaternion< U > &q, V s )
	{
		return Quaternion< U >( q._data * s );
	}

	template< typename U, typename V >
	Quaternion< U > operator*( V s, const Quaternion< U > &q )
	{
		return Quaternion< U >( q._data * s );
	}

	template< typename U, typename V >
	Quaternion< U > operator/( const Quaternion< U > &q, V s )
	{
		if ( s == 0 ) {
			s = Numeric< U >::ZERO_TOLERANCE;
		}
		
		return Quaternion< U >( q._data / s );
	}

	template< typename U >
	std::ostream &operator<<( std::ostream &out, const Quaternion< U > &q )
	{
		out << std::setiosflags( std::ios::fixed | std::ios::showpoint  )
			<< std::setprecision( 10 )
		   	<< "[r = " << q.getReal() << ", i = " << q.getImaginary() << "]";
		return out;
	}

	typedef Quaternion< int > Quaternion4i;
	typedef Quaternion< float > Quaternion4f;

}

#endif

