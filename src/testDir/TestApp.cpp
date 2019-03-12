/*
 * TestApp.cpp
 *
 *  Created on: Mar 12, 2019
 *      Author: vivek
 */

#include "TestApp.h"
#include "../X11Helper.h"
TestApp::TestApp()
{
	// TODO Auto-generated constructor stub

}

TestApp::~TestApp()
{
	// TODO Auto-generated destructor stub
}

void TestApp::runTests()
{
	// TEST screenCap
	testSendEvent();
}

void TestApp::testScreenCap()
{
	X11Helper x11helper;
	x11helper.OpenDisplay();
	x11helper.CaptureScreen();
	x11helper.SaveAsPng();
	x11helper.CloseDisplay();
}

void TestApp::testSendEvent()
{
	X11Helper x11helper;
	x11helper.OpenDisplay();
	x11helper.sendEvent(0, 200, 250, Button1);
}
