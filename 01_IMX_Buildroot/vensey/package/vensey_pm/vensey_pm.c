/* ************************************************************************
 * Copyright (C) 2017 VeNSeY Technology
 * ************************************************************************/

/** @file vensey_pm.c
 This file defines the main power management driver functionalities
 
 @project       VeNSeY Board
 @author	Muersel Yildiz
 @date		2017-01-31 (YYY-MM-DD)
 **************************************************************************/

/***************************************************************************
 * HEADER FILE INCLUDES
 **************************************************************************/
#include "vensey_pm.h"

/***************************************************************************
 * CONSTANTS
 ***************************************************************************/

/***************************************************************************
 * MACROS
 ***************************************************************************/

/***************************************************************************
 * GLOBAL VARIABLES
 ***************************************************************************/

/*
 * File operations for the /proc entry regarding power management
 */
const struct file_operations vensey_pm_debug_fops = {
    .owner      = THIS_MODULE,
    .open       = vensey_pm_debug_open,
    .read       = seq_read,
    .write      = vensey_pm_debug_write,
    .llseek     = seq_lseek,
    .release    = single_release,
};

struct proc_dir_entry* pm_proc_debug_file;

/*
 * Test parameters
 */
int len = 0;
char *msg;

/***************************************************************************
 * FUNCTION DEFINITIONS
 ***************************************************************************/

/**
 * @brief       Writing an integer to the memory space (physical address)
 *
 * @param       long address: register address (physical) 
 * @param       int size: use 0 always, the function will be extended for block reading
 * @param       int offset: use 0 always, the function will be extended
 ***********************************************************************/
int register_read_int(long address, int size, int offset)
{
    uint *mapped_region; // pointer for the requested resource
    int register_value;
    
    mapped_region = allocate_mem_region(address, size);
    if (!mapped_region)
    {
        printk("%s: unable to map memory region\n", __func__);
        release_mem_region (address, size);
        return -ENOMEM;
    }
    /*
     * return the register value
     */
    register_value = ioread32(mapped_region + offset);
    
    /*
     * Finally release the memory again.
     */
    release_mem_region (address, size);
    
    /*
     * Return the read value
     */
    return register_value;
}

/**
 * @brief	Writing an integer to the memory space (physical address)
 *
 * @param 	long address: register address (physical) 
 * @param	int offset: offset it necessary, use 0 for the specific address
 * @param 	int value: value to be written to the address
 ***********************************************************************/
int register_write_int(long address, int size, int offset, int value)
{
    uint *mapped_region; // pointer for the requested resource
    
    //TODO: This is to be removed... This has a conflict with the RTC driver
    /*
     * WARNING!!! THIS IS JUST SPECIFIC FOR SOME USE CASES, WHICH IS NOT A CLEAR WAY 
     * OF DOING THINGS IN KERNEL....
     */
    if( address == HW_RTC_PERSISTENT0_CLR ){
        mapped_region = allocate_mem_region(address, size);
        if (!mapped_region)
        {
            printk("%s: unable to map memory region for HW_RTC_PERSISTENT0_CLR properly, \n", __func__);
        }
        release_mem_region (address, size);
    }
    /*
     * WARNING!!! finished...
     */
    
    /*
     * Allocate region for write operation.
     */
    mapped_region = allocate_mem_region(address, size);
    if (!mapped_region)
    {
        printk("%s: unable to map memory region\n", __func__);
        release_mem_region (address, size);
        return -ENOMEM;
    }
    /*
     * now write the input to the related register.
     */
    iowrite32(value, mapped_region + offset);
    
    /*
     * write operation is completed, we shall release the memory
     */
    release_mem_region (address, size);
    return 0;
}


/**
 * @brief 	Memory allocation on kernel space 
 * 
 * @param	long address: memory address (physical) 
 * @param	int size: space size to be allocated 
 ***********************************************************************/
