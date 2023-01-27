#!/bin/bash
verb="${MOUNTDIR:?Not intended to be run as independent utility!}"

cp -r /usr/lib/grub/x86_64-efi "${MOUNTDIR}"/boot/grub/

mkdir -pv "$WORKROOT"/EFI/BOOT/

grub-mkimage \
    --directory '/usr/lib/grub/x86_64-efi/' \
    --prefix '()/boot/grub' \
    --format 'x86_64-efi' \
    --output "$WORKROOT"/EFI/BOOT/bootx64.efi \
    'part_msdos' 'fat' 'iso9660' 'ls' 'search' 'part_gpt' 'ext2'

cat <<STARTUPNSH >"${WORKROOT}/startup.nsh"
fs0:\\EFI\\BOOT\\bootx64.efi
STARTUPNSH

mformat -C -f 2880 -L 16 -i ${MOUNTDIR}/boot/efi.img ::.

mcopy -s -i ${MOUNTDIR}/boot/efi.img "$WORKROOT"/EFI/ "${WORKROOT}/startup.nsh" ::.

unset verb
