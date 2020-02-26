GCC_ARM_PATH :=  D:\gcc
THUMB_PATH := $(GCC_ARM_PATH)\arm-none-eabi\lib\thumb
CROSS_COMPILE := $(GCC_ARM_PATH)\bin\arm-none-eabi-

PATCH_PATH = ..\..\patch
UTIL_PATH = ..\..\..\util
INC = .\$(GCC_ARM_PATH)\arm-none-eabi\include
SDK_ROOT = ..\..
INCLUDES = -I. -I$(SDK_ROOT)/ -I$(SDK_ROOT)/cmsis/ \
	  -I$(SDK_ROOT)/device/ -I$(SDK_ROOT)/drivers/hal/ \
	  -I$(SDK_ROOT)/drivers/uart/ -I$(SDK_ROOT)/libcore/uart/ \
	  -I$(SDK_ROOT)/drivers/timer/ -I$(SDK_ROOT)/drivers/cvsd/ \
	  -I$(SDK_ROOT)/drivers/keypad/ -I$(SDK_ROOT)/drivers/adc/ \
	  -I$(SDK_ROOT)/patch/  -I$(SDK_ROOT)/libcore/sys/ \
	  -I$(SDK_ROOT)/drivers/adc/ -I$(SDK_ROOT)/drivers/mic/ \
	  -I$(SDK_ROOT)/drivers/dac/  -I$(SDK_ROOT)/drivers/bt/ \
	  -I$(SDK_ROOT)/libcore/util/ -I$(SDK_ROOT)/drivers/log/ \
	  -I$(SDK_ROOT)/drivers/gpio/ -I$(SDK_ROOT)/drivers/keyscan/\
	  -I$(SDK_ROOT)/drivers/led/  -I$(SDK_ROOT)/drivers/qspi/\
	  -I$(SDK_ROOT)/drivers/adpcm/ -I$(SDK_ROOT)/drivers/battery/\
	  -I$(SDK_ROOT)/drivers/voiceprompt/ -I$(SDK_ROOT)/drivers/wdt/ \
	   -I$(SDK_ROOT)/drivers/pwm/ \
      -I$(GCC_ARM_PATH)/arm-none-eabi/include

SRC_FILES = ./main.c  yc_RemoteCtr.c Drv_bt.c Drv_debug.c bt_hfp.c\
            $(SDK_ROOT)/drivers/timer/yc_timer.c \
			$(SDK_ROOT)/drivers/keypad/yc_keypad.c \
			$(SDK_ROOT)/drivers/keypad/yc_queue.c \
            $(SDK_ROOT)/drivers/bt/yc11xx_bt.c \
            $(SDK_ROOT)/drivers/hal/yc_drv_common.c \
			$(SDK_ROOT)/drivers/led/yc_led.c \
			$(SDK_ROOT)/drivers/gpio/yc11xx_gpio.c \
			$(SDK_ROOT)/drivers/qspi/yc11xx_qspi.c \
			$(SDK_ROOT)/drivers/voiceprompt/yc_voiceprompt.c \
			$(SDK_ROOT)/drivers/battery/yc_battery.c \
			$(SDK_ROOT)/drivers/wdt/yc11xx_wdt.c \
			$(SDK_ROOT)/drivers/pwm/yc11xx_pwm.c \
			



OBJ = ${patsubst %.c, %.o, ${SRC_FILES}} 

LDSCRIPTS := -T  gcc.ld -lgcc

LIBFILES := -lm -I${INC} -L.\$(THUMB_PATH) -lc
LIBDIR   := -L.
LIBDIR   += -L $(GCC_ARM_PATH)\lib\gcc\arm-none-eabi\5.4.1\armv6-m
LIBDIR   += -L $(GCC_ARM_PATH)\arm-none-eabi\lib\armv6-m

#$(warning   OBJIEAT is $(OBJ)) 

all : output\a.out
	$(CROSS_COMPILE)objdump -S $^ > output\disasm
	$(CROSS_COMPILE)objcopy $^ -O ihex output\out.hex
	perl $(UTIL_PATH)\hex2rom.pl 0 10000 < output\out.hex > output\out.rom
#   copy output\out.rom ..\rom\output\out.rom
	copy $(PATCH_PATH)\ramcode.rom .\output\ramcode.rom
	copy $(PATCH_PATH)\program.lis .\output\program.lis
	copy $(PATCH_PATH)\patch.dat + $(PATCH_PATH)\user.dat + $(PATCH_PATH)\rf.dat .\output\sched.rom
#   copy .\patch\sched.rom .\output\sched.rom
	copy $(PATCH_PATH)\memmap.format .\output\memmap.format
OBJS = startup_m0.o
OBJO3 = fir_8to48.o
OBJO4 = drv_media.o

output\a.out : $(OBJ) $(OBJS) $(OBJO3) $(OBJO4)
#	$(CROSS_COMPILE)ld $^ -o $@ -Ttext 0 -Tdata 0x10011000 -Map= $(SDK_ROOT)/output\memmap -lm -I${INC} -L.\$(THUMB_PATH)
	$(CROSS_COMPILE)ld $^ -o $@ $(LIBDIR) $(LIBFILES) $(LDSCRIPTS) -Map=.\output\memmap

$(OBJS) :  startup_m0.s
	$(CROSS_COMPILE)as $^ -o $@ -mthumb -mcpu=cortex-m0 

$(OBJ) : %.o : %.c
	$(CROSS_COMPILE)gcc $^ -c $(INCLUDES) -g -O0 -o $@ -mthumb -mcpu=cortex-m0 -msoft-float

$(OBJO3) : fir_8to48.c
	$(CROSS_COMPILE)gcc $^ -c $(INCLUDES) -g -O3 -o $@ -mthumb -mcpu=cortex-m0 -msoft-float

$(OBJO4) : drv_media.c
	$(CROSS_COMPILE)gcc $^ -c $(INCLUDES) -g -O3 -o $@ -mthumb -mcpu=cortex-m0 -msoft-float
	
clean :
	del /Q output\*.*
	del /Q /S .\*.o
	del /Q /S ..\..\*.o