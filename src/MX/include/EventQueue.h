

#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_

#include "stdlib.h"
#include "pthread.h"
#include "sys/sem.h"

//VARIABLES
typedef struct stQueue {

	int        iEventType;
	char       *cpData;
	int        iLength;
	struct stQueue *next;

} stEventQueue;

union semun {
	int                val; /* value for SETVAL */
	struct semid_ds    *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short int *array; /* array for GETALL, SETALL */
	struct seminfo     *__buf; /* buffer for IPC_INFO */
};

//FUNCTIONS
int         setMxEvent(int iEventType, char *cData, int iLength);
int         getMxEvent(char *cData, int *iLength);
void        initialiseEventQueue();
void        initialiseMxEventQueue();
//extern int 		sem_id;
/*
static int  getNumberOfEvent(void);
static int  aquireMutex();
static int  releaseMutex();
static int  initMutex();
static int  destroyMutex();
static int  initSemaphore();
static void delSemValue();
static int  semaphoreLock();
static int  semaphoreUnlock();
int         getNumberOfEvent(void);
int         aquireMutex();
int         releaseMutex();
int         initMutex();
int         destroyMutex();
*/
#endif
