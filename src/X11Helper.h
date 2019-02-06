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
class X11Helper {
private:
	Display *display;
	XWindowAttributes rootWindowAttr;
	Window rootWindowDrawable;
	XImage *pXImg;
public:
	X11Helper();
	bool OpenDisplay();
	bool CloseDisplay();
	bool CaptureScreen();
	bool SaveAsPng(XImage *);
	virtual ~X11Helper();

};

#endif /* SRC_X11HELPER_H_ */
