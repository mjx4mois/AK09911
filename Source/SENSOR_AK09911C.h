/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: E-COMPASS
     File Name		: SENSOR_AK09911C.h
     Function		: SENSOR_AK09911C
     Create Date	: 2017/11/09
---------------------------------------------------------------------- */
#ifndef __AK09911C_HEADER__ 
#define __AK09911C_HEADER__  

/* Note : RETN must be connect to Vcc , avoid reset.*/
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/
/* AK09911C SLAVE ADDRESS*/

#define AK09911C_CAD0_ADDR_PIN		 	(0)	 		/* PIN2  ,connect  "0" GND or "1" Vcc*/

#if   (AK09911C_CAD0_ADDR_PIN == 0) 
	#define AK09911C_SLAVE_ADDRESS 		(0x18)		/* Slave address*/
#elif (AK09911C_CAD0_ADDR_PIN == 1)  
	#define AK09911C_SLAVE_ADDRESS 		(0x1A)		/* Slave address*/
#endif
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/


/* ------------- DEFINE AK09911C REGISTER  ------------*/
/*------------------------------------------------*/
/*Campany ID ; value = 0x48*/
#define AK09911C_REG_WAI1					(0x00)							/*R*/
/*------------------------------------------------*/
/*Device ID ; value = 0x05*/
#define AK09911C_REG_WAI2					(0x01)							/*R*/
/*------------------------------------------------*/
/*Information 1*/
/*INFO1[7:0] : Device information of AKM.*/
#define AK09911C_REG_INFO1					(0x02)							/*R*/
/*------------------------------------------------*/
/*Information 2*/
/*INFO2[7:0]: Device information of AKM.*/
#define AK09911C_REG_INFO2					(0x03)							/*R*/
/*------------------------------------------------*/
/*
	Status 1 - Data Status  ;  default : 0x00
	bit 7 [HSM] I2C Hs-mode
	0: Standard/Fast mode ;  1 : Hs-mode
	HSM bit turns to ¡§1¡¨ when I2C bus interface is changed from Standard or Fast mode to High-speed mode (Hs-mode).
	---------------------------------
	bit 6 ~2  set  "0" 
	---------------------------------
	bit 1 [DOR] Data Overrun
	0 : Normal ; 1 : Data overrun
	DOR bit turns to ¡§1¡¨ when data has been skipped in Continuous measurement mode 1, 2, 3, 4.
	It returns to ¡§0¡¨ when any one of ST2 register or measurement data register (HXL to TMPS) is read.
	---------------------------------
	bit 0 [DRDY] DRDY: Data Ready
	0 : Normal ;  1 : Data is ready
	DRDY bit turns to ¡§1¡¨ when data is ready in Single measurement mode, 
	Continuous measurement mode 1, 2, 3, 4 or Self-test mode.
	It returns to ¡§0¡¨ when any one of ST2 register or measurement data register (HXL to TMPS) is read.
*/
#define AK09911C_REG_ST1						(0x10)							/*R*/
/*------------------------------------------------*/
/*
	Measurement Magnetic Data - X axis Data - low byte
	HXL[7:0]: X-axis measurement data lower 8-bit
	Measurement data is stored in two¡¦s complement and Little Endian format.
	Measurement range of each axis is -8190 to 8190.
*/
#define AK09911C_REG_HXL						(0x11)							/*R*/
/*------------------------------------------------*/
/*
	Measurement Magnetic Data - X axis Data - high byte
	HXH[15:8]: X-axis measurement data higher 8-bit
	Measurement data is stored in two¡¦s complement and Little Endian format.
	Measurement range of each axis is -8190 to 8190.
*/
#define AK09911C_REG_HXH					(0x12)							/*R*/
/*------------------------------------------------*/
/*
	Measurement Magnetic Data - Y axis Data - low byte
	HYL[7:0]: Y-axis measurement data lower 8-bit
	Measurement data is stored in two¡¦s complement and Little Endian format.
	Measurement range of each axis is -8190 to 8190.
*/
#define AK09911C_REG_HYL						(0x13)							/*R*/
/*------------------------------------------------*/
/*
	Measurement Magnetic Data - Y axis Data - high byte
	HYH[15:8]: Y-axis measurement data higher 8-bit
	Measurement data is stored in two¡¦s complement and Little Endian format.
	Measurement range of each axis is -8190 to 8190.
*/
#define AK09911C_REG_HYH					(0x14)							/*R*/
/*------------------------------------------------*/
#define AK09911C_REG_HZL						(0x15)							/*R*/
/*
	Measurement Magnetic Data - Z axis Data - low byte
	HZL[7:0]: Z-axis measurement data lower 8-bit
	Measurement data is stored in two¡¦s complement and Little Endian format.
	Measurement range of each axis is -8190 to 8190.
*/
/*------------------------------------------------*/
/*
	Measurement Magnetic Data - Z axis Data - high byte
	HZH[15:8]: Z-axis measurement data higher 8-bit
	Measurement data is stored in two¡¦s complement and Little Endian format.
	Measurement range of each axis is -8190 to 8190.
*/
#define AK09911C_REG_HZH					(0x16)							/*R*/
/*------------------------------------------------*/
/*
	Status 2 - Data Status ;  default : 0x00
	bit 7~4 & bit2~0 set "0" 
	---------------------------------
	bit 3 [HOFL] HOFL: Magnetic sensor overflow
	0 : Normal  ; 1 : Magnetic sensor overflow occurred
	In Single measurement mode, Continuous measurement mode 1, 2, 3, 4, and Self-test mode, 
	magnetic sensor may overflow even though measurement data regiseter is not saturated. 

	In this case, measurement data is not correct and HOFL bit turns to ¡§1¡¨. 
	When next measurement start, it returns to ¡§0¡¨. Refer to 6.4.3.6 for detailed information.
	ST2 register has a role as data reading end register, also.
	When any of measurement data register (HXL to TMPS) is read in Continuous measurement mode 1, 2, 3, 4,
	****** it means data reading start and taken as data reading until ST2 register is read. 
	****** Therefore, when any of measurement data is read, be sure to read ST2 register at the end.
*/
#define AK09911C_REG_ST2						(0x18)							/*R*/

