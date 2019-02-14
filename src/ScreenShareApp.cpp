/*
 * ScreenShareApp.cpp
 *
 *  Created on: 13-Feb-2019
 *      Author: VivekK4
 */

#include "ScreenShareApp.h"

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
	}
}
