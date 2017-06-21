/* ************************************************************************
 * Copyright (C) 2017 VeNSeY Technology
 * ************************************************************************/

/** @file vensey_pm.h
 This file defines the main application
 
 @project       VeNSeY Board
 @author	Muersel Yildiz
 @date		2017-01-31 (YYY-MM-DD)
 **************************************************************************/


/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "imx233_regs.h"

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/
#define LRADC_IRQ_PENDING   0x00000003
#define CH0_SCHEDULE        0x00000003
#define NUM_SAMPLES         0x10
#define FIX_ALARM           0x00001111 // 16 seconds for testing alarm functionality
#define VENSEY_PM_DEBUG     "pm_experimental"

/***************************************************************************
 * MACROS
 ***************************************************************************/

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/
unsigned int hw_octp_rom0 = 0;
unsigned int hw_power_ctr = 0;
unsigned int hw_power_5vc = 0;
unsigned int hw_power_min = 0;
unsigned int hw_power_ver = 0;
unsigned int hw_power_res = 0;
unsigned int hw_rtc_pers0 = 0;
unsigned int hw_rtc_alarm = 0;
unsigned int hw_power_chr = 0;
unsigned int hw_power_d4p = 0;
unsigned int hw_power_msc = 0;
unsigned int hw_power_lpc = 0;
unsigned int hw_power_sts = 0;
unsigned int hw_power_spd = 0;
unsigned int hw_power_bmt = 0;
unsigned int hw_power_dbg = 0;
unsigned int hw_power_spc = 0;
unsigned int hw_pw_vdddct = 0;
unsigned int hw_pw_vddact = 0;
unsigned int hw_pw_vddioc = 0;
unsigned int hw_pw_memctr = 0;

/***************************************************************************
 * FUNCTION DECLARATIONS
 ***************************************************************************/

int vensey_pm_open(struct inode *inode, struct file *file);
int vensey_pm_read(struct file *filp,char *buf,size_t count,loff_t *offp);
int vensey_pm_debug_write(struct file *filp,const char *buf,size_t count, loff_t *offp);
int create_debug_proc_entry_pm(char* file_name, struct file_operations pm_fops);
int vensey_pm_debug_provide_data(struct seq_file *m, void *v);
int vensey_pm_debug_open(struct inode *inode, struct file *file);
int register_read_int(long address, int size, int offset);
int register_write_int(long address, int size, int offset, int value);
uint * allocate_mem_region(long address, int size);
void read_out_all_power_registers(void); 
void printout_power_registers(struct seq_file *m);
void init_registers_nooli(void); 
int __init vensey_pm_init(void);
void __exit vensey_pm_exit(void);



