################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
CC1310_LAUNCHXL.obj: ../CC1310_LAUNCHXL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI" --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI" --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI/smartrf_settings" --include_path="/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="CC1310_LAUNCHXL.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ccfg.obj: ../ccfg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI" --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI" --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI/smartrf_settings" --include_path="/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="ccfg.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-768789599:
	@$(MAKE) -Onone -f subdir_rules.mk build-768789599-inproc

build-768789599-inproc: ../rfExamples.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"/Applications/ti/xdctools_3_32_00_06_core/xs" --xdcpath="/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/packages;/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages;/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages;/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/uia_2_01_00_01/packages;/Applications/ti/ccsv7/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC1310F128 -r release -c "/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS" --compileOptions "-mv7M3 --code_state=16 --float_support=vfplib -me --include_path=\"/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI\" --include_path=\"/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI\" --include_path=\"/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI/smartrf_settings\" --include_path=\"/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272\" --include_path=\"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include\" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-768789599 ../rfExamples.cfg
configPkg/compiler.opt: build-768789599
configPkg/: build-768789599

rfWakeOnRadioTx.obj: ../rfWakeOnRadioTx.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI" --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI" --include_path="/Users/nathanshen95/Documents/workspace/rfWakeOnRadioTx_CC1310_LAUNCHXL_TI/smartrf_settings" --include_path="/Applications/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/cc13xxware_2_04_03_17272" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="rfWakeOnRadioTx.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


