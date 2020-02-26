#ifndef _DRV_DEBUG_H_
#define _DRV_DEBUG_H_

#include "yc11xx.h"
#include "yc11xx_uart.h"
#include "yc11xx_gpio.h"
#include <string.h>

//#define DEF_PRINTHEX



/**
  *@brief Debug log buffer address.
  */
#define DEBUG_LOG_BUF_START_ADDR 				reg_map(DEBUG_LOG_BUFFER_START_ADDR)
#define DEBUG_LOG_BUF_INITIAL_FLAG_ADDR			reg_map(DEBUG_LOG_INITIAL_FLAG_ADDR)
#define DEBUG_LOG_BUF_LOCK_FLAG_CM0_ADDR		reg_map(DEBUG_LOG_LOCK_FLAG_CM0_ADDR)
#define DEBUG_LOG_BUF_LOCK_FLAG_RESPIN_ADDR	reg_map(DEBUG_LOG_LOCK_FLAG_RESPIN_ADDR)
#define DEBUG_LOG_BUF_LOCK_VICTIM_ADDR			reg_map(DEBUG_LOG_LOCK_VICTIM_ADDR)
#define DEBUG_LOG_BUF_READ_INDEX_ADDR			reg_map(DEBUG_LOG_READ_INDEX_ADDR)
#define DEBUG_LOG_BUF_WRITE_INDEX_ADDR			reg_map(DEBUG_LOG_WRITE_INDEX_ADDR)






#define LOG_PROTECT_OVERFLOW

// Macro Protocol
#define DEBUG_PROTOCL_TYPE_LOG				(0x00)   // The basic log type
#define DEBUG_PROTOCL_TYPE_ROUND_BUFFER	(0x01)   // 



#define DEBUG_PROTOCL_PAYLOAD_START_OFFSET   (3)
#define DEBUG_PROTOCL_HEADER_SIZE                            (4)
#define DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH      (50)
#define DEBUG_PROTOCL_PAYLOAD_ALLOW_LENGTH  (DEBUG_PROTOCL_TOTAL_ALLOW_LENGTH-DEBUG_PROTOCL_HEADER_SIZE)


// Macro Log
#define LOG_BUFFER_SIZE (DEBUG_LOG_BUFFER_CNT)
#define LOG_LEVEL_NORMAL    (0)
#define LOG_LEVEL_INFORM    (1)
#define LOG_LEVEL_CORE        (2)
#define LOG_LEVEL_ERROR     (3)

#ifdef DEF_PRINTHEX
void write_long_value(uint8_t* addr, uint32_t value);
void print_round_buffer(uint8_t index, uint32_t start_addr, uint32_t end_addr, uint32_t write_addr, uint32_t read_addr);
void uart_txBuffer(uint8_t* send_buf, uint32_t length);
void uart_txChar(unsigned char c);
void init_debugUart();
void debugUart_pollTx();
void yichip_debug_uart_protocol(uint8_t type, uint8_t length
	, uint8_t* uart_send_buf, uint8_t* uart_send_length_ptr);
void init_print_log(void);
bool print_log_buffer_is_empty(void);
void print_log_to_uart(void);
void print_log(uint16_t flag, uint16_t value);
void print_log_get_lock(void);
void print_log_free_lock(void);
#define DEBUG_INIT()	init_debugUart()
#define DEBUG_LOG(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE)  print_log(FLAG,VALUE)
#define DEBUG_LOG_2(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1)  \
	print_log(FLAG,(((VALUE_0) & 0xff)<<8)|(((VALUE_1) & 0xff)<<0))
#define DEBUG_LOG_4(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3) \
	print_log(FLAG,(((VALUE_0) & 0x0f)<<12)|(((VALUE_1) & 0x0f)<<8)|(((VALUE_2) & 0x0f)<<4)|(((VALUE_3) & 0x0f)<<0))
#define DEBUG_LOG_8(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3,VALUE_4,VALUE_5,VALUE_6,VALUE_7)\
	print_log(FLAG,(((VALUE_0) & 0x03)<<14)|(((VALUE_1) & 0x03)<<12)|(((VALUE_2) & 0x03)<<10)|(((VALUE_3) & 0x03)<<8)|(((VALUE_4) & 0x03)<<6)|(((VALUE_5) & 0x03)<<4)|(((VALUE_6) & 0x03)<<2)|(((VALUE_3) & 0x07)<<0))
#define DEBUG_POLLING_PRINT() debugUart_pollTx()
#define DEBUG_LOG_BUFFER_IS_EMPTY() (print_log_buffer_is_empty())
#define DEBUG_LOG_PRINT_ROUND_BUFFER(INDEX, START, END, WRITE, READ) (print_round_buffer(INDEX, START, END, WRITE, READ))
#else
#define DEBUG_INIT() {while(0);}
#define DEBUG_LOG(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE) {while(0);}
#define DEBUG_LOG_2(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1)  {while(0);}
#define DEBUG_LOG_4(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3) {while(0);}
#define DEBUG_LOG_8(LOG_LEVEL,TYPE_STR,STR,FLAG,VALUE_0,VALUE_1,VALUE_2,VALUE_3,VALUE_4,VALUE_5,VALUE_6,VALUE_7) {while(0);}
#define DEBUG_POLLING_PRINT() {while(0);}
#define DEBUG_LOG_BUFFER_IS_EMPTY() (true)
#define DEBUG_LOG_PRINT_ROUND_BUFFER(INDEX, START, END, WRITE, READ) {while(0);}
#endif






