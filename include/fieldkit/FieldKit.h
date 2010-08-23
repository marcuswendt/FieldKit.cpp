/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

//! FieldKit globals
namespace fieldkit {};
// Create a namepace alias as shorthand for fieldkit::
namespace fk = fieldkit;

#include "fieldkit/Logger.h"

// -- STL ---------------------------------------------------------------------
#include <list>
#include <vector>
#include <string>

using namespace std;
using std::endl;

// -- Boost ------------------------------------------------------------------
#include <boost/foreach.hpp>

// -- Cinder -----------------------------------------------------------------
#include "cinder/Cinder.h"
#include "cinder/Vector.h"

using ci::Vec2f;
using ci::Vec3f;
using ci::EPSILON_VALUE;