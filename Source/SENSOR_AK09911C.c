/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: E-COMPASS
     File Name		: SENSOR_AK09911C.c
     Function		: SENSOR_AK09911C
     Create Date	: 2017/11/27
---------------------------------------------------------------------- */

/* Note : RETN must be connect to Vcc , avoid reset.*/
#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_AK09911C.h>


AK09911C5_ASA_DATA	 AK09911C_ASA_XYZ_DATA;	/*asa x,y,z reg data  ;  initial read ASA register & write in */


/*------------------------------- get the 1,2,3&4 quadrant RAW DATA -------------------------------*/ 
#define AK09911C_USE_RANDOM_COLLECT_DATA		(0)			/*if "1" -> use random way to collect raw data ; if "0" -> find fix angle to collect raw data */


static CHAR8U first_quadrant_cnt=0,second_quadrant_cnt=0,third_quadrant_cnt=0,fourth_quadrant_cnt=0;

#if(AK09911C_USE_RANDOM_COLLECT_DATA==1)

#define AK09911C_data_size 								(50)		/* 50 (x,y) point */
	static INT16S first_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' points in first_quadrant_data*/ 
	static INT16S second_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' points in second_quadrant_data*/ 
	static INT16S third_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' points in third_quadrant_data*/ 
	static INT16S fourth_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' points in fourth_quadrant_data*/ 

#elif(AK09911C_USE_RANDOM_COLLECT_DATA==0) /* use fix angle to obtain a good round shape */

	/* unit : 2.5 degree */
#define AK09911C_data_size 								(36)		/* 36 (x,y) point */

#define AK0911C_min_get_point_num						(15)		/* the min get the point data number will pass */

	static INT16S first_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' in first_quadrant_data*/ 
	static INT16S second_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' in second_quadrant_data*/ 
	static INT16S third_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' in third_quadrant_data*/ 
	static INT16S fourth_quadrant_data[2][AK09911C_data_size]={0};		/* get (x,y) 'AK09911C_data_size points' in fourth_quadrant_data*/ 

	/* 90degree / 2.5degree = 36 equal parts thus need 5 CHAR8U variable to record */
	static CHAR8U first_fix_angle_flag[5]={0};		
	static CHAR8U second_fix_angle_flag[5]={0};	
	static CHAR8U third_fix_angle_flag[5]={0};	
	static CHAR8U fourth_fix_angle_flag[5]={0};	

#endif
/*------------------------------- get the 1,2,3&4 quadrant RAW DATA -------------------------------*/ 


