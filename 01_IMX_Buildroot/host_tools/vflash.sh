#!/bin/sh

########################################################################################################
########################################################################################################
########################## Test Script for Flashing the SD Card ########################################
########################################################################################################
########################################################################################################

# Print out the usage if the path to disk is not provided 
if [ "$#" -le 1 ] ; then
 	echo "Usage: $0 [dev_path] [image_type] -d" >&2
 	echo " - [dev_path]: Path to the device path for the sd card, e.g. /dev/sdbX" >&2
 	echo " - [image_type]: mbr, bcb: careful about the image type as the imx233 boots with bcb unless fused for mbr" >&2
 	echo " - [-d] option for debug" >&2
	exit 1
fi

# Unmount mounted partitions
for fs in `grep $1 /proc/mounts|cut -d ' ' -f 1` ; do
	echo Unmounting $fs
	umount $fs
done

# Check whether the preffered image type is mbr or bcb
if [ "$2" = "bcb" ]; then
	echo Erasing the existing partition table on the medium for BCB image 
	dd if=/dev/zero of=$1 bs=1024 count=3
	sync 

	# Get the size of the medium in blocks 
        blockSize=$(blockdev --getsize64 $1)
        echo Block size is ${blockSize}         

        # Calculate the position for the BCB configutaion 
        bcbConfigPosition=$(expr ${blockSize} / 512 - 1)
        echo BCB Configuration file position is ${bcbConfigPosition} 

	# Calculate the maximum size for the rootfs image 
	rootfsMaxSize=$(expr ${blockSize} / 1024 / 1024 - 9)
	echo Maximum rootfs size is ${rootfsMaxSize}
	
	# Generate a partition table 
	echo Generating a partition table for the disk 
	sed 's/#.*//' << EOF | tr -d " \t"  | sudo fdisk $1
                n       # new partition
                p       # primary partition
                1       # number 1
                        # default start
                +7M     # 7MB
                t       # New Type field
                53      # OnTrack DM6 Aux3
                n       # new partition
                p       # primary partition
                2       # number 2
                        # default start
                +${rootfsMaxSize}M       
                w       # write changes
EOF
	sync 
	
	# Flashing the Uboot loader to the BCB image 
	echo Flashing the bootloader u-boot
	dd if=output/images/u-boot.sb seek=2048 of=$1
	sync

	# Flashing the root file system to the BCB image
	echo Flashing the root file system  
	dd if=output/images/rootfs.ext2 seek=16384 of=$1
	sync

	# Finally flashing the bcb configuration file to the final block of the device 
	echo Flashing the bcb configuration image 
	dd if=vensey/images/bcb_512.cfg bs=512 count=1 seek=${bcbConfigPosition} of=$1 
	sync 

	# Check whether the debug option is provided by the user 
	if [ "$3" = "-d" ]; then 
		# Just for the debugging purposes, printing out the flashed portions for a comparison 
		echo Printing out the flashed u-boot image \(first block only\) 
		dd if=$1 bs=512 count=1 skip=2048 2> /dev/null | hexdump -C 
		dd if=output/images/u-boot.sb bs=512 count=1 2> /dev/null | hexdump -C 
	
		echo Printing out the flashed root fs image \(first 5 blocks only\) 
		dd if=$1 bs=512 count=5 skip=16384 2> /dev/null | hexdump -C
		dd if=output/images/rootfs.ext2 bs=512 count=5 2> /dev/null | hexdump -C
	
		echo Printing out the flashed bcb configuration image \(first block only\) 
		dd if=$1 bs=512 count=1 skip=${bcbConfigPosition} 2> /dev/null | hexdump -C
		dd if=vensey/images/bcb_512.cfg bs=512 count=1 2> /dev/null | hexdump -C
	fi 

elif [ "$2" = "mbr" ] ; then 
        echo Erasing the existing partition table on the medium for MBR image
        dd if=/dev/zero of=$1 bs=512 count=10
	sync 
	
	 # Get the size of the medium in blocks 
        blockSize=$(blockdev --getsize64 $1)
        echo Block size is ${blockSize}         

        # Calculate the maximum size for the rootfs image 
        rootfsMaxSize=$(expr ${blockSize} / 1024 / 1024 - 9)
        echo Maximum rootfs size is ${rootfsMaxSize}

	# Generate a partition table 
        echo Generating a partition table for the disk 
        sed 's/#.*//' << EOF | tr -d " \t"  | sudo fdisk $1
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
                	# default stop      
                w       # write changes
EOF
        sync
	
	# Get the device name
	deviceName=$1
	echo Device at ${deviceName} 
	
	# Flash the u-boot 
	echo Flashing u-boot to ${deviceName}1
	dd if=output/images/u-boot.sd of="$1"1
	#dd if=output/images/u-boot.sd of=${deviceName}1
	sync 

	# Flash the rootfs 
	echo Flashing the rootfs ${deviceName}2
	dd if=output/images/rootfs.ext2 of="$1"2 bs=512
	sync 

	# Note that the mbr image is generated by generatembr.sh file for easier transfer options
	# Thus the generated mbr image is directly flashed to the provided disk drive
	#dd if=output/images/imx_mbr.img of=$1 
	sync 
	
else
	echo "Usage: $0 [dev_path] [image_type]" >&2
        echo " - [dev_path]: Path to the device path for the sd card, e.g. /dev/sdbX" >&2
        echo " - [image_type]: mbr, bcb: careful about the image type as the imx233 boots with bcb unless fused for mbr" >&2
        exit 1
fi 

echo End of Flashing


