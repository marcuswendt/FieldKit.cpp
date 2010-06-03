/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#include "fieldkit/vision/camera/implementations/PTGreyBumblebee2.h"

namespace fk { namespace vision 
{
	PTGreyBumblebee2::PTGreyBumblebee2()
	{
		width = bb2Width;
		height = bb2Height;
	}
	
	
	// -------------------------------------------------------------------------
	// INIT
	// -------------------------------------------------------------------------
	int PTGreyBumblebee2::init()
	{
		if(isStarted) return ERROR;
		
		dc1394camera_list_t* list;
		
		// init libDC1394
		dc1394 = dc1394_new();
		err = dc1394_camera_enumerate(dc1394, &list);
		
		printf("dc1394_camera_enumerate %i\n", list->num);
		
		if (list->num == 0) return ERROR;

		// init camera
		camera = dc1394_camera_new(dc1394, list->ids[0].guid);
		dc1394_camera_free_list(list);

		// print camera info
		printf("---------------------------------------------------------------------------------\n");
		printf("CAMERA\n");
		printf("---------------------------------------------------------------------------------\n");
		dc1394_camera_print_info(camera, stdout);
		printf("---------------------------------------------------------------------------------\n");
		
		// allocate memory for image buffers
		//bufferDeinterlace = (ImagePtr) malloc(bb2ImageSize * 2);
		// TODO need to create IplImage
		
		// load factory defaults
		err = err = dc1394_memory_load(camera, 0);
		if(err != DC1394_SUCCESS) return ERROR;
		
		err = dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
		if(err != DC1394_SUCCESS) return ERROR;
		
		err = dc1394_video_set_mode(camera, DC1394_VIDEO_MODE_FORMAT7_3);
		if(err != DC1394_SUCCESS) return ERROR;
		
		err = dc1394_video_set_operation_mode(camera, DC1394_OPERATION_MODE_1394B);
		if(err != DC1394_SUCCESS) return ERROR;
		
		
		//printf("---------------------------------------------------------------------------------\n");
		//printf("FEATURES\n");
		//printf("---------------------------------------------------------------------------------\n");	
	
		/*
		setMode(FEATURE_EXPOSURE, MODE_MANUAL);
		setMode(FEATURE_SHUTTER, MODE_MANUAL);
		setMode(FEATURE_GAIN, MODE_MANUAL);
		
		setValue(FEATURE_EXPOSURE, 0.5);
		getValue(FEATURE_EXPOSURE);
		*/
		
		//dc1394featureset_t features;
		//dc1394_feature_get_all(camera, &features);
		//dc1394_feature_print_all(&features, stdout);
		
		/*
		 dc1394feature_info_t *feature;
		 
		 
		 for (int i=0; i<DC1394_FEATURE_NUM; i++) {
		 feature = &(features.feature[i]);
		 
		 switch(feature->id) {
		 case DC1394_FEATURE_EXPOSURE:
		 break;
		 
		 case DC1394_FEATURE_SHUTTER:
		 break;
		 
		 case DC1394_FEATURE_GAIN:
		 break;
		 }
		 }
		 */
		//printf("---------------------------------------------------------------------------------\n");
		
		isInitialized = true;
		
		setFramerate(bb2DefaultFPS);
		
		return SUCCESS;
	}
	
	// -------------------------------------------------------------------------
	// START
	// -------------------------------------------------------------------------
	int PTGreyBumblebee2::start()
	{
		if(!isInitialized) return ERROR;
		if(isStarted) return ERROR;

		err = dc1394_capture_setup(camera, 8, DC1394_CAPTURE_FLAGS_DEFAULT);
		if(err != DC1394_SUCCESS) return ERROR;
		
		err = dc1394_video_set_transmission(camera, DC1394_ON);
		if(err != DC1394_SUCCESS) return ERROR;
		
		isStarted = true;
		return SUCCESS;
	}
	
	// -------------------------------------------------------------------------
	// STOP
	// -------------------------------------------------------------------------
	int PTGreyBumblebee2::stop()
	{
		if(!isInitialized) return ERROR;
		if(!isStarted) return ERROR;
		
		err = dc1394_video_set_transmission(camera, DC1394_OFF);
		if(err != DC1394_SUCCESS) return ERROR;

		err = dc1394_capture_stop(camera);
		if(err != DC1394_SUCCESS) return ERROR;
		
		isStarted = false;
		return SUCCESS;		
	}
	
