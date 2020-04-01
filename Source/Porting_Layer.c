/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     File Name		: Porting_Layer.c
     Function		: porting DataType & I2C API Function 
     Create Date	: 2020/03/30
---------------------------------------------------------------------- */
#ifndef __PORTING_LAYER_FUNCTION___
#define __PORTING_LAYER_FUNCTION___


#include "Porting_Layer.h"


//--------------------------------------------------------------------------------------------------
/* Do I2C STOP condition */
void i2c_stop_hang(void)
{    
	// Place your code here 

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Write 1byte sequence 

    slave_address : device address
    register_address : device control register address
    data : write into register data
    
*/
CHAR8S i2c_write_1_byte_data(CHAR8U slave_address,CHAR8U register_address,CHAR8U data)
{
	// Place your code here 
	
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Write n-byte sequence 

    slave_address : device address
    start_register : write start register address 
    n_byte : write data total number
    *w_data_stream : write data (point to a array)    
*/
CHAR8S i2c_write_n_byte_data(CHAR8U slave_address,CHAR8U start_reg_addr,CHAR8U n_byte,CHAR8U *w_data_stream)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Read 1byte sequence 

    slave_address : device address
    register_address : device control register address
    *data : read from device data
    
*/
CHAR8S i2c_read_1_byte_data(CHAR8U slave_address,CHAR8U register_address,CHAR8U *data)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Read nbyte sequence 

    slave_address : device address
    register_address : device control register address
    n_byte : write data total number
    *data : read from device data(point to a array)   

*/
CHAR8S i2c_read_n_byte_data(CHAR8U slave_address,CHAR8U start_reg_addr,CHAR8U n_byte,CHAR8U *r_data_stream)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Write 1byte sequence 

    slave_address : device address
    reg_high_addr: register high byte address
    reg_low_addr: register low byte address
    data : write into register data
    
*/
CHAR8S i2c_write_1_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U data)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Write n-byte sequence 

    slave_address : device address
    reg_high_addr: register high byte address
    reg_low_addr: register low byte address
    n_byte : write data total number
    *w_data_stream : write data (point to a array)  
    
*/
CHAR8S i2c_write_n_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U n_byte,CHAR8U *w_data_stream)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Read 1byte sequence 

    slave_address : device address
    reg_high_addr: register high byte address
    reg_low_addr: register low byte address
    *data : read from device data
    
*/
CHAR8S i2c_read_1_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U *data)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Do I2C Read n-byte sequence 

    slave_address : device address
    reg_high_addr: register high byte address
    reg_low_addr: register low byte address
    n_byte : write data total number
    *data : read from device data(point to a array)   
    
*/
CHAR8S i2c_read_n_byte_data_two_reg_addr(CHAR8U slave_address,CHAR8U reg_high_addr,CHAR8U reg_low_addr,CHAR8U n_byte,CHAR8U *r_data_stream)
{
	// Place your code here 
}
//--------------------------------------------------------------------------------------------------


#endif		//#ifndef __PORTING_LAYER_FUNCTION___ 
