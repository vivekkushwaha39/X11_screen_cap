/*
 * Logging.h
 *
 *  Created on: 14-Feb-2019
 *      Author: VivekK4
 */

#ifndef SRC_LOGGING_H_
#define SRC_LOGGING_H_
#include <syslog.h>

#define LOGD(x) syslog(LOG_DEBUG, x)

#endif /* SRC_LOGGING_H_ */
