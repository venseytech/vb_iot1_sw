#############################################################
#
# oracle_jdk
#
#############################################################

ORACLE_JDK_VERSION = 1.8.0_111
ORACLE_JDK_SITE = $(TOPDIR)/../vensey/package/oracle_jdk
ORACLE_JDK_SITE_METHOD = local
ORACLE_JDK_INSTALL_STAGING = YES
ORACLE_JDK_LICENSE = PROPRIETARY
#PLATFORMFILES_DEPENDENCIES = sysvinit

EJDK_HOME = $(ORACLE_JDK_SITE)/ejdk1.8.0_111
JAVA_HOME = /usr/lib/jvm/java-1.8.0-openjdk-i386 

define ORACLE_JDK_INSTALL_STAGING_CMDS
	echo "staging cmds at oracle jdk files" 
endef

define ORACLE_JDK_INSTALL_TARGET_CMDS
	mkdir -p $(EJDK_HOME)/tmpdir
	rm -rf $(EJDK_HOME)/tmpdir/* 
	mkdir -p $(TARGET_DIR)/opt/local/ejdk1.8.0_111 
	rm -rf $(TARGET_DIR)/opt/local/ejdk1.8.0_111
	$(EJDK_HOME)/bin/jrecreate.sh --vm all --dest $(TARGET_DIR)/opt/local/ejdk1.8.0_111 
endef

$(eval $(generic-package))

