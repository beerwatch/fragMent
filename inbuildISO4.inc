#!/bin/bash
OUT_NAME="${PKG_NAM}.iso"
PACKOUT="$PACKAGE_CACHE"/"${OUT_NAME}"

DISKOUT="$WORK_TEMP"/"${OUT_NAME}"
PACKSET="installer"
INITSET="initrd"
BOOTLABEL="rockin-mendel"
BOOTLOADER="grubiso"

# Create work directories
WORKROOT="$WORK_TEMP"
MOUNTDIR="$WORKROOT/d/"
mkdir -pv "$MOUNTDIR"

. "$INC"/inc/install/copypkglist.inc "/factory/dev/repos/packlists/defpack-"${PKG_ID#ISO-}".lst" "/factory/dev/repos/packlists/defpack-"${PACKSET}".lst"

. "$INC"/inc/install/mkinitrd.inc "/factory/dev/repos/packlists/defpack-"${INITSET}".lst"

. "$INC"/inc/install/instboot-"${BOOTLOADER}"-bios.inc

. "$INC"/inc/install/instboot-"${BOOTLOADER}"-efi.inc

. "$INC"/inc/install/mkisoimage.inc

note="
Done. ${PACKOUT} is ready to be booted by ${BOOTLOADER^^} via BIOS (and maybe UEFI).
Now you may use QEMU to begin testing
BIOS:
    qemu-system-x86_64 -curses -machine q35 -m 1G \\
	-netdev bridge,id=hnet0,br=br9 \\
	-device e1000,netdev=hnet0,id=net0 \\
	-cdrom ${PACKOUT}
    ...or
	-usbdevice disk:format=raw:${PACKOUT}
EFI:
    qemu-system-x86_64 -vnc :2 -machine q35 -m 1G \\
	-netdev bridge,id=hnet0,br=br9 \\
	-device e1000,netdev=hnet0,id=net0 \\
	-drive file=/usr/share/OVMF/OVMF_CODE.fd,if=pflash,format=raw,unit=0,readonly=on \\
	-drive file=ovmf_vars.fd,if=pflash,format=raw,unit=1 \\
	-cdrom ${PACKOUT}
"
