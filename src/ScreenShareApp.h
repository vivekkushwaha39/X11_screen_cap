/*
 * ScreenShareApp.h
 *
 *  Created on: 13-Feb-2019
 *      Author: VivekK4
 */

#ifndef SRC_SCREENSHAREAPP_H_
#define SRC_SCREENSHAREAPP_H_
#include "X11Helper.h"
#include <vector>
class ScreenShareApp {
private:
	bool isCapturingEnabled;
	X11Helper x11Helper;
	std::vector<>
public:
	ScreenShareApp();
	virtual ~ScreenShareApp();
	void StartCapturing();

};

#endif /* SRC_SCREENSHAREAPP_H_ */
