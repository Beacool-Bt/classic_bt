#include <stdlib.h>
#include <stdio.h> 
#include "yc11xx_usb.h"
#include "Drv_debug.h"
#include "yc11xx_gpio.h"

const device_descriptor devicedesc =
{
    0x12,
    0x01,
    0x0002,
    0x00,
    0x00,
    0x00,
    EP0_PACKET_SIZE,
    0x1224,
    0x1218,
    0x0200,
    0x01,
    0x02,					
    0x00,
    0x01,
}; //end of DEVICEDESC


const uint8_t hidreportdesc_kb[HID_KB_RP_DESCRIPTOR_LEN] =
{
    //kb hid
0x05,0x01,	// Usage Page(Generic Desktop)
0x09,0x06, 	// Usage(Keyboard)
0xa1,0x01, 	// Collection(Application)
0x05,0x07, 	// Usage Page(Keyboard/Keypad)
0x19,0xe0, 	// Usage Minimum(224)
0x29,0xe7,	// Usage Maximum(231)
0x15,0x00, 	// Logical Minimum(0)
0x25,0x01, 	// Logical Maximum(1)
0x75,0x01,	// Report Size(1)
0x95,0x08,	// Report Count(8)
0x81,0x02, 	// Input(Data Variable Absolute) ; Modifier byte
0x95,0x01, 	// Report Count(1)
0x75,0x08, 	// Report Size(8)
0x81,0x01, 	// Input(Padding) ; Reserved byte
0x95,0x05, 	// Report Count(5)
0x75,0x01, 	// Report Size(1)
0x05,0x08, 	// Usage Page(LEDs)
0x19,0x01, 	// Usage Minimum(1)
0x29,0x05, 	// Usage Maximum(5)
0x91,0x02, 	// Output(Data Variable Absolute) ; LED report
0x95,0x01, 	// Report Count(1)
0x75,0x03, 	// Report Size(3)
0x91,0x01, 	// Output(Padding) ; LED report padding
0x95,0x06, 	// Report Count(6)
0x75,0x08, 	// Report Size(8)
0x15,0x00, 	// Logical Minimum(0)
0x25,0x68, 	// Usage Maximum(104)
0x05,0x07, 	// Usage Page(Keyboard/Keypad)              
0x19,0x00, 	// Usage Minimum(0)
0x29,0x68, 	// Usage Maximum(104)
0x81,0x00, 	// Input(Data Array) ; Key arrarys 6 bytes
0xc0    
};

const uint8_t hidreportdesc_m[HID_MO_RP_DESCRIPTOR_LEN]={
//====================鼠标====75====================================
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01, 				   // Report ID (1)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x05,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x03,                    //     REPORT_SIZE (3)
    0x81, 0x01,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x16, 0x01, 0xF8,			   //	  Logical Minimum (-2047)
    0x26, 0xFF, 0x07,			   //	  Logical Maximum (2047) 
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x02,                    //     REPORT_COUNT (2)    
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)  
    0x05, 0x0c,					   //	  USAGE_PAGE(Consumer Devices)
    0x0a, 0x38,0x02,
    0x95, 0x01,					   //     REPORT_COUNT (1)	
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0,                           // END_COLLECTION
//====================电源控制======31==============================
    0x05, 0x01, 	// Usage Page (Generic Desktop),
    0x09, 0x80,	 	// USAGE (3-D Digitizer)
    0xa1, 0x01,	 	// Collection (Application),
    0x85, 0x02, 	// Report ID (2)
    0x05, 0x01, 	// Usage Page (Generic Desktop),
    0x19, 0x81, 	// Usage Minimum (), 
    0x29, 0x83, 	// Usage Maximum (),
    0x15, 0x00, 	// Logical Minimum (0),
    0x25, 0x01, 	// Logical Maximum (1),
    0x95, 0x03, 	// Report Count (3),
    0x75, 0x01, 	// Report Size (1),
    0x81, 0x06, 	// Input (Data,Var,Rel)
    0x95, 0x01, 	// Report Count (1),  
    0x75, 0x05, 	// Report Size (5),	
    0x81, 0x01,		// Input (Const,Ary,Abs)
    0xc0,			// END_COLLECTION

//====================多媒体按键控制=======25========================
    0x05, 0x0c,		 // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,		 // USAGE (Consumer Control)
    0xa1, 0x01,  	 // COLLECTION (Application)
    0x85, 0x03, 	 // Report ID (3)
    0x15, 0x00,		 // LOGICAL_MINIMUM (0)
    0x26, 0x80, 
    0x03, 0x19,  
    0x00, 0x2a, 
    0x80, 0x03,  
    0x75, 0x10, 	 // REPORT_SIZE (0x10)
    0x95, 0x01, 	 // REPORT_COUNT (1)
    0x81, 0x00, 	 // Input (Data,Ary,Abs)
    0xc0, 			 // END_COLLECTION

