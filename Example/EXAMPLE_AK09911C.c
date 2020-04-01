/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: E-COMPASS
     File Name		: EXAMPLE_AK09911C.c
     Function		: EXAMPLE_AK09911C
     Create Date	: 2017/11/27
---------------------------------------------------------------------- */

#ifndef __AK09911C_EXAMPLE__
#define __AK09911C_EXAMPLE__

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <alcd.h>
#include <i2c.h>

#include "SENSOR_AK09911C.h"
#include "Porting_Layer.h"


// Note : RETN must be connect to Vcc , avoid reset.

//------- Define AK09911C variable ------
//if = 1 , read AK09911C inifite .
//if = 0 , read once . 
#define EXAMPLE_AK09911C_INIFITE_READ 	1


//------- Define AK09911C variable ------

AK09911C_MODE_SELECT AK09911C_MODE;				//mode

extern AK09911C5_ASA_DATA  AK09911C_ASA_XYZ_DATA;	// asa x,y,z reg data    ;  initial read ASA register & write in .

AK09911C_AXIS_DATA AK09911C5_XYZ_DATA;				// read x,y,z raw data

//------- Define AK09911C variable ------

void  EXAMPLE_AK09911C(void);

void  EXAMPLE_AK09911C(void)
{

	CHAR8U id_data,info_data[4]={0},check_mode;
	CHAR8S status = 0 ;
    	CHAR8U lcd_char_data[3][4]={0}; 
		
               printf("-------------------- E-COMPASS AK09911C --------------------\r\n");
               //E-COMPASS AK09911C ;


			//read AK09911C ID & INFO
			status = AK09911C_GET_ID_INFO(&info_data[0]);
			if(status != 0)  printf("read AK09911C ID fail!\r\n");
			else {
					printf("read AK09911C ID = INFO[0] = 0x%x \r\n",info_data[0]);
					printf("read AK09911C ID = INFO[1] = 0x%x \r\n",info_data[1]);
					printf("read AK09911C ID = INFO[2] = 0x%x \r\n",info_data[2]);
					printf("read AK09911C ID = INFO[3] = 0x%x \r\n",info_data[3]);
				}

			
			//first initial AK09911C :reset AK09911C ; read ASA reg ; SET single mode ;
			status = AK09911C_SET_INITIAL();
			if(status != 0)  printf("initial AK09911C fail!\r\n");
			else printf("initial AK09911C OK!\r\n");	
			
			//CHECK AUTO TEST , CHECK IC WORKING NORMALLY.
			// -30 <= x <= +30
			// -30 <= y <= +30
			// -400 <= z <= -50
#if 1
			status = AK09911C_SET_SELF_TEST(&AK09911C5_XYZ_DATA);
			if(status !=0)
				{
					printf("AK09911C auto test fail\r\n");					
				}
			else 
				{
					printf("X axis = %d\r\n",AK09911C5_XYZ_DATA.X_AXIS);
					printf("Y axis = %d\r\n",AK09911C5_XYZ_DATA.Y_AXIS);		
					printf("Z axis = %d\r\n",AK09911C5_XYZ_DATA.Z_AXIS);
					printf("AK09911C auto test OK \r\n");
				}

			delay_ms(3);
#endif



			delay_ms(1);
	
			
			//read AK09911C mode -> check AK09911C in power down mode
			status = AK09911C_GET_MODE(&check_mode);
			if(status != 0) 
				{
					printf("read AK09911C mode fail!\r\n");
				}
			else  
				{
					printf("read AK09911C mode ok\r\n");
					if(check_mode == AK09911C_POWER_DOWN_MODE) printf("power down mode\r\n");
					else 
						{printf("else mode\r\n");
					while(1); // stop next sequence
				 }
                }	


#if EXAMPLE_AK09911C_INIFITE_READ == 1
		while(1)
			{
#endif
			//read AK09911C XYZ data.
			status = AK09911C_GET_XYZ_DATA(&AK09911C5_XYZ_DATA);
			if(status != 0)
						{
							printf("read AK09911C data fail!\r\n");
						 }
			else{

							printf("read AK09911C data OK! \r\n");

							printf("X axis = %d \r\n",AK09911C5_XYZ_DATA.X_AXIS);
							printf("Y axis = %d \r\n",AK09911C5_XYZ_DATA.Y_AXIS);		
							printf("Z axis = %d \r\n",AK09911C5_XYZ_DATA.Z_AXIS);	
                   
                     
	                     // X AXIS   
	                     lcd_char_data[0][0]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS/1000)%10;
	                     lcd_char_data[0][1]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS/100)%10;   
	                     lcd_char_data[0][2]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS/10)%10;  
	                     lcd_char_data[0][3]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS)%10;   
	                     
	                     // Y AXIS   
	                     lcd_char_data[1][0]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS/1000)%10;
	                     lcd_char_data[1][1]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS/100)%10;   
	                     lcd_char_data[1][2]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS/10)%10;  
	                     lcd_char_data[1][3]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS)%10;         
	                     
	                     
	                     // Z AXIS   
	                     lcd_char_data[2][0]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS/1000)%10;
	                     lcd_char_data[2][1]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS/100)%10;   
	                     lcd_char_data[2][2]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS/10)%10;  
	                     lcd_char_data[2][3]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS)%10;         


			   	/* SHOW X axis DATA */                    
	            		lcd_gotoxy(0,0);
	            		lcd_putsf("X axis:");     
	            		if(AK09911C5_XYZ_DATA.X_AXIS<0)
	            		{
	            			lcd_putchar(45);		/* LCD show "-"  mean negative */
				}                             
	           		else
				{
					lcd_putchar(32);		/* LCD show " "  mean positive */
				}   
					
				  /* show X axis data on LCD */
		                lcd_putchar(48+lcd_char_data[0][0]);
		                lcd_putchar(48+lcd_char_data[0][1]);
		                lcd_putchar(48+lcd_char_data[0][2]);
		                lcd_putchar(48+lcd_char_data[0][3]);

						
				/* SHOW Y axis DATA */    
	          		 lcd_gotoxy(0,1);
	           		 lcd_putsf("Y axis:"); 
	             		if(AK09911C5_XYZ_DATA.Y_AXIS<0)
	           		{
	           			lcd_putchar(45);		/* LCD show "-"  mean negative */
				}                             
	            		else
				{
					lcd_putchar(32);		/* LCD show " "  mean positive */
				}   


				/* show Y axis data on LCD */
	                 	lcd_putchar(48+lcd_char_data[1][0]);
	                	lcd_putchar(48+lcd_char_data[1][1]);
	                 	lcd_putchar(48+lcd_char_data[1][2]);
	                 	lcd_putchar(48+lcd_char_data[1][3]);
	                    
	  
				/* SHOW Z axis DATA */    
	            		lcd_gotoxy(0,2);
	            		lcd_putsf("Z axis:"); 
	           		if(AK09911C5_XYZ_DATA.Z_AXIS<0)
	            		{
	            			lcd_putchar(45);
				}                             
	            		else
				{
					lcd_putchar(32);
				}

									   
				/* show Y axis data on LCD */
	                 	lcd_putchar(48+lcd_char_data[2][0]);
	                 	lcd_putchar(48+lcd_char_data[2][1]);
	                 	lcd_putchar(48+lcd_char_data[2][2]);
	                 	lcd_putchar(48+lcd_char_data[2][3]);
                    


							
				}
			delay_ms(120);			//delay 100 ms 
			
#if EXAMPLE_AK09911C_INIFITE_READ == 1
			}
#endif
		
                printf("-------------------- E-COMPASS AK09911C --------------------\r\n");
}


#endif		//#ifndef __AK09911C_EXAMPLE__

