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


// -- Random Number Generators ---------------------------------------------
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

const  bool fieldkit::math::flipCoin( float chance )
{
	return randFloat() > chance;
}