	// -------------------------------------------------------------------------
	// CLOSE
	// -------------------------------------------------------------------------
	int PTGreyBumblebee2::close()
	{
		if(!isInitialized) return ERROR;
		if(isStarted) return ERROR;
		
		dc1394_camera_free(camera);
		dc1394_free(dc1394);

		free(bufferDeinterlace);		
		return SUCCESS;		
	}
	
	// -------------------------------------------------------------------------
	// UPDATE
	// -------------------------------------------------------------------------
	int PTGreyBumblebee2::update()
	{
		if(!isInitialized) return ERROR;
		if(!isStarted) return ERROR;
				
		// 1. retrieve frame from cameras ringbuffer
		err = dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_WAIT, &frame);
		if(err!=DC1394_SUCCESS) return ERROR;
	
		// 2. convert stereo image to two mono images
		//err = dc1394_deinterlace_stereo(frame->image, bufferDeinterlace, width, height * 2);
		// TODO need to create ipl image
		if(err!=DC1394_SUCCESS) return ERROR;
		bufferLeft = bufferDeinterlace;
		bufferRight = bufferDeinterlace + bb2ImageSize;

		// 3. enqueue frame in the ringbuffer again
		err = dc1394_capture_enqueue(camera, frame);
		if(err!=DC1394_SUCCESS) return ERROR;
		
		return SUCCESS;
	}
	
	// -------------------------------------------------------------------------
	// HELPERS
	// -------------------------------------------------------------------------
	#pragma mark -- Helpers --
	int PTGreyBumblebee2::setFramerate(int framerate)
	{
		if(!isInitialized) return ERROR;
		if(isStarted) return ERROR;

		printf("PTGreyBumblebee2: setting framerate to %i \n", framerate);

		// sets the framerate => bb2PacketSize * fps
		err = dc1394_format7_set_roi(camera, DC1394_VIDEO_MODE_FORMAT7_3, DC1394_COLOR_CODING_MONO16, bb2PacketSize * framerate, 0, 0, width, height);
		if(err!=DC1394_SUCCESS) return ERROR;
		return SUCCESS;
	};
	
	int PTGreyBumblebee2::setMode(Feature feature, Mode mode)
	{
		dc1394feature_t _feature = (dc1394feature_t)feature;

		// check if feature is present
		dc1394bool_t isPresent;
		err = dc1394_feature_is_present(camera, _feature, &isPresent);
		if(!isPresent || err != DC1394_SUCCESS) return ERROR;

		// set mode
		dc1394feature_mode_t _mode = (dc1394feature_mode_t) mode;
		err = dc1394_feature_set_mode(camera, _feature, _mode);
		return (err == DC1394_SUCCESS) ? SUCCESS : ERROR;
	}
	
	int PTGreyBumblebee2::setValue(Feature feature, float normalizedValue)
	{
		dc1394feature_t _feature = (dc1394feature_t)feature;

		// check if feature is present
		dc1394bool_t isPresent;
		err = dc1394_feature_is_present(camera, _feature, &isPresent);
		if(!isPresent || err != DC1394_SUCCESS) return ERROR;
		
		// make sure the normalizedValue is within [0, 1] range
		if(normalizedValue > 1.0f) normalizedValue = 1.0f;
		if(normalizedValue < 0.0f) normalizedValue = 0.0f;
		
		// get range
		uint32_t min, max;
		err = dc1394_feature_get_boundaries(camera, _feature, &min, &max);
		if(err != DC1394_SUCCESS) return ERROR;
		
		// set value
		uint32_t value = min + (normalizedValue * (max - min));
		err = dc1394_feature_set_value(camera, _feature, value);
		return (err == DC1394_SUCCESS) ? SUCCESS : ERROR;
	}
	
	float PTGreyBumblebee2::getValue(Feature feature)
	{
		dc1394feature_t _feature = (dc1394feature_t)feature;
		
		// check if feature is present
		dc1394bool_t isPresent;
		err = dc1394_feature_is_present(camera, _feature, &isPresent);
		if(!isPresent || err != DC1394_SUCCESS) return 0;
		
		// get range
		uint32_t min, max;
		err = dc1394_feature_get_boundaries(camera, _feature, &min, &max);
		if(err != DC1394_SUCCESS) return 0;
		
		// get value
		uint32_t value;
		err = dc1394_feature_get_value(camera, _feature, &value);
		if(err != DC1394_SUCCESS) return 0;
		
		// return normalized value
		return (value - min) / (float)(max - min);
	}
	
	IplImage* PTGreyBumblebee2::getImage(int channel)
	{
		LOG_ERR("CURRENTLY DOESNT WORK, NEED TO REIMPLEMENT");
		return NULL;
		//return (channel == 0) ? bufferLeft : bufferRight;
	};
} } // namespace fk::vision