/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#include "fieldkit/vision/vision/processors/CVBlobDetector.h"

namespace fieldkit { namespace vision 
{
	// -------------------------------------------------------------------------
	//#pragma mark -- Constructor --
	// -------------------------------------------------------------------------
	CVBlobDetector::CVBlobDetector()
	{
		initStages(STAGE_MAX);
		cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 2);
		useAdaptiveTresholding = false;
	}

	// -------------------------------------------------------------------------
	//#pragma mark -- Destructor --
	// -------------------------------------------------------------------------
	CVBlobDetector::~CVBlobDetector()
	{
		cvReleaseMat(&warpMatrix);	
		//free(&font);
	}

	// -------------------------------------------------------------------------
	//#pragma mark -- Init --
	// -------------------------------------------------------------------------
	int CVBlobDetector::init()
	{
		// properties
		addProperty(PROC_BACKGROUND, "background");
		addProperty(PROC_THRESHOLD, "theshold", 0, 255);
		addProperty(PROC_DILATE, "dilate", 0, 35);
		addProperty(PROC_ERODE, "erode", 0, 35);
		addProperty(PROC_CONTOUR_MIN, "contour min");
		addProperty(PROC_CONTOUR_MAX, "contour max");
		addProperty(PROC_CONTOUR_REDUCE, "contour reduce", 0, 10);
		addProperty(PROC_TRACK_RANGE, "track range");
		addProperty(PROC_HULL_MINDEPTH, "hull min depth",0.0f,30.0f);
		addProperty(PROC_GESTURE_RANGE, "gesttrack range",0.0f,1.0f);

		// property defaults
		set(PROC_BACKGROUND, 0.0f);
		set(PROC_THRESHOLD, 0.1f);
		set(PROC_DILATE, 0.05f);
		set(PROC_ERODE, 0.05f);
		set(PROC_CONTOUR_MIN, 0.005f);
		set(PROC_CONTOUR_MAX, 1.0f);
		set(PROC_CONTOUR_REDUCE, 0.3f);
		set(PROC_TRACK_RANGE, 0.5f);
		set(PROC_HULL_MINDEPTH, 0.2f);
		set(PROC_GESTURE_RANGE, 0.2f);
		// warp
		warpMatrix = cvCreateMat(3, 3, CV_32FC1);

		setWarp(0, 0,
				1, 0,
				1, 1,
				0, 1);

		// blobs
		blobCount = VISION_BLOB_COUNT;
		foundBlobs = new Blob*[blobCount];
		trackedBlobs = new Blob*[blobCount];
		for (int i=0; i<blobCount; i++) {
			foundBlobs[i] = new Blob(i);
			trackedBlobs[i] = new Blob(i);
		}
		
		// gestures
		gesturetracker.init();

		// background
		doResetBackground = true;
		
		// contours ------------------------------------------------------------
		contourStorage = cvCreateMemStorage(0);
		
		
		// stages
		return VISION_SUCCESS;
	};
	