/*------------------------------------------------*/
/*
	Control 2 - Control settings ;  default : 0x00
	bit 7~5 set "0"
	---------------------------------
	bit 4~0 [MODE] Operation mode setting
	¡§00000¡¨: Power-down mode
	¡§00001¡¨: Single measurement mode
	¡§00010¡¨: Continuous measurement mode 1
	¡§00100¡¨: Continuous measurement mode 2
	¡§00110¡¨: Continuous measurement mode 3
	¡§01000¡¨: Continuous measurement mode 4
	¡§10000¡¨: Self-test mode
	¡§11111¡¨: Fuse ROM access mode
	Other code settings are prohibited
	When each mode is set, AK09911 transits to the set mode.
*/
#define AK09911C_REG_CNTL2					(0x31)							/*R/W*/

/*------------------------------------------------*/
/*
	Control 3 - Control settings ;  default : 0x00
	bit 7~1 set "0"
	---------------------------------
	bit 0 [SRST]Soft reset
	¡§0¡¨: Normal ; ¡§1¡¨: Reset
	When ¡§1¡¨ is set, all registers are initialized. After reset, SRST bit turns to ¡§0¡¨ automatically.
*/
#define AK09911C_REG_CNTL3					(0x32)							/*R/W*/

/*------------------------------------------------*/
/*
	X-axis sensitivity adjustment value	Fuse ROM
	Sensitivity adjustment data for each axis is stored to fuse ROM on shipment.
	ASAX[7:0]:Magnetic sensor X-axis sensitivity adjustment value
*/
#define AK09911C_REG_ASAX					(0x60)							/*R*/
/*------------------------------------------------*/
/*
	Y-axis sensitivity adjustment value	Fuse ROM
	Sensitivity adjustment data for each axis is stored to fuse ROM on shipment.
	ASAY[7:0]:Magnetic sensor Y-axis sensitivity adjustment value
*/
#define AK09911C_REG_ASAY					(0x61)							/*R*/
/*------------------------------------------------*/
/*
	Z-axis sensitivity adjustment value	Fuse ROM
	Sensitivity adjustment data for each axis is stored to fuse ROM on shipment.
	ASAZ[7:0]:Magnetic sensor Z-axis sensitivity adjustment value
*/
#define AK09911C_REG_ASAZ					(0x62)							/*R*/
/*------------------------------------------------*/
/*
	** See AK09911C Spec page.30
	Hadj = H *(      (ASA - 128 ) /256 + 1     )
	Hadj -> adjusted measurement data.
	H -> the measurement data read out from the measurement data register.
	ASA -> REG_ASAX,Y,Z data. 
*/
/* ------------- DEFINE AK09911C REGISTER  ------------*/