//====================MCE键盘控制=======26===========================
    0x06, 0xbc, 0xff, // USAGE_PAGE (Vendor Defined Page *)
    0x09, 0x88,   	  // USAGE(?)
    0xa1, 0x01, 	  // Collection (Application),
    0x85, 0x04, 	  // Report ID (4)
    0x19, 0x00, 	  // USAGE_MINIMUM (undefined)
    0x2a, 0xff, 0x00, 
    0x15, 0x00, 	  // Logical Minimum (0),
    0x26, 0xff, 0x00, 
    0x75, 0x08,  	  // REPORT_SIZE (8)
    0x95, 0x01,  	  // REPORT_COUNT (1)
    0x81, 0x00,  	  // Input (Data,Ary,Abs)
    0xc0,  			  // END_COLLECTION

//====================beken============38======================
    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x0e,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0xBA,      // Report ID
    0x95, 0x1f,  //   REPORT_COUNT ( )
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)

    0x85, 0xBA,      // Report ID
    0x95, 0x1f,  //   REPORT_COUNT ( )
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xC0 
};

const uint8_t confdesc[HID_CONFIG_DESCRIPTOR_LEN] = {
// From "USB Device Class Definition for Human Interface Devices (HID)".
// Section 7.1:
// "When a Get_Descriptor(Configuration) request is issued,
// it returns the Configuration confdesc, all Interface descriptors,
// all Endpoint descriptors, and the HID confdesc for each interface."
// configuration_descriptor hid_configuration_descriptor

    //config
    0x09,//0xLength	
    0x02,//0xType
    HID_CONFIG_DESCRIPTOR_LEN, 0x00,//0xTotallength
    0x02,//0xNumInterfaces
    0x01,//0xbConfigurationValue
    0x00,//0xstringindexConfiguration
    0xa0,//0xbmAttributes
    0x32,//0xMaxPower0x(in0x2mA0xunits)

    //0xinterface_descriptor0xhid_interface_descriptor--mouse
    0x09,//0xbLength
    0x04,//0xbDescriptorType
    0x00,//0xbInterfaceNumber
    0x00,//0xbAlternateSetting
    0x01,//0xbNumEndpoints
    0x03,//0xbInterfaceClass0x(30x=0xHID)
    0x01,//0xbInterfaceSubClass
    0x01,//0xbInterfaceProcotol0x0x0x;0x0x2=mouse
    0x00,//0xiInterface

    //0xclass_descriptor0xhid_descriptor--mouse	0x0x0x0x0x
    0x09,//0xbLength
    0x21,//0xbDescriptorType
    0x00,0x02,//0xbcdHID
    0x00,//0xbCountryCode
    0x01,//0xbNumDescriptors
    0x22,//0xbDescriptorType
    HID_KB_RP_DESCRIPTOR_LEN,0x00,//0xwItemLength0x(tot.0xlen.0xof0xreport0xconfdesc)

    //0xendpoint_descriptor0xhid_endpoint_in_descriptor--mouse
    0x07,//0xbLength
    0x05,//0xbDescriptorType
    0x81,//0xbEndpointAddress
    0x03,//0xbmAttributes
    EP1_PACKET_SIZE,0x00,//0xMaxPacketSize0x
    0x02,//0xbInterval
	     
    0x09,
    0x04,	//bDescriptorType   
    0x01,		//bInterfaceNumber  
    0x00,		//bAlternateSetting
    0x01,		//bNumEndpoints  
    0x03,		//bInterfaceClass (3 = HID)   
    0x01,		//bInterfaceSubClass  
    0x02,		//bInterfaceProcotol ; 2=mouse  
    0x00,		//iInterface

   //class_descriptor hid_descriptor--mouse 
 
   0x09,		//bLength   
   0x21,		//bDescriptorType  
   0x00,0x02,	//bcdHID 
   0x00,		//bCountryCode  
   0x01,		//bNumDescriptors  
   0x22,		//bDescriptorType
   HID_MO_RP_DESCRIPTOR_LEN,0x00,	//wItemLength (tot. len. of report confdesc)
 
 //endpoint_descriptor hid_endpoint_in_descriptor--mouse
 
   0x07,		//bLength  
   0x05,		//bDescriptorType  
   0x82,		//bEndpointAddress  
   0x03,		// bmAttributes  
   EP2_PACKET_SIZE, 0x00,	//MaxPacketSize (LITTLE  ENDIAN)  
   0x02,		//bInterval

};



#define STR0LEN 4
const uint8_t string0[STR0LEN] =
{
   STR0LEN, 0x03, 0x09, 0x04
}; //end of string0_desc

#define STR1LEN sizeof ("YICHIP") * 2
const uint8_t string1[] =
{
   STR1LEN, 0x03,
   'Y', 0,
   'i', 0,
   'C', 0,
   'h', 0,
   'i', 0,
   'p', 0 
}; //end of string1_desc

#define STR2LEN sizeof ("YICHIP 2.4G Device") * 2
const uint8_t string2[] =
{
   STR2LEN, 0x03,
   'Y', 0,
   'i', 0,
   'C', 0,
   'h', 0,
   'i', 0,
   'p', 0, 
   ' ', 0,
   '2', 0,
   '.', 0,
   '4', 0,
   'G', 0,
   ' ', 0,
   'D', 0,
   'e', 0,
   'v', 0,
   'i', 0,
   'c', 0,
   'e', 0
}; //end of string2_desc

