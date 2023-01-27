#!/bin/bash
# installs GRUB2 bootloader
verb="${MOUNTDIR:?Not intended to be run as independent utility!}"

mkdir -pv "${MOUNTDIR}"/boot/grub/fonts

cp -r /usr/lib/grub/i386-pc "${MOUNTDIR}"/boot/grub/

cp /boot/grub/fonts/unicode.pf2 ${MOUNTDIR}/boot/grub/fonts/
cp ${BINPAKSDIR}/../artifacts/logos/* ${MOUNTDIR}/boot/grub/

krnl="$(find ${MOUNTDIR}/boot/ -name 'vmlinuz*' -not -name 'vmlinuz' -printf '%f')"
kver="${krnl:8}"

# Create BIOS grub config
cat <<GRUBCFG > ${MOUNTDIR}/boot/grub/grub.cfg
set default=0
set timeout=5

insmod iso9660
insmod ext2
insmod part_gpt
insmod biosdisk
insmod search_fs_uuid

search --label "${BOOTLABEL}" --set root

if loadfont /boot/grub/fonts/unicode.pf2 ; then
  set gfxmode=auto
  if [ x$feature_all_video_module = xy ]; then
    insmod all_video
  else
    insmod efi_gop
    insmod efi_uga
    insmod vbe
    insmod vga
    insmod video_bochs
    insmod video_cirrus
  fi
  insmod gfxterm
  insmod gettext
fi

menuentry "Rockin' Mendel, Linux ${kver}" {
    linux   /boot/vmlinuz console=ttyS0,115200 console=tty1
    initrd  /boot/initrd
}
GRUBCFG

grub-mkimage -O i386-pc -o "$WORKROOT"/core.img --prefix=/boot/grub iso9660 biosdisk

cat /usr/lib/grub/i386-pc/cdboot.img "$WORKROOT"/core.img > ${MOUNTDIR}/boot/eltorito.img
