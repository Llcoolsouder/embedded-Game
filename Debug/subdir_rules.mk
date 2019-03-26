################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"E:/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="E:/ti/ccs900/ccs/ccs_base/msp430/include" --include_path="E:/Users/lonni/workspace_v9/msp430fr5994-game" --include_path="E:/ti/ccs900/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/include" --include_path="E:/Users/Lonnie II/Desktop/School/EmbeddedSoftware/embedded-software/include" --include_path="E:/Users/Lonnie II/Desktop/School/EmbeddedSoftware/embedded-software/hal/MSP430/MSP430FR5994" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ --define=_MPU_ENABLE -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


