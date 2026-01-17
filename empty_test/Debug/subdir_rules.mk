################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ruanjian/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -mthumb -Oz -I"C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test" -I"C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test/Debug" -I"C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source" -I"C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/kernel/nortos" -I"C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/kernel/nortos/posix" -gdwarf-3 -march=armv7e-m -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1011525899: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ruanjian/ti/ccs2040/sysconfig_1.21.1/sysconfig_cli.bat" -s "C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/.metadata/product.json" --script "C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test/empty.syscfg" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-1011525899 ../empty.syscfg
syscfg/ti_drivers_config.c: build-1011525899
syscfg/ti_drivers_config.h: build-1011525899
syscfg/ti_utils_build_linker.cmd.genlibs: build-1011525899
syscfg/ti_utils_build_linker.cmd.genmap: build-1011525899
syscfg/ti_utils_build_compiler.opt: build-1011525899
syscfg/syscfg_c.rov.xs: build-1011525899
syscfg: build-1011525899

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ruanjian/ti/ccs2040/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mlittle-endian -mthumb -Oz -I"C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test" -I"C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test/Debug" -I"C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/source" -I"C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/kernel/nortos" -I"C:/ruanjian/ti/ccs2040/simplelink_cc13xx_cc26xx_sdk_8_32_00_07/kernel/nortos/posix" -gdwarf-3 -march=armv7e-m -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/ruanjian/ti/ccs2040/ccs_setup_20.4.0.00013.lnk/empty_test/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


