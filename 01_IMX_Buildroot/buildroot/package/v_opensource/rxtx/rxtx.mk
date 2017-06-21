#############################################################
#
# rxtx
#
#############################################################
RXTX_VERSION:=2.2pre2
RXTX_SOURCE:=rxtx-$(RXTX_VERSION).zip
RXTX_SITE:=http://rxtx.qbang.org/pub/rxtx
RXTX_DIR:=$(BUILD_DIR)/rxtx-$(RXTX_VERSION)
RXTX_BINARY:=RXTXcomm.jar
RXTX_TARGET_BINARY:=$(TARGET_DIR)/opt/local/ejdk1.8.0_111/lib/arm
RXTX_TARGET_JAVA:=$(TARGET_DIR)/opt/local/ejdk1.8.0_111/lib/ext
JHOME:=$(TARGET_DIR)/opt/local/ejdk1.8.0_111/jre/lib/ext
JPATH:=$(TARGET_DIR)/opt/local/ejdk1.8.0_111
JAVAINCLUDEDIR:=$(TARGET_DIR)/../../vensey/jdk_include

RXTX_DEPENDENCIES = oracle_jdk

define RXTX_EXTRACT_CMDS
	unzip $(DL_DIR)/$(RXTX_SOURCE) -d $(BUILD_DIR)
        touch $@
endef

define RXTX_CONFIGURE_CMDS
	(cd $(RXTX_DIR); rm -rf config.cache; \
        $(TARGET_CONFIGURE_OPTS) \
        $(TARGET_CONFIGURE_ARGS) \
        ./configure \
	JAVAINCLUDEDIR=$(JAVAINCLUDEDIR) \
        --target=$(GNU_TARGET_NAME) \
        --host=$(GNU_TARGET_NAME) \
        --build=$(GNU_HOST_NAME) \
        --prefix=/usr \
        --sysconfdir=/etc \
        )
        touch $@
endef 

define RXTX_BUILD_CMDS
	$(MAKE)  -C $(RXTX_DIR) JAVAINCLUDEDIR=$(JAVAINCLUDEDIR)
endef 

define RXTX_INSTALL_TARGET_CMDS 
	echo "At build install targets" 
	cp -rdpf $(RXTX_DIR)/arm-buildroot-linux-gnueabi/.libs/librxtx*.so* $(RXTX_TARGET_BINARY)
        cp -rdpf $(RXTX_DIR)/RXTXcomm.jar $(RXTX_TARGET_JAVA)
        -$(STRIPCMD) $(STRIP_STRIP_UNNEEDED) $(RXTX_TARGET_BINARY)/librxtx*.so*
endef 

$(eval $(generic-package))

