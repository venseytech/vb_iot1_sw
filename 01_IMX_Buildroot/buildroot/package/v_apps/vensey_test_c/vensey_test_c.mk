#############################################################
#
# vensey_test_c
#
#############################################################

VENSEY_TEST_C_VERSION = 1.0.0
VENSEY_TEST_C_SITE = $(TOPDIR)/../vensey/package/vensey_test_c
VENSEY_TEST_C_SITE_METHOD = local
VENSEY_TEST_C_INSTALL_STAGING = YES
VENSEY_TEST_C_LICENSE = VeNSeY Technology 2017  
VENSEY_TEST_C_DIR = $(BUILD_DIR)/vensey_test_c-$(VENSEY_TEST_C_VERSION)/src
VENSEY_TEST_C__DEPENDENCIES = libglib2 host-pkg-config


define VENSEY_TEST_C_CONFIGURE_CMDS
	echo "At Configure CMDS" 
endef

define VENSEY_TEST_C_BUILD_CMDS
	echo $(VENSEY_TEST_C_DIR)/src
	echo "$(TARGET_CONFIGURE_OPTS)"
	(cd $(@D); \
                PKG_CONFIG_SYSROOT_DIR="$(STAGING_DIR)" \
                PKG_CONFIG="$(PKG_CONFIG_HOST_BINARY)" \
                PKG_CONFIG_PATH="$(STAGING_DIR)/usr/lib/pkgconfig:$(PKG_CONFIG_PATH)" \
                $(TARGET_MAKE_ENV) \
                $(MAKE) CC=$(TARGET_CC) LD=$(TARGET_LD) -C $(@D)/src all \
        )
endef

define VENSEY_TEST_C_INSTALL_TARGET_CMDS
	cp -rdpf $(@D)/src/vensey_test_c $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))

