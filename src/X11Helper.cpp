/*
 * X11Helper.cpp
 *
 *  Created on: 28-Jan-2019
 *      Author: VivekK4
 */

#include "X11Helper.h"
#include <png.h>
X11Helper::X11Helper() {
	/*display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);

	XWindowAttributes attributes = {0};
	XGetWindowAttributes(display, root, &attributes);
	printf("Display is  %dX%d\n",attributes.width, attributes.height);
	printf("Sleeping for 15 sec");
	sleep(15);

	XImage *pImage = NULL;
	pImage = XGetImage(display, root, 0, 0, attributes.width, attributes.height,AllPlanes, ZPixmap);

	XCloseDisplay(display);*/
	display = NULL;
	pXImg = NULL;
}

X11Helper::~X11Helper()
{
	// TODO: vivekk4: delete screen pointer if it is allocated
}


bool X11Helper::OpenDisplay()
{
	bool isOpened = true;
	display = XOpenDisplay(NULL);

	if ( display == NULL )
	{
		isOpened = false;
	}
	else
	{
		rootWindowDrawable =  DefaultRootWindow(display);
		XGetWindowAttributes(display,rootWindowDrawable, &rootWindowAttr);
	}

	return isOpened;
}

bool X11Helper::CaptureScreen()
{
	bool isCaptured = true;
	if ( pXImg == NULL )
	{
		pXImg = XGetImage(display, rootWindowDrawable, 0, 0, rootWindowAttr.width, rootWindowAttr.height,
				AllPlanes,ZPixmap );
		isCaptured = true;
	}
	else
	{
		// TODO: vivekk4: delete screen-cap pointer
	}
	return isCaptured;
}


bool X11Helper::SaveAsPng(XImage *img)
{
	bool isSaved = false;

	if( img != NULL )
	{

	}
	return isSaved;
}