uint * allocate_mem_region(long address, int size)
{
    struct resource *resource_handler;  // resource handler
    uint *mapped_region; // pointer for the requested resource
    
    /*
     * Request the resource from the kernel.
     */
    resource_handler = request_mem_region (address, size, "vensey_pm");   // map 16 bytes
    if (!resource_handler)
    {
        printk("%s: unable to acquire memory region\n", __func__);
        return NULL;
    }
    /*
     * now the resource is requested,
     * map it to the mapped_region pointer
     */
    mapped_region = ioremap (address, size);
    if (!mapped_region)
    {
        printk("%s: unable to map memory region\n", __func__);
        release_mem_region (address, size);
        return NULL;
    }
    return mapped_region;
}


/**
 * @brief
 * @param
 ***********************************************************************/
int vensey_pm_debug_open(struct inode *inode, struct file *file)
{
    return single_open(file, vensey_pm_debug_provide_data, NULL);
}

/**
 * @brief
 * @param
 ***********************************************************************/
int vensey_pm_debug_provide_data(struct seq_file *m, void *v)
{
    printout_power_registers(m);
    return 0;
}

/**
 * @brief	File output for the user space. 
 * 
 * @param	seq_file *m: allocated user space file  
 ***********************************************************************/
void printout_power_registers(struct seq_file *m)
{
    read_out_all_power_registers();
    seq_printf(m, "hw_octp_rom0: 0x%08x \n", hw_octp_rom0);
    seq_printf(m, "hw_power_ctr: 0x%08x \n", hw_power_ctr);
    seq_printf(m, "hw_power_5vc: 0x%08x \n", hw_power_5vc);
    seq_printf(m, "hw_power_min: 0x%08x \n", hw_power_min);
    seq_printf(m, "hw_power_ver: 0x%08x \n", hw_power_ver);
    seq_printf(m, "hw_power_res: 0x%08x \n", hw_power_res);
    seq_printf(m, "hw_power_chr: 0x%08x \n", hw_power_chr);
    seq_printf(m, "hw_power_d4p: 0x%08x \n", hw_power_d4p);
    seq_printf(m, "hw_power_msc: 0x%08x \n", hw_power_msc);
    seq_printf(m, "hw_power_lpc: 0x%08x \n", hw_power_lpc);
    seq_printf(m, "hw_power_sts: 0x%08x \n", hw_power_sts);
    seq_printf(m, "hw_power_spd: 0x%08x \n", hw_power_spd);
    seq_printf(m, "hw_power_bmt: 0x%08x \n", hw_power_bmt);
    seq_printf(m, "hw_power_dbg: 0x%08x \n", hw_power_dbg);
    seq_printf(m, "hw_power_spc: 0x%08x \n", hw_power_spc);
    seq_printf(m, "hw_rtc_pers0: 0x%08x \n", hw_rtc_pers0);
    seq_printf(m, "hw_rtc_alarm: 0x%08x \n", hw_rtc_alarm);
    seq_printf(m, "hw_pw_vdddct: 0x%08x \n", hw_pw_vdddct);
    seq_printf(m, "hw_pw_vddact: 0x%08x \n", hw_pw_vddact);
    seq_printf(m, "hw_pw_vddioc: 0x%08x \n", hw_pw_vddioc);
    seq_printf(m, "hw_pw_memctr: 0x%08x \n", hw_pw_memctr);
    
}

/**
 * @brief	Reading critical power management related registers
 * 
 * @param
 ***********************************************************************/
