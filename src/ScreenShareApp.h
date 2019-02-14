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
	void StartCapturing();
	void CallImageProcessors();

};

#endif /* SRC_SCREENSHAREAPP_H_ */
