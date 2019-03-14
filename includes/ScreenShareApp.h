/*
 * ScreenShareApp.h
 *
 *  Created on: 13-Feb-2019
 *      Author: VivekK4
 */

#ifndef SRC_SCREENSHAREAPP_H_
#define SRC_SCREENSHAREAPP_H_
#include "X11Helper.h"
#include <stdlib.h>
#include <unistd.h>
#include <iterator>
#include <vector>
#include "IScrenProcessor.h"
#include "FTPPUTImageProcessor.h"

class ScreenShareApp {
private:
	bool isCapturingEnabled;
	X11Helper x11Helper;
	std::vector<IScrenProcessor *>processors;
public:
	ScreenShareApp();
	virtual ~ScreenShareApp();
	/**
	 * Start capturing the screenshot
	 */
	void StartCapturing();
	/**
	 * Call image processors to process capturing png file
	 */
	void CallImageProcessors();
	void Signal(int);
	/**
	 * Send mouse event to screen
	 * @param int x axis
	 * @param int y axis
	 * @param int button (i.e left/right/middle)
	 */
	void SendMouseEvent(int, int, int);
};

#endif /* SRC_SCREENSHAREAPP_H_ */