	int CVBlobDetector::update(Camera *camera)
	{	
		// images --------------------------------------------------------------
		srcImage32F = cache->getTmp(IMAGE_SRC32F, roiSize, IPL_DEPTH_32F, 1);
		dstImage32F = cache->getTmp(IMAGE_DST32F, roiSize, IPL_DEPTH_32F, 1);
		srcImage8U = cache->getTmp(IMAGE_SRC8U, roiSize, IPL_DEPTH_8U, 1);
		dstImage8U = cache->getTmp(IMAGE_DST8U, roiSize, IPL_DEPTH_8U, 1);
		prevImage = cache->getTmp(IMAGE_PREV, roiSize, IPL_DEPTH_8U, 1);
		
		// input ---------------------------------------------------------------
		IplImage* sourceImage = camera->getImage(cameraSource);
		IplImage* inputImage;

		// check if we need to convert the source image into greyscale
		if(sourceImage->nChannels == 1) {
			inputImage = sourceImage;
//			inputImage = cache->getTmp(IMAGE_INPUT, cvSize(camera->getWidth(), camera->getHeight()));
			//cvCvtColor(sourceImage, inputImage, CV_BGR2GRAY);
//			cvCvtColor(sourceImage, inputImage, CV_GR);
			
//			inputImage = cache->getTmp(IMAGE_INPUT, cvSize(camera->getWidth(), camera->getHeight()));
//			cvConvert(sourceImage, inputImage);

		} else {
			inputImage = cache->getTmp(IMAGE_INPUT, cvSize(camera->getWidth(), camera->getHeight()));
			cvCvtColor(sourceImage, inputImage, CV_BGR2GRAY);
		}

		// resize inputImage if necessary
		IplImage* inputImageResized = cache->getTmp(IMAGE_INPUT_RESIZED, size);
		if(camera->getWidth() != size.width || camera->getHeight() != size.height) {
			cvResize(inputImage, inputImageResized, CV_INTER_LINEAR);
		} else {
			inputImageResized = inputImage;
		}
		
		copyStage(STAGE_INPUT, inputImageResized);	
		
		// crop region of interest out of inputImageResized into srcImage
		setMode(MODE_8U);
		if(inputImageResized->width > roi.width && inputImageResized->height > roi.height) {
			cvGetRectSubPix(inputImageResized, srcImage, cvPoint2D32f(roi.x + roi.width / 2, roi.y + roi.height / 2));
		} else {
			//srcImage = inputImageResized;
			cvCopy(inputImageResized, srcImage);
		}
				
		// warp ----------------------------------------------------------------
		if(warpEnabled) {
			setMode(MODE_8U);
			cvWarpPerspective(srcImage, dstImage, warpMatrix, CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
			copyStage(STAGE_WARP, dstImage);
			swap();
		}
		
		// optical flow -------------
		//doOpticalFlow(srcImage, prevImage);
		

		// background ----------------------------------------------------------
		setMode(MODE_32F);
		float bgValue = doResetBackground ? 1.0f : get(PROC_BACKGROUND);
		IplImage *bgImage = cache->getTmp(IMAGE_BG, roiSize, IPL_DEPTH_32F, 1);
		//IplImage *bgImage = cache->getTmp(IMAGE_BG, srcImage);

		/*
		printf("bgImage %i %i \n", bgImage->width, bgImage->height);
		printf("srcImage %i %i \n", srcImage->width, srcImage->height);
		
		if(bgImage->roi != NULL) {
			printf("bgImage roi %i %i \n", bgImage->roi->width, bgImage->roi->height);
		}
		
		if(srcImage->roi != NULL) {
			printf("srcImage roi %i %i \n", srcImage->roi->width, srcImage->roi->height);
		}
		*/
		
		if (bgValue > 0) {
			cvRunningAvg(srcImage, bgImage, bgValue);
			copyStage(STAGE_BACKGROUND, bgImage);
			doResetBackground = false;
		}
		
		// difference ----------------------------------------------------------
		setMode(MODE_32F);
		cvAbsDiff(srcImage, bgImage, dstImage);
		copyStage(STAGE_DIFFERENCE, dstImage);
		swap();

		// threshold -----------------------------------------------------------
		if(useAdaptiveTresholding) {
			setMode(MODE_8U);
			// adaptive treshold works with 8U images, we need to convert the source image first
			//cvAdaptiveThreshold(srcImage8U, dstImage8U, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 5, -7);
			cvAdaptiveThreshold(srcImage, dstImage, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 9, -5);
		} else {
			cvThreshold(srcImage, dstImage, get(PROC_THRESHOLD), 255, CV_THRESH_BINARY);			
		}
		copyStage(STAGE_THRESHOLD, dstImage);
		swap();
		
		// dilate --------------------------------------------------------------
		float dilate = get(PROC_DILATE);
		if(dilate > 0) {
			setMode(MODE_8U);
			cvDilate(srcImage, dstImage, NULL,(int) dilate);
			copyStage(STAGE_DILATE, dstImage);	
			swap();
		}
		
		// erode ---------------------------------------------------------------
		float erode = get(PROC_ERODE);
		if(erode > 0) {
			setMode(MODE_8U);
			cvErode(srcImage, dstImage, NULL, (int)erode);
			copyStage(STAGE_ERODE, dstImage);	
			swap();
		}

		
		// contours ------------------------------------------------------------
		setMode(MODE_8U);
		findContours();
		drawContours();
		swap();
		
		// blobs ---------------------------------------------------------------
		findBlobs();
		drawBlobs(STAGE_DETECTION, foundBlobs);
		
		trackBlobs();
		drawBlobs(STAGE_TRACKING, trackedBlobs);
		
		//-------------------------------------------------------------------------
		// find convex hull points -> concave points 
		cvZero(dstImage);
		findAllConvexHullPoints();
		copyStage(STAGE_HULLPOINTS, dstImage);	
		swap();
		
		
		

		return VISION_SUCCESS;
	}

	//////////// opitcal flow, 
	
	void CVBlobDetector::doOpticalFlow(IplImage *cur, IplImage *prev)
	{
		if(!cur) return;
		if(!prev) return;

		IplImage *smoothcur = cache->getTmp(IMAGE_SMOOTHCUR, roiSize, IPL_DEPTH_8U, 1);
		
		cvSmooth(cur, smoothcur, 2, 3);  
		int w = cur->width;
		int h = cur->height;
		CvSize size, winsize;
		size.width=w;
		size.height= h;
		winsize.width = 7; winsize.height =7;
		IplImage* velx;
		IplImage* vely;
		
		velx = cache->getTmp(IMAGE_VELX, size, IPL_DEPTH_32F, 1);
		vely = cache->getTmp(IMAGE_VELY, size, IPL_DEPTH_32F, 1);
		//cvSetZero(velx);
		//cvSetZero(vely);
		cvCalcOpticalFlowLK( prev, smoothcur, winsize, velx, vely );
		
		/// do seomthing with it
		cvZero(dstImage);
		float dx, dy;
		CvPoint p1;
		CvPoint p2;
		int blocksize = 15;
		float f = 1.0f/(float)(blocksize*blocksize);
		
		int i,j,p,q;
		for(i =0; i< w;i+=blocksize)
		{
			for(j =0; j< h;j+=blocksize)
			{
				
				float sumdx = 0;
				float sumdy= 0;

				for(p =0; p< blocksize;p++)
				{	
					for(q =0; q< blocksize;q++)
					{
						if(j+p < h && i+q < w)
						{
							dx = (float)cvGetReal2D( velx, j+p, i+q );
							dy = (float)cvGetReal2D( vely, j+p, i+q );
							sumdx += dx;
							sumdy += dy;
						}
					}
				}
				sumdx *= f;
				sumdy *= f;

				p1.x = i + blocksize/2;
				p1.y = j + blocksize/2;
				p2.x =int( p1.x + sumdx*0.1f);
				p2.y =int(p1.y + sumdy*0.1f);
				cvLine(dstImage, p1, p2,CV_RGB(0,0,255),1, CV_AA, 0 );
			}
		}
		
		//cvReleaseImage(&velx);
		//cvReleaseImage(&vely);

		copyStage(STAGE_OPTICALFLOW, dstImage);	
		//copyStage(STAGE_OPTICALFLOW, dstImage);	
		cvCopyImage(smoothcur,prevImage);
		
	}


	////////////// gesture points, concave points


	void CVBlobDetector::findAllConvexHullPoints()
	{
		gesturetracker.setTrackingRange(get(PROC_GESTURE_RANGE));
		gesturetracker.setFrameSize(0.0f,0.0f,(float)size.width, (float)size.height);
		gesturetracker.beginUpdate();

		if(contourFirst != NULL) {
			CvSeq* contour = contourFirst;
			//CvSeq *approxContour;
			for( ; contour != 0; contour = contour->h_next ) {
				findConvexHullPointsForContour(contour);
			}
		}

		gesturetracker.endUpdate();
		
		if(!getStage(STAGE_HULLPOINTS)->isEnabled) return;
		// draw gesture track points, and history tails if any
		for(int i=0;i< gesturetracker.MAX_GESTURES;i++)
		{
			Gesture *gesture = gesturetracker.getTrackedGesture(i);
			if(!gesture->isActive) continue;
			CvPoint p;
			p.x = int(gesture->position.x*size.width); p.y = int(gesture->position.y*size.height);
			
			if(gesture->nhistory == 0)
				cvCircle( dstImage, p, 10, CV_RGB(255,255,255), -1, 8,0);
			else{

				cvCircle( dstImage, p, 6, CV_RGB(255,255,255), -1, 8,0);
				CvPoint prevp;
				prevp.x = p.x;
				prevp.y = p.y;
				for(int j =gesture->nhistory -1; j> 0;j--)
				{
					p.x =int( gesture->positionHistory[j].x*(float)size.width);
					p.y =int( gesture->positionHistory[j].y*(float)size.height);
					cvLine(dstImage, p, prevp,CV_RGB(255,255,255),2, CV_AA, 0);
					prevp.x = p.x;
					prevp.y = p.y;
				}
			}
		}
	}

	void CVBlobDetector::findConvexHullPointsForContour(CvSeq* contours)
	{
		int i;
		CvSeq* seqhull  =0 ;
		CvSeq* defects =0 ;
		int* hull =0 ;
		int count ;
		int hullsize;
		CvPoint* PointArray =0 ;
		CvConvexityDefect* defectArray =0 ;	
		CvMemStorage* stor03;

		stor03 = cvCreateMemStorage(0);

		count = contours->total; 
   
		// Alloc memory for contour point set.    
        PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) );
       
