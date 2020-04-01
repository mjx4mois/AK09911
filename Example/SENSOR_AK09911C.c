/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: E-COMPASS
     File Name		: SENSOR_AK09911C.c
     Function		: SENSOR_AK09911C
     Create Date	: 2017/11/27
---------------------------------------------------------------------- */
#ifndef __AK09911C_FUNCTION__ 
#define __AK09911C_FUNCTION__  


// Note : RETN must be connect to Vcc , avoid reset.
#include <stdio.h>
#include <math.h>
#include <delay.h>

#include "Porting_Layer.h"
#include "SENSOR_AK09911C.h"


AK09911C5_ASA_DATA	 AK09911C_ASA_XYZ_DATA;	// asa x,y,z reg data    ;  initial read ASA register & write in 


//********************************************* SYSTEM *************************************************
//----------	----------------------------------------------------------------------------------------
//READ AK09911C ID & INFO  
//id_info -> 4 data   ; Read Reg 0x00 ~0x03
//if read fail -> return -1
CHAR8S AK09911C_GET_ID_INFO(CHAR8U *id_info)
{
		CHAR8U read_data[4] = {0};
		CHAR8S status = 0;
		
		i2c_stop_hang();

		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_WAI1,4,&read_data[0]);
		if(status !=1) return -1; // read fail
  
		id_info[0] = read_data[0];		//AK09911C_REG_WAI1 ; value = 0x48
		id_info[1] = read_data[1];		//AK09911C_REG_WAI2 ; value = 0x05
		id_info[2] = read_data[2];		//AK09911C_REG_INFO1
		id_info[3] = read_data[3];		//AK09911C_REG_INFO2
		return 0;
	
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// initial AK09911C 
CHAR8S AK09911C_SET_INITIAL(void)
{
		CHAR8S status = 0;
		i2c_stop_hang();
		
		//step1 reset the AK09911C
		status = AK09911C_SET_RESET();
		if(status !=0) return -1; // reset fail

		delay_ms(50);
		i2c_stop_hang();

		//step2 set the system mode
		status = AK09911C_SET_SYSTEM_MODE(AK09911C_POWER_DOWN_MODE);
		if(status !=0) return -1; // set the system mode fail
		delay_ms(2);
		
		i2c_stop_hang();
		
		//step get XYZ-axis sensitivity adjustment value	Fuse ROM
		status = AK09911C_GET_ASA_DATA(&AK09911C_ASA_XYZ_DATA);
		if(status !=0) return -1; // get ASA data fail
		
		return 0 ; // initial the AK09911C success
}
//--------------------------------------------------------------------------------------------------
//----------	----------------------------------------------------------------------------------------
//reset the AK09911C
CHAR8S AK09911C_SET_RESET(void)
{
		CHAR8S status = 0;
		
		i2c_stop_hang();		
		i2c_stop_hang();
		
		status = i2c_write_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL3,AK09911C_SW_RESET);
		if(status !=1) return -1; // read fail
		return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// set AK09911C  power down mode  
CHAR8S AK09911C_SET_POWER_DOWN_MODE(void)
{

		CHAR8S status = 0;
		CHAR8U write_data = AK09911C_POWER_DOWN_MODE & 0x1F; //mask : 0x1F

		i2c_stop_hang();
		i2c_stop_hang();
		
		status = i2c_write_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL2,write_data);
		if(status !=1) return -1; // set the mode fail
		return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//  AK09911C 
// mode : use enum AK09911C_MODE_SELECT
CHAR8S AK09911C_SET_SYSTEM_MODE(CHAR8U mode)
{
		CHAR8S status = 0;
		CHAR8U write_data=mode & 0x1F; //mask : 0x1F
		
             // printf(" SET_SYSTEM_MODE = 0x%x\r\n",write_data);
			  
		i2c_stop_hang();
		i2c_stop_hang();	
		i2c_stop_hang();
	
		status = i2c_write_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL2,write_data);
		if(status !=1) {  
                        return -1; // set the mode fail
                        }
		return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// GET AK09911C mode 
