/*
 * IScrenProcessor.h
 *
 *  Created on: 14-Feb-2019
 *      Author: VivekK4
 */

#ifndef SRC_ISCRENPROCESSOR_H_
#define SRC_ISCRENPROCESSOR_H_

class IScrenProcessor {
public:
	IScrenProcessor();
	virtual ~IScrenProcessor();
	virtual int ProcessImage()=0;
};

#endif /* SRC_ISCRENPROCESSOR_H_ */