#define STR3LEN sizeof ("b120300001") * 2
const uint8_t string3[]= 
{
STR3LEN,0x03,
'b','0',
'1','0',
'2','0',
'0','0',
'3','0',
'0','0',
'0','0',
'0','0',
'0','0',
'1','0',
};//"b120300001";

const uint8_t* const string_table [] =
{
   string0,
   string1,
   string2,
   string3
};

extern uint8_t ms_data[8];
extern uint8_t kb_data[8];
extern uint8_t kb_multikey[8];
extern uint8_t usb_ep1_data,usb_ep2_data;

uint8_t ONES_PACKET[2] ={0x01,0x00};
uint8_t ZERO_PACKET[2] ={0x00,0x00};
uint8_t TWO_PACKET[2] ={0x02,0x00};
usb_setup usb0_setup ;
const uint8_t *bufptr;
uint8_t  remain,isstr;
uint8_t usb_state,usb0_state,usb_tx_enable,usb_wakestate_onetime_flag;
uint8_t  usb_tx_wait,usb_clear_halt,usb_tx_win_enable,usb_mac_wake_trig;
uint8_t usb_set_high_addr_flag,usb_get_protocol_flag,
             usb_device_enumeration_endflag;
uint8_t usb_idle_flag,usb_set_protocol_status,
            usb_set_protocol_value;
uint8_t usb_remote_wakeup,usb_idle_rate,usb_clear_remote_wakeup;
uint8_t usb_ep0_stall_status,usb_ep1_stall_status,usb_ep2_stall_status,usb_ep3_stall_status;

uint32_t wakeup_delay =0;
uint32_t nak_clk_now =0;

void usb_init()
{
	//uint8_t  temp;
	HWRITE(CORE_USB_CONFIG,0x00);
	delay_us(850);
	HWRITE(CORE_USB_TRIG,0xc0);
	//usb clock
	HWCOR(CORE_CLKOFF + 1, 0x04);
	
//	temp = HREAD(USB_CLOCK);
//	temp = temp & 0xFB;
//	HWRITE(0x8051,temp);
	
	HWRITE(CORE_USB_CONFIG,0x3c);
	//HWRITE(USB_INT_MASK,0x00);
	//HWRITE(USB_INT_MASK+1,0x00);
	//HWRITE(USB_INT_MASK+2,0xFF);
	
	HWRITE(CORE_USB_STATUS,0xFF);
	HWRITE(CORE_USB_FIFO_EMPTY,0xFF);
	HWRITE(CORE_USB_ADDR,0x00);
	usb_state = USB_STAY_BY;
	usb0_state = DEV_DEFAULT;
	usb_tx_wait = 0;
	usb_set_protocol_status = 0;
	usb_get_protocol_flag = 0;
	usb_idle_flag = 0;
	#ifdef  SDK_DEBUG
	printf("usb init\r\n");
	#endif

}

static void usb_wakeup()
{
	if((HREAD(CORE_USB_CONFIG)>>7))
	{
		HWRITE(CORE_USB_CONFIG,0xfc);
		delay_ms(2);
		delay_us(500);
		HWRITE(CORE_USB_CONFIG,0x3c);
	}
	else
	{
		return;
		//break;
	}

}

static void usb0_reply_zerolen(void)
{
	HWRITE(CORE_USB_CLEAR,0X40);
}

static void usb0_forcce_stall(void)
{
	HWRITE(CORE_USB_STALL,HREAD(CORE_USB_STALL) |0x01);
	usb0_reply_zerolen();
}



static void usb_trig(int ep)
{
	HWRITE(CORE_USB_TRIG,1 <<ep) ;
	usb_tx_wait   |= ( 1 << ep);
}

static void usb0_tx()
{
	uint8_t len,i;
	if(remain)
	{
		len= remain > EP0_PACKET_SIZE ? EP0_PACKET_SIZE : remain;
		for(i = 0;i<len;i++)
		{
		 HWRITE(CORE_USB_DFIFO(0) , bufptr[i]) ;
		 delay_us(1);
//		 #ifdef  SDK_DEBUG
//		printf("  %x,%d\r\n",bufptr[i],usb0_setup.bValue.c[MSB]);
//		 #endif
//		if(isstr) CORE_USB_EP(0) = 0;
		}
		bufptr+=len;
		usb_trig(0);
	}
	else
	{
		return;
	}
//		bufptr+=len;
		remain-=len;
		delay_us(50);
//		#ifdef  SDK_DEBUG
//		printf("指针地址= %x,剩余buf大小= %d\r\n",bufptr,remain);
//		#endif
	if(remain)
	{
		return;
	}
			
	if(usb_state == USB_GOT_REPORT_REQ)
		usb_state = USB_CONNECTED;
	printf("usb状态机= %d\r\n",usb_state);
}

