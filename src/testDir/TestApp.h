/*
 * TestApp.h
 *
 *  Created on: Mar 12, 2019
 *      Author: vivek
 */

#ifndef SRC_TESTDIR_TESTAPP_H_
#define SRC_TESTDIR_TESTAPP_H_

class TestApp {
public:
	TestApp();
	void runTests();
	void testScreenCap();
	virtual ~TestApp();
	void testSendEvent();
};

#endif /* SRC_TESTDIR_TESTAPP_H_ */
