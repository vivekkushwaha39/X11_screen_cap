/*
 * X11Helper.h
 *
 *  Created on: 28-Jan-2019
 *      Author: VivekK4
 */

#ifndef SRC_X11HELPER_H_
#define SRC_X11HELPER_H_
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <stdint.h>
#include <syslog.h>
#include <string.h>

// STATUS CODES
#define UNINITIALIZED 0x0
#define DISPLAY_OPENED 0x1

class X11Helper {
private:
	Display *display;
	XWindowAttributes rootWindowAttr;
	Window rootWindowDrawable;
	XImage *pXImg;
	bool isOpened;
public:
	X11Helper();
	bool OpenDisplay();
	bool CloseDisplay();
	bool CaptureScreen();
	bool SaveAsPng(std::string = "screencap.png");
	int GetStatus();
	virtual ~X11Helper();
	/**
	 * Send  event to Xorg
	 * @param int Event Type
	 * @param int 1st dependent param
	 * @param int 2nd dependent param
	 *  
	 */
	void sendEvent(int, int, int, int);

};

#endif /* SRC_X11HELPER_H_ */
