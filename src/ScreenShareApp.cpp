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
}

ScreenShareApp::~ScreenShareApp()
{

}

void ScreenShareApp::StartCapturing()
{
	if ( x11Helper.GetStatus() == UNINITIALIZED )
		return;

	isCapturingEnabled = true;

	while( isCapturingEnabled )
	{
		x11Helper.CaptureScreen();
		x11Helper.SaveAsPng();
		CallImageProcessors();
	}
}

void ScreenShareApp::CallImageProcessors()
{
	for( vector<IScrenProcessor *>::iterator it = processors.begin();
			it != processors.end(); it++)
	{
		if ( (*it) == NULL)
			(*it)->ProcessImage();
	}
}
