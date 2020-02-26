#include "delayqueue.h"
#include "ycpool.h"
#include "kservice.h"
#include "systick.h"

#define DQ_NODE_SIZE 700
#define DQ_NODE_COUNT 5

uint32_t fLastSyncTime = 0;
struct mempool dqmp;
static list_t dqlist = LIST_OBJECT_INIT(dqlist);

error_t DQ_Init()
{
	return MP_Create(&dqmp, DQ_NODE_COUNT, DQ_NODE_SIZE);
}

bool DQ_CheckEmpty()
{
	return LIST_Isempty(&dqlist);
}

bool DQ_CheckFull()
{
	return dqmp.block_free_count==0;
}

tDELAYQUEUE* DQ_GetHead()
{
	return LIST_FirstEntry(&dqlist, tDELAYQUEUE, list);
}

void DQ_RemoveNode(tDELAYQUEUE* node)
{
	LIST_Remove(&node->list);
	MP_Free(node);
}

static void dqSynchronize()
{
	uint32_t timeNow = systick_get_us();

	/* time now is smaller than lastsync time,impossible,return */
	if (timeNow < fLastSyncTime) {
		fLastSyncTime = timeNow;
		return;
	}

	/*get time since last sync */
	uint32_t timeSinceLastSync = timeNow - fLastSyncTime;

	/*update lastsync timer */
	fLastSyncTime = timeNow;

	/*if media buffer is empty abandon this sync */
	if (DQ_CheckEmpty()) return;
	
	tDELAYQUEUE* currHead = DQ_GetHead();
	while (timeSinceLastSync >= currHead->timerRemaining) {
		timeSinceLastSync -= currHead->timerRemaining;
		currHead->timerRemaining = DELAY_ZERO;
		if (currHead->list.next == &dqlist) return;
		currHead = LIST_Entry(currHead->list.next, tDELAYQUEUE, list);
	}
	currHead->timerRemaining -= timeSinceLastSync;
}

tDELAYQUEUE* DQ_ApplyForNewNode(uint32_t timeStamp)
{
	tDELAYQUEUE* newNode = (tDELAYQUEUE*)MP_Alloc(&dqmp);

	/* check mp mempool for delay queue */
	if (newNode == NULL)
		return NULL;

	/* sync time */
	dqSynchronize();

	/*get time since last sync */
	uint32_t timeSinceLastSync = timeStamp>fLastSyncTime? (timeStamp - fLastSyncTime) : DELAY_ZERO;

#ifdef DEBUG_DQ
	printf("Tsp:%d,Tsyc:%d\r\n", timeStamp, fLastSyncTime);
#endif
		
	/* current media buffer is empty */
	if (DQ_CheckEmpty()) {
		LIST_InsertAfter(&dqlist, &newNode->list);
		newNode->timerRemaining = timeSinceLastSync;
		return (tDELAYQUEUE*)newNode;
	}

	/* put node into media buffer */
	tDELAYQUEUE* currHead = DQ_GetHead();
	while (timeSinceLastSync >= currHead->timerRemaining) {
		timeSinceLastSync -= currHead->timerRemaining;
		if (currHead->list.next == &dqlist) break;
		currHead = LIST_Entry(currHead->list.next, tDELAYQUEUE, list);
	}
	LIST_InsertAfter(&currHead->list, &newNode->list);
	newNode->timerRemaining = timeSinceLastSync;
	return (tDELAYQUEUE*)newNode;
}

uint32_t DQ_GetTime2NextAlerm()
{		
	/* check if media buffer is empty*/
	if (DQ_CheckEmpty()) return DELAY_INFINITY;

	/* check if first node*/
	tDELAYQUEUE* currHead = DQ_GetHead();
	if (currHead->timerRemaining == DELAY_ZERO) return DELAY_ZERO;

	/* sync time */
	dqSynchronize();
	return currHead->timerRemaining;
}

void DQ_HandleAlarm(tDQAlarmCb alarmcb)
{
	/* check if media buffer is empty*/
	if (DQ_CheckEmpty()) return;
	
	tDELAYQUEUE* currHead = DQ_GetHead();
	/* try to sync time */
	if (currHead->timerRemaining != DELAY_ZERO)
		dqSynchronize();
	/* callback */
	if (currHead->timerRemaining == DELAY_ZERO) {
		alarmcb(currHead->pnode, currHead->size);

		/* free mp buffer */
		DQ_RemoveNode(currHead);
	}
}
