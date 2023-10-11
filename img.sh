#!/bin/bash

set -e

source build.sh

rm -rf image.hdd | true

dd if=/dev/zero bs=1M count=0 seek=64 of=image.hdd
sgdisk image.hdd -n 1:2048 -t 1:ef00
./limine/limine bios-install image.hdd
mformat -i image.hdd@@1M
mmd -i image.hdd@@1M ::/EFI ::/EFI/BOOT
mcopy -i image.hdd@@1M sysroot/kernel.elf limine.cfg limine/limine-bios.sys ::/
mcopy -i image.hdd@@1M limine/BOOTX64.EFI ::/EFI/BOOT
mcopy -i image.hdd@@1M limine/BOOTIA32.EFI ::/EFI/BOOT
