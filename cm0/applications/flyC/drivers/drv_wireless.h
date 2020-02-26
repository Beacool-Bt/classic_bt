#ifndef _DRV_WIRELESS_H_
#define _DRV_WIRELESS_H_

void ws_init();

void ws_rx_data_single();

uint16_t  ws_rx_data(uint8_t *rx);

int ws_tx_data(uint8_t *p,uint8_t len);

#endif