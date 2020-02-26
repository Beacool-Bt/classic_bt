#include "mesh.h"
#include "yc_drv_common.h"

void MSH_SetAdvData(uint8_t* Dt, uint16_t Size)
{
	IPC_TxCommon(IPC_MESH_ADV_DATA, Dt, Size);
}

void MSH_SetScanData(uint8_t* Dt, uint16_t Size)
{
	IPC_TxCommon(IPC_MESH_RSP_DATA, Dt, Size);
}

void MSH_StopAdv()
{
	IPC_TxControlCmd(IPC_CMD_STOP_ADV);
}

void MSH_StartAdv()
{
	IPC_TxControlCmd(IPC_CMD_START_ADV);
}

void MSH_StartScan()
{
	IPC_TxControlCmd(IPC_CMD_START_SCAN);
}

void MSH_StopScan()
{
	IPC_TxControlCmd(IPC_CMD_STOP_SCAN);
}


void MSH_SetMacAddr(tBTADDR Addr)
{
	for (uint8_t i=0; i< BT_ADDR_SIZE; i++) {
		HWRITE(mem_le_lap+i, Addr[BT_ADDR_SIZE-1-i]);
	}
}

void MSH_SetAdvType(enum advType Type)
{
	HWRITE(mem_le_adv_type, Type);
}

void MSH_SetScanRspType(enum scanType Type)
{
	HWRITE(mem_le_scan_type, Type);
}

void MSH_SetAdvInterval(uint16_t Intv)
{
	Intv = Intv/0.625;
	HW_REG_16BIT(reg_map(mem_le_adv_interval_max), Intv);
}

void MSH_SetScanInterval(uint16_t Intv)
{
	Intv = Intv/0.625;
	HW_REG_16BIT(reg_map(mem_le_scan_interval), Intv);
}
