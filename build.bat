@echo off

set CCP=D:\Progs\gcc_riscv_v12\bin

"%CCP%\riscv-none-elf-gcc" -Os -g -std=c99 -march=rv32ec -mabi=ilp32e -msmall-data-limit=0 -msave-restore -nostartfiles -fmessage-length=0 -fmax-errors=10 -ffunction-sections -fdata-sections -fvisibility=hidden -fshort-enums -funsigned-char -ffunction-sections -fdata-sections -fno-ident -I. main.c startup_ch32v003.s -T ch32v003.ld -o firmware.elf

"%CCP%\riscv-none-elf-objcopy" -Obinary firmware.elf firmware.bin

rem set OPENOCD_PATH=C:\MounRiver\MounRiver_Studio\toolchain\OpenOCD\bin
rem "C:\MounRiver\MounRiver_Studio\toolchain\OpenOCD\bin\openocd.exe" -s "C:\MounRiver\MounRiver_Studio\toolchain\OpenOCD\bin" -f wch-riscv.cfg -c "program firmware.elf verify; reset; shutdown;"