static void usb0_respend( const uint8_t *buff,uint8_t size)
{
//	if(isstr)
//	{
//		CORE_USB_EP(0) = size-- *2;
//		CORE_USB_EP(0) = 3;
//	}
	if(size > usb0_setup.bLength.i)
	{
		size = usb0_setup.bLength.i;
	}
		bufptr = buff;
		remain = size;
		usb0_tx();
}

void Msdata_normal_mode()
{
	uint8_t i;
	
	for(i=0;i<8;i++)
	{
		HWRITE(CORE_USB_DFIFO(2),ms_data[i]);
	}
	usb_trig(2);
}

void Msdata_mac_boot_mode()
{
	uint8_t i;
	if((ms_data[1] == R_KEY) ||(ms_data[1] == LR_KEY))
	{
		Msdata_normal_mode();
	}
	else
	{
		ms_data[0] = ms_data[1];
		ms_data[1] = ms_data[2];
		ms_data[2] = ms_data[4];
		ms_data[3] = ms_data[6];
		for(i=0;i<4;i++)
		{
			HWRITE(CORE_USB_DFIFO(2),ms_data[i]);
		}
		usb_trig(2);
	}
	
}


void usb_tx_ms_data()
{
	if((usb_idle_flag &usb_get_protocol_flag) == 0)
	{
		usb_ep2_data =0;
		Msdata_normal_mode();
		return;
	}
	else if((usb_idle_flag & usb_set_protocol_status) == MAC_BOOT_MODE)
	{
		usb_ep2_data =0;
		Msdata_mac_boot_mode();
		return;
	}
	else
	{
		usb_ep2_data =0;
		Msdata_normal_mode();
	}
	
}

void usb_tx_kb_normal_data()
{
	uint8_t i;
	usb_ep1_data =0;
	for(i=0;i<8;i++)
	{
	HWRITE(CORE_USB_DFIFO(1),kb_data[i]);
	}
	usb_trig(1);
}
void usb_tx_kb_multikey_data()
{
	uint8_t i;
	usb_ep1_data =0;
	for(i=0;i<3;i++)
	{
		HWRITE(CORE_USB_DFIFO(1),kb_multikey[i]);
		//kb_multikey[0] = KB_MULTIKEY_REPORT_ID;
	}
	usb_trig(1);
}

void usb_tx_ep2()
{
	if(usb_tx_enable &0x04)
	{
		if(usb_tx_wait &0x04)
		{
			return;
		}
		else
		{
			usb_tx_enable &= 0xfb;
			//ep2_data
			if(usb_ep2_data == USB_EP2_MS)
			{
				usb_tx_ms_data();
			}
			else if(usb_ep2_data == USB_EP2_MULTIKEY)
			{
				usb_tx_kb_multikey_data();
			}
			else
			{
				while(1);
			}
		}
	}
	else
	{
		return;
	}

}

void usb_tx_ep1()
{
	if(usb_tx_enable &0x02)
	{
		if(usb_tx_wait &0x02)
		{
			return;
		}
		else
		{
			usb_tx_enable &= 0xfd;
			//ep1_data
			if(usb_ep1_data == USB_EP1_KB )
			{
				usb_tx_kb_normal_data();
			}
		}
	}
	else
	{
		return;
	}

}

void usb_tx()
{
	if(usb_tx_enable == 0)
	{
		return;
		//break;
	}
	else if(usb_tx_wait)
	{
		switch (usb_state)
		{
		  case USB_SLEEP:
		  	usb_tx_wait = 0;
		  	break;
		  case USB_CONNECTED:
		  	HWRITE(CORE_USB_CONFIG,HREAD(CORE_USB_CONFIG) |0x80);
		  	usb_mac_wake_trig = 1;
		  	break;
		  default:
		  	break;
		}
	}
	else 
	{
		
		if(usb_device_enumeration_endflag)
		{
			if(usb_state == USB_SLEEP)
			{
				//timer
				if(wakeup_delay)
				{
					if(systick_get_ms() -wakeup_delay > 3000)
					{
						wakeup_delay =0;
						usb_wakeup();
					}
				}
				return;
			}
			else if(usb_state != USB_CONNECTED )
			{
				//buf_release
				return;
			}
			else if(usb_clear_remote_wakeup)
			{
				//buf_release
				return;
			}
			usb_tx_ep1();
			usb_tx_ep2();
//		#ifdef  SDK_DEBUG
//		printf(" %d\r\n",usb_state);
//		 #endif
		}
		else
		{
			//buf_release
			return;
		}
	}
}

void Set_Protocol()
{
	if(usb0_setup.bLength.c[LSB])
	{
		usb0_forcce_stall();
	}
	else
	{
		switch (usb0_setup.bValue.c[LSB])
		{
		  case BOOT_PROTOCOL:
		  	usb_set_protocol_status = 0;
		  	usb_set_protocol_value = 1;
		  	usb0_reply_zerolen();
		  	break;
		  case REPORT_PROTOCOL:
		  	usb_set_protocol_status = 1;
		  	usb_set_protocol_value = 0;
		  	usb0_reply_zerolen();
		  	break;
		  default:
		  	usb0_forcce_stall();
		  	break;
		}
	}
}