		// Alloc memory for indices of convex hull vertices.
        hull = (int*)malloc(sizeof(int)*count);

		// Get contour point set.
		cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);
        
		// Find convex hull for curent contour.
	    cvConvexHull( PointArray,
                      count,
                      NULL,
                      CV_COUNTER_CLOCKWISE,
                      hull,
                      &hullsize);

		// Find convex hull for current contour.
        // This required for cvConvexityDefects().
		//fprintf(stderr,"cvConvexHull2\n");
        seqhull = cvConvexHull2( contours,0,
                             CV_COUNTER_CLOCKWISE,
                             0);
		
		// This required for cvConvexityDefects().
        // Otherwise cvConvexityDefects() falled.
        if( hullsize < 4 )
            return;

		 //fprintf(stderr,"cvConvexityDefects\n");
        defects = cvConvexityDefects( contours,
                            seqhull,
                            stor03);
        int j=0;


		 // This cycle marks all defects of convexity of current contours.
        for(;defects;defects = defects->h_next)
        {
			int nomdef = defects->total; // defect amount
			//outlet_float( m_nomdef, nomdef );
            
            if(nomdef == 0)
                continue;
             
            // Alloc memory for defect set.   
			//fprintf(stderr,"malloc\n");
            defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
            
            // Get defect set.
			//fprintf(stderr,"cvCvtSeqToArray\n");
            cvCvtSeqToArray(defects,defectArray, CV_WHOLE_SEQ);
         

			// Draw marks for  defects.
            for(i=0; i<nomdef; i++)
            {
                cvLine(dstImage, *(defectArray[i].start), *(defectArray[i].depth_point),CV_RGB(0,0,255),1, CV_AA, 0 );
                //cvCircle( dstImage, *(defectArray[i].depth_point), 5, CV_RGB(255,255,255), 3, 8,0);
                if(defectArray[i].depth > get(PROC_HULL_MINDEPTH))
				{
					float x = (float)defectArray[i].start->x;
					float y = (float)defectArray[i].start->y;
					//Gesture *gesture = gesturetracker.addFoundPoint(x,y);	
					CvPoint p;
					p.x = (int)x; p.y =(int) y;
					//cvCircle( dstImage, p, 3, CV_RGB(255,255,255), -1, 8,0);
				}
				if(getStage(STAGE_HULLPOINTS)->isEnabled) cvLine(dstImage, *(defectArray[i].depth_point), *(defectArray[i].end),CV_RGB(0,0,255),1, CV_AA, 0 );
				
				/*
				t_atom rlist[7];
				SETFLOAT(&rlist[0], i);
				SETFLOAT(&rlist[1], defectArray[i].start->x);
				SETFLOAT(&rlist[2], defectArray[i].start->y);
				SETFLOAT(&rlist[3], defectArray[i].depth_point->x);
				SETFLOAT(&rlist[4], defectArray[i].depth_point->y);
				SETFLOAT(&rlist[5], defectArray[i].end->x);
				SETFLOAT(&rlist[6], defectArray[i].end->y);
				outlet_list( m_dataout, 0, 7, rlist );
				*/
            }

			j++;
            // Free memory.       
            free(defectArray);
		}

		// Draw current contour.
        //cvDrawContours( dstImage, contours, CV_RGB(255,0,0), CV_RGB(0,255,0), 2, 2, CV_AA, cvPoint(0,0)  );

		 // Draw convex hull for current contour.        
        /*
		for(i=0; i<hullsize-1; i++)
        {
            cvLine(dstImage, PointArray[hull[i]],  PointArray[hull[i+1]],CV_RGB(255,255,255),1, CV_AA, 0 );
        }
        cvLine(dstImage, PointArray[hull[hullsize-1]], PointArray[hull[0]],CV_RGB(255,255,255),1, CV_AA, 0 );
        */

		 // Free memory.          
        free(PointArray);
        free(hull);
            /* replace CV_FILLED with 1 to see the outlines */
            //cvDrawContours( x->cnt_img, contours, CV_RGB(255,0,0), CV_RGB(0,255,0), x->levels, 3, CV_AA, cvPoint(0,0)  );
			//cvConvexityDefects( contours, cvConvexHull2( contours, 0, CV_CLOCKWISE, 0 ), stor022 );
		cvReleaseMemStorage( &stor03 );
		if (defects) cvClearSeq(defects);
		if (seqhull) cvClearSeq(seqhull);

	}




	
	// -------------------------------------------------------------------------
	// CONTOURS
	// -------------------------------------------------------------------------
	//#pragma mark ---- Contours ----
	void CVBlobDetector::findContours()
	{
		cvClearMemStorage(contourStorage);
		cvFindContours(srcImage, contourStorage, &contourFirst, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		//cvFindContours(srcImage, contourStorage, &contourFirst, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	}
	
	// draws the contours onto dstImage
	void CVBlobDetector::drawContours()
	{
		if(!getStage(STAGE_CONTOUR)->isEnabled) return;
				
		cvZero(dstImage);
		
		// no contours found
		if(contourFirst != NULL) {
			CvSeq* contour = contourFirst;
			CvSeq *approxContour;
			for( ; contour != 0; contour = contour->h_next ) {
				approxContour = cvApproxPoly(contour, sizeof(CvContour), contourStorage, CV_POLY_APPROX_DP, get(PROC_CONTOUR_REDUCE));
				cvDrawContours(dstImage, approxContour, cGrey, cDarkGrey, 3, 2);
			}
		}
		copyStage(STAGE_CONTOUR, dstImage);
	}

	
	// -------------------------------------------------------------------------
	// BLOBS
	// -------------------------------------------------------------------------
	//#pragma mark ---- Blobs ----
	void CVBlobDetector::findBlobs()
	{
		// reset active blobs
		for(int i=0; i<VISION_BLOB_COUNT; i++)
			foundBlobs[i]->init();
		

		// no contours found
		if(contourFirst == NULL) return;

		int imageArea = (size.width * size.height) / 2;
		int minArea = (int)get(PROC_CONTOUR_MIN) * imageArea;
		int maxArea = (int)get(PROC_CONTOUR_MAX) * imageArea;
		
		CvSeq *approxContour;	
		CvMoments moments;
			
		// find new blobs
		int foundBlobCount=0;
		
		CvSeq* contour = contourFirst;
		for( ; contour != 0; contour = contour->h_next ) {
			approxContour = cvApproxPoly(contour, sizeof(CvContour), contourStorage, CV_POLY_APPROX_DP, get(PROC_CONTOUR_REDUCE));
			float area = (float) fabs(cvContourArea(approxContour));
			
			if (area > minArea && area < maxArea) {
				if(foundBlobCount >= VISION_BLOB_COUNT) {
					LOG("Too many contours");
					return;
				}
				// get blob properties
				cvContourMoments(approxContour, &moments);	
				
				// update blob
				Blob *b = foundBlobs[foundBlobCount++];
				b->isActive = true;
				b->position = cvPoint((int) (moments.m10/moments.m00),(int) (moments.m01/moments.m00));				
				b->position64f = cvPoint2D64f(moments.m10/moments.m00, moments.m01/moments.m00);
				b->bounds = cvBoundingRect(approxContour, 0);
				b->contour = approxContour;
			}
		}
	}
	
	// TODO : check this, need to add dispatchEventActive somewhere
	void CVBlobDetector::trackBlobs() 
	{
		Blob *found, *tracked, *match;
		float dist, distClosest;
		
		int imageArea = size.width * size.height;
		float distMax = get(PROC_TRACK_RANGE) * imageArea;
		distMax *= distMax;
		int i,j;
		// loop through tracked blobs
		for (i=0; i<VISION_BLOB_COUNT; i++) {
			tracked = trackedBlobs[i];
			if(!tracked->isActive) continue;
			match = NULL;
			distClosest = distMax;
			
			// try to find a match among the new found blobs within the slider range
			for (j=0; j<VISION_BLOB_COUNT; j++) {
				found = foundBlobs[j];
				if(!found->isActive || found->isAssigned) continue;
				dist =(float) ptDistanceSquared(found->position, tracked->position);
				if(dist < distClosest) {
					distClosest = dist;
					match = found;
				}
			}
			
			if(match == NULL) {  // no match found
				tracked->isActive = false;
				dispatchEventInActive(tracked);
			} else { // match found, continue updating this
				//printf("found %i => %i \n", tracked->id, match->id);
				tracked->isActive = true;
				tracked->set(match);
				match->isAssigned = true;
				dispatchEventUpdate(tracked);
			}
		}
		
		
		Blob *newtracked;
		// for each new found blob not assigned, add the tracking list and dispatch active event
		for(i = 0 ; i < VISION_BLOB_COUNT;i++)
		{
			found = foundBlobs[i];
			if(found->isActive && !found->isAssigned)
			{
				found->isAssigned  = true;
				newtracked = getFreeTrackedBlob();
				newtracked->set(found);
				newtracked->isAssigned = true;
				newtracked->isActive = true;
				dispatchEventActive(newtracked);
			}
		}

	}

	Blob *CVBlobDetector::getFreeTrackedBlob()
	{
		Blob *b;
		for(int i = 0;i<VISION_BLOB_COUNT;i++)
		{
			b = trackedBlobs[i];
			if(!b->isActive)
			{
				return b;
			}
		}
		return 0;
	}
	
	void CVBlobDetector::drawBlobs(int stage, Blob* blobs[])
	{
		if(!getStage(stage)->isEnabled) return;
		
		cvZero(dstImage);
		
		char* label = (char *) malloc(10);
		
		for(int i=0; i<VISION_BLOB_COUNT; i++) {
			Blob *b = blobs[i];
			
			if(b->isActive) {
				//CvScalar c = cvScalarAll(255 - i * (255/ blobNum) );
				CvScalar c = cvScalarAll(i * (255/ blobCount) );
				CvRect br = b->bounds;
				cvRectangle(dstImage, cvPoint(br.x, br.y), cvPoint(br.x + br.width, br.y + br.height), cWhite,4);
				//cvCircle(dstImage, b->position, 2, cWhite, 2, CV_AA );
				
				sprintf(label, "%i", i);
				cvPutText(dstImage, label, b->position, &font, cWhite);		
			}
		}
		
		free(label);
		copyStage(stage, dstImage);
	}
	
	
	// -------------------------------------------------------------------------
	// HELPERS
	// -------------------------------------------------------------------------
	//#pragma mark ---- Helpers ----
	void CVBlobDetector::swap()
	{
		IplImage *swap;

		swap = srcImage32F;
		srcImage32F = dstImage32F;
		dstImage32F = swap;
		
		swap = srcImage8U;
		srcImage8U = dstImage8U;
		dstImage8U = swap;
		
		setMode(this->mode);
	}
	
	
	void CVBlobDetector::setMode(Mode mode) 
	{
		// convert current working image to new format
		if(this->mode != mode) {
			switch(mode) {
				case MODE_8U:
					cvConvert(srcImage32F, srcImage8U);
					break;
					
				case MODE_32F:		
					cvConvert(srcImage8U, srcImage32F);
					break;
			}
		}
		
		// set image pointers depending on format
		switch(mode) {
			case MODE_8U:
				this->srcImage = srcImage8U;
				this->dstImage = dstImage8U;
				break;
				
			case MODE_32F:
				this->srcImage = srcImage32F;
				this->dstImage = dstImage32F;
				break;
		}
		
		this->mode = mode;
	}
	
	void CVBlobDetector::resetBackground()
	{
		doResetBackground = true;
	}

	// -------------------------------------------------------------------------
	// SETTERS
	// -------------------------------------------------------------------------
	//#pragma mark ---- Setters ----	
	void CVBlobDetector::setWarp(float sx1, float sy1,
								 float sx2, float sy2,
								 float sx3, float sy3,
								 float sx4, float sy4)
	{
		float w = (float)size.width;
		float h = (float)size.height;

		// init source matrix
		CvMat* src = cvCreateMat(4, 2, CV_32FC1);
		src->data.fl[0]=sx1*w;
		src->data.fl[1]=sy1*h;
		src->data.fl[2]=sx2*w;
		src->data.fl[3]=sy2*h;
		src->data.fl[4]=sx3*w;
		src->data.fl[5]=sy3*h;
		src->data.fl[6]=sx4*w;
		src->data.fl[7]=sy4*h;
		
		// init destination matrix
		CvMat* dst = cvCreateMat(4, 2, CV_32FC1);
		dst->data.fl[0]=0;
		dst->data.fl[1]=0;
		dst->data.fl[2]=(float)size.width;
		dst->data.fl[3]=0;
		dst->data.fl[4]=(float)size.width;
		dst->data.fl[5]=(float)size.height;
		dst->data.fl[6]=0;
		dst->data.fl[7]=(float)size.height;

		// check if warp is necessary
		if (src->data.fl[0] == dst->data.fl[0] && 
			src->data.fl[1] == dst->data.fl[1] && 
			src->data.fl[2] == dst->data.fl[2] && 
			src->data.fl[3] == dst->data.fl[3] && 
			src->data.fl[4] == dst->data.fl[4] && 
			src->data.fl[5] == dst->data.fl[5] && 
			src->data.fl[6] == dst->data.fl[6] && 
			src->data.fl[7] == dst->data.fl[7]) 
		{
			//LOG("warp NOT enabled");
			warpEnabled = false;
			//free(src);
			//free(dst);
			
		} else {
			//LOG("warp enabled");
			warpEnabled = true;
			cvFindHomography(src, dst, warpMatrix);
		}
	}
	
	
	// -------------------------------------------------------------------------
	// GETTERS
	// -------------------------------------------------------------------------
	//#pragma mark ---- Getters ----	
	Blob** CVBlobDetector::getBlobs()
	{
		return trackedBlobs;
	}

} } // namespace fieldkit::vision