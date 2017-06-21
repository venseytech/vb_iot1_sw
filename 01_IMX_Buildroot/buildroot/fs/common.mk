#
# Macro that builds the needed Makefile target to create a root
# filesystem image.
#
# The following variable must be defined before calling this macro
#
#  ROOTFS_$(FSTYPE)_CMD, the command that generates the root
#  filesystem image. A single command is allowed. The filename of the
#  filesystem image that it must generate is $$@.
#
# The following variables can optionaly be defined
#
#  ROOTFS_$(FSTYPE)_DEPENDENCIES, the list of dependencies needed to
#  build the root filesystem (usually host tools)
#
#  ROOTFS_$(FSTYPE)_PRE_GEN_HOOKS, a list of hooks to call before
#  generating the filesystem image
#
#  ROOTFS_$(FSTYPE)_POST_GEN_HOOKS, a list of hooks to call after
#  generating the filesystem image
#
#  ROOTFS_$(FSTYPE)_POST_TARGETS, the list of targets that should be
#  run after running the main filesystem target. This is useful for
#  initramfs, to rebuild the kernel once the initramfs is generated.
#
# In terms of configuration option, this macro assumes that the
# BR2_TARGET_ROOTFS_$(FSTYPE) config option allows to enable/disable
# the generation of a filesystem image of a particular type. If
# the configuration options BR2_TARGET_ROOTFS_$(FSTYPE)_GZIP,
# BR2_TARGET_ROOTFS_$(FSTYPE)_BZIP2 or
# BR2_TARGET_ROOTFS_$(FSTYPE)_LZMA exist and are enabled, then the
# macro will automatically generate a compressed filesystem image.

FAKEROOT_SCRIPT = $(BUILD_DIR)/_fakeroot.fs
FULL_DEVICE_TABLE = $(BUILD_DIR)/_device_table.txt
ROOTFS_DEVICE_TABLES = $(call qstrip,$(BR2_ROOTFS_DEVICE_TABLE) \
	$(BR2_ROOTFS_STATIC_DEVICE_TABLE))
USERS_TABLE = $(BUILD_DIR)/_users_table.txt
ROOTFS_USERS_TABLES = $(call qstrip,$(BR2_ROOTFS_USERS_TABLES))

# Since this function will be called from within an $(eval ...)
# all variable references except the arguments must be $$-quoted.
define ROOTFS_TARGET_INTERNAL

# extra deps
ROOTFS_$(2)_DEPENDENCIES += host-fakeroot host-makedevs \
	$$(if $$(PACKAGES_USERS)$$(ROOTFS_USERS_TABLES),host-mkpasswd)

ifeq ($$(BR2_TARGET_ROOTFS_$(2)_GZIP),y)
ROOTFS_$(2)_COMPRESS_EXT = .gz
ROOTFS_$(2)_COMPRESS_CMD = gzip -9 -c
endif
ifeq ($$(BR2_TARGET_ROOTFS_$(2)_BZIP2),y)
ROOTFS_$(2)_COMPRESS_EXT = .bz2
ROOTFS_$(2)_COMPRESS_CMD = bzip2 -9 -c
endif
ifeq ($$(BR2_TARGET_ROOTFS_$(2)_LZMA),y)
ROOTFS_$(2)_DEPENDENCIES += host-lzma
ROOTFS_$(2)_COMPRESS_EXT = .lzma
ROOTFS_$(2)_COMPRESS_CMD = $$(LZMA) -9 -c
endif
ifeq ($$(BR2_TARGET_ROOTFS_$(2)_LZO),y)
ROOTFS_$(2)_DEPENDENCIES += host-lzop
ROOTFS_$(2)_COMPRESS_EXT = .lzo
ROOTFS_$(2)_COMPRESS_CMD = $$(LZOP) -9 -c
endif
ifeq ($$(BR2_TARGET_ROOTFS_$(2)_XZ),y)
ROOTFS_$(2)_DEPENDENCIES += host-xz
ROOTFS_$(2)_COMPRESS_EXT = .xz
ROOTFS_$(2)_COMPRESS_CMD = xz -9 -C crc32 -c
endif

$$(BINARIES_DIR)/rootfs.$(1): target-finalize $$(ROOTFS_$(2)_DEPENDENCIES)
	@$$(call MESSAGE,"Generating root filesystem image rootfs.$(1)")
	$$(foreach hook,$$(ROOTFS_$(2)_PRE_GEN_HOOKS),$$(call $$(hook))$$(sep))
	rm -f $$(FAKEROOT_SCRIPT)
	rm -f $$(TARGET_DIR_WARNING_FILE)
	rm -f $$(USERS_TABLE)
	echo '#!/bin/sh' > $$(FAKEROOT_SCRIPT)
	echo "set -e" >> $$(FAKEROOT_SCRIPT)
	echo "chown -h -R 0:0 $$(TARGET_DIR)" >> $$(FAKEROOT_SCRIPT)
ifneq ($$(ROOTFS_USERS_TABLES),)
	cat $$(ROOTFS_USERS_TABLES) >> $$(USERS_TABLE)
endif
	$$(call PRINTF,$$(PACKAGES_USERS)) >> $$(USERS_TABLE)
	PATH=$$(BR_PATH) $$(TOPDIR)/support/scripts/mkusers $$(USERS_TABLE) $$(TARGET_DIR) >> $$(FAKEROOT_SCRIPT)
ifneq ($$(ROOTFS_DEVICE_TABLES),)
	cat $$(ROOTFS_DEVICE_TABLES) > $$(FULL_DEVICE_TABLE)
ifeq ($$(BR2_ROOTFS_DEVICE_CREATION_STATIC),y)
	$$(call PRINTF,$$(PACKAGES_DEVICES_TABLE)) >> $$(FULL_DEVICE_TABLE)
