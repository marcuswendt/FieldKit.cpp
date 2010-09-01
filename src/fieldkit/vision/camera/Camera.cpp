/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#include "fieldkit/vision/camera/Camera.h"

using namespace fieldkit::vision;

Camera::Camera() {
	isInitialized = false;
	isStarted = false;
	color = false;
	width = VISION_DEFAULT_WIDTH;
	height = VISION_DEFAULT_HEIGHT;
	fps = VISION_DEFAULT_FPS;
}
	
Camera::~Camera() {
	this->close();
}

void Camera::setSize(int width, int height) {
	if(isStarted) {
		LOG_ERROR("Camera: Cannot set size, since camera is already started.")
		return;
	}
	this->width = width;
	this->height = height;
}
	
void Camera::setFramerate(int fps) {
	if(isStarted) {
		LOG_ERROR("Camera: Cannot set framerate, since camera is already started.")
		return;
	}
	this->fps = fps;
}

int Camera::init() {
	isInitialized = true;
	return VISION_SUCCESS;
}
	
int Camera::start() {
	// check if we need to initialize first
	if(!isInitialized) {
		err = this->init();
		if(err != VISION_SUCCESS) return err;
	}
		
	isStarted = true;
		
	return VISION_SUCCESS;
}
	
int Camera::update() {
	return VISION_SUCCESS;
}

int Camera::stop() {
	isStarted = false;
	return VISION_SUCCESS;
}
	
int Camera::close() {
	if(isStarted) 
		this->stop();
			
	isInitialized = false;
	return VISION_SUCCESS;
}

IplImage* Camera::getImage(int channel) {
	return NULL;
}