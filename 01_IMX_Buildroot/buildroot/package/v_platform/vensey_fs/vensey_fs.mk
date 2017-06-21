#############################################################
#
# vensey_fs
#
#############################################################

VENSEY_FS_VERSION = 1.0.0
VENSEY_FS_SITE = $(TOPDIR)/../vensey/package/vensey_fs
VENSEY_FS_SITE_METHOD = local
VENSEY_FS_INSTALL_STAGING = YES
VENSEY_FS_LICENSE = PROPRIETARY

#VENSEY_FS_DEPENDENCIES = xapp_xinit omxplayer

define VENSEY_FS_INSTALL_STAGING_CMDS
	echo "staging cmds at platform files" 
endef

define VENSEY_FS_INSTALL_TARGET_CMDS
	rm -rf $(TARGET_DIR)/resolv.conf
	cp --remove-destination -rdpf $(VENSEY_FS_SITE)/* $(TARGET_DIR)/
endef

$(eval $(generic-package))

