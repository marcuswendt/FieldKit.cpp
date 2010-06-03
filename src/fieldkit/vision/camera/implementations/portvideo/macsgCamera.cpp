/*  portVideo, a cross platform camera framework
    Copyright (C) 2005 Martin Kaltenbrunner <mkalten@iua.upf.es>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "fieldkit/vision/camera/implementations/portvideo/macsgCamera.h"

macsgCamera::macsgCamera()
{
	cameraID = -1;
	buffer = NULL;
	pbuffer = NULL;	
}

macsgCamera::~macsgCamera()
{
	if (buffer!=NULL) delete []buffer;
	if (pbuffer!=NULL) delete []pbuffer;
}

bool macsgCamera::findCamera() {
	int num_components = 0;
	Component c = 0;
	ComponentDescription cd;
     
	cd.componentType = SeqGrabComponentType;
	cd.componentSubType = 0;
	cd.componentManufacturer = 0;
	cd.componentFlags = 0;
	cd.componentFlagsMask = 0;
     
	while((c = FindNextComponent(c, &cd)) != 0) {
		// add component c to the list
		num_components++;  
	}
            
    //fprintf(stdout, "number of SGcomponents: %d\n",num_components);

	if(num_components==0) {
		fprintf(stderr, "no quicktime camera found\n");
		return false;
	} else {
		cameraID = 0;
		return true;
	}
}

bool macsgCamera::initCamera(int width, int height, bool colour) {

	if (cameraID < 0) return false;

	this->width = width;
	this->height = height;
	this->colour = colour;
	this->fps = 30;
	
	bytes = (colour?3:1);
	int rowlength= width*bytes;
	
	switch (colour) {
		case false: {
			pixelFormat = k8IndexedGrayPixelFormat;
			//pixelFormat = kYVYU422PixelFormat;
			break;
		} case true: {
			pixelFormat = k24RGBPixelFormat;
			break;
		} 
	}
	
	OSErr result;
    Rect srcRect = {0,0, height, width};
   
    sg = OpenDefaultComponent(SeqGrabComponentType, 0);
    if(sg==NULL){
		fprintf(stderr, "could not open default component\n");
    }
	
	result = SGInitialize(sg);
    if(result!=noErr){
         fprintf(stdout, "could not initialize SG\n");
    }
    
	
    result = SGSetDataRef(sg, 0, 0, seqGrabDontMakeMovie);
        if (result != noErr){
             fprintf(stdout, "dataref failed\n");
        }
        
    result = SGNewChannel(sg, VideoMediaType, &vc);		
    if(result!=noErr){
         //fprintf(stdout, "could not make new SG channnel\n");
		 return false;
    }

//	result = SGSettingsDialog ( sg, vc ,0 ,NULL ,seqGrabSettingsPreviewOnly,NULL,0);
//    if(result!=noErr){
//         fprintf(stdout, "could not get settings from dialog\n");
//    }
    
    result = SGSetChannelBounds(vc, &srcRect);
    if(result!=noErr){
         fprintf(stdout, "could not set SG ChannelBounds\n");
    }
	
	/*result = SGSetFrameRate (vc, fps);
    if(result!=noErr){
         fprintf(stdout, "could not set SG FrameRate\n");
    }*/
      
    result = SGSetChannelUsage(vc, seqGrabPreview);
    if(result!=noErr){
         fprintf(stdout, "could not set SG ChannelUsage\n");
    }
    
	result = SGSetChannelPlayFlags(vc, channelPlayAllData);
	if(result!=noErr){
         fprintf(stdout, "could not set SG AllData\n");
	};

	buffer = new unsigned char[width*height*bytes];

	result = QTNewGWorldFromPtr (&srcGWorld,
									pixelFormat,
                                    &srcRect, 
                                    NULL, 
                                    NULL, 
                                    0, 
                                    buffer, 
                                    rowlength);
        
	if (result!= noErr)
  	{
		fprintf(stdout, "%d error at QTNewGWorldFromPtr\n", result);
		delete []buffer;
		buffer = NULL;
		return false;
	}  
	
    if (srcGWorld == NULL)
	{
		fprintf(stdout, "Could not allocate off screen\n");
		delete []buffer;
		buffer = NULL;
		return false;
	}
	
    result = SGSetGWorld(sg,(CGrafPtr)srcGWorld, NULL);
	if (result != noErr) {
		fprintf(stdout, "Could not set SGSetGWorld\n");
		delete []buffer;
		buffer = NULL;
		return false;
	}

    result = SGPrepare(sg, TRUE, FALSE);
    if (result != noErr) {
            fprintf(stderr, "SGPrepare Preview failed\n");
	}

	pbuffer = new unsigned char[width*height*bytes];
	return true;
}

unsigned char* macsgCamera::getFrame()
{
	OSErr result = SGIdle(sg);
   if (result != noErr) {
            fprintf(stderr, "SGIdle failed\n");
			return NULL;
	}
	
	switch (colour) {
		case false: {
			for(int i=0;i<width*height;i++) {
				pbuffer[i]=256-buffer[i];
			}
			break;
		}
		case true: {
			memcpy(pbuffer,buffer,width*height*bytes);
			break;
		}
	}
	
	return pbuffer;
}

bool macsgCamera::startCamera()
{
    OSErr result = SGStartPreview(sg);
    if (result != noErr) {
            fprintf(stderr, "SGStartPreview failed\n");
	}

	return true;
}

bool macsgCamera::stopCamera()
{
	//SGStopPreview(sg);
	return true;
}

bool macsgCamera::stillRunning() {
	return true;
}

bool macsgCamera::resetCamera()
{
  return (stopCamera() && startCamera());
}

bool macsgCamera::closeCamera()
{
    if (vc) {
		if (::SGDisposeChannel(sg, vc)) {
			fprintf (stderr, "Unable to dispose a video channel\n");
		}
		vc = NULL;
	}
	if (sg) {
		if (::CloseComponent(sg)) {
			fprintf(stderr, "Unable to dispose a sequence grabber component\n");
		}
		sg = NULL;
        if (srcGWorld) {
			::DisposeGWorld(srcGWorld);
			srcGWorld = NULL;
		}
    }
	
	return true;
}

void macsgCamera::showSettingsDialog() {
	printf("not yet implemented\n");
}
