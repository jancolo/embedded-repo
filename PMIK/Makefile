####################################################	
#
# NAME: Makefile
#
####################################################	 

####################################################
####     Tools and Source Path Definition       ####
####################################################

#TOPDIR 		= /srv/Temp/workspace/newHello
VERSION 	= 4.4.1
TARGET		= arm-none-eabi
TOOLDIR 	= /home/janek/toolchains/arm-2009q3
LIBCDIR 	= $(TOOLDIR)/$(TARGET)/lib
LIBGCCDIR 	= $(TOOLDIR)/lib/gcc/$(TARGET)/$(VERSION)
LSCRIPT		= STM32F103-MD.ld

INC_LCD		= ./LCD
INC_TOUCH	= ./touch
INC_STMLIB	= ./STM32F10x_StdPeriph_Driver/inc
INC_CORE	= ./CMSIS/CM3/CoreSupport
INC_DEVICE	= ./CMSIS/CM3/DeviceSupport/ST/STM32F10x

AS		= $(TOOLDIR)/bin/$(TARGET)-as
#CC		= $(TOOLDIR)/bin/$(TARGET)-gcc
CC 		= arm-none-eabi-gcc
LD 		= arm-none-eabi-g++
#LD 		= $(TOOLDIR)/bin/$(TARGET)-g++
OBJCOPY	 	= $(TOOLDIR)/bin/$(TARGET)-objcopy
OBJDUMP 	= $(TOOLDIR)/bin/$(TARGET)-objdump	  

SRC 		= $(wildcard *.c)
ASRC 		= $(wildcard *.S)

C_SRCS	= \
./main.c \
./panel.c \
./stm32f10x_it.c

#OBJS    	= $(SRC:.c=.o) $(ASRC:.S=.o)

C_SRCS += \
./CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c \
./CMSIS/CM3/CoreSupport/core_cm3.c \
./STM32F10x_StdPeriph_Driver/src/misc.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c \
./src/touch/touch.c \
./src/LCD/ili932x.c \
./newlib_stubs.c

OBJS += \
./system_stm32f10x.o \
./core_cm3.o \
./misc.o \
./stm32f10x_adc.o \
./stm32f10x_bkp.o \
./stm32f10x_can.o \
./stm32f10x_cec.o \
./stm32f10x_crc.o \
./stm32f10x_dac.o \
./stm32f10x_dbgmcu.o \
./stm32f10x_dma.o \
./stm32f10x_exti.o \
./stm32f10x_flash.o \
./stm32f10x_fsmc.o \
./stm32f10x_gpio.o \
./stm32f10x_i2c.o \
./stm32f10x_iwdg.o \
./stm32f10x_pwr.o \
./stm32f10x_rcc.o \
./stm32f10x_rtc.o \
./stm32f10x_sdio.o \
./stm32f10x_spi.o \
./stm32f10x_tim.o \
./stm32f10x_usart.o \
./stm32f10x_wwdg.o \
./touch.o \
./ili932x.o \
./newlib_stubs.o \
./startup_stm32f10x_md.o \
./main.o \
./panel.o \
./stm32f10x_it.o

C_DEPS += \
./CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.d \
./CMSIS/CM3/CoreSupport/core_cm3.d \
./STM32F10x_StdPeriph_Driver/src/misc.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_can.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.d \
./STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.d \
./src/touch/touch.d \
./src/LCD/ili932x.d 


####################################################
####             Option Definition              ####
####################################################
INCLUDE	= -I"./include" \
	-I"$(INC_LCD)" -I"$(INC_TOUCH)" -I"$(INC_STMLIB)" -I"$(INC_CORE)" -I"$(INC_DEVICE)"

#CFLAGS	= 	$(INCLUDE) -g -W -Wall -Wstrict-prototypes -Wno-trigraphs -O0
#CFLAGS	+= 	-fno-strict-aliasing -fno-common -pipe
#CFLAGS	+= 	-mcpu=cortex-m3 -mthumb -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD
#CFLAGS	+=	-ffunction-sections -fdata-sections -c -MMD -MP -gdwarf-2

CFLAGS	= -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD $(INCLUDE) -O0 -ffunction-sections -fdata-sections -Wall \
	-mcpu=cortex-m3 -mthumb -g

#LDFLAGS	= 	--cref -Bstatic -nostdlib -nostartfiles -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -Xlinker -p -EL -T $(LSCRIPT) -Map \
#			$(OUTPUT).map 

LDFLAGS	= -T"$(LSCRIPT)" -Xlinker --gc-sections -Wl,-Map,$(OUTPUT).map -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2

#LDFLAGS		= -T $(LSCRIPT) -Xlinker --gc-sections -mthumb -mcpu=cortex-m3 -g3 -gdwarf-2	

OCFLAGS = -O binary

OUTPUT = program

all: $(OUTPUT).bin

$(OUTPUT).bin : $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTPUT).elf $(OBJS)
	#$(OBJCOPY) $(OCFLAGS) $(OUTPUT).elf $@

%.o : %.S
	$(CC) -x assembler-with-cpp -c $(CFLAGS) -o $@ $<
	
%.o : ./STM32F10x_StdPeriph_Driver/src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o : ./CMSIS/CM3/DeviceSupport/ST/STM32F10x/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
%.o : ./CMSIS/CM3/CoreSupport/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o : ./touch/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
%.o : ./LCD/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<
	
clean:
	rm -f $(OBJS)
	rm -f $(OUTPUT).elf
	rm -f *.dmp
	rm -f $(OUTPUT).map
	rm -f $(OUTPUT).bin
	rm -f *.d
	rm -f *.lst


