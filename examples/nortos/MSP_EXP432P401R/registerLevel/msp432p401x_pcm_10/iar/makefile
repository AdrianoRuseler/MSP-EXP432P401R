SIMPLELINK_MSP432_SDK_INSTALL_DIR ?= $(abspath ../../../../../..)

include $(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/imports.mak

ASM = "$(IAR_ARMCOMPILER)/bin/iasmarm"
CC = "$(IAR_ARMCOMPILER)/bin/iccarm"
LNK = "$(IAR_ARMCOMPILER)/bin/ilinkarm"

OBJECTS = system_msp432p401r.obj startup_msp432p401r_ewarm.obj msp432p401x_pcm_10.obj

NAME = msp432p401x_pcm_10

AFLAGS = -I.. "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" -D__MSP432P401R__ -DDeviceFamily_MSP432P401x -S --cpu Cortex-M4F --endian little --thumb "-I$(IAR_ARMCOMPILER)/inc/c"

CFLAGS = -I.. "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source" "-I$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/CMSIS/Include" -D__MSP432P401R__ -DDeviceFamily_MSP432P401x --cpu=Cortex-M4F --debug --silent -e --aeabi --thumb --diag_suppress=Pa050 "-I$(IAR_ARMCOMPILER)/inc/c"

LFLAGS = "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/display/lib/display.arm4f" "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/grlib/lib/iar/m4f/grlib.a" "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/spiffs/lib/iar/m4f/spiffs.a" "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/drivers/lib/drivers_msp432p401x.arm4f" "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/third_party/fatfs/lib/iar/m4f/fatfs.a" "$(SIMPLELINK_MSP432_SDK_INSTALL_DIR)/source/ti/devices/msp432p4xx/driverlib/iar/msp432p4xx_driverlib.a" --config ../iar/msp432p401r.icf --map "$(NAME).map" --silent --cpu=Cortex-M4F --redirect _Printf=_PrintfSmall --redirect _Scanf=_ScanfSmall --semihosting=iar_breakpoint

all: $(NAME).out

system_msp432p401r.obj: ../system_msp432p401r.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

startup_msp432p401r_ewarm.obj: ../iar/startup_msp432p401r_ewarm.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

msp432p401x_pcm_10.obj: ../msp432p401x_pcm_10.c
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< -o $@

$(NAME).out: $(OBJECTS)
	@ echo linking...
	@ $(LNK)  $(OBJECTS) $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