/* ------------- DEFINE AK09911C Parameter -------------*/

/*** REGISTER : AK09911C_REG_CNTL2   [4:0] */
typedef enum 
{
	AK09911C_POWER_DOWN_MODE=(0x0),			/* Power-down mode*/
	AK09911C_SINGLE_MODE=(0x01),					/* Single measurement mode*/
	AK09911C_CONTI_MODE_1=(0x02),				/* Continuous measurement mode 1*/
	AK09911C_CONTI_MODE_2=(0x04),				/* Continuous measurement mode 2*/	
	AK09911C_CONTI_MODE_3=(0x06),				/* Continuous measurement mode 3*/
	AK09911C_CONTI_MODE_4=(0x08),				/* Continuous measurement mode 4*/
	AK09911C_SELF_TEST=(0x10),					/* Self-test mode*/
	AK09911C_FUSE_ROM=(0x1F)						/* Fuse ROM access mode*/
/*Other code settings are prohibited  ; When each mode is set, AK09911 transits to the set mode.*/
}AK09911C_MODE_SELECT;

typedef struct AK09911C_DATA
{	
/*
	AK09911C resolution -> 13 bit
	Note : Little endian
	      two's complemnt         Hex	  	Dec			Magnetic flux density [uT]
		0000 1111 1111 1111    0FFF		4095    	 		1229(max.)
		1111 0000 0000 0000    F000		-4096   			-1229(min.)
*/
	INT16S X_AXIS;
	INT16S Y_AXIS;
	INT16S Z_AXIS;		
}AK09911C_AXIS_DATA;


typedef struct AK09911C_ASA
{	
	/*AK8975 resolution -> 13 bit*/
	CHAR8U ASA_X;
	CHAR8U ASA_Y;
	CHAR8U ASA_Z;
}AK09911C5_ASA_DATA;


typedef struct AK09911C_CALIBRATE_S
{	

	FLOAT X_OFFSET;
	FLOAT Y_OFFSET;
	FLOAT Z_OFFSET;
	FLOAT X_FACTOR_BASE;
	FLOAT Y_FACTOR_BASE;
	FLOAT Z_FACTOR_BASE;
	
}AK09911C_CALIBRATE;

typedef struct AK09911C_4_QUADRANT_DATA_POINT_S
{	
	/* if any quadant get point number OK set "1" or set "0" */
	CHAR8U first_quadant_data_pointok;
	CHAR8U second_quadant_data_pointok;
	CHAR8U third_quadant_data_pointok;
	CHAR8U fourth_quadant_data_pointok;
	
}AK09911C_4_QUADRANT_DATA_POINT;



/*
	**register : 0x31 [AK09911C_REG_CNTL2]
	**function : AK09911C_SET_SYSTEM_MODE
	use enum :  AK09911C_MODE_SELECT
	set the AK09911C system mode
*/
#define AK09911C_SYSTEM_MODE				(AK09911C_SINGLE_MODE)
/*
	**register : 0x32 [AK09911C_REG_CNTL3]
	**function : AK09911C_SET_RESET
	for reset the AK09911C 
*/
#define AK09911C_SW_RESET					(0x01)			/*"1" set sw reset*/


