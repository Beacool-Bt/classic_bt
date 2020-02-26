/*
 * Copyright 2016, yichip Semiconductor
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Yichip Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Yichip Semiconductor.
 */
 
/** 
  *@file yc11xx_usb.h
  *@brief USB support for application.
  */
#ifndef _USB_H_
#define _USB_H_
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yc11xx.h"
#include "yc_drv_common.h"
#include "systick.h"

/**
  *@brief USB type.
  */
#define SDK_DEBUG	//Debug switch
#define USB_TEST
#define MS_REPORT_ID 	       		    0X01
#define KB_NORMAL_REPORT_ID   	    0X00
#define KB_MULTIKEY_REPORT_ID  	    0X03
#define USB_EP1_KB				    0x10
#define USB_EP2_MS				    0x20
#define USB_EP2_MULTIKEY		    0x21
#define MAC_BOOT_MODE			    0x01
#define R_KEY					    0x02
#define LR_KEY					    0x03

/**
  *@brief USB request.
  */
#define  STANDARD_REQ    0x00
#define  CLASS_REQ	        0x01
#define  HCI_CLASS_REQUEST_TYPE  0x20   /*!< SetReport HID Request */                               
#define  HID_CLASS_REQUEST_TYPE  0x21 
#define  HID_TESTS_REQUEST_TYPE  0xa1   /*!< usb 2 cv : hid tests */
#define  CLEAR_PORT_REQUEST_TYPE 0x23
#define  GET_PORT_REQUEST_TYPE  0xa3
#define  ZERO_CLASS_REQUEST_TYPE  0X00

/**
  *@brief Standard Request Codes.
  */
#define  GET_STATUS              0x00  /*!< Code for Get Status */
#define  CLEAR_FEATURE           0x01  /*!< Code for Clear Feature */
#define  SET_FEATURE             0x03  /*!< Code for Set Feature */
#define  SET_ADDRESS             0x05  /*!< Code for Set Address */
#define  GET_DESCRIPTOR          0x06  /*!< Code for Get Descriptor */
#define  SET_DESCRIPTOR          0x07  /*!< Code for Set Descriptor(not used) */
#define  GET_CONFIGURATION       0x08  /*!< Code for Get Configuration */
#define  SET_CONFIGURATION       0x09  /*!< Code for Set Configuration */
#define  GET_INTERFACE           0x0A  /*!< Code for Get Interface */
#define  SET_INTERFACE           0x0B  /*!< Code for Set Interface */
#define  SYNCH_FRAME             0x0C  /*!< Code for Synch Frame(not used) */

/**
  *@brief Standard Descriptor Types.
  */
#define  DSC_DEVICE              0x01  /*!< Device Descriptor */
#define  DSC_CONFIG              0x02  /*!< Configuration Descriptor */
#define  DSC_STRING              0x03  /*!< String Descriptor */
#define  DSC_INTERFACE           0x04  /*!< Interface Descriptor */
#define  DSC_ENDPOINT            0x05  /*!< Endpoint Descriptor */

/**
  *@brief HID Descriptor Types.
  */
#define  DSC_HID			     0x21  /*!< HID Class Descriptor */
#define  DSC_HID_REPORT			 0x22  /*!< HID Report Descriptor */

/**
  *@brief Define bmRequestType bitmaps.
  */
#define  IN_DEVICE               0x80 /*!< Request made to device, direction is IN*/
                                      
#define  OUT_DEVICE              0x00 /*!< Request made to device,direction is OUT */
                                       
#define  IN_INTERFACE            0x81 /*!< Request made to interface,direction is IN */
                                      
#define  OUT_INTERFACE           0x01 /*!< Request made to interface,direction is OUT */
                                      
#define  IN_ENDPOINT             0x82 /*!< Request made to endpoint, direction is IN*/
                                       
#define  OUT_ENDPOINT            0x02 /*!< Request made to endpoint */

/**
  *@brief HID Request Codes.
  */
#define  GET_REPORT 		     0x01   /*!< Code for Get Report */
#define  GET_IDLE				 0x02   /*!< Code for Get Idle */
#define  GET_PROTOCOL			 0x03   /*!< Code for Get Protocol */
#define  SET_REPORT				 0x09   /*!< Code for Set Report */
#define  SET_IDLE				 0x0A   /*!< Code for Set Idle */
#define  SET_PROTOCOL			 0x0B   /*!< Code for Set Protocol */

#define  HID_REPORT_ID           0xF2

/**
  *@brief EP packet size.
  */
#define  EP0_PACKET_SIZE         0x40 
#define  EP1_PACKET_SIZE         0x40
#define  EP2_PACKET_SIZE         0x40
#define  HID_REPORT_SIZE         16
#define  HID_REPORT_DESCRIPTOR_SIZE    0x001B

/**
  *@brief USB state.
  */
