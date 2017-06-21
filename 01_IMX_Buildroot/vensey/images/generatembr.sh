#!/bin/sh
pwd
export TARGET_DIR=$(pwd)/../output/target     
echo $TARGET_DIR
rm $TARGET_DIR/../../output/images/imx_mbr.img
dd if=/dev/zero of=$TARGET_DIR/../../output/images/imx_mbr.img iflag=fullblock bs=1M count=200 && sync
echo "Making the partition table for imx_mbr.img" 
sudo parted $TARGET_DIR/../../output/images/imx_mbr.img  mklabel msdos 
sed 's/#.*//' << EOF | tr -d " \t"  | sudo fdisk $TARGET_DIR/../../output/images/imx_mbr.img
	n       # new partition
	p       # primary partition
	1       # number 1
		# default start
	+16M    # 16MB
	t       # New Type field
	53      # OnTrack DM6 Aux3
	n       # new partition
	p       # primary partition
	2       # number 2
		# default start
		# default size
	w       # write changes
EOF
sudo fdisk -l $TARGET_DIR/../../output/images/imx_mbr.img 
sudo losetup -d /dev/loop1
sudo losetup -d /dev/loop2
echo "Binding loop device to the imx_mbr.img" 
sudo losetup -o 1048576 /dev/loop1 $TARGET_DIR/../../output/images/imx_mbr.img
sudo losetup -o 17825792 /dev/loop2 $TARGET_DIR/../../output/images/imx_mbr.img
sudo losetup -a  
sudo dd if=$TARGET_DIR/../../output/images/u-boot.sd of=/dev/loop1 
sudo dd if=$TARGET_DIR/../../output/images/rootfs.ext2 of=/dev/loop2
sudo sync
sudo losetup -d /dev/loop1
sudo losetup -d /dev/loop2

