#include "kdevice.h"
#include "kservice.h"
#include <string.h>
#include "ycpool.h"

static list_t devlist = LIST_OBJECT_INIT(devlist);
struct mempool devpool;

void DEV_PoolEnable(uint8_t Count)
{
	MP_Create(&devpool, Count,sizeof(struct device));
}

error_t DEV_Register(enum device_id Devid, tReinit R, tEnterlpm E)
{
	/*check paramters*/
	if (R == NULL||E == NULL)
		return ERR_ILLEGAL_PARAM;

	device_t dev=  MP_Alloc(&devpool);
	dev->id = Devid;
	dev->enterlpm = E;
	dev->reinit = R;
	LIST_InsertAfter(&devlist, &dev->list);
	return SUCCESS;
}

error_t DEV_UnRegister(enum device_id Devid)
{
	struct list_node *node = NULL;
	device_t pdevice = NULL;
	for(node = devlist.next; node != &devlist; node = node->next) {
		pdevice = LIST_Entry(node, struct device, list);
		if (pdevice->id == Devid) {LIST_Remove(&pdevice->list); break;}
	}

}


void DEV_RestartAll(void)
{
	struct list_node *node = NULL;
	device_t pdevice = NULL;
	for(node = devlist.next; node != &devlist; node = node->next) {
		pdevice = LIST_Entry(node, struct device, list);
		if (pdevice->reinit != NULL)
			pdevice->reinit();
	}
}

void DEV_EnterLpmMode(void)
{
	struct list_node *node = NULL;
	device_t pdevice = NULL;
	for(node = devlist.next; node != &devlist; node = node->next) {
		pdevice = LIST_Entry(node, struct device, list);
		if (pdevice->enterlpm != NULL)
			pdevice->enterlpm();
	}
}