// Debug Log Point Define, we should use log function by those macro
// To judge use, in respin, we only allow use point 0x1000-0x8FFF
// To judge use, in cm0, we only allow use point 0x9000-0xEFFF
// 0xF000-0xFFFF reserve for some special use case
enum {
	// For IPC info
	LOG_POINT_9000 = 0x9000,
	LOG_POINT_9001 = 0x9001,
	LOG_POINT_9002 = 0x9002,
	LOG_POINT_9003 = 0x9003,
	LOG_POINT_9004 = 0x9004,
	LOG_POINT_9005 = 0x9005,
	LOG_POINT_9006 = 0x9006,
	LOG_POINT_9007 = 0x9007,
	LOG_POINT_9008 = 0x9008,
	LOG_POINT_9009 = 0x9009,
	LOG_POINT_900A = 0x900A,
	LOG_POINT_900B = 0x900B,
	LOG_POINT_900C = 0x900C,
	LOG_POINT_900D = 0x900D,
	LOG_POINT_900E = 0x900E,

	
	// For UI info
	// Voice
	LOG_POINT_A000 = 0xA000,
	LOG_POINT_A001 = 0xA001,
	// Led
	LOG_POINT_A100 = 0xA100,
	LOG_POINT_A101 = 0xA101,
	// Battery
	LOG_POINT_A200 = 0xA200,
	LOG_POINT_A201 = 0xA201,
	LOG_POINT_A202 = 0xA202,
	LOG_POINT_A203 = 0xA203,
	LOG_POINT_A204 = 0xA204,
	LOG_POINT_A205 = 0xA205,
	LOG_POINT_A206 = 0xA206,
	LOG_POINT_A207 = 0xA207,
	LOG_POINT_A208 = 0xA208,
	LOG_POINT_A209 = 0xA209,
	LOG_POINT_A20A = 0xA20A,
	// Key
	LOG_POINT_A300 = 0xA300,


	// TWS
	// Audio sync
	LOG_POINT_B000 = 0xB000,
	LOG_POINT_B001 = 0xB001,
	LOG_POINT_B002 = 0xB002,
	LOG_POINT_B003 = 0xB003,
	LOG_POINT_B004 = 0xB004,
	LOG_POINT_B005 = 0xB005,
	LOG_POINT_B006 = 0xB006,
	LOG_POINT_B007 = 0xB007,
	LOG_POINT_B008 = 0xB008,
	LOG_POINT_B009 = 0xB009,
	LOG_POINT_B00A = 0xB00A,
	LOG_POINT_B00B = 0xB00B,
	LOG_POINT_B00C = 0xB00C,
	LOG_POINT_B00D = 0xB00D,
	LOG_POINT_B00E = 0xB00E,
	LOG_POINT_B00F = 0xB00F,
	LOG_POINT_B010 = 0xB010,
	LOG_POINT_B011 = 0xB011,
	LOG_POINT_B012 = 0xB012,
	LOG_POINT_B013 = 0xB013,
	LOG_POINT_B014 = 0xB014,
	LOG_POINT_B015 = 0xB015,

	LOG_POINT_B020 = 0xB020,
	LOG_POINT_B021 = 0xB021,
	
	LOG_POINT_B030 = 0xB030,
	LOG_POINT_B031 = 0xB031,
	LOG_POINT_B032 = 0xB032,
	LOG_POINT_B033 = 0xB033,
	LOG_POINT_B034 = 0xB034,
	LOG_POINT_B035 = 0xB035,
	LOG_POINT_B036 = 0xB036,
	
	// Key Remap
	LOG_POINT_C000 = 0xC000,
	LOG_POINT_C001 = 0xC001,
	LOG_POINT_C002 = 0xC002,
	LOG_POINT_C003 = 0xC003,
	LOG_POINT_C004 = 0xC004,
	LOG_POINT_C005 = 0xC005,
	LOG_POINT_C006 = 0xC006,
	
	//Led Remap
	LOG_POINT_C100 = 0xC100,
	LOG_POINT_C101 = 0xC101,
	LOG_POINT_C102 = 0xC102,
	LOG_POINT_C103 = 0xC103,


	// For Error Debug, from 0xfff9 to 0xfffe use for cm0
	LOG_POINT_FFF9 = 0xFFF9,
};


enum {
	// For cm0 error code
	ERROR_CODE_WATCH_DOG = 0x0001,
};






















// Debug Round Buffer Index Define, one buffer should only use one
enum {
	DEBUG_ROUND_BUFFER_INDEX_00 = 0x00,
	DEBUG_ROUND_BUFFER_INDEX_01 = 0x01,
	DEBUG_ROUND_BUFFER_INDEX_02 = 0x02,
	DEBUG_ROUND_BUFFER_INDEX_03 = 0x03,
	DEBUG_ROUND_BUFFER_INDEX_04 = 0x04,
	DEBUG_ROUND_BUFFER_INDEX_05 = 0x05,
	DEBUG_ROUND_BUFFER_INDEX_06 = 0x06,
	DEBUG_ROUND_BUFFER_INDEX_07 = 0x07,
	DEBUG_ROUND_BUFFER_INDEX_08 = 0x08,
	DEBUG_ROUND_BUFFER_INDEX_09 = 0x09,
	DEBUG_ROUND_BUFFER_INDEX_0A = 0x0A,
	DEBUG_ROUND_BUFFER_INDEX_0B = 0x0B,
};






#endif //_DRV_DEBUG_H_

