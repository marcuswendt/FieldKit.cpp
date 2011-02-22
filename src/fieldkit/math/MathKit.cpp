/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 28/06/2010.
 */

#include "fieldkit/math/MathKit.h"

#include "cinder/Rand.h"

namespace fieldkit { namespace math {
	
using ci::Vec3f;

float invSqrt(float x) {
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // store floating-point bits in integer
	i = 0x5f3759d5 - (i >> 1); // initial guess for Newton's method
	x = *(float*)&i; // convert new bits into float
	x = x*(1.5f - xhalf*x*x); // One round of Newton's method
	return x;
}

bool flipCoin( float chance )
{
	return randFloat() > chance;
}


// -- Random Float ----------------------------------------------------------
float randFloat() 
{
	return ci::Rand::randFloat();
}

float randFloat( float range )
{
	return ci::Rand::randFloat(range);
}

float randFloat( float from, float to )
{
	return ci::Rand::randFloat(from, to);
}


// -- Random Vec3 -----------------------------------------------------------
Vec3f randVec3f()
{
	return Vec3f(randFloat(), randFloat(), randFloat());
}

Vec3f randVec3f( float range )
{
	return Vec3f(randFloat(range), randFloat(range), randFloat(range));
}

Vec3f randVec3f( float from, float to )
{
	return Vec3f(randFloat(from, to), randFloat(from, to), randFloat(from, to));
}

Vec3f randNormal()
{
	return randVec3f(-1.0f, 1.0f);
}

} } // fieldkit::math