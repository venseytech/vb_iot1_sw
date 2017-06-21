#############################################################
#
# vensey_pm
#
#############################################################

VENSEY_PM_VERSION = 1.0.0
VENSEY_PM_SITE = $(TOPDIR)/../vensey/package/vensey_pm
VENSEY_PM_SITE_METHOD = local
VENSEY_PM_INSTALL_STAGING = YES
VENSEY_PM_LICENSE = VeNSeY Technology 
VENSEY_PM_DIR = $(BUILD_DIR)/vensey_pm-$(VENSEY_PM_VERSION)

#define VENSEY_PM_BUILD_CMDS
#	echo $(LINUX_HEADERS_DIR) 
#	$(MAKE) CC=$(TARGET_CC) LD=$(TARGET_LD) KDIR=$(LINUX_HEADERS_DIR) -C $(@D) all
#endef

#define INTERNAL_TEMPERATURE_INSTALL_TARGET_CMDS
#	cp -rdpf $(@D)/vensey_pm.ko $(TARGET_DIR)/
#endef

$(eval $(kernel-module))
$(eval $(generic-package))

