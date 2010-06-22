/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include <list>
#include <vector>
#include <string>

#include <boost/foreach.hpp>
#include <boost/format.hpp> 

#include "cinder/Cinder.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"

using boost::format;
using boost::io::group;

using namespace std;

// uses from Cinder
using ci::Vec3f;
using ci::EPSILON_VALUE;
using namespace ci::app;

namespace fieldkit {
	// fieldkit globals
};

// Create a namepace alias as shorthand for fieldkit::
namespace fk = fieldkit;