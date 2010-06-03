/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef CV_FRAME_PROCESSOR_H
#define CV_FRAME_PROCESSOR_H

#include <map>

#include "fieldkit/vision/camera/Camera.h"
#include "fieldkit/vision/vision/CVProperty.h"
#include "fieldkit/vision/vision/CVImageCache.h"

namespace fk { namespace vision { 

	class Stage
	{
	public:
		int key;
		bool isEnabled;
		IplImage* image;
		
		Stage(int key) {
			this->key = key;
			this->isEnabled = true;
			this->image = NULL;
		}
	};
	
	//
	// base class for all frame processor types
	//
	class CVFrameProcessor
	{
	public:
		CVFrameProcessor();
		virtual ~CVFrameProcessor();
		
		virtual int init() = 0;
		virtual int update(Camera *camera) = 0;

		// size
		virtual void setSize(int width, int height);
		CvSize getSize() { return size; };

		// roi
		virtual void setROI(int x, int y, int w, int h);
		CvRect getROI() { return roi; };
		
		// stages
		Stage* getStage(int key);
		void setStageEnabled(bool enabled, int key=-1);
		virtual IplImage* getImage(int key);
		int getStageNum();
			
		// sliders
		void addProperty(int key, float min=0, float max=1);
		CVProperty* getProperty(int key);
		void set(int key, float value);
		float get(int key);

		// warp
		virtual void setWarp(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {};
		
	protected:
		CvSize size;
		CvRect roi;
		CvSize roiSize;
		CVImageCache* cache;
		
		CvScalar cWhite, cGrey, cDarkGrey, cLightGrey, cBlack;
		CvScalar cRed, cGreen, cBlue;
		
		void initStages(int num);
		void copyStage(int stage, IplImage *image);
		
	private:	
		std::map<int, CVProperty*> properties;
		std::map<int, Stage*> stages;
		
		inline int max(int a, int b) { return a > b ? a : b; };
		inline int min(int a, int b) { return a > b ? b : a; };
	};
} } // namespace fk::vision

#endif