/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: E-COMPASS
     File Name		: EXAMPLE_AK09911C.c
     Function		: EXAMPLE_AK09911C
     Create Date	: 2017/11/27
---------------------------------------------------------------------- */

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <alcd.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_AK09911C.h>

/* Note : RETN must be connect to Vcc , avoid reset. */


#define AK09911C_SELF_TEST_D 				(0)		/* set "1" -> enable self test ; "0" -> disable self test */
#define AK09911C_GET_RAW_DATA_ONLY		(0)		/* enable print data to UART TX only */
#define AK09911C_ANGLE_COMPSATION		(0)		/* if set "1" -> the e-compass WITH angle compensation ; set "0" -> the e-compass WITHOUT angle compensation  */
#define AK09911C_CALBRATE_SEQUENCE		(1)		/* if set "1" -> start calibrate test sequence ; set "0" disable calibrate test sequence. */


/*------- Define AK09911C variable ------*/

AK09911C_MODE_SELECT AK09911C_MODE;				/*mode*/

extern AK09911C5_ASA_DATA  AK09911C_ASA_XYZ_DATA;	/*asa x,y,z reg data    ;  initial read ASA register & write in .*/

AK09911C_AXIS_DATA AK09911C5_XYZ_DATA;				/*read x,y,z raw data*/

AK09911C_CALIBRATE AK09911C_CALBRATE_DATA;			/* calbrate data */

AK09911C_4_QUADRANT_DATA_POINT	AK09911C_QUADRANT_POINT_READY_FLAG;		/*check 4-quadrant data ready ? */
/* 
the pitch & roll angle data from other 3-Axis Accelerometer. 
example : KXSD9 , LIS3DH , ADXL345 , MMA8452 , LSM303DLHC..etc
*/
FLOAT pitch_angle=0.0, roll_angle=0.0;		

/*------- Define AK09911C variable ------*/


FLOAT test_data;
void  EXAMPLE_AK09911C(void);

