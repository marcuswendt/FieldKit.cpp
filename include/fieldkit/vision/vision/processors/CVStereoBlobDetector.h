/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef CV_STEREO_BLOB_DETECTOR_H
#define CV_STEREO_BLOB_DETECTOR_H

#include "fieldkit/vision/vision/CVFrameProcessor.h"
#include "fieldkit/vision/vision/processors/CVBlobDetector.h"

namespace fk { namespace vision 
{
	//
	// OpenCV Stereo Blob Detector
	//
	class CVStereoBlobDetector : public CVFrameProcessor
	{
	public:
		enum StageName { STAGE_COMBINED = CVBlobDetector::STAGE_MAX,
						 STAGE_MAX
		};
		
		enum Image {
			IMAGE_BLUE = 10,
			IMAGE_COMPOSED = 20,
		};
		
		CVStereoBlobDetector();
		~CVStereoBlobDetector();
		
		int init();
		int update(Camera *camera);
		
		// Overridden 
		void set(int slider, float value);
		void setStageEnabled(bool enabled, int key=-1);
		
		// Getters
		IplImage* getImage(int key);
		Blob** getBlobs();
		int getBlobCount();
		
		// Setters
		void setSize(int width, int height);
		void setROI(int x, int y, int w, int h);
		void setWarp(float sx1, float sy1,
					 float sx2, float sy2,
					 float sx3, float sy3,
					 float sx4, float sy4);	
		
	protected:		
		CVBlobDetector *left;
		CVBlobDetector *right;
		Blob **blobs;
		int blobNum;
		
		void findFingerPoints();
		void findFurthestPoint(CvPoint2D64f *result, Blob *blob);
		void drawFingerPoints();
		
	private:
		int err;
	};
} } // namespace fk::vision
#endif