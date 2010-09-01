/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#pragma once

#include "fieldkit/vision/VisionKit.h"
#include "fieldkit/vision/camera/Camera.h"
#include "fieldkit/vision/vision/CVFrameProcessor.h"

namespace fieldkit { namespace vision 
{
	class Vision
	{
	public:
		Vision();
		~Vision();
		
		int init();
		int start();
		virtual int update();
		int stop();
		
		// setters
		void setSize(int width, int height);
		void setFramerate(int fps);
		void setCamera(Camera *camera);
		void setProcessor(CVFrameProcessor *processor);
		
		// getters
		int getWidth() { return width; };
		int getHeight() { return height; };
		int getFPS() { return fps; };
		
		Camera* getCamera() { return camera; };
		CVFrameProcessor* getProcessor() { return processor; };
		
	protected:
		bool isInitialized;
		bool isStarted;
		int width, height, fps;
		Camera *camera;
		CVFrameProcessor *processor;
		
	private:
		int err;
	};

} } // namespace fieldkit::vision
