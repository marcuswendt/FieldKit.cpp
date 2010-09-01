/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 01/09/2010.
 */

#pragma once

// FieldKit
#include "fieldkit/FieldKit.h"
using namespace fieldkit;

// OpenCV
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv/cvcompat.h"

// Constants
namespace fieldkit { namespace vision {

	#define consti const static int

	// error codes
	consti VISION_ERROR	= 0;
	consti VISION_SUCCESS	= 1;

	consti ERR_INVALID_ARGUMENT =		-10;

	consti ERR_NOT_CREATED =			-20;
	consti ERR_NOT_INITIALIZED =		-21;

	// camera types
	consti CAMERA_OPENCV =				0;
	consti CAMERA_OPENCV_FIRST =		1;
	consti CAMERA_OPENCV_SECOND	=		2;
	consti CAMERA_OPENCV_THIRD =		3;
	consti CAMERA_OPENCV_FOURTH	=		4;
	consti CAMERA_PTGREY_BUMBLEBEE =	10;
	consti CAMERA_CINDER =			11;

	// frame processor properties
	consti PROC_BACKGROUND =			0;
	consti PROC_THRESHOLD =				1;
	consti PROC_DILATE =				2;
	consti PROC_ERODE =					3;
	consti PROC_CONTOUR_MIN =			4;
	consti PROC_CONTOUR_MAX =			5;
	consti PROC_CONTOUR_REDUCE =		6;
	consti PROC_TRACK_RANGE =			7;
	consti PROC_HULL_MINDEPTH =			8;
	consti PROC_GESTURE_RANGE =			9;

	// defaults
	consti VISION_DEFAULT_WIDTH = 320;
	consti VISION_DEFAULT_HEIGHT = 240;
	consti VISION_DEFAULT_FPS = 30;
	consti VISION_BLOB_COUNT = 30;

	//
	// INLINE FUNCTIONS
	//
	inline int ptDistanceSquared(CvPoint a, CvPoint b)
	{
		int dx = a.x - b.x;
		int dy = a.y - b.y;	
		return dx*dx + dy*dy;
	}

	inline float pt64fDistanceSquared(CvPoint2D64f a, CvPoint2D64f b)
	{
		double dx = a.x - b.x;
		double dy = a.y - b.y;	
		return (float) (dx*dx + dy*dy);
	}

	inline int iDistanceSquared(int x1, int y1, int x2, int y2)
	{
		int dx = x1 - x2;
		int dy = y1 - y2;	
		return dx*dx + dy*dy;
	}

	inline float dDistanceSquared(double x1, double y1, double x2, double y2)
	{
		double dx = x1 - x2;
		double dy = y1 - y2;	
		return(float)( dx*dx + dy*dy);
	}

	inline double dSqr(double a)
	{
		return a * a;
	}

	// --
	inline int isLittleEndian()
	{
		short int word = 0x0001;
		char *byte = (char *) &word;
		return(byte[0] ? 1 : 0);
	}


	inline int little2bigEndianINT(int i)
	{
		return((i&0xff)<<24)+((i&0xff00)<<8)+((i&0xff0000)>>8)+((i>>24)&0xff);
	}

} } // namespace fieldkit::vision