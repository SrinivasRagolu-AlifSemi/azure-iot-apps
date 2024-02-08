arm-none-eabi-gcc -c -g -O0 -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -marm -mthumb-interwork -DTX_ENABLE_VFP_SUPPORT -fpic -fno-plt -mno-pic-data-is-text-relative -msingle-pic-base txm_module_preamble.S
arm-none-eabi-gcc -c -g -O0 -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -marm -mthumb-interwork -DTX_ENABLE_VFP_SUPPORT -fpic -fno-plt -mno-pic-data-is-text-relative -msingle-pic-base gcc_setup.S
arm-none-eabi-gcc -c -g -O0 -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -marm -mthumb-interwork -DTX_ENABLE_VFP_SUPPORT -fpic -fno-plt -mno-pic-data-is-text-relative -msingle-pic-base -I../inc -I../../../../common/inc -I../../../../common_modules/inc -I../../../../common_modules/module_manager/inc sample_threadx_module.c
rem arm-none-eabi-gcc -g  --elf --ro 0 --first txm_module_preamble.o(Init) --entry=_txm_module_thread_shell_entry --ropi --rwpi --remove --map --symbols --list sample_threadx_module.map txm_module_preamble.o sample_threadx_module.o txm.a
rem arm-none-eabi-gcc -g -mcpu=cortex-a7 -T sample_threadx_module.ld -mfloat-abi=hard -mfpu=neon-vfpv4 -marm -mthumb-interwork --specs=nosys.specs -e _txm_module_thread_shell_entry -o sample_threadx_module.out -Wl,-Map=sample_threadx_module.map gcc_setup.o txm_module_preamble.o sample_threadx_module.o txm.a
arm-none-eabi-ld -A cortex-a7 -T sample_threadx_module.ld txm_module_preamble.o gcc_setup.o sample_threadx_module.o -e _txm_module_thread_shell_entry txm.a -o sample_threadx_module.axf -M > sample_threadx_module.map 