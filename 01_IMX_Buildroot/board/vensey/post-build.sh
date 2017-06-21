#!/bin/sh
cp -r $TARGET_DIR/../../vensey/package/vensey_fs/* $TARGET_DIR/
echo "Copied the interface files" 
cat $TARGET_DIR/etc/network/interfaces
echo "Content of the interfaces file is printed" 
cp $BINARIES_DIR/zImage.imx23-vensey $TARGET_DIR/boot/vmlinuz-vensey

