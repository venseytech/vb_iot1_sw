#############################################################
#
# VENSEY_TEST_JDK
#
#############################################################

VENSEY_TEST_JDK_VERSION = 1.0.0
VENSEY_TEST_JDK_SITE = $(TOPDIR)/../vensey/package/vensey_test_jdk
VENSEY_TEST_JDK_SITE_METHOD = local
VENSEY_TEST_JDK_INSTALL_STAGING = YES
VENSEY_TEST_JDK_LICENSE = PROPRIETARY
VENSEY_TARGET_JAVA = $(TARGET_DIR)/opt/local/ejdk1.8.0_111/lib/ext
JAVAINCLUDEDIR:=$(TARGET_DIR)/../../vensey/jdk_include
JAVAINCLUDEDIRLINUX:=$(TARGET_DIR)/../../vensey/jdk_include/linux

define VENSEY_TEST_JDK_INSTALL_STAGING_CMDS
	echo "staging cmds for vensey test jdk" 
	rm -rf $(VENSEY_TEST_JDK_SITE)/bin
	mkdir -p $(VENSEY_TEST_JDK_SITE)/bin
	mkdir -p $(VENSEY_TEST_JDK_SITE)/jni
endef

define VENSEY_TEST_JDK_INSTALL_TARGET_CMDS
	javac -d $(VENSEY_TEST_JDK_SITE)/bin -sourcepath $(VENSEY_TEST_JDK_SITE)/src -cp $(VENSEY_TEST_JDK_SITE)/Libs/RXTXcomm.jar:$(VENSEY_TEST_JDK_SITE)/Libs/commons-logging-1.2.jar:$(VENSEY_TEST_JDK_SITE)/Libs/httpcore-4.4.6.jar:$(VENSEY_TEST_JDK_SITE)/Libs/httpclient-4.5.3.jar $(VENSEY_TEST_JDK_SITE)/src/main/MainThread.java
	javah -d $(VENSEY_TEST_JDK_SITE)/jni -classpath $(VENSEY_TEST_JDK_SITE)/bin serialconnection.I2CConnectionService
	javah -d $(VENSEY_TEST_JDK_SITE)/jni -classpath $(VENSEY_TEST_JDK_SITE)/bin application_components.MainApplication
	jar cfve $(VENSEY_TEST_JDK_SITE)/bin/vensey_test_jdk.jar main.MainThread -C $(VENSEY_TEST_JDK_SITE)/bin/ . 
	cp -rdpf $(VENSEY_TEST_JDK_SITE)/bin/vensey_test_jdk.jar $(TARGET_DIR)/usr/bin/
	cp -rdpf $(VENSEY_TEST_JDK_SITE)/Libs/httpcore-4.4.6.jar $(VENSEY_TARGET_JAVA)
	cp -rdpf $(VENSEY_TEST_JDK_SITE)/Libs/httpcore-4.4.6.jar $(VENSEY_TARGET_JAVA)
	cp -rdpf $(VENSEY_TEST_JDK_SITE)/Libs/commons-logging-1.2.jar $(VENSEY_TARGET_JAVA)
	(cd $(@D); \
                PKG_CONFIG_SYSROOT_DIR="$(STAGING_DIR)" \
                PKG_CONFIG="$(PKG_CONFIG_HOST_BINARY)" \
                PKG_CONFIG_PATH="$(STAGING_DIR)/usr/lib/pkgconfig:$(PKG_CONFIG_PATH)" \
                $(TARGET_MAKE_ENV) \
                $(MAKE) JAVAINCLUDEDIR=$(JAVAINCLUDEDIR) JAVAINCLUDEDIRLINUX=$(JAVAINCLUDEDIRLINUX) CC=$(TARGET_CC) LD=$(TARGET_LD) -C $(@D)/c_src all \
	)
	cp -rdpf $(@D)/c_src/libVeNSeYi2c.so $(TARGET_DIR)/usr/lib/
endef

$(eval $(generic-package))