#define  USB_STATUS_SETUP  0x10
#define  USB_STATUS_NAK       0x40

/**
  *@brief USB event.
  */
#define  USB_STAY_BY			    0
#define  USB_GOT_REPORT_REQ  1
#define  USB_CONNECTED		    2
#define  USB_SLEEP			    3
#define  USB_RESUME			    4
#define  USB_SETIDLE_1		    5

/**
  *@brief Define device states.
  */
#define  DEV_ATTACHED	  	0X00		/*!< Device is in Attached State */
#define  DEV_POWERED	  	0X01		/*!< Device is in Powered State */
#define  DEV_DEFAULT	  		0X02	/*!< Device is in Default State */
#define  DEV_ADDRESS	  	0X03		/*!< Device is in Addressed State */
#define  DEV_CONFIGURED          0X04	/*!< Device is in Configured State */
#define  DEV_SUSPENDED           0X05	/*!< Device is in Suspended State */

/**
  *@brief Define wIndex bitmaps.
  */
#define  IN_EP1               0x81      /*!< Index values used by Set and Clear */
#define  OUT_EP1           0x01         /*!< commands for Endpoint_Halt */
#define  IN_EP2               0x82
#define  OUT_EP2           0x02 
#define  IN_EP3               0x83 
#define  OUT_EP3     	0x03

/**
  *@brief Define wValue bitmaps for Standard Feature Selectors.
  */
#define  ENDPOINT_HALT       		  0x00       	/*!< Endpoint_Halt feature selector */
#define  DEVICE_REMOTE_WAKEUP 0x01      			/*!< Remote wakeup feature(not used) */
#define  BOOT_PROTOCOL			 0x00 
#define  REPORT_PROTOCOL		 0x01
#define  USB_MAX_NUM_CFG		 0x01

/**
  *@brief PC report.
  */
#define  PC_GET_REPORT			0x01ba
#define  PC_SET_REPORT			0x02ba
#define  PC_SET_REPORT_bValue	0x0200
#define  PC_SET_REPORT_wIndex	0x0000
#define  PC_SET_REPORT_bLength	0x0001
#define  PC_REPORT_ID			0xba

#ifndef  MSB
	#define  MSB 1
#endif

#ifndef  LSB
	#define  LSB 0
#endif

typedef union{uint16_t i;uint8_t c[2];} WORDER;
/**
  *@brief USB set up.
  */
typedef struct
{
	uint8_t  bmRequestType;
	uint8_t  bRequest;
	WORDER bValue;
	WORDER wIndex;
	WORDER bLength;
	uint32_t redundant_buf1;
	uint32_t redundant_buf2;
} usb_setup;

/**
  *@brief Standard Device Descriptor Type Definition.
  */
typedef /*code*/ struct
{
   uint8_t  bLength;              /*!< Size of this Descriptor in Bytes */
   uint8_t  bDescriptorType;      /*!< Descriptor Type (=1) */
   uint16_t bcdUSB;               /*!< USB Spec Release Number in BCD */
   uint8_t  bDeviceClass;         /*!< Device Class Code */
   uint8_t  bDeviceSubClass;      /*!< Device Subclass Code */
   uint8_t  bDeviceProtocol;      /*!< Device Protocol Code */
   uint8_t  bMaxPacketSize0;      /*!< Maximum Packet Size for EP0 */
   uint16_t idVendor;             /*!< Vendor ID */
   uint16_t idProduct;            /*!< Product ID */
   uint16_t bcdDevice;            /*!< Device Release Number in BCD */
   uint8_t  iManufacturer;        /*!< Index of String Desc for Manufacturer */
   uint8_t  iProduct;             /*!< Index of String Desc for Product */
   uint8_t  iSerialNumber;        /*!< Index of String Desc for SerNo */
   uint8_t  bNumConfigurations;   /*!< Number of possible Configurations */
} device_descriptor;            

/**
  *@brief Standard Configuration Descriptor Type Definition.
  */
typedef /*code*/ struct
{
   uint8_t  bLength;              /*!< Size of this Descriptor in uint8_ts */
   uint8_t  bDescriptorType;      /*!< Descriptor Type (=2) */
   uint16_t wTotalLength;         /*!< Total Length of Data for this Conf */
   uint8_t  bNumInterfaces;       /*!< No of Interfaces supported by this */

   uint8_t  bConfigurationValue;  /*!< Designator Value for *this */

   uint8_t  iConfiguration;       /*!< Index of String Desc for this Conf */
   uint8_t  bmAttributes;         /*!< Configuration Characteristics (see below) */
   uint8_t  bMaxPower;            /*!< Configuration Characteristics (see below) */
                                  /*!< Conf (*2mA) */
} configuration_descriptor;       /*!< End of Configuration Descriptor Type */

/**
  *@brief Standard Interface Descriptor Type Definition.
  */
