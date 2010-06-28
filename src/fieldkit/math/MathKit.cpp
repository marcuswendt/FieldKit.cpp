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

const  bool fieldkit::math::flipCoin( float chance )
{
	return randFloat() > chance;
}


// -- Random Float ----------------------------------------------------------
const float fieldkit::math::randFloat() 
{
	return ci::Rand::randFloat();
}

const float fieldkit::math::randFloat( float range )
{
	return ci::Rand::randFloat(range);
}

const float fieldkit::math::randFloat( float from, float to )
{
	return ci::Rand::randFloat(from, to);
}


// -- Random Vec3 -----------------------------------------------------------
const Vec3f fieldkit::math::randVec3f()
{
	return Vec3f(randFloat(), randFloat(), randFloat());
}

const Vec3f fieldkit::math::randVec3f( float range )
{
	return Vec3f(randFloat(range), randFloat(range), randFloat(range));
}

const Vec3f fieldkit::math::randVec3f( float from, float to )
{
	return Vec3f(randFloat(from, to), randFloat(from, to), randFloat(from, to));
}

const Vec3f fieldkit::math::randNormal()
{
	return randVec3f(-1.0f, 1.0f);
}
