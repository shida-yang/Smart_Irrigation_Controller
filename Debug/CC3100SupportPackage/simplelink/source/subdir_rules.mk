################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
CC3100SupportPackage/simplelink/source/%.obj: ../CC3100SupportPackage/simplelink/source/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/CCS/ccs/ccs_base/arm/include" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/cc3100_usage" --include_path="D:/ti/xdctools_3_55_02_22_core_win32/xdctools_3_55_02_22_core/packages/" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/BoardSupportPackage/DriverLib" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/BoardSupportPackage/inc" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/board" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/http_client" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/simplelink" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/G8RTOS" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/simplelink/include" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/json" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/SL_Common" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller/CC3100SupportPackage/spi_cc3100" --include_path="D:/CCS/ccs/ccs_base/arm/include/CMSIS" --include_path="D:/UF_HW/2020_spring_hw/EEL4930/Lab/Code/Smart_Irrigation_Controller" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=__TI_TIME_USES_64 --define=HTTPCli_LIBTYPE_MIN --define=__SL__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CC3100SupportPackage/simplelink/source/$(basename $(<F)).d_raw" --obj_directory="CC3100SupportPackage/simplelink/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