void read_out_all_power_registers(void)
{
    hw_octp_rom0 = register_read_int(HW_OCOTP_ROM0, 8, 0);
    hw_power_ctr = register_read_int(HW_POWER_CTRL, 8, 0); //read HW_POWER_CTRL with 8 byte
    hw_power_5vc = register_read_int(HW_POWER_5VCTRL, 8, 0); //read HW_POWER_5VCTRL with 8 byte
    hw_power_min = register_read_int(HW_POWER_MINPWR, 8, 0); //read HW_POWER_MINPWR with 8 byte
    hw_power_ver = register_read_int(HW_POWER_VERSION, 8, 0); //read HW_POWER_VERSION with 8 byte
    hw_power_res = register_read_int(HW_POWER_RESET, 8, 0); //read HW_POWER_RESET with 8 byte
    hw_power_chr = register_read_int(HW_POWER_CHARGE, 8, 0); //read HW_POWER_CHARGE with 8 byte
    hw_power_d4p = register_read_int(HW_POWER_DCDC4P2, 8, 0); //read HW_POWER_DCDC4P2 with 8 byte
    hw_power_msc = register_read_int(HW_POWER_MISC, 8, 0); //read HW_POWER_MISC with 8 byte
    hw_power_lpc = register_read_int(HW_POWER_LOOPCTRL, 8, 0); //read HW_POWER_LOOPCTRL with 8 byte
    hw_power_sts = register_read_int(HW_POWER_STS, 8, 0); //read HW_POWER_STS with 8 byte
    hw_power_spd = register_read_int(HW_POWER_SPEED, 8, 0); //read HW_POWER_SPEED with 8 byte
    hw_power_bmt = register_read_int(HW_POWER_BATTMONITOR, 8, 0); //read HW_POWER_BATTMONITOR with 8 byte
    hw_power_dbg = register_read_int(HW_POWER_DEBUG, 8, 0); //read HW_POWER_DEBUG with 8 byte
    hw_power_spc = register_read_int(HW_POWER_SPECIAL, 8, 0); //read HW_POWER_SPECIAL with 8 byte
    hw_rtc_pers0 = register_read_int(HW_RTC_PERSISTENT0, 8, 0); //read HW_RTC_PERSISTENT0 with 8 byte
    hw_rtc_alarm = register_read_int(HW_RTC_ALARM, 8, 0); //read HW_RTC_ALARM with 8 byte
    hw_pw_vdddct = register_read_int(HW_POWER_VDDDCTRL, 8, 0); //read HW_RTC_ALARM with 8 byte
    hw_pw_vddact = register_read_int(HW_POWER_VDDACTRL, 8, 0); //read HW_RTC_ALARM with 8 byte
    hw_pw_vddioc = register_read_int(HW_POWER_VDDIOCTRL, 8, 0); //read HW_RTC_ALARM with 8 byte
    hw_pw_memctr = register_read_int(HW_POWER_VDDMEMCTRL, 8, 0); //read HW_RTC_ALARM with 8 byte
}


/**
 * @brief	Copying inputs from the user. 
 * 
 * @param	file *: input file on user space
 * @param 	char *: buffer 
 * @param	size_t: input string size 
 * @param 	loff_t *
 ***********************************************************************/
