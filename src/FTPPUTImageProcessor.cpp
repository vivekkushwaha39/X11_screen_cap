/*
 * FTPPUTImageProcessor.cpp
 *
 *  Created on: 14-Feb-2019
 *      Author: VivekK4
 */

#include "FTPPUTImageProcessor.h"

FTPPUTImageProcessor::FTPPUTImageProcessor() {
	// TODO Auto-generated constructor stub

}

FTPPUTImageProcessor::~FTPPUTImageProcessor() {
	// TODO Auto-generated destructor stub
}


int FTPPUTImageProcessor::ProcessImage()
{
	LOGD("sending image");
	system("ftpput 192.168.31.202 screencap.png");
	return 0;
}
