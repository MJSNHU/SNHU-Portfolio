################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./7-1.o" \
"./syscfg/ti_drivers_config.o" \
"./main_nortos.o" \
"../cc32xxs_nortos.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-lti_utils_build_linker.cmd.genlibs \
-Wl,-l"ti/devices/cc32xx/driverlib/ticlang/Release/driverlib.a" \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
7-1.out 

EXE_OUTPUTS__QUOTED += \
"7-1.out" 

CUSTOM_TOOL_OUTPUTS_803256257 += \
7-1.bin 

CUSTOM_TOOL_OUTPUTS_803256257__QUOTED += \
"7-1.bin" 

CUSTOM_TOOL_OUTPUTS_1677627825 += \
syscfg/7-1.sli 

CUSTOM_TOOL_OUTPUTS_1677627825__QUOTED += \
"syscfg\7-1.sli" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "7-1.out" secondary-outputs

# Tool invocations
7-1.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -mcpu=cortex-m4 -mfloat-abi=soft -mfpu=none -mlittle-endian -mthumb -Oz -DDeviceFamily_CC3220 -DNORTOS_SUPPORT -gdwarf-3 -march=armv7e-m -Wl,-m"7-1.map" -Wl,-i"C:/ti/simplelink_cc32xx_sdk_7_10_00_13/source" -Wl,-i"C:/ti/simplelink_cc32xx_sdk_7_10_00_13/kernel/nortos" -Wl,-i"C:/Users/matth/Documents/SNHU/CS-350 Emerging Sys Arch and Tech/Module7/7-1/7-1/MCU+Image/syscfg" -Wl,-i"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="7-1_linkInfo.xml" -Wl,--rom_model -o "7-1.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

7-1.bin: $(EXE_OUTPUTS) 7-1.out
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: TI Arm ObjCopy'
	"C:/ti/ccs1280/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmobjcopy" -O binary --only-section .text --only-section .const --only-section .cinit --only-section .rodata --only-section .resetVecs "7-1.out" "7-1.bin"
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

syscfg/7-1.sli: $(EXE_OUTPUTS) 7-1.bin syscfg/ti_drivers_net_wifi_config.json
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Image Creator'
	"C:/ti/simplelink_cc32xx_sdk_7_10_00_13/source/ti/drivers/net/imagecreator/bin/SLImageCreator.exe" syscfg create_image --sdk_path "C:/ti/simplelink_cc32xx_sdk_7_10_00_13" --json "C:/Users/matth/Documents/SNHU/CS-350 Emerging Sys Arch and Tech/Module7/7-1/7-1/MCU+Image/syscfg/ti_drivers_net_wifi_config.json" --file "C:/Users/matth/Documents/SNHU/CS-350 Emerging Sys Arch and Tech/Module7/7-1/7-1/MCU+Image/syscfg/7-1.sli" --mcu "C:/Users/matth/Documents/SNHU/CS-350 Emerging Sys Arch and Tech/Module7/7-1/7-1/MCU+Image/7-1.bin"
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CUSTOM_TOOL_OUTPUTS_1677627825__QUOTED)$(CUSTOM_TOOL_OUTPUTS_803256257__QUOTED)$(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "7-1.o" "syscfg\ti_drivers_config.o" "main_nortos.o" 
	-$(RM) "7-1.d" "syscfg\ti_drivers_config.d" "main_nortos.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(CUSTOM_TOOL_OUTPUTS_803256257) $(CUSTOM_TOOL_OUTPUTS_1677627825)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

