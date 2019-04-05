#include "EventQueue.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include "Debug.h"
#include "General.h"



/****GLOBAL VARIABLES****/
static stEventQueue 	*pgEventFirstNode = NULL;
pthread_mutex_t eventMutex;
/****GLOBAL VARIABLES****/

/****MODULE INITIALIZED****/
//int 		sem_id; //Chnaged to static entity
static int 		sem_id;
static int 		getNumberOfEvent(void);
static int 		initSemaphore();
static int 		semaphoreLock();
static int 		semaphoreUnlock();
static int 		setSemValue();
/****MODULE INITIALIZED****/

void initialiseMxEventQueue(){
	_TRACE("Initializing Semaphore ");
	initSemaphore();
}

int setMxEvent(int iEventType, char *cData, int iLength) {

	debug_sprintf((gszDebugMessage,"-setEvent- EventType[%d] len [%d] data[%s] " ,iEventType, iLength, cData));	
	_TRACE(gszDebugMessage);	
	int ret;
	semaphoreLock();
	stEventQueue *node = (stEventQueue *) malloc (sizeof(stEventQueue));

	if(node == NULL) 
	{
		ret = FAILURE;
	}
	node->iEventType = iEventType;
	node->iLength = iLength;
	node->cpData = (char *) malloc (iLength + 1);
	memset(node->cpData, 0, iLength + 1);
	if(node->cpData == NULL) {			
		ret = FAILURE;
	}
	
	memcpy(node->cpData, cData, iLength);
	node->next = NULL;
	
	if(pgEventFirstNode == NULL)
	{ /* if queue is empty */
		pgEventFirstNode = node;		
		ret = SUCCESS;		
		//_TRACE("Node is empty...setting as first node");
	} else { /* if queue is not empty */
		stEventQueue *tempNode = pgEventFirstNode;
		
		while(tempNode->next != NULL)
			tempNode = tempNode->next;
		
		tempNode->next = node;		
		ret =  SUCCESS;
		//_TRACE("Appended the node to the queue");
	}
	semaphoreUnlock();
	/*int nLenQueue=getNumberOfEvent() ;
	debug_sprintf((gszDebugMessage, "%s - queue Length=[%d]", __FUNCTION__,nLenQueue));
	_TRACE(gszDebugMessage);*/
	return ret;
}

void displayQueue()
{
	
}

int getMxEvent(char *cData, int *iLength){
	int        eventType = 0;
	int nLenQueue=0;
	
	//displayQueue();
	
	semaphoreLock();
	
	nLenQueue=getNumberOfEvent() ;
	/*debug_sprintf((gszDebugMessage, "%s - queue Length=[%d]", __FUNCTION__,nLenQueue));
	_TRACE(gszDebugMessage);*/
		
	if(nLenQueue == 0)
	{
		eventType = 0;
	} 
	else 
	{
		stEventQueue *tempNode = pgEventFirstNode->next;
		memset(cData,0,pgEventFirstNode->iLength+1);
		/* copy data */
		memcpy(cData, pgEventFirstNode->cpData, pgEventFirstNode->iLength);
		*iLength = 	pgEventFirstNode->iLength; /* copy length of the data */
		eventType = pgEventFirstNode->iEventType; /* copy event type */
		debug_sprintf((gszDebugMessage, "%s - getEvent-type=[%d] data = [%s] and size [%d]", __FUNCTION__,eventType, cData, *iLength));
		_TRACE(gszDebugMessage);
		
		free(pgEventFirstNode->cpData); /* free allocated memory */
		free(pgEventFirstNode); /* free allocated memory */
		pgEventFirstNode = tempNode; /* Make next node as first node */	
	}
	semaphoreUnlock();
	
	return eventType;
}

static int getNumberOfEvent(void) 
{
	int        noOfNode  = 0;
	if(pgEventFirstNode == NULL)
	{		
		return 0;
	} 
	else
	{
		stEventQueue *tempNode = pgEventFirstNode;
		
		while(tempNode){
			tempNode = tempNode->next;
			noOfNode++;
		}
		return noOfNode;
	}		
}
/*
static int aquireMutex(){

	pthread_mutex_lock(&eventMutex);
	return SUCCESS;
}

static int releaseMutex(){
	
	pthread_mutex_unlock(&eventMutex);
	return SUCCESS;
}

static int initMutex(){
	
	if (pthread_mutex_init(&eventMutex, NULL) != 0) {
		printf("event mutex initialization failed");
		return FAILURE;
	}
	return SUCCESS;
}

static int destroyMutex(){

	pthread_mutex_destroy(&eventMutex);
	return SUCCESS;
}

static void delSemValue(){
	union semun sem_union;
	semctl(sem_id, 0, IPC_RMID, sem_union);
}
*/
static int initSemaphore() {
	_TRACE("initSemaphore() ");
	sem_id = semget((key_t)5896,1,0666 | IPC_CREAT);
	if(sem_id == -1)
		_TRACE("*********Abdul********* Failed to get Semaphore  ");
	setSemValue();
	return SUCCESS;
}

static int setSemValue() 
{
	union      semun sem_union;
	_TRACE("setSemValue()  ");
	sem_union.val = 1;
	if(semctl(sem_id, 0, SETVAL,sem_union) == -1)
		return -1;	
	return 0;
}

static int semaphoreLock(){
	
	//_TRACE("semaphoreLock()");
	struct     sembuf sem_b;
	//union      semun sem_union;
	
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;	
	
	if(semop(sem_id,&sem_b,1) == -1)
		return -1;
	return 0;
}

static int semaphoreUnlock(){
	
	//_TRACE("semaphoreUnlock()");
	struct     sembuf sem_b;
	
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	
	if(semop(sem_id,&sem_b,1) == -1)
		return -1;
	return 0;	
}