/*	AK09911C_POWER_DOWN_MODE=(0x0<<0),		// Power-down mode
	AK09911C_SINGLE_MODE=(0x01<<0),				// Single measurement mode
	AK09911C_CONTI_MODE_1=(0x01<<1),			// Continuous measurement mode 1
	AK09911C_CONTI_MODE_2=(0x01<<2),			// Continuous measurement mode 2		
	AK09911C_CONTI_MODE_3=(0x03<<2),			// Continuous measurement mode 3	
	AK09911C_CONTI_MODE_4=(0x01<<3),			// Continuous measurement mode 4	
	AK09911C_SELF_TEST=(0x01<<4),				// Self-test mode
	AK09911C_FUSE_ROM=(0x1F)						//Fuse ROM access mode
*/
CHAR8S AK09911C_GET_MODE(CHAR8U *mode)
{
		CHAR8U read_data = 0;
		CHAR8S status = 0;


		i2c_stop_hang();	
		i2c_stop_hang();
		
		status = i2c_read_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_CNTL2,&read_data);		//read mode	
		if(status !=1) return -1; // read fail
		
              printf("AK09911C_GET_MODE = 0x%x\r\n",read_data);
			  
		if(read_data == AK09911C_POWER_DOWN_MODE)//power-down mode
			{
				*mode = AK09911C_POWER_DOWN_MODE;
				 return 0;
			}
		else if(read_data == AK09911C_SINGLE_MODE)//single measurement mode
			{
				*mode = AK09911C_SINGLE_MODE;
				 return 0;			
			}
		else if(read_data == AK09911C_CONTI_MODE_1)// Continuous measurement mode 1
			{
				*mode = AK09911C_CONTI_MODE_1;
				 return 0;
		 	}
		else if(read_data == AK09911C_CONTI_MODE_2)// Continuous measurement mode 2		
			{
				*mode = AK09911C_CONTI_MODE_2;
				  return 0;
			}
		else if(read_data == AK09911C_CONTI_MODE_3)// Continuous measurement mode 3	
			{
				*mode = AK09911C_CONTI_MODE_3;
				 return 0;
			}
			else if(read_data == AK09911C_CONTI_MODE_4)// Continuous measurement mode 4	
			{
				*mode = AK09911C_CONTI_MODE_4;
				 return 0;
			}
		else if(read_data == AK09911C_SELF_TEST)// Self-test mode
			{
				*mode = AK09911C_SELF_TEST;
				 return 0;
			}	
		else if(read_data == AK09911C_FUSE_ROM)//Fuse ROM access mode
			{
				*mode = AK09911C_FUSE_ROM;
				 return 0;
			}					
		else 
			{
			// nothing.
			}
		return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// AK09911C 
