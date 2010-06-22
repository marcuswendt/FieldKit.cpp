/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef CV_IMAGE_CACHE_H
#define CV_IMAGE_CACHE_H

#include <map>
#include "fieldkit/vision/opencv.h"


namespace fieldkit { namespace vision {

	class CVImageCache
	{
	public:
		const static int IMAGE_TMP = 100;
		const static int IMAGE_STAGE = 200;

		CVImageCache();
		~CVImageCache();

		// image accessors
		IplImage* get(int key, CvSize size, int depth=IPL_DEPTH_8U, int channels=1);
		
		// utilities
		IplImage* getTmp(int key, CvSize size, int depth=IPL_DEPTH_8U, int channels=1);
		IplImage* getTmp(int key, IplImage* target);
		IplImage* getStage(int key, CvSize size, int depth=IPL_DEPTH_8U, int channels=1);
		
		std::map<int, IplImage*>::iterator begin() { return map.begin(); };
		std::map<int, IplImage*>::iterator end() { return map.end(); };
		
	private:
		std::map<int, IplImage*> map;
	};
} } // namespace fieldkit::vision
#endif