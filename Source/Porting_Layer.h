/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     File Name		: Porting_Layer.h
     Function		: porting DataType & I2C API Function 
     Create Date	: 2020/03/30
---------------------------------------------------------------------- */
#ifndef __PORTING_LAYER_HEADER___
#define __PORTING_LAYER_HEADER___




/*
|---------------------------------------------------------------------------|
| Remame |         Name         |    Size     |            Range            |
|--------+------------------------------------------------------------------|
|n/a     |     bit              |    ? byte   |    0 or 1                   |
|BOOL    |     bool             |    1 byte   |    0 or 1                   |
|CHAR8S  |     char             |    1 byte   |    -128 ~ 127               |
|CHAR8U  |     unsigned char    |    1 byte   |    0    ~ 255               |
|n/a     |     signed char      |    1 byte   |    -128   ~ 127             |
|INT16S  |     int              |    2 byte   |    -32768 ~ 32767           |
|n/a     |     short int        |    2 byte   |    -32768 ~ 32767           |
|INT16U  |     unsigned int     |    2 byte   |    0 ~ 65535                |
|n/a     |     signed int       |    2 byte   |    -32768 ~ 32767           |
|INT32S  |     long int         |    4 byte   |    -2147483648 ~ 2147483647 |
|INT32U  |     unsigned long    |    4 byte   |    0 ~ 4294967295           |
|n/a     |     signed long      |    4 byte   |    -2147483648 ~ 2147483647 |
|FLOAT   |     float            |    4 byte   |    ��1.175e-38 ~ ��3.402e38   |
|DOUBLE  |     double           |    4 byte   |    ��1.175e-38 ~ ��3.402e38   |
|---------------------------------------------------------------------------|*/


#define  BOOL			bool
#define  CHAR8S 		char
#define  CHAR8U		unsigned char
#define  INT16S			int
#define  INT16U			unsigned int
#define  INT32S     		long int
#define  INT32U    		unsigned long int
#define  FLOAT     		float
#define  DOUBLE     		double



// ----------- STATUS DEFINE ------------
#define SWI2C_STATUS_OK			(1)
#define SWI2C_STATUS_FAIL		(-1)
// ----------- STATUS DEFINE ------------

// ----------- PROTOCOL DEFINE ------------
#define WRITE_BIT					(0)
#define READ_BIT					(1)
#define MASTER_ACK				(1)
#define MASTER_NACK				(0)
// ----------- PROTOCOL DEFINE ------------


//--------------------------------------------------------------------------------------------------
// Stop I2C bus hanging
// parameter:
//				N/A
//  ex:
//				if Read / Write Fail , try this function to stop I2C bus hang!
//				After use this function , the I2C bus still hang 
//				-> maybe slave device pull low the SCL line. It's not the host device error!!
//		
void i2c_stop_hang(void);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// write one data
// parameter:
//				1.slave_address : the i2c slave id
//				2.register_address : write register address
//				3.data : data to write ;
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			start_reg_addr : 0x00
//			data : 0xAA
CHAR8S i2c_write_1_byte_data(CHAR8U slave_address,CHAR8U register_address,CHAR8U data);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Continue write n-bytes data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.start_reg_addr : start write register address
//				3.n_byte : n byte data to write (continue write) , MAX 255.
//				4.*data_stream : data to write ; the data number must equal n_byte.
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			start_reg_addr : 0x00
//			n_byte : 4
//			*data_stream = &a[0] ;      a[4]={0x13,0x26,0xAE,0x64};
CHAR8S i2c_write_n_byte_data(CHAR8U slave_address,CHAR8U start_reg_addr,CHAR8U n_byte,CHAR8U *w_data_stream);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// read ONE data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.register_address : read register address
//				3.data : data to read ;
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			start_reg_addr : 0x00
//			*data ->  ex: CHAR8U temperature_data ; * data = &temperature_data;
CHAR8S i2c_read_1_byte_data(CHAR8U slave_address,CHAR8U register_address,CHAR8U *data);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Continue read data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.start_reg_addr : start read register address
//				3.n_byte : n byte data to read (continue write) , MAX 255.
//				4.*data_stream : data to read ; the data number must equal n_byte.
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			start_reg_addr : 0x00
//			n_byte : 4
//			*data_stream = &a[0] ;      a[4]={0,0,0,0};// ready to read data.
CHAR8S i2c_read_n_byte_data(CHAR8U slave_address,CHAR8U start_reg_addr,CHAR8U n_byte,CHAR8U *r_data_stream);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// write ONE data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.reg_high_addr : high byte register address
//				3.reg_low_addr : low byte register address
//				4.data : data to write 
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			reg_high_addr : 0x00
//			reg_low_addr : 0x1E
//			data : 0xAA
CHAR8S i2c_write_1_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U data);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Continue write data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.reg_high_byte : high byte register address
//				3.reg_low_byte : low byte register address
//				4.n_byte : n byte data to write (continue write) , MAX 255.
//				5.*data_stream : data to write ; the data number must equal n_byte.
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			reg_high_byte : 0x00
//			reg_low_byte : 0x1E
//			n_byte : 4
//			*data_stream = &a[0] ;      a[4]={0x13,0x26,0xAE,0x64};
CHAR8S i2c_write_n_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U n_byte,CHAR8U *w_data_stream);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// read ONE data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.reg_high_addr : high byte register address
//				3.reg_low_addr : low byte register address
//				4.data : data to read
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			reg_high_addr : 0x00
//			reg_low_addr : 0x1E
//			*data ->  ex: CHAR8U temperature_data ; * data = &temperature_data;
CHAR8S i2c_read_1_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U *data);
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Continue read data :
// parameter:
//				1.slave_address : the i2c slave id
//				2.reg_high_addr : high byte register address
//				3.reg_low_addr : low byte register address
//				4.n_byte : n byte data to read (continue write) , MAX 255.
//				5.*data_stream : data to read ; the data number must equal n_byte.
//  ex:
//			slave_address : 0x30 (8bit) dont care Read/Write bit0 .
//			reg_high_addr : 0x00
//			reg_low_addr : 0x1E
//			n_byte : 4
//			*data_stream = &a[0] ;      a[4]={0,0,0,0};// ready to read data.
CHAR8S i2c_read_n_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U n_byte,CHAR8U *r_data_stream);
//--------------------------------------------------------------------------------------------------


#endif		//#ifndef __PORTING_LAYER_HEADER___ 
