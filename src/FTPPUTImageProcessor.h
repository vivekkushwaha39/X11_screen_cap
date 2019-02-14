/*
 * FTPPUTImageProcessor.h
 *
 *  Created on: 14-Feb-2019
 *      Author: VivekK4
 */

#ifndef SRC_FTPPUTIMAGEPROCESSOR_H_
#define SRC_FTPPUTIMAGEPROCESSOR_H_
#include "IScrenProcessor.h"
#include <stdlib.h>
#include "Logging.h"
class FTPPUTImageProcessor: public IScrenProcessor  {
public:
	FTPPUTImageProcessor();
	virtual ~FTPPUTImageProcessor();
	virtual int ProcessImage();
};

#endif /* SRC_FTPPUTIMAGEPROCESSOR_H_ */