/* ------------- DEFINE AK09911C Parameter -------------*/




/********************************************** SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*
	READ AK09911C ID & INFO  
	id_info -> 4 data   ; Read Reg 0x00 ~0x03
	if read fail -> return -1
*/
CHAR8S AK09911C_GET_ID_INFO(CHAR8U *id_info);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*initial AK09911C*/
CHAR8S AK09911C_SET_INITIAL(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*reset the AK09911C*/
CHAR8S AK09911C_SET_RESET(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set AK09911C  power down mode*/  
CHAR8S AK09911C_SET_POWER_DOWN_MODE(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*  
	AK09911C 
	mode : use enum AK09911C_MODE_SELECT
*/
CHAR8S AK09911C_SET_SYSTEM_MODE(CHAR8U mode);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	GET AK09911C mode 
	AK09911C_POWER_DOWN_MODE=(0x0<<0),	->Power-down mode
	AK09911C_SINGLE_MODE=(0x01<<0),		->Single measurement mode
	AK09911C_CONTI_MODE_1=(0x01<<1),		->Continuous measurement mode 1
	AK09911C_CONTI_MODE_2=(0x01<<2),		->Continuous measurement mode 2		
	AK09911C_CONTI_MODE_3=(0x03<<2),		->Continuous measurement mode 3	
	AK09911C_CONTI_MODE_4=(0x01<<3),		->Continuous measurement mode 4	
	AK09911C_SELF_TEST=(0x01<<4),			-> Self-test mode
	AK09911C_FUSE_ROM=(0x1F)				->Fuse ROM access mode
*/
CHAR8S AK09911C_GET_MODE(CHAR8U *mode);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* AK09911C */
/* read the Data status  [reg 0x10] */
CHAR8S AK09911C_GET_DATA_READY_STATUS(CHAR8U *dataready_status);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* get AK09911C the overflow status [bit3]*/
/* *** when any of measurement data is read, be sure to read ST2 register at the end.*/
CHAR8S AK09911C_GET_OVER_FLOW_STATUS(CHAR8U *overflow_status);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*get  AK09911C  XYZ axis Magnetic Data*/
CHAR8S AK09911C_GET_XYZ_DATA(AK09911C_AXIS_DATA *raw_data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* get the AK09911C  XYZ-axis sensitivity adjustment value*/
CHAR8S AK09911C_GET_ASA_DATA(AK09911C5_ASA_DATA *asa_data);
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
void AK09911C_GET_CAL(AK09911C_AXIS_DATA *data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	step 1. Set Power-down mode. (MODE[4:0]=¡§00000¡¨)
	step 2. Set Self-test mode. (MODE[4:0]=¡§10000¡¨)
	step 3. Check Data Ready or not by polling DRDY bit of ST1 register ; When Data Ready, proceed to the next step.
	step 4. Read measurement data (HXL to HZH)
*/
CHAR8S AK09911C_SET_SELF_TEST(AK09911C_AXIS_DATA *data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*for AK09011C delay*/
void AK09011C_DELAY(INT32U count);
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
CHAR8U AK09011C_GET_CALIBRATE_DATA(AK09911C_AXIS_DATA data,AK09911C_4_QUADRANT_DATA_POINT *quadrant_ready);
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
CHAR8U AK09911C_GET_CALIBRATE_FACTOR(AK09911C_CALIBRATE *cal_result);
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
FLOAT AK09011C_GET_AZIMUTH_WITHOUT_COMPENSATION(AK09911C_AXIS_DATA data);
/*------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
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
FLOAT AK09011C_GET_AZIMUTH_WITH_COMPENSATION(AK09911C_AXIS_DATA data,FLOAT pitch,FLOAT roll);
/*--------------------------------------------------------------------------------------------------*/
/********************************************** ADVANCED APPLICATIONS **************************************************/





#endif 		/*#ifndef __AK09911C_HEADER__ */