typedef /*code*/ struct
{
   uint8_t  bLength;              /*!< Size of this Descriptor in uint8_ts */
   uint8_t  bDescriptorType;      /*!< Descriptor Type (=4) */
   uint8_t  bInterfaceNumber;     /*!< Number of *this* Interface (0..) */
   uint8_t  bAlternateSetting;    /*!< Alternative for this Interface (if any) */
   uint8_t  bNumEndpoints;        /*!< No of EPs used by this IF (excl. EP0) */
   uint8_t  bInterfaceClass;      /*!< Interface Class Code */
   uint8_t  bInterfaceSubClass;   /*!< Interface Subclass Code */
   uint8_t  bInterfaceProtocol;   /*!< Interface Protocol Code */
   uint8_t  iInterface;           /*!< Index of String Desc for this Interface */
} interface_descriptor;           /*!< End of Interface Descriptor Type */

/**
  *@brief Standard Class Descriptor Type Definition.
  */
typedef /*code */struct
{
   uint8_t  bLength;              /*!< Size of this Descriptor in uint8_ts (=9) */
   uint8_t  bDescriptorType;      /*!< Descriptor Type (HID=0x21) */
   uint16_t bcdHID;    			  /*!< HID Class Specification */
                                  // release number (=1.01)
   uint8_t  bCountryCode;         /*!< Localized country code */
   uint8_t  bNumDescriptors;	  /*!< Number of class descriptors to follow */
   uint8_t  bReportDescriptorType;/*!< Report descriptor type (HID=0x22) */
   uint16_t wItemLength;	  		/*!< Total length of report descriptor table */
} class_descriptor;               /*!< End of Class Descriptor Type */

/**
  *@brief Standard Endpoint Descriptor Type Definition.
  */
typedef /*code*/ struct
{
   uint8_t  bLength;              /*!< Size of this Descriptor in uint8_ts */
   uint8_t  bDescriptorType;      /*!< Descriptor Type (=5) */
   uint8_t  bEndpointAddress;     /*!< Endpoint Address (Number + Direction) */
   uint8_t  bmAttributes;         /*!< Endpoint Attributes (Transfer Type) */
   uint16_t wMaxPacketSize;	      /*!< Max. Endpoint Packet Size */
   uint8_t  bInterval;            /*!< Polling Interval (Interrupt) ms */
} endpoint_descriptor;            

/**
  *@brief HID Configuration Descriptor Type Definition.
  */
/**
  *From "USB Device Class Definition for Human Interface Devices (HID)".
  *Section 7.1:
  *"When a Get_Descriptor(Configuration) request is issued,
  *it returns the Configuration descriptor, all Interface descriptors,
  *all Endpoint descriptors, and the HID descriptor for each interface." 
  */
typedef /*code*/ struct {
	configuration_descriptor 	hid_configuration_descriptor;
	interface_descriptor 		hid_keyboard_interface_descriptor;
	class_descriptor 		hid_keyboard_descriptor;
	endpoint_descriptor 		hid_keyboard_endpoint_in_descriptor;
	interface_descriptor 		hid_ms_interface_descriptor;
	class_descriptor 		hid_ms_descriptor;
	endpoint_descriptor 		hid_ms_endpoint_in_descriptor;
} hid_configuration_descriptor;

#define HID_KB_RP_DESCRIPTOR_LEN  0x3f
#define HID_MO_RP_DESCRIPTOR_LEN  0xc3
#define HID_CONFIG_DESCRIPTOR_LEN 0x3b
/*      configuration 
const hid_configuration_descriptor hid_confdesc;
#define confdesc                   (hid_confdesc.hid_configuration_descriptor)
#define KeyInterfaceDesc   (hid_confdesc.hid_keyboard_descriptor)
#define KeyHidDesc             (hid_confdesc.hid_keyboard_endpoint_in_descriptor)
#define KeyEndpiontDesc  (hid_confdesc.hid_keyboard_interface_descriptor)
#define MsInterfaceDesc     (hid_confdesc.hid_ms_descriptor)
#define MsHidDesc		      (hid_confdesc.hid_ms_endpoint_in_descriptor)
#define MsEndpiontDesc    (hid_confdesc.hid_ms_interface_descriptor)
*/

const device_descriptor devicedesc;
const uint8_t hidreportdesc_kb[HID_KB_RP_DESCRIPTOR_LEN];
const uint8_t hidreportdesc_m[HID_MO_RP_DESCRIPTOR_LEN];
const uint8_t confdesc[HID_CONFIG_DESCRIPTOR_LEN];
const uint8_t* const string_table [4];

/**
  *@brief USB initialization.
  *@param None.
  *@return None.
  */
void usb_init();

/**
  *@brief USB interrupt handler.
  *@param None.
  *@return None.
  */
void USB_IRQHandler();

#endif
