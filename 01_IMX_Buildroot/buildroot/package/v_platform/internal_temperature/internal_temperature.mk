#############################################################
#
# internal_temperature
#
#############################################################

INTERNAL_TEMPERATURE_VERSION = 1.0.0
INTERNAL_TEMPERATURE_SITE = $(TOPDIR)/../vensey/package/internal_temperature
INTERNAL_TEMPERATURE_SITE_METHOD = local
INTERNAL_TEMPERATURE_INSTALL_STAGING = YES
INTERNAL_TEMPERATURE_LICENSE = Open Source
INTERNAL_TEMPERATURE_DIR = $(BUILD_DIR)/internal_temperature-$(INTERNAL_TEMPERATURE_VERSION)/src

define INTERNAL_TEMPERATURE_BUILD_CMDS
	echo $(INTERNAL_TEMPERATURE_DIR)/src 
	$(MAKE) CC=$(TARGET_CC) LD=$(TARGET_LD) -C $(@D)/src all
endef

define INTERNAL_TEMPERATURE_INSTALL_TARGET_CMDS
	cp -rdpf $(@D)/src/internal_temperature $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))