// read the Data status  [reg 0x10] 
CHAR8S AK09911C_GET_DATA_READY_STATUS(CHAR8U *dataready_status)
{

		CHAR8S status = 0;
		CHAR8U read_data=0;

		i2c_stop_hang();

		status = i2c_read_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_ST1,&read_data);
		if(status !=1) return -1; // read status 1 fail

		*dataready_status = read_data;
		return 0 ; //read stats 1 success
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// get AK09911C the overflow status [bit3]
// *** when any of measurement data is read, be sure to read ST2 register at the end.
CHAR8S AK09911C_GET_OVER_FLOW_STATUS(CHAR8U *overflow_status)
{

		CHAR8S status = 0;
		CHAR8U read_data=0;

		i2c_stop_hang();
		i2c_stop_hang();
		
		status = i2c_read_1_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_ST2,&read_data);
		if(status !=1) return -1; // read status 2  fail

		*overflow_status = read_data;
		return 0 ; //read stats 2  success
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//get  AK09911C  XYZ axis Magnetic Data
CHAR8S AK09911C_GET_XYZ_DATA(AK09911C_AXIS_DATA *raw_data)
{
//******************************* undone *******************************
		CHAR8U read_data[6] = {0};
		CHAR8S status = 0;
		CHAR8U data_ready =0,over_flow=0;	

		i2c_stop_hang();


		status = AK09911C_SET_SYSTEM_MODE(AK09911C_SINGLE_MODE);
		if(status !=0) return -1; // set the system mode fail	
	
		i2c_stop_hang();
		
		delay_ms(50);
		
		status = AK09911C_GET_DATA_READY_STATUS(&data_ready);
		if(status !=0) return -1; // read fail	
     
		i2c_stop_hang();
		//printf("data_ready = 0x%x\r\n",data_ready);
		//check status   no skipped data"0"in [bit1]   &   data ready"1"in [bit0])
		if(data_ready & 0x03 != 0x03)  return -3 ;   // data not ready or skipped data
		
		
		i2c_stop_hang();

		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_HXL,6,&read_data[0]);
		if(status !=1) return -1; // read fail
  		
		
		raw_data->X_AXIS=(INT16U)(read_data[1]<<8) + (INT16U)read_data[0];		//Measurement Magnetic Data - X axis Data 
		raw_data->Y_AXIS=(INT16U)(read_data[3]<<8) + (INT16U)read_data[2];		//Measurement Magnetic Data - Y axis Data 
		raw_data->Z_AXIS=(INT16U)(read_data[5]<<8) + (INT16U)read_data[4];		//Measurement Magnetic Data - Z axis Data 
		
		i2c_stop_hang();

		//** when any of measurement data is read, be sure to read ST2 register at the end.
		status = AK09911C_GET_OVER_FLOW_STATUS(&over_flow);
		if(status !=0) return -1; // read fail
		
		i2c_stop_hang();
		
		//check overflow 
		if(over_flow & 0x08) return -4; // data overflow!! 
		
			
		return 0;

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// get the AK09911C  XYZ-axis sensitivity adjustment value
CHAR8S  AK09911C_GET_ASA_DATA(AK09911C5_ASA_DATA *asa_data)
{
		CHAR8U read_data[3] = {0};
		CHAR8S status = 0;
		
		i2c_stop_hang();

		status = AK09911C_SET_SYSTEM_MODE(AK09911C_FUSE_ROM);	//set FUSE_ROM mode  to READ ASA data
		if(status !=0) return -1; // set the system mode fail
		

		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_ASAX,3,&read_data[0]);
		if(status !=1) return -1; // read fail

		asa_data->ASA_X = read_data[0];		//X-axis sensitivity adjustment value	
		asa_data->ASA_Y = read_data[1];		//Y-axis sensitivity adjustment value	
		asa_data->ASA_Z = read_data[2];		//Z-axis sensitivity adjustment value	


		        printf("AK09911C ASA_X = 0x%x\r\n",AK09911C_ASA_XYZ_DATA.ASA_X);
		        printf("AK09911C ASA_Y = 0x%x\r\n",AK09911C_ASA_XYZ_DATA.ASA_Y);
			 printf("AK09911C ASA_Z = 0x%x\r\n",AK09911C_ASA_XYZ_DATA.ASA_Z);

		status = AK09911C_SET_SYSTEM_MODE(AK09911C_POWER_DOWN_MODE); 		//After read out the ROM data -> power down mode
		if(status !=0) return -1; // set the system mode fail	
		
		return 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//calculate the XYZ data of AK09911C  XYZ axis & XYZ ASA data
//Sensitivity adjustment data for each axis is stored to fuse ROM on shipment.
// ** See AK09911C Spec page.30

// Hadj -> adjusted measurement data.
// H 	 -> the measurement data read out from the measurement data register.
// ASA  -> REG_ASAX,Y,Z data. 

//  Hadj = H *(      (ASA + 128 ) / 128 )

//	H is in the range of -8190 to 8190.  The magnetometer has a range of
// 	+4912uT & -4912uT .  To go from the raw value to uT is:
//
//HuT = H * 4912/8190, or  H*6/10.
//
// Since 1uT = 100 gauss, our final scale factor becomes:
//
// Hadj = H * ((ASA + 128) / 128) * 6/10 * 100

