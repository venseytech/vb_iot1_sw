// This file will create the 512 byte BCB config block that must be located
//   at the last block of the SD card when booting from SD with BCB.
//   This is for STMP378x or i.MX233 BCB boot creation.
//   You can use dd in Linux to wriet this one out to the SD card.

#include <stdio.h>

// NEVER CHANGE ANY OF THE FOLLOWING:
#define CONFIG_BLOCK_SIGNATURE 0x00112233 // This MUST be bcb_config[0] 
#define CONFIG_BLOCK_VERSION   0x1        // This must be bcb_config[1]
#define BOOTIMAGE_TAG          0x50       // This is the boot image tag ROM looks
                                          //  to decide where to start booting.
#define NUM_DRIVES             0x4        // not really used by the ROM.

// The following defines may need to be changed.

// Image Start Block
// Change this number to the starting sector of the boot image.
//#define IMAGE_START_BLOCK 63 // for this example, boot image will start at
#define IMAGE_START_BLOCK 2048 // for this example, boot image will start at
                            //    60th sector of the SD card.  each sector is 512 bytes.

FILE *f1;     

void main(void)
{
  unsigned int i, dat = CONFIG_BLOCK_SIGNATURE, NUM_WORDS_WRITTEN;
   f1 = fopen ("bcb_512.cfg", "wt");

   fwrite(&dat, sizeof(dat), 1, f1); // 1st byte is signature.
   
   dat = CONFIG_BLOCK_VERSION;
   fwrite(&dat, sizeof(dat), 1, f1); // 2nd byte is version.
   
   dat = NUM_DRIVES;
   fwrite(&dat, sizeof(dat), 1, f1);

   // Go ahead and write another couple  dummy words, ROM won't use anyways...
   fwrite(&dat, sizeof(dat), 1, f1);
   fwrite(&dat, sizeof(dat), 1, f1);

   // Now write the sector where the actual boot image will be.
   dat = IMAGE_START_BLOCK;
   fwrite(&dat, sizeof(dat), 1, f1);

   // Now we can write the boot tag ROM will be looking for.
   dat = BOOTIMAGE_TAG;
   fwrite(&dat, sizeof(dat), 1, f1);
   fwrite(&dat, sizeof(dat), 1, f1);
   fwrite(&dat, sizeof(dat), 1, f1);
   fwrite(&dat, sizeof(dat), 1, f1);

   dat = 0xDEADBEEF;
   fwrite(&dat, sizeof(dat), 1, f1);

   NUM_WORDS_WRITTEN = 11; // number of times called fwrite.
   
   // Need to make the .cfg file 512 bytes long, so...
   for(i=0; i < ((512/4) - NUM_WORDS_WRITTEN); i++){
	   fwrite(&i, sizeof(i), 1, f1);
   }

   fclose (f1);
}
