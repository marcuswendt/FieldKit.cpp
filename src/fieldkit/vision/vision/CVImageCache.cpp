/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#include "fieldkit/vision/vision/CVImageCache.h"

namespace fk { namespace vision
{
	CVImageCache::CVImageCache() 
	{
		map = std::map<int, IplImage*>();	
	}
	
	CVImageCache::~CVImageCache()
	{
		std::map<int, IplImage*>::iterator i;
		for(i = map.begin(); i != map.end(); i++) {
			int key = (*i).first;
			IplImage *image = (*i).second;
			// do not release images with key 100 (which is the input image provided by the camera)
			if(key != 100) cvReleaseImage(&image);
		}
	}
	
	// ------------------------------------------------------------------------------------------------------------
	//#pragma mark -- CV related --
	// ------------------------------------------------------------------------------------------------------------
	
	IplImage* CVImageCache::get(int key, CvSize size, int depth, int channels)
	{
		std::map<int, IplImage*>::iterator i = map.find(key);
		IplImage* image;
		
		// key does exist
		if(i != map.end()) {
			// check if requested size matches the size of the cached image
			image = (*i).second;
			if(image->width == size.width && image->height == size.height) {
				return image;
			} else {
				map.erase(i);
				cvReleaseImage(&image);
			}
		}
		
		//printf("CVImageCache::get key: %i width: %i height: %i depth: %i channels: %i \n", key, size.width, size.height, depth, channels);
		image = cvCreateImage(size, depth, channels);
		map.insert(std::make_pair(key, image));
		return image;
	}
	
	IplImage* CVImageCache::getTmp(int key, CvSize size, int depth, int channels)
	{
		return get(IMAGE_TMP + key, size, depth, channels);
	}
	
	IplImage* CVImageCache::getTmp(int key, IplImage* target)
	{
		int w, h;
		if(target->roi == NULL) {
			w = target->width;
			h = target->height;
		} else {
			w = target->roi->width;
			h = target->roi->height;
		}
		
		return get(key, cvSize(w, h), target->depth, target->nChannels);
	}
	
	IplImage* CVImageCache::getStage(int key, CvSize size, int depth, int channels)
	{
		return get(IMAGE_STAGE + key, size, depth, channels);		
	}
} } // namespace fk::vision