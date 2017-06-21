#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "imx233_regs.h"

#define LRADC_IRQ_PENDING 0x00000003
#define CH0_SCHEDULE 0x00000003

// NUM_SAMPLES is a 5-bit value
#define NUM_SAMPLES 0x10
//#define NUM_SAMPLES 0x01

extern int imx233_rd(long offset);
extern int imx233_wr(long offset, long value);
extern void usleep(int value);

int main(int argc, char **argv) {

   unsigned value;
   unsigned value0;
   unsigned value1;
   struct timeval tv;

   // If the name of the offset ends in CLR, the bits that are hi will be set to 0.
   // If the name of the offset ends in SET, the bits that are hi will be set to 1.
   // If the name of the offset ends in TOG, the bits that are hi will be toggled.
   
   imx233_wr(HW_LRADC_CTRL4_CLR, 0xFF);  // Clear mux
   imx233_wr(HW_LRADC_CTRL4_SET, 0x89);  // Set LRADC1=channel9, LRADC0=channel8
   imx233_wr(HW_LRADC_CTRL2_CLR, 0x8000);  // Clear TEMPSENSE_PWD
   imx233_wr(HW_LRADC_CTRL1_CLR, 0x00000003);  // Clear interrupt bit
   
   imx233_wr(HW_LRADC_CH0_SET, 0x10000000);  // Set accumulate bit
   imx233_wr(HW_LRADC_CH0_CLR, 0x0003ffff);  // Clear accumulator
   imx233_wr(HW_LRADC_CH0_CLR, 0x1f000000);  // Clear num samples
   imx233_wr(HW_LRADC_CH1_SET, 0x10000000);  // Set accumulate bit
   imx233_wr(HW_LRADC_CH1_CLR, 0x0003ffff);  // Clear accumulator
   imx233_wr(HW_LRADC_CH1_CLR, 0x1f000000);  // Clear num samples
   if ( NUM_SAMPLES > 0 ) {
      value = (NUM_SAMPLES-1) << 24;
      printf( "samples=0x%08x \n", value );
      imx233_wr(HW_LRADC_CH0_SET, value);  // Set num samples
      imx233_wr(HW_LRADC_CH1_SET, value);  // Set num samples
   }
   
   gettimeofday(&tv,NULL);
   printf( "Start of conversion: %d %d\n", tv.tv_sec, tv.tv_usec );
            
   // Check for interrupt bit going hi, which indicates that all conversions
   // are complete.
   while ( !(imx233_rd(HW_LRADC_CTRL1) & LRADC_IRQ_PENDING) ) {
      	// Schedule a conversion on ch 1
      	imx233_wr(HW_LRADC_CTRL0_SET, CH0_SCHEDULE);
      	printf( "conversion scheduled...\n" );
	  
      	// Wait for schedule bit to be cleared, indicating conversion complete
      	while ( (imx233_rd(HW_LRADC_CTRL0) & CH0_SCHEDULE) ) { }
      		printf( "conversion complete.\n" );
	  
	// The next line is a hack. I've found that the data is somtimes not
	// ready even though the processor cleared the schedule bit. This slight
      	// delay is enough time to allow the data to become ready.
      	sleep(1);	  
	//break; 
   }
   
   gettimeofday(&tv,NULL);
   //printf( "End of conversion: %d %d\n", tv.tv_sec, tv.tv_usec );
            
   value1 = imx233_rd(HW_LRADC_CH0) & 0x3ffff; // Value is in the 12 LSBs
   value0 = imx233_rd(HW_LRADC_CH1) & 0x3ffff; // Value is in the 12 LSBs

   //printf("v0=%d v1=%d\n", value0, value1);
   value = (value1-value0) * 1012/(4000*NUM_SAMPLES)-273;
   printf( "TEMP=%dC\n", value );
   
   return 0;

}