void  EXAMPLE_AK09911C(void)
{

	CHAR8U id_data,info_data[4]={0},check_mode;
	CHAR8S status = 0 ;
    	CHAR8U lcd_char_data[3][4]={0}; 
	FLOAT x_data,y_data,result_data,cal;
	CHAR8U	lcd_char_angle_data[5]={0};	
		
		printf("-------------------- E-COMPASS AK09911C --------------------\r\n");
		/*E-COMPASS AK09911C ;*/


		/*read AK09911C ID & INFO*/
		status = AK09911C_GET_ID_INFO(&info_data[0]);
		if(status != 0) 
		{
			printf("read AK09911C ID fail!\r\n");
		}
		else
		{
			printf("read AK09911C ID = INFO[0] = 0x%x \r\n",info_data[0]);
			printf("read AK09911C ID = INFO[1] = 0x%x \r\n",info_data[1]);
			printf("read AK09911C ID = INFO[2] = 0x%x \r\n",info_data[2]);
			printf("read AK09911C ID = INFO[3] = 0x%x \r\n",info_data[3]);
		}

			
		/*first initial AK09911C :reset AK09911C ; read ASA reg ; SET single mode ;*/
		status = AK09911C_SET_INITIAL();
		if(status != 0)
		{
			printf("initial AK09911C fail!\r\n");
		}
		else
		{
			printf("initial AK09911C OK!\r\n");	
		}



#if (AK09911C_SELF_TEST_D==1)
		/*
		CHECK AUTO TEST , CHECK IC WORKING NORMALLY.
		 -30 <= x <= +30
		 -30 <= y <= +30
		 -400 <= z <= -50
		*/

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

			
		/*read AK09911C mode -> check AK09911C in power down mode*/
		status = AK09911C_GET_MODE(&check_mode);
		if(status != 0) 
		{
			printf("read AK09911C mode fail!\r\n");
		}
		else  
		{
			printf("read AK09911C mode ok\r\n");
			if(check_mode == AK09911C_POWER_DOWN_MODE) 
			{
				printf("power down mode\r\n");
			}
			else 
			{
				printf("else mode\r\n");
				while(1); /* stop next sequence*/
			}
		}	



	/* calibrate test sequence */
	#if (AK09911C_CALBRATE_SEQUENCE==1)
	printf("start calibrate test sequence please rotate e-compass!!\r\n");

	/* SHOW start calibrate */    
	lcd_gotoxy(0,0);
	lcd_putsf("START CAL");	
	lcd_gotoxy(0,1);
	lcd_putsf("PLEASE ROTATE");
	lcd_gotoxy(0,2);
	lcd_putsf("E-COMPASS");
	lcd_gotoxy(0,0);		
	
	while(1)
	{
		/*read AK09911C XYZ data.*/
		status = AK09911C_GET_XYZ_DATA(&AK09911C5_XYZ_DATA);
		if(status != 0)
		{
				
		}
		else
		{
			/* get the calibrate factor */
			status=AK09011C_GET_CALIBRATE_DATA(AK09911C5_XYZ_DATA,&AK09911C_QUADRANT_POINT_READY_FLAG);

		#if 0	
			if(AK09911C_QUADRANT_POINT_READY_FLAG.first_quadant_data_pointok!=1)	/* the 1-quadant data is not ready */
			{
				lcd_gotoxy(0,3);
				lcd_putsf("1:N");
			}
			else	/* the 1-quadant data is ready */
			{
				lcd_gotoxy(0,3);
				lcd_putsf("1:K");
			}
			
			if(AK09911C_QUADRANT_POINT_READY_FLAG.second_quadant_data_pointok!=1)	/* the 2-quadant data is not ready */
			{
				lcd_gotoxy(4,3);
				lcd_putsf("2:N");
			}
			else	/* the 2-quadant data is ready */
			{
				lcd_gotoxy(4,3);
				lcd_putsf("2:K");
			}
			
			
			if(AK09911C_QUADRANT_POINT_READY_FLAG.third_quadant_data_pointok!=1)	/* the 3-quadant data is not ready */
			{
				lcd_gotoxy(8,3);
				lcd_putsf("3:N");
			}
			else	/* the 3-quadant data is ready */
			{
				lcd_gotoxy(8,3);
				lcd_putsf("3:K");
			}
					
			
			if(AK09911C_QUADRANT_POINT_READY_FLAG.fourth_quadant_data_pointok!=1)	/* the 4-quadant data is not ready */	
			{
				lcd_gotoxy(12,3);
				lcd_putsf("4:N");
			}
			else	/* the 4-quadant data is ready */
			{
				lcd_gotoxy(12,3);
				lcd_putsf("4:K");
			}
		
		#endif

			
			if(status==1) /* calibrate finish need to break calibrate test sequence */
			{
				break;	/* break the while loop */
			}
		}
		
			

		delay_ms(60);	/* tiny -delay */ /*solwly rotate to detect */
		
	}


	/* calculate the calibrate factor */
	status =AK09911C_GET_CALIBRATE_FACTOR(&AK09911C_CALBRATE_DATA);
	if(status == 1)
	{
		printf("get the AK09911C calibrate factor !!\r\n");
	}
	#endif /* (AK09911C_CALBRATE_SEQUENCE==1) */



	#if	(AK09911C_GET_RAW_DATA_ONLY==1)
	
	lcd_gotoxy(0,0);	
	lcd_putsf("GET RAW DATA");
	lcd_gotoxy(0,1);	
	lcd_putsf("CHECK UART DATA");		


	AK09911C_CALBRATE_DATA.X_OFFSET=1.25;
	AK09911C_CALBRATE_DATA.Y_OFFSET=-21.0;	
	while(1)
	{
		/*read AK09911C XYZ data.*/
		status = AK09911C_GET_XYZ_DATA(&AK09911C5_XYZ_DATA);
		if(status != 0)
		{
				
		}
		else
		{
			/* Calibrate the data */
			/* without X-gain & Y-gain calibrate */
			AK09911C5_XYZ_DATA.X_AXIS =(AK09911C5_XYZ_DATA.X_AXIS -AK09911C_CALBRATE_DATA.X_OFFSET);
			AK09911C5_XYZ_DATA.Y_AXIS =(AK09911C5_XYZ_DATA.Y_AXIS -AK09911C_CALBRATE_DATA.Y_OFFSET);

			#if 0
			/*print x,y,z */
			printf("%d %d %d\r\n",AK09911C5_XYZ_DATA.X_AXIS,AK09911C5_XYZ_DATA.Y_AXIS,AK09911C5_XYZ_DATA.Z_AXIS);
			#endif
			
			#if 1
			/*print x,y only */
			printf("%d %d \r\n",AK09911C5_XYZ_DATA.X_AXIS,AK09911C5_XYZ_DATA.Y_AXIS);
			#endif
			delay_ms(50); /* tiny delay */
		}
	}
	#endif	 /* (AK09911C_GET_RAW_DATA_ONLY==1) */


	lcd_clear();
	delay_ms(1000);	/* tiny -delay */ /*solwly rotate to detect */
	lcd_gotoxy(0,0);	
	lcd_clear();
	delay_ms(1000);	/* tiny -delay */ /*solwly rotate to detect */
	lcd_gotoxy(0,0);

		while(1)
		{
			/*read AK09911C XYZ data.*/
			status = AK09911C_GET_XYZ_DATA(&AK09911C5_XYZ_DATA);
			if(status != 0)
			{
				printf("read AK09911C data fail!\r\n");
			}
			else
			{
			
				/*
					calibrate the raw data 

					formula:
					X_HMC = Xgain *(X_HMC -Xoffset);
					Y_HMC = Ygain * (Y_HMC -Yoffset);

					true code:
					X_H = AK09911C_CALBRATE_DATA.X_FACTOR_BASE *(X_raw_data -AK09911C_CALBRATE_DATA.X_OFFSET);
					Y_H = AK09911C_CALBRATE_DATA.Y_FACTOR_BASE *(Y_raw_data -AK09911C_CALBRATE_DATA.Y_OFFSET);
				*/

				/* with X-gain & Y-gain calibrate */
				//AK09911C5_XYZ_DATA.X_AXIS =AK09911C_CALBRATE_DATA.X_FACTOR_BASE * (AK09911C5_XYZ_DATA.X_AXIS -AK09911C_CALBRATE_DATA.X_OFFSET);
				//AK09911C5_XYZ_DATA.Y_AXIS =AK09911C_CALBRATE_DATA.Y_FACTOR_BASE * (AK09911C5_XYZ_DATA.Y_AXIS -AK09911C_CALBRATE_DATA.Y_OFFSET);

				/* without X-gain & Y-gain calibrate */
				AK09911C5_XYZ_DATA.X_AXIS =(AK09911C5_XYZ_DATA.X_AXIS -AK09911C_CALBRATE_DATA.X_OFFSET);
				AK09911C5_XYZ_DATA.Y_AXIS =(AK09911C5_XYZ_DATA.Y_AXIS -AK09911C_CALBRATE_DATA.Y_OFFSET);
				
				// 1
				//	AK09911C5_XYZ_DATA.X_AXIS = AK09911C5_XYZ_DATA.X_AXIS -2;
				//	AK09911C5_XYZ_DATA.Y_AXIS = AK09911C5_XYZ_DATA.Y_AXIS -(-19.5);

				// 2
				//	AK09911C5_XYZ_DATA.X_AXIS = AK09911C5_XYZ_DATA.X_AXIS -(-3);
				//	AK09911C5_XYZ_DATA.Y_AXIS = AK09911C5_XYZ_DATA.Y_AXIS -(8);



				
				printf("read AK09911C data OK! \r\n");

				printf("X axis = %d \r\n",AK09911C5_XYZ_DATA.X_AXIS);
				printf("Y axis = %d \r\n",AK09911C5_XYZ_DATA.Y_AXIS);		
				printf("Z axis = %d \r\n",AK09911C5_XYZ_DATA.Z_AXIS);	
                   
               
	                     /*X-AXIS*/
	                     lcd_char_data[0][0]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS/1000)%10;
	                     lcd_char_data[0][1]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS/100)%10;   
	                     lcd_char_data[0][2]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS/10)%10;  
	                     lcd_char_data[0][3]=(INT32U)(AK09911C5_XYZ_DATA.X_AXIS)%10;   
	                     
	                     /*Y-AXIS*/   
	                     lcd_char_data[1][0]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS/1000)%10;
	                     lcd_char_data[1][1]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS/100)%10;   
	                     lcd_char_data[1][2]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS/10)%10;  
	                     lcd_char_data[1][3]=(INT32U)(AK09911C5_XYZ_DATA.Y_AXIS)%10;         
	                     
	                     
	                     /*Z-AXIS*/   
	                     lcd_char_data[2][0]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS/1000)%10;
	                     lcd_char_data[2][1]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS/100)%10;   
	                     lcd_char_data[2][2]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS/10)%10;  
	                     lcd_char_data[2][3]=(INT32U)(AK09911C5_XYZ_DATA.Z_AXIS)%10;         


			   	/* SHOW X axis DATA */    
		   #if 1  		
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
				delay_us(80);	
				/* show X axis data on LCD */
				lcd_putchar(48+lcd_char_data[0][0]);
				delay_us(80);	
				lcd_putchar(48+lcd_char_data[0][1]);
				delay_us(80);	
				lcd_putchar(48+lcd_char_data[0][2]);
				delay_us(80);	
				lcd_putchar(48+lcd_char_data[0][3]);
				delay_us(80);	
				delay_ms(10);
   						
				/* SHOW Y axis DATA */    
				lcd_gotoxy(0,1);	
					delay_us(80);	
				lcd_putsf("Y axis:"); 
					delay_us(80);	
	             		if(AK09911C5_XYZ_DATA.Y_AXIS<0)
	           		{
	           			lcd_putchar(45);		/* LCD show "-"  mean negative */
				}                             
	            		else
				{
					lcd_putchar(32);		/* LCD show " "  mean positive */
				}   
					delay_us(80);	

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
                    
	#endif						
			}


			/* calculate the azimuth group */
			{

			#if (AK09911C_ANGLE_COMPSATION == 1)
				/* calculate the azimuth WITH angle (pitch & roll) */
				result_data =AK09011C_GET_AZIMUTH_WITHOUT_COMPENSATION(AK09911C5_XYZ_DATA,pitch_angle,roll_angle);
			#else
				/* simple calculate */
				/* calculate the WITHOUT angle (pitch & roll)  */
				result_data =AK09011C_GET_AZIMUTH_WITHOUT_COMPENSATION(AK09911C5_XYZ_DATA);
			#endif
					
				printf("angle %f  \r\n",result_data);

				/* SHOW Angle DATA */    
				lcd_gotoxy(0,3);			
				lcd_putsf("Ang:"); 
							
				/* calculate azimuth DATA */   
				lcd_char_angle_data[0]=(INT32U)((INT32U)result_data/1000)%10;
				lcd_char_angle_data[1]=(INT32U)((INT32U)result_data/100)%10;   
				lcd_char_angle_data[2]=(INT32U)((INT32U)result_data/10)%10;  
				lcd_char_angle_data[3]=(INT32U)((INT32U)result_data)%10;         
				lcd_char_angle_data[4]=(INT32U)(result_data*10)%10;      

								   
				/* SHOW azimuth DATA */  
				lcd_putchar(48+lcd_char_angle_data[0]);
				lcd_putchar(48+lcd_char_angle_data[1]);			 		
				lcd_putchar(48+lcd_char_angle_data[2]);		
				lcd_putchar(48+lcd_char_angle_data[3]);
				lcd_putchar(46);    /* print "."  */ 	
				lcd_putchar(48+lcd_char_angle_data[4]);						 
		
			}

			
				/* SHOW Position */  
				lcd_gotoxy(12,3);				
				if((result_data < 22.5) || (result_data > 337.5 ))
				{
					lcd_putsf("E  "); 
					printf("\r\nEast\r\n");
				}
				else if((result_data > 22.5) && (result_data < 67.5 ))
				{
					lcd_putsf("N-E");
					printf("\r\nNorth-Eest\r\n");
				}
				else if((result_data > 67.5) && (result_data < 112.5 ))
				{
					lcd_putsf("N  ");
					printf("\r\nNorth\r\n");
				}
				else if((result_data > 112.5) && (result_data < 157.5 ))
				{
					lcd_putsf("N-W");
					printf("\r\nNorth-West\r\n");
				}
				else if((result_data > 157.5) && (result_data< 202.5 ))
				{
					lcd_putsf("W  ");
					printf("\r\nWest\r\n");
				}
				else if((result_data > 202.5) && (result_data < 247.5 ))
				{
					lcd_putsf("S-W");
					printf("\r\nSouth-West\r\n");
				}
				else if((result_data > 247.5) && (result_data < 292.5 ))
				{
					lcd_putsf("S  ");
					printf("\r\nSouth\r\n");
				}
				else if((result_data > 292.5) && (result_data < 337.5 ))
				{
					lcd_putsf("S-E");
					printf("\r\nSouth-East\r\n");
				}		
			
			
			delay_ms(60);	/* tiny delay */
			
		}

                printf("-------------------- E-COMPASS AK09911C --------------------\r\n");
}
