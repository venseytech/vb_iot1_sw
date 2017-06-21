Buildroot
=========
The VeNSeY Board software is based on buildroot environment [1]. We have customized the buildroot environment slightly in order to provide the developers with the chance of generating their own system. We are happy to announce that we could build up 7 / 24 running example applications with complex power management on VeNSeY Board. To this end, we have additionally made use of our Atmega326 framework software [2]. We charge nothing for the provided software, you can re-use, distribute and change the code accordingly. Long live open source community!

[1] https://buildroot.org 
[2] //TODO: provide link to the atmega software as well 

Dependencies
------------

Our host machine is 64bit Ubuntu 14.04.5 LTS. It has not been tested yet, but we believe that the software can be built with the newer versions of Ubuntu. The following packages are to be installed to the system before starting the build process: 

	sudo apt-get update 
	sudo apt-get upgrade
	sudo apt-get install libblkid-dev git qt4-dev-tools pkg-config make build-essential gcc doxygen gcc-multilib g++-multilib flex texinfo bison ncurses-dev gawk libtool openconnect openvpn vpnc lzop libxml-parser-perl xorriso unzip build-essential git subversion cvs unzip whois ncurses-dev bc mercurial f2fs-tools build-essential git subversion cvs unzip whois ncurses-dev bc mercurial libssl-dev device-tree-compiler lzma gettext libcurl4-gnutls-dev:i386 libexpat1-dev:i386

In addition to these packages, due to certain VeNSeY applications, one shall install the opendk8-jre. For the 14.04.5 - LTS, this action requires the following activities: 
	
	sudo add-apt-repository ppa:openjdk-r/ppa
	sudo apt-get update
	sudo apt-get install openjdk-8-jdk

Building
--------

Download the repository with: 
	
	//TODO: change the repository accordingly, this is the wrong repo... 
	git clone http://YOUR_USERNAME@.../scm/~YOUR_USERNAME/vensey_board_sw.git

Get into the reporitory 

	cd vensey_board_sw

Make the default configuration 

	./host_tools/vbuild.sh vensey_defconfig 

Update the system with new packages if necessary 

	./host_tools/vbuild.sh xconfig

Start the build process with:

	./host_tools/vbuild.sh 

Generating Bootable Image for Deployment on a VeNSeY Board 
----------------------------------------------------------

It should be highlighted that the system images shall be ready for flashing once the building operation is successfully performed. Two final bootable flash images are to be generated, namely: 
	
	imx_mbr.img 
	imx_bcb.img 

Note that the "mbr" image can be directly flashed to an sd card having a capacity greater than 200Mb. We kept this as an example for fast flashing on disk. 200Mb was sufficient for running the test software. The "bcb" image, however, is copied over an SD card with a capacity of 128Mb. You shall find an SD card with exactly 128Mb in order to directly flash this image. 

You can change the size of th mbr image by means of the "count" parameter in the "vensey/images/generatembr.sh" file. Change the following line accordingly. 
	
	dd if=/dev/zero of=$TARGET_DIR/../../output/images/imx_mbr.img iflag=fullblock bs=1M count=200 && sync

We additionally provide a flashing script below the host tools. Just use this tool to flash your SD card directly. The mbr image will have the capacity of previously described count parameter. However, the bcb image will have the size of the SD card. An example usage of this script is: 
	
	./host_tools/vflash.sh /dev/sdc mbr // Flash the mbr image to the SD card mounted by /dev/sdc
	./host_tools/vflash.sh /dev/sdc bcb // Flash the SD card with a bcb image and use the size of SD card itself. 
 
Please note that flashing the SD card can take some time depending on your system capacity. One can send a SIGINFO for checking the progress with "CTRL+T". 


Login
-----

You can login to the system using *uart* debugger. The default user name and password are **root**. It is configurable with xconfig.

