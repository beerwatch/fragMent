#!/bin/bash
verb="${MOUNTDIR:?Not intended to be run as independent utility!}"

#make ISO
MKISO="xorriso -as mkisofs"
$MKISO -R -V "${BOOTLABEL}" \
    -b boot/eltorito.img \
    -boot-load-size 4 -boot-info-table -no-emul-boot \
    --grub2-boot-info --grub2-mbr /usr/lib/grub/i386-pc/boot_hybrid.img \
    --efi-boot boot/efi.img -efi-boot-part --efi-boot-image \
    --protective-msdos-label \
    -o "$DISKOUT" \
    "$MOUNTDIR"/

unset verb