void Get_Protocol()
{
	if(usb0_setup.bLength.c[LSB] != 1)
	{
	 		usb0_forcce_stall();
	 		return;
	 }
	 	usb_get_protocol_flag =1;
	 	if(usb_set_protocol_value)
	 		usb0_respend(ZERO_PACKET,1);
	 	else
	 	{
			usb0_respend(ONES_PACKET,1);
	 	}
}

void Set_Report()
{
	//match code
	#ifdef  SDK_DEBUG
	printf("set_report usb_state = %d\r\n",usb_state);
	#endif
	usb0_reply_zerolen();
	#ifdef  SDK_DEBUG
	printf("set_report usb_state = %d\r\n",usb_state);
	#endif

}

void Get_Report()
{
	if(usb0_setup.bValue.i != PC_GET_REPORT)
		usb0_forcce_stall();
	else
	{
		while(1);
		//match code
	}
	
}

void Clear_Feature()
{
	if((usb0_state != DEV_CONFIGURED) ||
	usb0_setup.bLength.c[MSB] ||usb0_setup.bLength.c[LSB] ||
	usb0_setup.bValue.c[MSB] ||usb0_setup.wIndex.c[MSB]) 
	{
		usb0_forcce_stall();
	}
	else
	{
		switch (usb0_setup.bmRequestType)
		{
		  case OUT_DEVICE:
		  	if(usb0_setup.bValue.c[LSB] != DEVICE_REMOTE_WAKEUP)
		  	{
				usb0_forcce_stall();
		  	}
		  	else
		  	{
				HWRITE(CORE_USB_CONFIG,HREAD(CORE_USB_CONFIG) &0x7f);
				usb_remote_wakeup = 0;
				usb_clear_remote_wakeup= 1;
				usb0_reply_zerolen();
		  	}
		  	break;
		  case OUT_INTERFACE:
		  	usb0_forcce_stall();
		  	break;
		  case OUT_ENDPOINT:
		  	 if(usb0_setup.bValue.c[LSB] != ENDPOINT_HALT )
		 		usb0_forcce_stall();
		 	else
		 	{
				if(usb0_setup.wIndex.c[LSB] == IN_EP1 )
				{
				 	usb_ep1_stall_status = 0;
				 	usb_clear_halt = 1;
				 	usb0_reply_zerolen();
				 }	
				else if(usb0_setup.wIndex.c[LSB] == IN_EP2)
				{
				 	usb_ep2_stall_status =0;
				 	usb_clear_halt = 1;
				 	usb0_reply_zerolen();			
				}
				else if(usb0_setup.wIndex.c[LSB] == IN_EP3)
				{
				 	usb_ep3_stall_status =0;
				 	usb_clear_halt = 1;
				 	usb0_reply_zerolen();		
				}
				else if((usb0_setup.wIndex.c[LSB] & 0x7f)== 0x00)
				{
				 	usb_ep0_stall_status =0;
				 	usb0_reply_zerolen();	
				}
				else
				{
					usb0_forcce_stall();
				}
		 	}
		 	break;
		 default:
		 	usb0_forcce_stall();
		 	break;


		}

	}

}

void Get_Idle()
{
	usb_idle_flag = 1;
	 usb0_respend(( uint8_t * )&usb_idle_rate,1);
}
void Set_Idle()
{
	usb_idle_flag =1;
	usb_idle_rate = usb0_setup.bValue.c[MSB];
	if(usb0_setup.wIndex.c[LSB] == 1)
	{
		usb_device_enumeration_endflag =1;
		usb_clear_remote_wakeup =0;
		usb_state = USB_RESUME;
	}
	if(usb_state != USB_RESUME )
	{
		usb0_reply_zerolen();
		return;
	}
	usb_state = USB_CONNECTED;
	usb0_reply_zerolen();
	delay_ms(5);
}

void Set_Feature()
{
	if(usb0_setup.bLength.c[MSB] || usb0_setup.bLength.c[LSB])
	{
		usb0_forcce_stall();
	}
	else
	{
		switch(usb0_setup.bmRequestType)
		{
		   case OUT_DEVICE:
		   	if(usb0_setup.bValue.c[LSB] != DEVICE_REMOTE_WAKEUP)
		   	{
		   		usb0_forcce_stall();
		   	}
		   	else 
		   	{
				HWRITE(CORE_USB_CONFIG,HREAD(CORE_USB_CONFIG) |0x80);
				usb_remote_wakeup = 1;
				usb_state =USB_SLEEP;
		//		usb_tx_win_enable = 0;
				usb0_reply_zerolen();
				//wake up timer
				wakeup_delay =systick_get_ms();
		   	}
		   	break;
		   case OUT_INTERFACE:
		   	usb0_forcce_stall();
		   	break;
		   case OUT_ENDPOINT:
		   	 if((usb0_setup.bValue.c[LSB] != ENDPOINT_HALT ) ||usb0_setup.bLength.c[MSB])
		 		usb0_forcce_stall();
		 	else
		 	{
				if(usb0_setup.wIndex.c[LSB] == IN_EP1 )
				{
				 	usb_ep1_stall_status = 1;
				 	usb0_reply_zerolen();
				 }	
				else if(usb0_setup.wIndex.c[LSB] == IN_EP2)
				{
				 	usb_ep2_stall_status =1;
				 	usb0_reply_zerolen();			
				}
				else if(usb0_setup.wIndex.c[LSB] == IN_EP3)
				{
				 	usb_ep3_stall_status =1;
				 	usb0_reply_zerolen();		
				}
				else if((usb0_setup.wIndex.c[LSB] & 0x7f)== 0x00)
				{
				 	usb_ep0_stall_status =1;
				 	usb0_reply_zerolen();	
				}
				else
				{
					usb0_forcce_stall();
				}
		 	}
		   	break;
		   default:
		   	usb0_forcce_stall();
		   	break;


		}
	}

}

