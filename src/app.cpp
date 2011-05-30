/*

 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com 
 
 */

#include "app.h"

app::app()
{
	
}
app::~app()
{
	delete faceTracker;
}

void app::setup(int w, int h, int n)
{
	width				= w;
	height				= h;
	
	// initialize face tracker
	numExamples			= n;								// number of images to train face model on
	faceTracker			= new pkmFaceTracker();				// shape model
	poseCalibrator		= new pkmPoseCalibrator();			// pose calibration
	
	// open a camera
	camera				= cvCreateCameraCapture(CV_CAP_ANY); 
	cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, (double)width);
	cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, (double)height);
	
	if(!camera)
	{
		printf("[ERROR]: Could not open any cameras!\n");
		std::exit(1);
	}
	
	// initialize a window
	window_faceTracker = "Face Tracking";
	cvNamedWindow(window_faceTracker.c_str(), 1);
	
	bPoseModelBuilt				= false;
}

void app::update()
{
	cameraImage = cvQueryFrame(camera); 
	if(!cameraImage)
		return; 
	frame = cameraImage;
	
	faceTracker->update(frame);
	//get3DTransformation
	
}

void app::draw()
{

	faceTracker->drawShapeModel(frame);
	char buf[256];
	sprintf(buf, "[\'r\']: Re-initialize the face tracker");
	putText(frame,
			buf,
			Point(10,60),
			CV_FONT_HERSHEY_PLAIN,
			1.0,
			CV_RGB(255,255,255));
	sprintf(buf, "[\'0\' - \'9\']: Add training example for calibration point\n");
	putText(frame,
			buf,
			Point(10,80),
			CV_FONT_HERSHEY_PLAIN,
			1.0,
			CV_RGB(255,255,255));	
	
	imshow(window_faceTracker.c_str(), frame);
}

void app::keyPressed(int c)
{
	// reset the shape model
	if(c == 'r')
		faceTracker->reset();
	
	// train for calibration 
	else if(c >= '1' && c <= '9' || c == '0' )
	{
		char cc = char(c);
		int point = atoi(&cc);
		Mat av = faceTracker->get3DTransformation();
		poseCalibrator->addExample(point, av);
	}
}
