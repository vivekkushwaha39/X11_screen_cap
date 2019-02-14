/*
 * ScreenShareApp.cpp
 *
 *  Created on: 13-Feb-2019
 *      Author: VivekK4
 */

#include "ScreenShareApp.h"
using namespace std;
ScreenShareApp::ScreenShareApp()
{
	isCapturingEnabled = false;
	x11Helper.OpenDisplay();
	IScrenProcessor *pro = new FTPPUTImageProcessor();
	processors.push_back(pro);
}

ScreenShareApp::~ScreenShareApp()
{
}

void ScreenShareApp::StartCapturing()
{

	isCapturingEnabled = true;
	syslog(LOG_DEBUG, "entering in while loop");
	while( isCapturingEnabled )
	{
		x11Helper.CaptureScreen();
		x11Helper.SaveAsPng();
		CallImageProcessors();
		sleep(2);
	}
	syslog(LOG_DEBUG, "getting out from while loop");
	x11Helper.CloseDisplay();
}

void ScreenShareApp::CallImageProcessors()
{
	for( vector<IScrenProcessor *>::iterator it = processors.begin();
			it != processors.end(); it++)
	{
		if ( (*it) != NULL)
			(*it)->ProcessImage();
	}
}


void ScreenShareApp::Signal(int data)
{
	syslog(LOG_DEBUG,  "clearing screen data");
	x11Helper.CloseDisplay();
}