void set_wait_address()
{
	HWRITE(CORE_USB_ADDR,usb0_setup.bValue.c[LSB]);
	usb0_reply_zerolen();	
	usb_set_high_addr_flag = 1;
}

void Set_Address()
{
	if((usb0_setup.bmRequestType != OUT_DEVICE) ||
	usb0_setup.bValue.c[MSB] ||usb0_setup.wIndex.c[MSB] ||
	usb0_setup.wIndex.c[LSB] ||usb0_setup.bLength.c[MSB] ||
	usb0_setup.bLength.c[LSB] ||(usb0_setup.bValue.c[LSB] & 0x80))
	{
		usb0_forcce_stall();
	}
	else
	{
		if(usb0_setup.bValue.c[LSB])
		{
			usb0_state = DEV_ADDRESS;
			set_wait_address();
		}
		else
		{
			usb0_state = DEV_DEFAULT;
			set_wait_address();
		}
	}

}



void Set_Interface()
{
	if((usb0_state != DEV_CONFIGURED) ||
	(usb0_setup.bmRequestType != OUT_INTERFACE) ||
	usb0_setup.bValue.c[MSB] ||usb0_setup.wIndex.c[MSB] ||
	usb0_setup.wIndex.c[LSB] ||usb0_setup.bLength.c[MSB] ||
	usb0_setup.bLength.c[LSB] ||usb0_setup.bValue.c[LSB])
	{
		usb0_forcce_stall();
	}
	else
	{
		usb0_reply_zerolen();		
	}
}

void Set_Descriptor()
{
	usb0_forcce_stall();
}


void set_Wait_Status()
{
	if(usb0_setup.bValue.c[LSB])
	{
		usb0_state = DEV_CONFIGURED;
		usb0_reply_zerolen();	
	}
	else 
	{
		usb0_state = DEV_ADDRESS;
		usb0_reply_zerolen();	
	}
}

void Set_Configuration()
{
	if((usb0_setup.bmRequestType != OUT_DEVICE) ||(usb0_state == DEV_DEFAULT)||
	usb0_setup.bLength.c[MSB] ||usb0_setup.bLength.c[LSB] ||
	usb0_setup.bValue.c[MSB] ||usb0_setup.wIndex.c[MSB] ||
	usb0_setup.wIndex.c[LSB] ||(usb0_setup.bValue.c[LSB] > USB_MAX_NUM_CFG) )
	{
		usb0_forcce_stall();
	}
	else
	{
		switch (usb0_state)
		{
		  case DEV_ADDRESS:
		  	set_Wait_Status ( );
		  	break;
		  case DEV_CONFIGURED:
		  	set_Wait_Status ( );
		  	break;
		  default:
		  	usb0_forcce_stall();
		  	break;

		}
	}
}



void Get_Configuration(void)
{
	if((usb0_setup.bmRequestType != IN_DEVICE) ||
	usb0_setup.bValue.c[MSB] ||usb0_setup.bValue.c[LSB] ||
	usb0_setup.wIndex.c[MSB] ||usb0_setup.wIndex.c[LSB] ||
	usb0_setup.bLength.c[MSB] ||(usb0_setup.bLength.c[LSB] != 1))
	{
		usb0_forcce_stall();
	}
	else
	{
		switch (usb0_state)
		{
		  case DEV_CONFIGURED:
		  	usb0_respend(ONES_PACKET,1);
		  	break;
		  case DEV_ADDRESS:
		  	usb0_respend(ZERO_PACKET,1);
		  	break;
		  default:
		  	usb0_forcce_stall();
		 	break;

		}
	}
}