int vensey_pm_debug_write(struct file *filp,const char *buf,size_t count, loff_t *offp)
{
    int ret_val;
    ret_val = copy_from_user(msg,buf,count);
    if( ret_val < 0 )
    {
         printk("ret_val for copy_from_user: %d...\n", ret_val);
    }
    
    /*
     * Now compare the message with different operational commands
     */
    if (strncmp( (const char*) msg, "clr_autostrt", count-1 ) == 0)
    {
        printk("%s: Clearing auto start due to RTC \n", __func__);
        /*
         * clear the:
         * i) auto_start
         * ii) disable_pswitch
         * iii) alarm_en
         */
        register_write_int(HW_RTC_PERSISTENT0_CLR, 8, 0, 0xFFFFFFFF);
        //register_write_int(HW_RTC_PERSISTENT0_CLR, 8, 0, 0xFFFEF0FF);
        
    }

    else if (strncmp( (const char*) msg, "s_down_f", count-1 ) == 0)
    {
        printk("%s: powering down imx... \n", __func__);
        /*
         * Shut down now...
         */
        register_write_int(HW_POWER_RESET, 8, 0, 0x3E770001);
    }

    else if (strncmp( (const char*) msg, "set_alarm", count-1 ) == 0)
    {
        printk("%s: set alarm... \n", __func__);
        /*
         * set the alarm, 
         * TODO: the following fix alarm shall be changed with an input from user.
         */
        register_write_int(HW_RTC_ALARM, 8, 0, FIX_ALARM);
    }
    else if (strncmp( (const char*) msg, "s_alarm", count-1 ) == 0)
    {
        printk("%s: powering imx down with alarm... \n", __func__);
        /*
         * clear the:
         * i) auto_start
         * ii) disable_pswitch
         */
        register_write_int(HW_RTC_PERSISTENT0_CLR, 8, 0, 0x00030000);
        
        /*
         * set the:
         * i) alarm_en
         */
        register_write_int(HW_RTC_PERSISTENT0_SET, 8, 0, 0x00000004);
        
        /*
         * Shut down now...
         */
        register_write_int(HW_POWER_RESET, 8, 0, 0x3E770001);
    }
    else if (strncmp( (const char*) msg, "en_batt", count-1 ) == 0)
    {
        printk("%s: enabling BATT.. \n", __func__);
        /*
         * Enable the 4P2 so that the battery load will be removed
         */
        
    }
    else if (strncmp( (const char*) msg, "en_4p2", count-1 ) == 0)
    {
        printk("%s: enabling DCDC_4P2.. \n", __func__);
        /*
         * Set the followings for the HW_POWER_DCDC4P2
         * i) Dropout control to 200mV
         * ii) Regulation voltage at 4,2V
         */
        register_write_int(HW_POWER_DCDC4P2, 8, 0, 0xF0C00000);
        /*
         * Enable the 4P2 (ENABLE_4P2)
         */
        register_write_int(HW_POWER_DCDC4P2, 8, 0, 0xF0C00000);
        /*
         * Enable load at hw charge
         */
        register_write_int(HW_POWER_CHARGE_SET, 8, 0, 0x00400000);
        /*
         * clear battery charger pwd
         */
        register_write_int(HW_POWER_CHARGE_CLR, 8, 0, 0x00010000);
        /*
         * set the current limit at 20mA, note that the limit
         * will charge the capacitor of the 4p2 for a short time,
         * later the value should be increased.
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00002000);
        /*
         * Power on 4P2 LinReg.
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00100000);
        /*
         * Ungate the path from 4P2 LinReg to DC-DC
         */
        register_write_int(HW_POWER_DCDC4P2, 8, 0, 0xF0C00000);
        
    }
    else if (strncmp( (const char*) msg, "set_4V_full", count-1 ) == 0)
    {
        /*
         * Note that this shall be done before the 4P2 is selected
         * each time after the boot up time for the training board
         */
        printk("%s: Setting maximum output current at 4p2.. \n", __func__);
        /*
         * Set the current limit at 780 mA
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x0003F000);
        /*
         * at this stage we want to remove reboot mechanism once 5V is lost
         */
        register_write_int(HW_POWER_5VCTRL_CLR, 8, 0, 0x00000020);
        
    }
    else if (strncmp( (const char*) msg, "set_4V_th", count-1 ) == 0)
    {
        /*
         * Note that this shall be done before the 4P2 is selected
         * each time after the boot up time for the training board
         */
        printk("%s: enabling VBUSVALID.. \n", __func__);
        /*
         * Turn on VBUS comparators
         * i) set PWRUP_VBUS_CMPS
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00000002);
        /*
         * Set the VBUSVALID_TRSH to 4.4V
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00000100);
        /*
         * Set the VBUSVALID_5VDETECT
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00000010);
        
    }
    else if (strncmp( (const char*) msg, "set_4.4V_th", count-1 ) == 0)
    {
        /*
         * Note that this shall be done before the 4P2 is selected
         * each time after the boot up time for the training board
         */
        printk("%s: enabling VBUSVALID.. \n", __func__);
        /*
         * Turn on VBUS comparators
         * i) set PWRUP_VBUS_CMPS
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00000002);
        /*
         * Set the VBUSVALID_TRSH to 4.4V
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00000500);
        /*
         * Set the VBUSVALID_5VDETECT
         */
        register_write_int(HW_POWER_5VCTRL_SET, 8, 0, 0x00000010);
    }
    else if (strncmp( (const char*) msg, "set_nooli", count-1 ) == 0)
    {
        printk("%s: activating nooli settings... \n", __func__);
        /*
         * increase the current & set the threshold to 4 volts & clear the PWDN_5VBRNOUT
         */
        register_write_int(HW_POWER_5VCTRL, 8, 0, 0x0403F193);
        /*
         * Whenever the 5V is available, we shall write the following to the charger, 
         * thus set the POLARITY_VBUSVALID to 1 in order to set IRQ for VBUSVALID
         */
        register_write_int(HW_POWER_CTRL_SET, 8, 0, 0x00000020);
        /*
         * Now read the HW_POWER_CTRL in order to check whether the VBUSVALID is set
         */
        hw_power_ctr = register_read_int(HW_POWER_CTRL, 8, 0);
        printk("%s: hw_power_ctr: 0x%08x ... \n", __func__, hw_power_ctr);
        /*
         * Check the VBUSVALID bit only.
         */
        if( (hw_power_ctr & 0x00000010) != 0){
            printk("%s: VBUSVALID is set... \n", __func__);
            /*
             * Now set the HW_POWER_CHARGE
             */
            //register_write_int(HW_POWER_CHARGE, 8, 0, 0x0000003F);
            register_write_int(HW_POWER_CHARGE, 8, 0, 0x00400002);
        }
       
    }
    else if (strncmp( (const char*) msg, "dis_load_res", count-1 ) == 0)
    {
        printk("%s: disabling load resistor on charger... \n", __func__);
        /*
         * Removing battery charger functionality HW_POWER_CHARGE
         */
        register_write_int(HW_POWER_CHARGE, 8, 0, 0x0000003F);
        
    }
    else if (strncmp( (const char*) msg, "release_nooli", count-1 ) == 0)
    {
        printk("%s: activating nooli settings... \n", __func__);
        /*
         * Removing battery charger functionality HW_POWER_CHARGE
         */
        register_write_int(HW_POWER_CHARGE, 8, 0, 0x00010000);
        
    }
    
    return count;
}

