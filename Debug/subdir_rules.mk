################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"E:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="E:/ti/ccs1000/ccs/ccs_base/arm/include" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/GUI/inc" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/cc3100_usage" --include_path="D:/ti/xdctools_3_55_02_22_core_win32/xdctools_3_55_02_22_core/packages/" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/BoardSupportPackage/DriverLib" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/BoardSupportPackage/inc" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/board" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/http_client" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/simplelink" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/G8RTOS" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/simplelink/include" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/json" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/SL_Common" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller/CC3100SupportPackage/spi_cc3100" --include_path="E:/ti/ccs1000/ccs/ccs_base/arm/include/CMSIS" --include_path="E:/CCS_Workspace/Smart_Irrigation_Controller" --include_path="E:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --advice:power="all" --define=__MSP432P401R__ --define=__TI_TIME_USES_64 --define=HTTPCli_LIBTYPE_MIN --define=__SL__ --define=ccs -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