/********************************************** SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*
	READ AK09911C ID & INFO  
	id_info -> 4 data   ; Read Reg 0x00 ~0x03
	if read fail -> return -1
*/
CHAR8S AK09911C_GET_ID_INFO(CHAR8U *id_info)
{
	CHAR8U read_data[4] = {0};
	CHAR8S status = 0;
		
		i2c_stop_hang();

		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_WAI1,4,&read_data[0]);
		if(status !=1) 
		{
			return -1;	/*read fail*/
		}
		
		id_info[0] = read_data[0];		/*AK09911C_REG_WAI1 ; value = 0x48*/
		id_info[1] = read_data[1];		/*AK09911C_REG_WAI2 ; value = 0x05*/
		id_info[2] = read_data[2];		/*AK09911C_REG_INFO1*/
		id_info[3] = read_data[3];		/*AK09911C_REG_INFO2*/
		
		return 0;
	
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* initial AK09911C */
CHAR8S AK09911C_SET_INITIAL(void)
{
	CHAR8S status = 0;
	
		i2c_stop_hang();
		
		/*step1 reset the AK09911C*/
		status = AK09911C_SET_RESET();
		if(status !=0) 
		{
			return -1; /* reset fail*/
		}
		
		delay_ms(50);
		i2c_stop_hang();

		/*step2 set the system mode*/
		status = AK09911C_SET_SYSTEM_MODE(AK09911C_POWER_DOWN_MODE);
		if(status !=0) 
		{
			return -1;	/* set the system mode fail*/
		}
		delay_ms(2);
		
		i2c_stop_hang();
		
		/*step get XYZ-axis sensitivity adjustment value Fuse ROM*/
		status = AK09911C_GET_ASA_DATA(&AK09911C_ASA_XYZ_DATA);
		if(status !=0) 
		{
			return -1;	/*get ASA data fail*/
		}
		
		return 0 ;	/*initial the AK09911C success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*reset the AK09911C*/
CHAR8S AK09911C_SET_RESET(void)
{
	CHAR8S status = 0;
		
		i2c_stop_hang();		
		i2c_stop_hang();
		
		status = i2c_write_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL3,AK09911C_SW_RESET);
		if(status !=1)
		{
			return -1; /*read fail*/
		}
		
		return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set AK09911C  power down mode  */
CHAR8S AK09911C_SET_POWER_DOWN_MODE(void)
{

	CHAR8S status = 0;
	CHAR8U write_data = AK09911C_POWER_DOWN_MODE & 0x1F; /*mask : 0x1F*/

		i2c_stop_hang();
		i2c_stop_hang();
		
		status = i2c_write_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL2,write_data);
		if(status !=1) 
		{
			return -1;/* set the mode fail*/
		}
		return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*  AK09911C */
/* mode : use enum AK09911C_MODE_SELECT*/
CHAR8S AK09911C_SET_SYSTEM_MODE(CHAR8U mode)
{
	CHAR8S status = 0;
	CHAR8U write_data=mode & 0x1F;	/*mask : 0x1F*/
		
             // printf(" SET_SYSTEM_MODE = 0x%x\r\n",write_data);
			  
		i2c_stop_hang();
		i2c_stop_hang();	
		i2c_stop_hang();
	
		status = i2c_write_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL2,write_data);
		if(status !=1) 
		{  
			return -1;	/* set the mode fail*/
		}
		
		return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* 
	GET AK09911C mode 
	AK09911C_POWER_DOWN_MODE=(0x0<<0),		// Power-down mode
	AK09911C_SINGLE_MODE=(0x01<<0),			// Single measurement mode
	AK09911C_CONTI_MODE_1=(0x01<<1),			// Continuous measurement mode 1
	AK09911C_CONTI_MODE_2=(0x01<<2),			// Continuous measurement mode 2		
	AK09911C_CONTI_MODE_3=(0x03<<2),			// Continuous measurement mode 3	
	AK09911C_CONTI_MODE_4=(0x01<<3),			// Continuous measurement mode 4	
	AK09911C_SELF_TEST=(0x01<<4),				// Self-test mode
	AK09911C_FUSE_ROM=(0x1F)					//Fuse ROM access mode
*/
CHAR8S AK09911C_GET_MODE(CHAR8U *mode)
{
	CHAR8U read_data = 0;
	CHAR8S status = 0;

		i2c_stop_hang();	
		i2c_stop_hang();
		
		status = i2c_read_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL2,&read_data);		/* read mode*/
		if(status !=1) 
		{
			return -1; /* read fail*/
		}

			  printf("AK09911C_GET_MODE = 0x%x\r\n",read_data);
			  
		if(read_data == AK09911C_POWER_DOWN_MODE)/* power-down mode*/
		{
			*mode = AK09911C_POWER_DOWN_MODE;
			 return 0;
		}
		else if(read_data == AK09911C_SINGLE_MODE)/* single measurement mode*/
		{
			*mode = AK09911C_SINGLE_MODE;
			 return 0;			
		}
		else if(read_data == AK09911C_CONTI_MODE_1)/*  Continuous measurement mode 1*/
		{
			*mode = AK09911C_CONTI_MODE_1;
			 return 0;
		}
		else if(read_data == AK09911C_CONTI_MODE_2)/*  Continuous measurement mode 2*/		
		{
			*mode = AK09911C_CONTI_MODE_2;
			  return 0;
		}
		else if(read_data == AK09911C_CONTI_MODE_3)/*  Continuous measurement mode 3*/	
		{
			*mode = AK09911C_CONTI_MODE_3;
			 return 0;
		}
		else if(read_data == AK09911C_CONTI_MODE_4)/*  Continuous measurement mode 4*/	
		{
			*mode = AK09911C_CONTI_MODE_4;
			 return 0;
		}
		else if(read_data == AK09911C_SELF_TEST)/*  Self-test mode*/
		{
			*mode = AK09911C_SELF_TEST;
			 return 0;
		}	
		else if(read_data == AK09911C_FUSE_ROM)/* Fuse ROM access mode*/
		{
			*mode = AK09911C_FUSE_ROM;
			 return 0;
		}					
		else 
		{
			/*  nothing.*/
		}
		
		return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* AK09911C */
/* read the Data status  [reg 0x10] */
CHAR8S AK09911C_GET_DATA_READY_STATUS(CHAR8U *dataready_status)
{

	CHAR8S status = 0;
	CHAR8U read_data=0;

		i2c_stop_hang();

		status = i2c_read_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_ST1,&read_data);
		if(status !=1) 
		{
			return -1; /* read status 1 fail*/
		}
		
		*dataready_status = read_data;
		return 0 ; /* read stats 1 success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* get AK09911C the overflow status [bit3]*/
/* *** when any of measurement data is read, be sure to read ST2 register at the end.*/
CHAR8S AK09911C_GET_OVER_FLOW_STATUS(CHAR8U *overflow_status)
{

	CHAR8S status = 0;
	CHAR8U read_data=0;

		i2c_stop_hang();
		i2c_stop_hang();
		
		status = i2c_read_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_ST2,&read_data);
		if(status !=1) 
		{
			return -1; /*read status 2  fail*/
		}
		
		*overflow_status = read_data;
		return 0 ; /*read stats 2  success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*get  AK09911C  XYZ axis Magnetic Data*/
CHAR8S AK09911C_GET_XYZ_DATA(AK09911C_AXIS_DATA *raw_data)
{

	CHAR8U read_data[6] = {0};
	CHAR8S status = 0;
	CHAR8U data_ready =0,over_flow=0;	

		i2c_stop_hang();


		status = AK09911C_SET_SYSTEM_MODE(AK09911C_SINGLE_MODE);
		if(status !=0)
		{
			return -1; /* set the system mode fail*/	
		}
		
		i2c_stop_hang();
		
		delay_ms(50);
		
		status = AK09911C_GET_DATA_READY_STATUS(&data_ready);
		if(status !=0) 
		{
			return -1;  /* read fail*/	
		}


		i2c_stop_hang();
		//printf("data_ready = 0x%x\r\n",data_ready);
		
		/* check status   no skipped data"0"in [bit1]   &   data ready"1"in [bit0]) */ 
		if(data_ready & 0x03 != 0x03)  return -3;	/* data not ready or skipped data*/
		
		
		i2c_stop_hang();

		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_HXL,6,&read_data[0]);
		if(status !=1) 
		{
			return -1; /* read fail*/
		}
		
		raw_data->X_AXIS=(INT16U)(read_data[1]<<8) + (INT16U)read_data[0];		//Measurement Magnetic Data - X axis Data */
		raw_data->Y_AXIS=(INT16U)(read_data[3]<<8) + (INT16U)read_data[2];		//Measurement Magnetic Data - Y axis Data */
		raw_data->Z_AXIS=(INT16U)(read_data[5]<<8) + (INT16U)read_data[4];		//Measurement Magnetic Data - Z axis Data */
		
		i2c_stop_hang();

		/** when any of measurement data is read, be sure to read ST2 register at the end.*/
		status = AK09911C_GET_OVER_FLOW_STATUS(&over_flow);
		if(status !=0) 
		{
			return -1; /* read fail*/
		}
		
		i2c_stop_hang();
		
		/*check overflow */
		if(over_flow & 0x08) 
		{
			return -4; /*data overflow!! */
		}
			
		return 0;

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* get the AK09911C  XYZ-axis sensitivity adjustment value*/
CHAR8S  AK09911C_GET_ASA_DATA(AK09911C5_ASA_DATA *asa_data)
{
	CHAR8U read_data[3] = {0};
	CHAR8S status = 0;
		
		i2c_stop_hang();

		status = AK09911C_SET_SYSTEM_MODE(AK09911C_FUSE_ROM);	/*set FUSE_ROM mode  to READ ASA data*/
		if(status !=0) 
		{
			return -1; 	/*set the system mode fail*/
		}

		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_ASAX,3,&read_data[0]);
		if(status !=1) 
		{
			return -1;	/*read fail*/
		}
		
		asa_data->ASA_X = read_data[0];		/*X-axis sensitivity adjustment value*/	
		asa_data->ASA_Y = read_data[1];		/*Y-axis sensitivity adjustment value*/	
		asa_data->ASA_Z = read_data[2];		/*Z-axis sensitivity adjustment value*/	


		printf("AK09911C ASA_X = 0x%x\r\n",AK09911C_ASA_XYZ_DATA.ASA_X);
		printf("AK09911C ASA_Y = 0x%x\r\n",AK09911C_ASA_XYZ_DATA.ASA_Y);
		printf("AK09911C ASA_Z = 0x%x\r\n",AK09911C_ASA_XYZ_DATA.ASA_Z);

		status = AK09911C_SET_SYSTEM_MODE(AK09911C_POWER_DOWN_MODE); 		/*After read out the ROM data -> power down mode*/
		if(status !=0) 
		{
			return -1; /*set the system mode fail	*/
		}
		
		return 0;
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	calculate the XYZ data of AK09911C  XYZ axis & XYZ ASA data
	Sensitivity adjustment data for each axis is stored to fuse ROM on shipment.
	** See AK09911C Spec page.30

	Hadj -> adjusted measurement data.
	H 	 -> the measurement data read out from the measurement data register.
	ASA  -> REG_ASAX,Y,Z data. 

	Hadj = H *(      (ASA + 128 ) / 128 )

	H is in the range of -8190 to 8190.  The magnetometer has a range of
	+4912uT & -4912uT .  To go from the raw value to uT is:

	HuT = H * 4912/8190, or  H*6/10.

	Since 1uT = 100 gauss, our final scale factor becomes:

	Hadj = H * ((ASA + 128) / 128) * 6/10 * 100

	Hadj = H * ((ASA + 128) * 60 / 128
*/
void AK09911C_GET_CAL(AK09911C_AXIS_DATA *data)
{
	INT16S temp=0;
	FLOAT temp2=0;    
		
	       data->X_AXIS = (data->X_AXIS *6 /10);
	       data->Y_AXIS = (data->Y_AXIS *6 /10);
	       data->Z_AXIS = (data->Z_AXIS *6 /10); 
		
		/*X axis*/
		temp2 = (FLOAT)data->X_AXIS *(((FLOAT)AK09911C_ASA_XYZ_DATA.ASA_X/128)+1);
		data->X_AXIS = (INT16S)temp2;

		/*Y axis*/
		temp2 = (FLOAT)data->Y_AXIS *(((FLOAT)AK09911C_ASA_XYZ_DATA.ASA_Y/128)+1);
		data->Y_AXIS = (INT16S)temp2;

		/*Z axis*/
		temp2 = (FLOAT)data->Z_AXIS *(((FLOAT)AK09911C_ASA_XYZ_DATA.ASA_Z/128)+1);
		data->Z_AXIS = (INT16S)temp2;		


}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	step 1. Set Power-down mode. (MODE[4:0]=¡§00000¡¨)
	step 2. Set Self-test mode. (MODE[4:0]=¡§10000¡¨)
	step 3. Check Data Ready or not by polling DRDY bit of ST1 register ; When Data Ready, proceed to the next step.
	step 4. Read measurement data (HXL to HZH)
*/
CHAR8S AK09911C_SET_SELF_TEST(AK09911C_AXIS_DATA *data)
{
	CHAR8S status = 0,status1 = 0;
	CHAR8U count = 0,read_data_ready_status=0,read_data_overflow_status=0;	
	CHAR8U raw_data[6] = {0};
		
		i2c_stop_hang();	
		i2c_stop_hang();	
		

		/*step 1. Set Power-down mode. (MODE[4:0]=¡§00000¡¨)*/
		/*write AK09911C_POWER_DOWN_MODE. */
		status = AK09911C_SET_POWER_DOWN_MODE();
		if(status != 0 )
		{
			return -1; /* write fail.*/		
		}
		delay_ms(1);

		i2c_stop_hang();	
		
		/*step 2. Set Self-test mode. (MODE[4:0]=¡§10000¡¨)*/
		/*write AK09911C_SELF_TEST. */
		status = AK09911C_SET_SYSTEM_MODE(AK09911C_SELF_TEST);
		if(status != 0 )
		{
			return -1; /* write fail.*/		
		}

		
		/*step 3. Check Data Ready or not by polling DRDY bit of ST1 register ; When Data Ready, proceed to the next step.*/
		for(count = 0 ; count < 8 ; count ++)	/*retry 8 times */
		{
			AK09011C_DELAY(0xFF0);
		
			status = AK09911C_GET_DATA_READY_STATUS(&read_data_ready_status);		/*check RDRY , DOR bit*/

			if(status == 0)
			{
				/*data ready & no error*/
				if(read_data_ready_status & 0x03 == 0x03)break;	/* no skipped data"0"in [bit1]   &   data ready"1"in [bit0]*/
					
			}
			else if(status == -1 )
			{
				/*read status fail*/
				status1 = -1;
			}
		}

		/*step 4. Read measurement data (HXL to HZH)*/

		i2c_stop_hang();
		i2c_stop_hang();
		/*read x,y,z axis raw data*/
		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_HXL,6,&raw_data[0]);
		if(status !=1) 
		{
			return -1; /*read fail*/
		}
		
		i2c_stop_hang();

		/*when any of measurement data is read, be sure to read ST2 register at the end.*/
		status = AK09911C_GET_OVER_FLOW_STATUS(&read_data_overflow_status);
		if(status !=0) 
		{
			return -1; /* read fail*/
		}
		
		/*check overflow*/
		if(read_data_overflow_status & 0x08) 
		{
			return -3; /* data overflow!!*/
		}

		/*** SET  AK09911C_POWER_DOWN_MODE*/
		i2c_stop_hang();
		i2c_stop_hang();
		
		/*write AK09911C_POWER_DOWN_MODE.*/ 
		status = AK09911C_SET_POWER_DOWN_MODE();
		if(status != 0 )
		{
			return -1; /* write fail.*/	
		}

		data->X_AXIS =     (raw_data[1]<<8)   + raw_data[0];
		data->Y_AXIS =     (raw_data[3]<<8)   + raw_data[2];		
		data->Z_AXIS =     (raw_data[5]<<8)   + raw_data[4];	

		AK09911C_GET_CAL(data);	
			
		return 0; /* read success ok!*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*for AK09011C delay*/
void AK09011C_DELAY(INT32U count)
{
	INT32U delay_cnt = 0;
	CHAR8U delay = 0 ;
	
		for(delay_cnt=0;delay_cnt<=count;delay_cnt++)
		{
			delay ++;
			delay --;
		}
}
/*--------------------------------------------------------------------------------------------------*/

/********************************************** SYSTEM **************************************************/





/********************************************** ADVANCED APPLICATIONS **************************************************/
/*------------------------------------------------------------------------------------------------------*/
/*
	find the calibrate factor & record the factor to calibrate X,Y,Z data.
	need to rotate e-compass 360 degree C or 720 degree C slowly.
	
				    (+y)
					|
					|
     second quadrant	|  first quadrant
					|
					|			
    ( -x ) --------------------------- (+x)
					|
					|
	third quadrant		|  fourth quadrant
					|
					|
				     ( -y)

	get the 1,2,3&4 quadrant raw data 

	the final result save in
	
		first_quadrant_data[2][AK09911C_data_size]
		second_quadrant_data[2][AK09911C_data_size]
		third_quadrant_data[2][AK09911C_data_size]
		fourth_quadrant_data[2][AK09911C_data_size]
		
*/
CHAR8U AK09011C_GET_CALIBRATE_DATA(AK09911C_AXIS_DATA data,AK09911C_4_QUADRANT_DATA_POINT *quadrant_ready)
{
	INT16U cnt=0,check_level=0,check_array=0,check_bit=0;
	FLOAT check_raw_data_theta=0;
	FLOAT down_data=0.0,up_data=0.0;
	CHAR8U tiny_cnt=0;
	
		//https://blog.csdn.net/mu399/article/details/81951786
		//https://zh.wikipedia.org/wiki/%E4%B8%89%E8%A7%92%E5%87%BD%E6%95%B0

		/* calculate the theta */
		check_raw_data_theta=atan2(data.Y_AXIS,data.X_AXIS) * 180 /3.14159;	
		if (check_raw_data_theta > 360)
		{
			check_raw_data_theta -= 360;
		}
		
		if (check_raw_data_theta < 0) 
		{
			check_raw_data_theta += 360;
		}
		
		printf("theta = %f C\r\n",check_raw_data_theta);	


#if (AK09911C_USE_RANDOM_COLLECT_DATA==1)	/* use random way collect point */

		if(check_raw_data_theta >=0 && check_raw_data_theta<90)/*first quadrant data*/
		{
			if(first_quadrant_cnt<AK09911C_data_size)
			{
				first_quadrant_data[0][first_quadrant_cnt]=data.X_AXIS;
				first_quadrant_data[1][first_quadrant_cnt]=data.Y_AXIS;
					
				quadrant_ready->first_quadant_data_pointok=0;		
				first_quadrant_cnt++;
			}
			else
			{
				quadrant_ready->first_quadant_data_pointok=1;
			
			}				
		}		
		else if(check_raw_data_theta >=90 && check_raw_data_theta<180)		/*second quadrant data*/
		{		
			if(second_quadrant_cnt<AK09911C_data_size)
			{
				second_quadrant_data[0][second_quadrant_cnt]=data.X_AXIS;
				second_quadrant_data[1][second_quadrant_cnt]=data.Y_AXIS;

				quadrant_ready->second_quadant_data_pointok=0;			
				second_quadrant_cnt++;
			}
			else
			{
				quadrant_ready->second_quadant_data_pointok=1;
			}			
		}
		else if(check_raw_data_theta >=180 && check_raw_data_theta<270)		/*third quadrant data*/
		{	
			if(third_quadrant_cnt<AK09911C_data_size)
			{
				third_quadrant_data[0][third_quadrant_cnt]=data.X_AXIS;
				third_quadrant_data[1][third_quadrant_cnt]=data.Y_AXIS;
				
				quadrant_ready->third_quadant_data_pointok=0;				
				third_quadrant_cnt++;
			}
			else
			{
				quadrant_ready->third_quadant_data_pointok=1;
			}			
		}
		else if(check_raw_data_theta >=270 && check_raw_data_theta<360)		/*fourth quadrant data*/	
		{		
			if(fourth_quadrant_cnt<AK09911C_data_size)
			{
				fourth_quadrant_data[0][fourth_quadrant_cnt]=data.X_AXIS;
				fourth_quadrant_data[1][fourth_quadrant_cnt]=data.Y_AXIS;

				quadrant_ready->fourth_quadant_data_pointok=0;
				fourth_quadrant_cnt++;
			}
			else
			{
				quadrant_ready->fourth_quadant_data_pointok=1;
			}			
		}
		
		printf("1_cnt:%d ; 2_cnt:%d ,3_cnt:%d ,4_cnt:%d\r\n",first_quadrant_cnt,second_quadrant_cnt,third_quadrant_cnt,fourth_quadrant_cnt);

#else

			if(check_raw_data_theta >=0 && check_raw_data_theta<90)/*first quadrant data*/
			{

				printf("1-quadrant %d\r\n",first_quadrant_cnt);
				if(first_quadrant_cnt<AK09911C_data_size)
				{	
					/* 90 / 2.5 = 36 -> 36 point */
					for(cnt=0;cnt<AK09911C_data_size;cnt++) /* 0~35 , total:36 */
					{
						/* calculate the down limit & up limit */
						/* example cnt = 0 ; down_data = 0 , up_data = 2.5 */
						/* example cnt = 1 ; down_data = 2.5 , up_data = 5 */					
						down_data= (FLOAT)cnt * 2.5;	/* 2.5 -> 2.5degree */
						up_data= (FLOAT)(cnt+1) * 2.5;
						//printf("%f ~ %f \r\n",down_data,up_data);
						if(check_raw_data_theta >=  down_data && check_raw_data_theta < up_data)  
						{
							check_level=(INT16U) (down_data/2.5);		/* get where bit must set "1" */	/* example: down_data =85 ; 85/2.5= 34 bit */
							check_array=check_level/8 ;				/* get where the array idx */
							check_bit=check_level%8;				/* get where the array bit*/
							
							printf("check_array %d , check_bit %d\r\n",check_array,check_bit);	
							//printf("pre check 0x%x\r\n",first_fix_angle_flag[check_array] & (1<<check_bit) );	
							/* check angle flag !=0 */ /* if check == 1 , means the angle raw data (X,Y) already have */
							if((first_fix_angle_flag[check_array] & (1<<check_bit)) ==0)	/* the angle dont have a raw data record need to save the raw data*/
							{
								/* set the angle flag as "1" */
								first_fix_angle_flag[check_array] = first_fix_angle_flag[check_array] | (1 <<check_bit);

								for(tiny_cnt=0;tiny_cnt<5;tiny_cnt++)
								{	
									printf("first_fix_angle_flag[%d] 0x%x\r\n",tiny_cnt,first_fix_angle_flag[tiny_cnt] );	
								}
									
								/*save the X,Y point data */
								first_quadrant_data[0][first_quadrant_cnt]=data.X_AXIS;
								first_quadrant_data[1][first_quadrant_cnt]=data.Y_AXIS;
								
								quadrant_ready->first_quadant_data_pointok=0;		
								first_quadrant_cnt++;

								break; /* break the for loop */
								
							}
							else	/* the angle already have raw_data */
							{
								/* do nothing next point check */
									printf("do nothing\r\n");
							}
						}
					}				
				}
				else
				{
					quadrant_ready->first_quadant_data_pointok=1;
				}				
			}
			else if(check_raw_data_theta >=90 && check_raw_data_theta<180)/*second quadrant data*/
			{
				printf("2-quadrant %d\r\n",second_quadrant_cnt);
				if(second_quadrant_cnt<AK09911C_data_size)
				{	
				
					/* 90 / 2.5 = 36 -> 36 point */
					for(cnt=0;cnt<AK09911C_data_size;cnt++) /* 0~35 , total:36 */
					{
						/* calculate the down limit & up limit */
						/* example cnt = 0 ; down_data = 0 , up_data = 2.5 */
						/* example cnt = 1 ; down_data = 2.5 , up_data = 5 */					
						down_data= (FLOAT)cnt * 2.5 +90.0;	/* 2.5 -> 2.5degree */ /*start -> 90 degree */
						up_data=(FLOAT)(cnt+1) * 2.5 +90.0;
						
						if(check_raw_data_theta >=  down_data && check_raw_data_theta < up_data)  
						{
							check_level=(INT16U) (down_data /2.5);		/* get where bit must set "1" */	/* example: down_data =85 ; 85/2.5= 34 bit */
							check_array=check_level/8 ;				/* get where the array idx */
							check_bit=check_level%8;
							
							printf("check_array %d , check_bit %d\r\n",check_array,check_bit);	

							/* check angle flag !=0 */ /* if check == 1 , means the angle raw data (X,Y) already have */
							if((second_fix_angle_flag[check_array] & (1<<check_bit)) ==0)	/* the angle dont have a raw data record need to save the raw data*/
							{
								/* set the angle flag as "1" */
								second_fix_angle_flag[check_array] = second_fix_angle_flag[check_array] | (1 <<check_bit);

								for(tiny_cnt=0;tiny_cnt<5;tiny_cnt++)
								{	
									printf("second_fix_angle_flag[%d] 0x%x\r\n",tiny_cnt,second_fix_angle_flag[tiny_cnt] );	
								}
									
								/*save the X,Y point data */
								second_quadrant_data[0][second_quadrant_cnt]=data.X_AXIS;
								second_quadrant_data[1][second_quadrant_cnt]=data.Y_AXIS;
								
								quadrant_ready->second_quadant_data_pointok=0;		
								second_quadrant_cnt++;
								
								break; /* break the for loop */
								
							}
							else	/* the angle already have raw_data */
							{
								/* do nothing next point check */
							}
						}
					}				
				}
				else
				{
					quadrant_ready->second_quadant_data_pointok=1;
				}				
			}
			else if(check_raw_data_theta >=180 && check_raw_data_theta<270)/*third quadrant data*/
			{
				printf("3-quadrant %d\r\n",third_quadrant_cnt);
				if(third_quadrant_cnt<AK09911C_data_size)
				{	
					/* 90 / 2.5 = 36 -> 36 point */
					for(cnt=0;cnt<AK09911C_data_size;cnt++) /* 0~35 , total:36 */
					{
						/* calculate the down limit & up limit */
						/* example cnt = 0 ; down_data = 0 , up_data = 2.5 */
						/* example cnt = 1 ; down_data = 2.5 , up_data = 5 */					
						down_data=(FLOAT)cnt * 2.5 +180.0;	/* 2.5 -> 2.5degree */ /*start -> 180 degree */
						up_data= (FLOAT)(cnt+1) * 2.5 +180.0;
						
						if(check_raw_data_theta >=  down_data && check_raw_data_theta < up_data)  
						{
							check_level=(INT16U) (down_data /2.5);		/* get where bit must set "1" */	/* example: down_data =85 ; 85/2.5= 34 bit */
							check_array=check_level/8 ;				/* get where the array idx */
							check_bit=check_level%8;
							
							printf("check_array %d , check_bit %d\r\n",check_array,check_bit);	

							/* check angle flag !=0 */ /* if check == 1 , means the angle raw data (X,Y) already have */
							if((third_fix_angle_flag[check_array] & (1<<check_bit)) ==0)	/* the angle dont have a raw data record need to save the raw data*/
							{
								/* set the angle flag as "1" */
								third_fix_angle_flag[check_array] = third_fix_angle_flag[check_array] | (1 <<check_bit);

								for(tiny_cnt=0;tiny_cnt<5;tiny_cnt++)
								{	
									printf("third_fix_angle_flag[%d] 0x%x\r\n",tiny_cnt,third_fix_angle_flag[tiny_cnt] );	
								}
									
								/*save the X,Y point data */
								third_quadrant_data[0][third_quadrant_cnt]=data.X_AXIS;
								third_quadrant_data[1][third_quadrant_cnt]=data.Y_AXIS;
								
								quadrant_ready->third_quadant_data_pointok=0;		
								third_quadrant_cnt++;
								
								break; /* break the for loop */
							}
							else	/* the angle already have raw_data */
							{
								/* do nothing next point check */
							}
						}
					}				
				}
				else
				{
					quadrant_ready->third_quadant_data_pointok=1;
				}				
			}
			else if(check_raw_data_theta >=270 && check_raw_data_theta<360)/*fourth quadrant data*/	
			{
				printf("4-quadrant %d\r\n",fourth_quadrant_cnt);
				if(fourth_quadrant_cnt<AK09911C_data_size)
				{	
					/* 90 / 2.5 = 36 -> 36 point */
					for(cnt=0;cnt<AK09911C_data_size;cnt++) /* 0~35 , total:36 */
					{
						/* calculate the down limit & up limit */
						/* example cnt = 0 ; down_data = 0 , up_data = 2.5 */
						/* example cnt = 1 ; down_data = 2.5 , up_data = 5 */					
						down_data=(FLOAT)cnt * 2.5 +270.0;	/* 2.5 -> 2.5degree */ /*start -> 270 degree */
						up_data= (FLOAT)(cnt+1) * 2.5 +270.0;
						
						if(check_raw_data_theta >=  down_data && check_raw_data_theta < up_data)  
						{
							check_level=(INT16U) (down_data /2.5);		/* get where bit must set "1" */	/* example: down_data =85 ; 85/2.5= 34 bit */
							check_array=check_level/8 ;				/* get where the array idx */
							check_bit=check_level%8;
							
							printf("check_array %d , check_bit %d\r\n",check_array,check_bit);	

							/* check angle flag !=0 */ /* if check == 1 , means the angle raw data (X,Y) already have */
							if((fourth_fix_angle_flag[check_array] & (1<<check_bit)) ==0)	/* the angle dont have a raw data record need to save the raw data*/
							{
								/* set the angle flag as "1" */
								fourth_fix_angle_flag[check_array] = fourth_fix_angle_flag[check_array] | (1 <<check_bit);

								for(tiny_cnt=0;tiny_cnt<5;tiny_cnt++)
								{	
									printf("fourth_fix_angle_flag[%d] 0x%x\r\n",tiny_cnt,fourth_fix_angle_flag[tiny_cnt] );	
								}
									
								/*save the X,Y point data */
								fourth_quadrant_data[0][fourth_quadrant_cnt]=data.X_AXIS;
								fourth_quadrant_data[1][fourth_quadrant_cnt]=data.Y_AXIS;
								
								quadrant_ready->fourth_quadant_data_pointok=0;		
								fourth_quadrant_cnt++;
								
								break; /* break the for loop */
							}
							else	/* the angle already have raw_data */
							{
								/* do nothing next point check */
							}
						}
					}				
				}
				else
				{
					quadrant_ready->fourth_quadant_data_pointok=1;
				}				
			}
#endif


		if(first_quadrant_cnt>=AK0911C_min_get_point_num && second_quadrant_cnt>=AK0911C_min_get_point_num \
			&& third_quadrant_cnt>=AK0911C_min_get_point_num && fourth_quadrant_cnt>=AK0911C_min_get_point_num)
	//	if(first_quadrant_cnt==AK09911C_data_size && second_quadrant_cnt==AK09911C_data_size && third_quadrant_cnt==AK09911C_data_size && fourth_quadrant_cnt==AK09911C_data_size)
		{	
			/* display data to UART */
			#if 1
			printf("===================================\r\n");
			for(cnt=0;cnt<first_quadrant_cnt;cnt++)
			{
				printf("%d %d\r\n",first_quadrant_data[0][cnt],first_quadrant_data[1][cnt]);
			}
			for(cnt=0;cnt<second_quadrant_cnt;cnt++)
			{
				printf("%d %d\r\n",second_quadrant_data[0][cnt],second_quadrant_data[1][cnt]);
			}
			for(cnt=0;cnt<third_quadrant_cnt;cnt++)
			{
				printf("%d %d\r\n",third_quadrant_data[0][cnt],third_quadrant_data[1][cnt]);
			}		
			for(cnt=0;cnt<fourth_quadrant_cnt;cnt++)
			{
				printf("%d %d\r\n",fourth_quadrant_data[0][cnt],fourth_quadrant_data[1][cnt]);
			}

			printf("===================================\r\n");
			#endif	
			
			return 1;	/*final collect data ok! */
		}
	
		return -1;	/*the raw data no ready */



}
/*------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/
/* 
	get calibrate factor the e-compass , eliminate Hard-iron & Soft-iron.

	use the final result save in
	 
		first_quadrant_data[2][AK09911C_data_size]		-> first_quadrant_data[0][AK09911C_data_size] as X-axis data ; first_quadrant_data[1][AK09911C_data_size] as Y-axis data
		second_quadrant_data[2][AK09911C_data_size]
		third_quadrant_data[2][AK09911C_data_size]
		fourth_quadrant_data[2][AK09911C_data_size]

	
	calculate the Xmax , Xmin , Ymax , Ymin ,Zmin ,Zmax find the Xoffset,Yoffset ,Zoffset.
			xoffset = (xmax + xmin)/2;
			yoffset = (ymax + ymin)/2;
			xgain =1;
			ygain = (final_x_max - final_x_min) / (final_y_max - final_y_min);

*/
CHAR8U AK09911C_GET_CALIBRATE_FACTOR(AK09911C_CALIBRATE *cal_result)
{
	INT16S x_max[4]={0},x_min[4]={0},y_max[4]={0},y_min[4]={0};	/*example x_max[0] -> first_quadrant Xmax ; x_max[1] -> second_quadrant Xmax ... etc */
	INT16S final_x_max=0,final_x_min=0,final_y_max=0,final_y_min=0;
	CHAR8U cnt =0;
	
	FLOAT x_offset=0.0,y_offset=0.0;
	FLOAT x_gain=0.0, y_gain=0.0;

	
		/* find x_max & y_min */
		for(cnt=0;cnt<AK09911C_data_size;cnt++)
		{
			/* find X max part  */
			if(first_quadrant_data[0][cnt]>x_max[0])
			{
				x_max[0]=first_quadrant_data[0][cnt];
			}

			if(second_quadrant_data[0][cnt]>x_max[1])
			{
				x_max[1]=second_quadrant_data[0][cnt];
			}
	
			if(third_quadrant_data[0][cnt]>x_max[2])
			{
				x_max[2]=third_quadrant_data[0][cnt];
			}
	
			if(fourth_quadrant_data[0][cnt]>x_max[3])
			{
				x_max[3]=fourth_quadrant_data[0][cnt];
			}

			/* find X min part  */
			if(first_quadrant_data[0][cnt]<=x_min[0])
			{
				x_min[0]=first_quadrant_data[0][cnt];
			}

			if(second_quadrant_data[0][cnt]<=x_min[1])
			{
				x_min[1]=second_quadrant_data[0][cnt];
			}
	
			if(third_quadrant_data[0][cnt]<=x_min[2])
			{
				x_min[2]=third_quadrant_data[0][cnt];
			}
	
			if(fourth_quadrant_data[0][cnt]<=x_min[3])
			{
				x_min[3]=fourth_quadrant_data[0][cnt];
			}


			/* find Y max part  */
			if(first_quadrant_data[1][cnt]>y_max[0])
			{
				y_max[0]=first_quadrant_data[1][cnt];
			}

			if(second_quadrant_data[1][cnt]>y_max[1])
			{
				y_max[1]=second_quadrant_data[1][cnt];
			}
	
			if(third_quadrant_data[1][cnt]>y_max[2])
			{
				y_max[2]=third_quadrant_data[1][cnt];
			}
	
			if(fourth_quadrant_data[1][cnt]>y_max[3])
			{
				y_max[3]=fourth_quadrant_data[1][cnt];
			}
			
			/* find Y min part  */
			if(first_quadrant_data[1][cnt]<=y_min[0])
			{
				y_min[0]=first_quadrant_data[1][cnt];
			}

			if(second_quadrant_data[1][cnt]<=y_min[1])
			{
				y_min[1]=second_quadrant_data[1][cnt];
			}
	
			if(third_quadrant_data[1][cnt]<=y_min[2])
			{
				y_min[2]=third_quadrant_data[1][cnt];
			}
	
			if(fourth_quadrant_data[1][cnt]<=y_min[3])
			{
				y_min[3]=fourth_quadrant_data[1][cnt];
			}
						
		}	

		/* find final x_max & y_min */
		for(cnt=0;cnt<4;cnt++)
		{
			/*final find Xmax,min part */
			if(x_max[cnt]>final_x_max)
			{
				final_x_max=x_max[cnt];
			}
			
			if(x_min[cnt]<final_x_min)
			{
				final_x_min=x_min[cnt];
			}

			/*final find Ymax,min part */
			if(y_max[cnt]>final_y_max)
			{
				final_y_max=y_max[cnt];
			}
			
			if(y_min[cnt]<final_y_min)
			{
				final_y_min=y_min[cnt];
			}

		}

		/* get the Xoffset & Yoffset */
		x_offset =	( (FLOAT)final_x_max + (FLOAT)final_x_min ) /2 ;
		y_offset =	( (FLOAT)final_y_max + (FLOAT)final_y_min ) /2 ;

		/* set the x_gain as 1 , and y_gain = (final_x_max - final_x_min) / (final_y_max - final_y_min) */
		x_gain=	1.0;
		y_gain= ( (FLOAT)final_x_max - (FLOAT)final_x_min ) /( (FLOAT)final_y_max - (FLOAT)final_y_min ) ;


		/* get the final calibrate factor */
		cal_result->X_OFFSET =x_offset;	
		cal_result->Y_OFFSET =y_offset;			
		cal_result->X_FACTOR_BASE=x_gain;	
		cal_result->Y_FACTOR_BASE=y_gain;	

		
		printf("final  X_offset %f ; Y_offset %f\r\n",cal_result->X_OFFSET,cal_result->Y_OFFSET);
		printf("final  X_factor %f ; Y_factor %f\r\n",cal_result->X_FACTOR_BASE,cal_result->Y_FACTOR_BASE);

		return 1;	/* find the calibrate factor */
		
}
/*------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/
/* simple calculate */
/* calculate the AK09011C azimuth*/
/* return degree */
/* the coordinates is 

			y
			^
			|     /
                   	|    /
			|   /  
                   	|  /		
			| /     degree
                   	|/  
			------------> x

     and point to South
*/
/* this API without pitch & roll compensation */
FLOAT AK09011C_GET_AZIMUTH_WITHOUT_COMPENSATION(AK09911C_AXIS_DATA data)
{
	FLOAT x_data,y_data,result_data,cal;
	
		/* calculate the azimuth */
		result_data =atan2((FLOAT)data.Y_AXIS,(FLOAT)data.X_AXIS)*180.0/3.14159 +180.0;			
		return result_data;
}
/*------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/
/* simple calculate */
/* calculate the AK09011C azimuth*/
/* return degree */
/* the coordinates is 

			y
			^
			|     /
                   	|    /
			|   /  
                   	|  /		
			| /     degree
                   	|/  
			------------> x

     and point to South
*/
/* this API with pitch & roll compensation */
/* the pitch & roll angle data from other 3-Axis Accelerometer. example : KXSD9 , LIS3DH , ADXL345 , MMA8452 , LSM303DLHC..etc*/
FLOAT AK09011C_GET_AZIMUTH_WITH_COMPENSATION(AK09911C_AXIS_DATA data,FLOAT pitch,FLOAT roll)
{

		return 0.0;
}
/*------------------------------------------------------------------------------------------------------*/
/********************************************** ADVANCED APPLICATIONS **************************************************/