endif
	$$(call PRINTF,$$(PACKAGES_PERMISSIONS_TABLE)) >> $$(FULL_DEVICE_TABLE)
	echo "$$(HOST_DIR)/usr/bin/makedevs -d $$(FULL_DEVICE_TABLE) $$(TARGET_DIR)" >> $$(FAKEROOT_SCRIPT)
endif
	$$(foreach s,$$(call qstrip,$$(BR2_ROOTFS_POST_FAKEROOT_SCRIPT)),\
		echo "echo '$$(TERM_BOLD)>>>   Executing fakeroot script $$(s)$$(TERM_RESET)'" >> $$(FAKEROOT_SCRIPT); \
		echo $$(s) $$(TARGET_DIR) $$(BR2_ROOTFS_POST_SCRIPT_ARGS) >> $$(FAKEROOT_SCRIPT)$$(sep))
	$$(call PRINTF,$$(ROOTFS_$(2)_CMD)) >> $$(FAKEROOT_SCRIPT)
	chmod a+x $$(FAKEROOT_SCRIPT)
	PATH=$$(BR_PATH) $$(HOST_DIR)/usr/bin/fakeroot -- $$(FAKEROOT_SCRIPT)
	$$(INSTALL) -m 0644 support/misc/target-dir-warning.txt $$(TARGET_DIR_WARNING_FILE)
	-@rm -f $$(FAKEROOT_SCRIPT) $$(FULL_DEVICE_TABLE)
ifneq ($$(ROOTFS_$(2)_COMPRESS_CMD),)
	PATH=$$(BR_PATH) $$(ROOTFS_$(2)_COMPRESS_CMD) $$@ > $$@$$(ROOTFS_$(2)_COMPRESS_EXT)
endif
	$$(foreach hook,$$(ROOTFS_$(2)_POST_GEN_HOOKS),$$(call $$(hook))$$(sep))
	echo "Finished all operations $(2) and $(1) " 
ifeq ($(1), ext2)
	@echo "ext2 file system is generated" 
	@echo "*****************************************************************************" 
	@echo "*****************************************************************************" 
	@echo "*** TARGET FOLDER: $(BOARD_DIR) / Generating VeNSeY Board Image  ************************" 
	@echo "*****************************************************************************"
	@echo "*****************************************************************************"  
	cp $(TARGET_DIR)/../../vensey/images/imx_bcb.img $(TARGET_DIR)/../../output/images/
	cp $(TARGET_DIR)/../../vensey/images/bcb_512.cfg $(TARGET_DIR)/../../output/images/
	@echo "Flashing u-boot.sb file "
	dd if=$(TARGET_DIR)/../../output/images/u-boot.sb seek=2048 of=$(TARGET_DIR)/../../output/images/imx_bcb.img
	@echo "Flashing rootfs.ext2 file "
	dd if=$(TARGET_DIR)/../../output/images/rootfs.ext2 seek=16384 of=$(TARGET_DIR)/../../output/images/imx_bcb.img
	@echo "Flashing bcb config file "
	dd if=$(TARGET_DIR)/../../output/images/bcb_512.cfg bs=512 count=1 seek=248319 of=$(TARGET_DIR)/../../output/images/imx_bcb.img  
	@echo "Check for flashing u-boot.sb at imx_bcb"
	dd if=$(TARGET_DIR)/../../output/images/imx_bcb.img  bs=512 count=1 skip=2048 2> /dev/null | hexdump -C
	@echo "Check for flashing u-boot.sb file "
	dd if=$(TARGET_DIR)/../../output/images/u-boot.sb bs=512 count=1 2>/dev/null | hexdump -C
	@echo "Check for flashing rootfs.ext2 file"
	dd if=$(TARGET_DIR)/../../output/images/rootfs.ext2 bs=512 count=5 2>/dev/null | hexdump -C 
	@echo "Check for flashing rootfs.ext2 file at imx_bcb.img file"
	dd if=$(TARGET_DIR)/../../output/images/imx_bcb.img  bs=512 count=5 skip=16384 2> /dev/null | hexdump -C 
	@echo "Check for flashing bcb_512.cfg file"
	dd if=$(TARGET_DIR)/../../output/images/bcb_512.cfg  bs=512 count=1 2> /dev/null | hexdump -C
	@echo "Check for flashing bcb_512.cfg file at imx_bcb.img file"
	dd if=$(TARGET_DIR)/../../output/images/imx_bcb.img  bs=512 count=1 skip=248319 2> /dev/null | hexdump -C

	@echo "*****************************************************************************" 
	@echo "*****************************************************************************" 
	@echo "*** TARGET FOLDER: $(BOARD_DIR) / Generating VeNSeY MBR Image  ************************" 
	@echo "*****************************************************************************"
	@echo "*****************************************************************************"  
	exec $(TARGET_DIR)/../../vensey/images/generatembr.sh 
endif 

rootfs-$(1)-show-depends:
	@echo $$(ROOTFS_$(2)_DEPENDENCIES)

rootfs-$(1): $$(BINARIES_DIR)/rootfs.$(1) $$(ROOTFS_$(2)_POST_TARGETS)

.PHONY: rootfs-$(1) rootfs-$(1)-show-depends

ifeq ($$(BR2_TARGET_ROOTFS_$(2)),y)
TARGETS_ROOTFS += rootfs-$(1)
PACKAGES += $$(filter-out rootfs-%,$$(ROOTFS_$(2)_DEPENDENCIES))
endif
endef

define ROOTFS_TARGET
	$(call ROOTFS_TARGET_INTERNAL,$(1),$(call UPPERCASE,$(1)))
endef

include $(sort $(wildcard fs/*/*.mk))