// Hadj = H * ((ASA + 128) * 60 / 128
void AK09911C_GET_CAL(AK09911C_AXIS_DATA *data)
{
		INT16S temp=0;
		FLOAT temp2=0;    
		
	       data->X_AXIS = (data->X_AXIS *6 /10);
	       data->Y_AXIS = (data->Y_AXIS *6 /10);
	       data->Z_AXIS = (data->Z_AXIS *6 /10); 
		
		//X axis
		temp2 = (FLOAT)data->X_AXIS *(((FLOAT)AK09911C_ASA_XYZ_DATA.ASA_X/128)+1);
		data->X_AXIS = (INT16S)temp2;
		//temp = data->X_AXIS *  (INT16S)(( (INT16S)AK09911C_ASA_XYZ_DATA.ASA_X +128) *30 /128);
		//data->X_AXIS = temp;

		//Y axis
		temp2 = (FLOAT)data->Y_AXIS *(((FLOAT)AK09911C_ASA_XYZ_DATA.ASA_Y/128)+1);
		data->Y_AXIS = (INT16S)temp2;
		//temp = data->Y_AXIS *  (INT16S)(( (INT16S)AK09911C_ASA_XYZ_DATA.ASA_Y +128)*30 /128);
		//data->Y_AXIS = temp;

		//Z axis
		temp2 = (FLOAT)data->Z_AXIS *(((FLOAT)AK09911C_ASA_XYZ_DATA.ASA_Z/128)+1);
		data->Z_AXIS = (INT16S)temp2;		
		//temp = data->Z_AXIS *  (INT16S)(( (INT16S)AK09911C_ASA_XYZ_DATA.ASA_Z +128) *30 /128);
		//data->Z_AXIS = temp;

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// step 1. Set Power-down mode. (MODE[4:0]=¡§00000¡¨)
// step 2. Set Self-test mode. (MODE[4:0]=¡§10000¡¨)
// step 3. Check Data Ready or not by polling DRDY bit of ST1 register ; When Data Ready, proceed to the next step.
// step 4. Read measurement data (HXL to HZH)
CHAR8S AK09911C_SET_SELF_TEST(AK09911C_AXIS_DATA *data)
{
		CHAR8S status = 0,status1 = 0;
		CHAR8U count = 0,read_data_ready_status=0,read_data_overflow_status=0;	
		CHAR8U raw_data[6] = {0};
		

		i2c_stop_hang();	
		i2c_stop_hang();	
		
// step 1. Set Power-down mode. (MODE[4:0]=¡§00000¡¨)
    
		//write AK09911C_POWER_DOWN_MODE. 
		status = AK09911C_SET_POWER_DOWN_MODE();
		if(status != 0 )return -1; // write fail.		

		delay_ms(1);

		i2c_stop_hang();					
// step 2. Set Self-test mode. (MODE[4:0]=¡§10000¡¨)

		//write AK09911C_SELF_TEST. 
		status = AK09911C_SET_SYSTEM_MODE(AK09911C_SELF_TEST);
		if(status != 0 )return -1; // write fail.		

//step 3. Check Data Ready or not by polling DRDY bit of ST1 register ; When Data Ready, proceed to the next step.

		for(count = 0 ; count < 8 ; count ++)	//retry 8 
			{
				AK09011C_DELAY(0xFF0);
		
				status = AK09911C_GET_DATA_READY_STATUS(&read_data_ready_status);		//check RDRY , DOR bit

				if(status == 0)
					{
					// data ready & no error
					if(read_data_ready_status & 0x03 == 0x03)break;	//  no skipped data"0"in [bit1]   &   data ready"1"in [bit0]
					
					}
				else if(status == -1 )
					{
					//read status fail
					status1 = -1;
					}
			}

// step 4. Read measurement data (HXL to HZH)

		i2c_stop_hang();
		i2c_stop_hang();
		//read x,y,z axis raw data
		status = i2c_read_n_byte_data(AK09911C_SLAVE_ADDRESS,AK09911C_REG_HXL,6,&raw_data[0]);
		if(status !=1) return -1; // read fail
		
		i2c_stop_hang();

		//when any of measurement data is read, be sure to read ST2 register at the end.
		status = AK09911C_GET_OVER_FLOW_STATUS(&read_data_overflow_status);
		if(status !=0) return -1; // read fail

		//check overflow 
		if(read_data_overflow_status & 0x08) return -3; // data overflow!! 
    

		//** SET  AK09911C_POWER_DOWN_MODE
		i2c_stop_hang();
		i2c_stop_hang();
		
		//write AK09911C_POWER_DOWN_MODE. 
		status = AK09911C_SET_POWER_DOWN_MODE();
		if(status != 0 )return -1; // write fail.	
        	
			data->X_AXIS =     (raw_data[1]<<8)   + raw_data[0];
			data->Y_AXIS =     (raw_data[3]<<8)   + raw_data[2];		
			data->Z_AXIS =     (raw_data[5]<<8)   + raw_data[4];	

			AK09911C_GET_CAL(data);	
			
			return 0; // read success ok!

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//for AK09011C delay
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
//--------------------------------------------------------------------------------------------------
//********************************************* SYSTEM *************************************************



#endif 		//#ifndef __AK09911C_FUNCTION__ 