/**
 * @brief
 * @param
 ***********************************************************************/
int create_debug_proc_entry_pm(char* file_name, struct file_operations pm_fops)
{
    return 0;
}



/**
 * @brief  Initialization function for the pm module
 * 
 * @param
 ***********************************************************************/
int __init vensey_pm_init(void)
{
    printk("VeNSeY PM driver initialization.\n");
    
    /*
     * Allocate a block for message exchange with the user.
     */
    msg=kmalloc(10*sizeof(char), GFP_KERNEL);
    if( msg == NULL )
    {
        printk("msg cannot be allocated...\n");
    }
    
    /*
     * Generating the power management debug file under /proc
     */
    pm_proc_debug_file =  proc_create(VENSEY_PM_DEBUG, 0, NULL, &vensey_pm_debug_fops);
    if (!pm_proc_debug_file)
    {
        printk("%s pm_proc_debug_file cannot be allocated...\n", VENSEY_PM_DEBUG);
        return -ENOMEM;
    }
     
    return 0;
}
module_init(vensey_pm_init);


/**
 * @brief  Module exit definition
 * @param
 ***********************************************************************/
void __exit vensey_pm_exit(void)
{
	printk("VeNSeY PM removed...\n");
	remove_proc_entry("pm_debug",NULL);
}
module_exit(vensey_pm_exit);


/***************************************************************************
 * Module description and related licenses.
 ***************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Muersel Yildiz <mursel@vensey.de");
MODULE_DESCRIPTION("VeNSeY imx233 complex power management module");
MODULE_VERSION("1.0.0");