void Get_Descriptor()
{	
	uint8_t index;
//	isstr = 0;
	switch(usb0_setup.bValue.c[MSB])
	{
	  case DSC_DEVICE:
	  	if(usb_set_high_addr_flag)
	  	{
	  		HWRITE(CORE_USB_ADDR,HREAD(CORE_USB_ADDR) |(1<<7));
			usb_set_high_addr_flag = 0;
	  	}
	  	usb0_respend(( uint8_t *)&devicedesc,sizeof(devicedesc));
	  	break;
	  case DSC_CONFIG:
	  	usb0_respend(confdesc,sizeof(confdesc));
	  	break;
	  case DSC_STRING:
	  	if(usb0_setup.bValue.c[LSB] >3)
	  	{
			usb0_forcce_stall();
			break;
	  	}
	  	index = usb0_setup.bValue.c[LSB] & 3;
//	  	isstr = index > 0;
	  	usb0_respend(string_table[index],string_table[index][0]);
	  	break;
	  case DSC_HID:
	  	usb_get_protocol_flag = 0;
	  	if(usb0_setup.wIndex.c[LSB])
	  		usb0_respend(confdesc + 18,confdesc[18]);
	  	else if(usb0_setup.wIndex.c[LSB] == 1)
	  		usb0_respend(confdesc + 43,confdesc[43]);
	  	else
	  		usb0_forcce_stall();
	  	break;
	  case DSC_HID_REPORT:
	  	if(usb0_setup.wIndex.i == 1)
	  		{
				usb_device_enumeration_endflag = 1;
				usb_state = USB_GOT_REPORT_REQ;
				usb0_respend(hidreportdesc_m,sizeof(hidreportdesc_m));
		#ifdef  SDK_DEBUG
		printf("usb 枚举结束\r\n");
		#endif
	  		}
	  	else
	  		{
	  		usb0_respend(hidreportdesc_kb,sizeof(hidreportdesc_kb));
	  		}
	  	break;
	  default:
	  	usb0_forcce_stall();
	  	break;
	}
}

void Get_Interface()
{
	if((usb0_state != DEV_CONFIGURED) ||
	(usb0_setup.bmRequestType != IN_INTERFACE) ||
	usb0_setup.bLength.c[MSB] ||(usb0_setup.bLength.c[LSB] != 1)||
	usb0_setup.bValue.c[MSB] ||usb0_setup.bValue.c[LSB] ||
	usb0_setup.wIndex.c[MSB] ||usb0_setup.wIndex.c[LSB]) 
	{
		usb0_forcce_stall();
	}
	else
	{
		usb0_respend(ZERO_PACKET,1);
	}
}
void Get_Status()
{
	if((usb_state == DEV_DEFAULT) ||usb0_setup.bValue.c[MSB] ||
	usb0_setup.bValue.c[LSB] ||usb0_setup.bLength.c[MSB] ||
	(usb0_setup.bLength.c[LSB] != 2) ||usb0_setup.wIndex.c[MSB])
	{
		usb0_forcce_stall();
	}
	else
	{
		switch (usb0_setup.bmRequestType)
		{
		 case IN_DEVICE:
		 	if(usb0_setup.wIndex.c[LSB])
		 	{
		 		usb0_forcce_stall();
		 		return;
		 	}
		 	else if(usb_remote_wakeup == 1)
		 		usb0_respend(TWO_PACKET,2);
		 	else
		 	{
				usb0_respend(ZERO_PACKET,2);
		 	}	
		 	break;
		 case IN_INTERFACE:
		 	if(usb0_state != DEV_CONFIGURED )
		 		usb0_forcce_stall();
		 	else
		 	{
				usb0_respend(ZERO_PACKET,2);	
		 	}
		 	break;
		 case IN_ENDPOINT:
		 if(usb0_state != DEV_CONFIGURED )
		 		usb0_forcce_stall();
		 else
		 {
				if(usb0_setup.wIndex.c[LSB] == IN_EP1 )
				{
				 	if(usb_ep1_stall_status)
				 		usb0_respend(ONES_PACKET,2);	
				 	else
				 	{
						usb0_respend(ZERO_PACKET,2);
				 	}
				}
				else if(usb0_setup.wIndex.c[LSB] == IN_EP2)
				{
				 	if(usb_ep2_stall_status)
				 		usb0_respend(ONES_PACKET,2);	
				 	else
				 	{
						usb0_respend(ZERO_PACKET,2);
				 	}
				}
				else if(usb0_setup.wIndex.c[LSB] == IN_EP3)
				{
				 	if(usb_ep3_stall_status)
				 		usb0_respend(ONES_PACKET,2);	
				 	else
				 	{
						usb0_respend(ZERO_PACKET,2);
				 	}
				}
				else if((usb0_setup.wIndex.c[LSB] & 0x7f)== 0x00)
				{
				 	if(usb_ep0_stall_status)
				 		usb0_respend(ONES_PACKET,2);	
				 	else
				 	{
						usb0_respend(ZERO_PACKET,2);
				 	}
				}
				else
				{
					usb0_forcce_stall();
				}
		 	}
		 	break;
		 default:
		 	usb0_forcce_stall();
	  		break;
		}
		
	}
}


void usb_in_class_req()
{
	switch (usb0_setup.bRequest)
	{
	 case GET_IDLE:
	 	Get_Idle(); 
	 	break;
	 case GET_REPORT:
	 	Get_Report();
	 	break;
	 case GET_PROTOCOL:
	 	Get_Protocol();	
	 	break;
	 default:
	 	usb0_forcce_stall();
	 	break;
	}

}
void usb_in_standard_req()
{
	switch(usb0_setup.bRequest)
	{
	case GET_CONFIGURATION:
		Get_Configuration();
		break;
	case GET_DESCRIPTOR:
		Get_Descriptor();
		break;
	case GET_INTERFACE:
		Get_Interface();
		break;
	case GET_STATUS:
		Get_Status();
		break;
	default:
		usb0_forcce_stall();
		break;	
	}
}


