/*
 * debug.h
 *
 *  Created on: Apr 3, 2019
 *      Author: VivekK4
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_
#define SET_STATUS_BREAK(X) \
	cout<<"status check on line "<<__LINE__<<endl; \
	X=false; \
	break;




#endif /* SRC_DEBUG_H_ */
