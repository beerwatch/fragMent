#!/bin/bash
# populate initrd and create initrd image for boot
verb="${MOUNTDIR:?Not intended to be run as independent utility!}"

# initrd is prepared in the target hierarchy
mkdir -vp "${MOUNTDIR}/"{initrd/mnt/src,boot,lib/modules}

# packages to install are targetted at initrd
mdd="${MOUNTDIR}"
MOUNTDIR="${mdd}/initrd"

. "${INC}/"inc/install/mkhier.inc

. "${INC}/"inc/install/instpkglist.inc "$@"

. "${INC}/"inc/install/initconf.inc

MOUNTDIR="${mdd}"
unset mdd

# move kernel image to /boot
mv -v "${MOUNTDIR}"/initrd/boot/* "${MOUNTDIR}/boot/"

# ammend the right configuration
mv -v "${MOUNTDIR}"/initrd/{linuxrc,init}
sed -i 's/${BOOTLABEL}/'"${BOOTLABEL}/" "${MOUNTDIR}/initrd/etc/fstab"

# pack it together
(cd "${MOUNTDIR}/initrd"; find . | cpio -o -H newc --quiet | gzip -9 | pv -b -N "Packing initrd" ) > ${MOUNTDIR}/boot/initrd

# clean up
rm -rf "${MOUNTDIR}/initrd" "${MOUNTDIR}/lib"