void usb_out_standard_req()
{
	switch (usb0_setup.bRequest)
	{
	  case CLEAR_FEATURE:
	  	Clear_Feature();
	  	break;
	  case SET_FEATURE:
	  	Set_Feature();
	  	break;
	  case SET_ADDRESS:
	  	Set_Address();
	  	break;
	  case SET_CONFIGURATION:
	  	Set_Configuration();
	  	break;
	  case SET_INTERFACE:
	  	Set_Interface();
	  	break;
	  case SET_DESCRIPTOR:
	  	Set_Descriptor();
	  	break;
	  default:
	  	usb0_forcce_stall();
	  	break;
	}
	

}


void usb_out_class_req()
{
	switch(usb0_setup.bRequest)
	{
	  case SET_IDLE:
	  	Set_Idle();
	  	break;
	  case SET_REPORT:
	  	Set_Report();
	  	break;
	  case SET_PROTOCOL:
	  	Set_Protocol();
	  	break;
	  default:
	  	usb0_forcce_stall();
	  	break;

	}

}


void usb_out_device()
{
	if(((usb0_setup.bmRequestType >> 5) & 0x03 )== STANDARD_REQ)
	{
		usb_out_standard_req();
	}
	else if (((usb0_setup.bmRequestType >> 5) & 0x03 )== CLASS_REQ)
	{
		usb_out_class_req();
	}
	else
	{
		while(1);
	}


}


void usb_in_device()
{
	if(((usb0_setup.bmRequestType >> 5) & 0x03) == STANDARD_REQ)
	{
		usb_in_standard_req();
	}
	else if (((usb0_setup.bmRequestType >> 5) & 0x03) == CLASS_REQ)
	{
		usb_in_class_req();
	}
	else
	{
		while(1);
	}
}

void usb_class_type()
{
	if((usb0_setup.bmRequestType & 0x80) == IN_DEVICE)
	{
		usb_in_device();
	}
	else if ((usb0_setup.bmRequestType & 0x80) == OUT_DEVICE)
	{
		usb_out_device();
	}
	else
	{
		while(1);
	}
}
	
void  usb_nak_state_judge()
{
	if(usb_state != USB_CONNECTED)
	{
		if(usb_clear_halt)
		{
			usb_clear_halt = 0;
			usb_state = USB_CONNECTED;
			usb_device_enumeration_endflag = 1;
		}
		
	}
	else
	{
		usb_wakestate_onetime_flag = 0;
		usb_mac_wake_trig = 0;
	}
	
}


void USB_IRQHandler()
{
	uint8_t  i,len,empty,status;
	uint8_t  * buf = (uint8_t *) &usb0_setup;
	if(HREAD(CORE_USB_STATUS) &(1<<7))
	{
		usb_init();
	}
	//nak_state_judge
	status = HREAD(CORE_USB_STATUS);
	empty = HREAD(CORE_USB_FIFO_EMPTY);
	if(status &0x40)
	{
		usb_nak_state_judge();
		nak_clk_now = systick_get_ms();
	}
	usb_tx_wait &= ~ HREAD(CORE_USB_FIFO_EMPTY);
//	#ifdef  SDK_DEBUG
//		printf("%x \r\n",usb_tx_wait);
//		 #endif
	
	HWRITE(CORE_USB_STATUS,0xe0);
//	HWRITE(0x8203,1);
	if(empty & 0x01)
	{
		HWRITE(CORE_USB_FIFO_EMPTY,0x1);
		usb0_tx();
	}
	if(status & 1)
	{
		HWRITE(CORE_USB_STATUS,0x11);
		len = HREAD(CORE_USB_EP_LEN);
		if(len <= 7)
		{
			return;
		}
		for (i=0;i<len;i++)
		{
			buf[i] = HREAD(CORE_USB_EP) ;
//		#ifdef  SDK_DEBUG
//		printf(" setup 地址=%x \r\n",&(buf[i]));
//		 #endif
		}
//		if(len <= 7)
//		{
//			return;
//		}
		if(status & USB_STATUS_SETUP )	
		{
//			 if((usb0_setup.bmRequestType & ~ 0x80) ==HCI_CLASS_REQUEST_TYPE)
//			{
//				usb0_reply_zerolen();	
//			}
//			else
//			{
			usb_class_type();
//			}
		}
		
	}
	
	usb_tx();
	if (systick_get_ms() -nak_clk_now > 400)
	{
		if(usb_wakestate_onetime_flag )
		{
			return;
		}
		else if (usb_device_enumeration_endflag)
		{
			switch (usb_state)
			{
				case USB_SLEEP:
					break;
				case USB_CONNECTED:
					if(usb_mac_wake_trig)
					{
						usb_wakestate_onetime_flag =1;
						usb_wakeup();
					}
					break;
				default:
					break;
				
			}
			
		}
	}
	
}






