/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     File Name		: Porting_Layer.c
     Function		: porting DataType & I2C API Function 
     Create Date	: 2020/03/30
---------------------------------------------------------------------- */
#ifndef __PORTING_LAYER_FUNCTION___
#define __PORTING_LAYER_FUNCTION___

#include <mega32a.h>
#include <i2c.h>
#include <delay.h>
#include <stdio.h>
#include "Porting_Layer.h"


//--------------------------------------------------------------------------------------------------
/* Do I2C STOP condition */
void i2c_stop_hang(void)
{    
	// Place your code here 
	CHAR8U try_count;

			for(try_count=0;try_count<=3;try_count++)
		 		{
					i2c_stop();
					delay_us(80);
				}
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

	CHAR8S ack=0,busy=0;
	
      i2c_stop();
      busy = i2c_start();
      if(busy)
       {
        delay_us(50);
           ack=i2c_write(slave_address | WRITE_BIT);
           if(ack == 1)
            {
             ack=i2c_write(register_address);
                 if(ack == 1)
                   {
                    ack=i2c_write(data);
                       if(ack == 1)
                       {
                            i2c_stop();
                            delay_us(10);
                            return SWI2C_STATUS_OK;
                       }
                       else
                       {

                        printf("data error\r\n");
                        goto EXIT;

                       }
                   }
                   else
                   {
                        printf("register error\r\n");
                        goto EXIT;
                   }
            }
            else
            {
                printf("address error\r\n");
                goto EXIT;

            }
     }
     else
     {
    EXIT:
         i2c_stop();
         delay_us(10);
         return SWI2C_STATUS_FAIL;
     }
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

    CHAR8S ack=0,busy=0,data_number,data_buffer=0;

      if(n_byte>255)
                    {
                     return SWI2C_STATUS_FAIL; ; //over max set 255!!
                     }
      i2c_stop();
      busy = i2c_start();
      if(busy)
       {
        delay_us(50);
           ack=i2c_write(slave_address | WRITE_BIT);
           if(ack == 1)
            {
             ack=i2c_write(start_reg_addr);
                 if(ack == 1)
                   {

                    for(data_number=0;data_number<n_byte;data_number++)
                    {

                            data_buffer = w_data_stream[data_number];
                            ack = i2c_write(data_buffer);
                            delay_us(100); // a little delay.

                                    if(ack != 1)  // if any byte write fail -> immediately return FAIL
                                    {
                                                i2c_stop();
                                                delay_us(10);
                                                return SWI2C_STATUS_FAIL;
                                     }

                    }

                            // ALL write finish!!
			               	i2c_stop();
			               	delay_us(10);
			              	return SWI2C_STATUS_OK;

                   }
                   else
                   {
                        printf("register error\r\n");
                        goto EXIT;
                   }
            }
            else
            {
                printf("address error\r\n");
                goto EXIT;

            }
     }
     else
     {
    EXIT:
         i2c_stop();
         delay_us(10);
         return SWI2C_STATUS_FAIL;
     }
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

      CHAR8S ack=0,busy=0;

      i2c_stop();
      busy = i2c_start();
      if(busy)
       {
        delay_us(50);
           ack=i2c_write(slave_address | WRITE_BIT);
           if(ack == 1)
            {
             ack=i2c_write(register_address);
                 if(ack == 1)
                   {
                        busy = i2c_start();
                         if(busy)
                           {
                         ack = i2c_write(slave_address | READ_BIT);
                           if(ack == 1)
                              {
                               *data=i2c_read(MASTER_NACK);
                               i2c_stop();
                               delay_us(10);

                               return SWI2C_STATUS_OK;
                              }
                           }
                           else
                           {
                             printf("register error\r\n");
                             goto EXIT;
                            }

                   }
                   else
                   {
                        printf("register error\r\n");
                        goto EXIT;
                   }
            }
            else
            {
                printf("address error\r\n");
                goto EXIT;

            }
     }
     else
     {
    EXIT:
         i2c_stop();
         delay_us(10);

         return SWI2C_STATUS_FAIL;
     }

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

	CHAR8S ack=0,busy=0,data_number;


      if(n_byte>255){
                    return SWI2C_STATUS_FAIL;  //over max set 255!!
                    }
      i2c_stop();
      busy = i2c_start();
      if(busy)
       {
        delay_us(50);
           ack=i2c_write(slave_address | WRITE_BIT);
           if(ack == 1)
            {
             ack=i2c_write(start_reg_addr);
                 if(ack == 1)
                   {
                        busy = i2c_start();
                         if(busy)
                           {
                         	ack = i2c_write(slave_address | READ_BIT);
                           if(ack == 1)
                                 {

					for(data_number=0;data_number<n_byte;data_number++)
					    {

						if(data_number == n_byte)
							{

							r_data_stream[data_number] = i2c_read(MASTER_NACK);
							delay_us(3); // a little delay.
							break;

							}
						else
							{

							 r_data_stream[data_number] = i2c_read(MASTER_ACK);
							 delay_us(3); // a little delay.

							}
					     }
						      // ALL read finish!!
			                           i2c_stop();
			                           delay_us(10);
						
			                           return SWI2C_STATUS_OK;

                              	}
					else
                           		{
                             	printf("address error\r\n");
                             	goto EXIT;
                            	}
                           }
                           else
                           {
                             printf("register error\r\n");
                             goto EXIT;
                            }

                   }
                   else
                   {
                        printf("register error\r\n");
                        goto EXIT;
                   }
            }
            else
            {
                printf("address error\r\n");
                goto EXIT;

            }
     }
     else
     {
    EXIT:
         i2c_stop();
         delay_us(10);

	  return SWI2C_STATUS_FAIL;
     }

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
	CHAR8S ack=0,busy=0;

	   i2c_stop();
	   busy = i2c_start();
	   if(busy)
		{
		 delay_us(50);
			ack=i2c_write(slave_address | WRITE_BIT);
			if(ack == 1)
			 {
			  ack=i2c_write(reg_high_addr);
				if(ack == 1)
				  {
				 ack=i2c_write(reg_low_addr);
				  if(ack == 1)
					{
					 ack=i2c_write(data);
						if(ack == 1)
						{
							 i2c_stop();
							 delay_us(10);
							 return SWI2C_STATUS_OK;
						}
						else
						{

						 printf("data error\r\n");
						 goto EXIT;

						}
					}
					else
					{
						 printf("register error\r\n");
						 goto EXIT;
					}
				  }
				  else
				  {
				      printf("register error\r\n");
					  goto EXIT;
				   }

			 }
			 else
			 {
				 printf("address error\r\n");
				 goto EXIT;

			 }
	  }
	  else
	  {
	 EXIT:
		  i2c_stop();
		  delay_us(10);
		  return SWI2C_STATUS_FAIL;
	  }

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

		CHAR8S ack=0,busy=0,data_number,data_buffer=0;

	      if(n_byte>255){
						return SWI2C_STATUS_FAIL; //over max set 255!!
	      				   }
	 i2c_stop();
	   busy = i2c_start();
	     if(busy)
	      {
	       delay_us(50);
	         ack=i2c_write(slave_address | WRITE_BIT);
	          if(ack == 1)
	           {
	            ack=i2c_write(reg_high_addr);
	               if(ack == 1)
	                 {
	                 ack=i2c_write(reg_low_addr);
	                   if(ack == 1)
	                    {

	                        for(data_number=0;data_number<n_byte;data_number++)
	                        {

	                            data_buffer = w_data_stream[data_number];
	                            ack = i2c_write(data_buffer);

                                delay_us(100); // a little delay.

	                                    if(ack != 1)  // if any byte write fail -> immediately return FAIL
	                                    {
	                                           i2c_stop();
	                                           delay_us(10);
	                                           return SWI2C_STATUS_FAIL;
	                                     }

	                        }
	                           			 // ALL write finish!!
				               	i2c_stop();
				               	delay_us(10);
				              	return SWI2C_STATUS_OK;

	                   }
	                   else
	                   {
	                        printf("register error\r\n");
	                        goto EXIT;
	                   }
			}
	              else
	              {
	                   printf("register error\r\n");
	                  goto EXIT;
	              }

	            }
	            else
	            {
	                printf("address error\r\n");
	                goto EXIT;

	            }
	     }
	     else
	     {
	    EXIT:
	         i2c_stop();
	         delay_us(10);
	         return SWI2C_STATUS_FAIL;
	     }


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

	     CHAR8S ack=0,busy=0;

	      i2c_stop();
	      busy = i2c_start();
	      if(busy)
	       {
	        delay_us(50);
	           ack=i2c_write(slave_address | WRITE_BIT);
	           if(ack == 1)
	            {
	             ack=i2c_write(reg_high_addr);
	                 if(ack == 1)
	                   {
	                 ack=i2c_write(reg_low_addr);
	                     if(ack == 1)
	                      {
	                        busy = i2c_start();
	                         if(busy)
	                           {
	                          ack = i2c_write(slave_address | READ_BIT);
	                           if(ack == 1)
	                              {
	                               *data=i2c_read(MASTER_NACK);
	                               i2c_stop();
	                               delay_us(10);
	                               return SWI2C_STATUS_OK;
	                              }
	                           }
	                           else
	                           {
	                             printf("register error\r\n");
	                             goto EXIT;
	                            }

	                   }
	                   else
	                   {
	                        printf("register error\r\n");
	                        goto EXIT;
	                   }
                     }
	              else
	              {
	                   printf("register error\r\n");
	                   goto EXIT;
	              }

	            }
	            else
	            {
	                printf("address error\r\n");
	                goto EXIT;

	            }
	     }
	     else
	     {
	    EXIT:
	         i2c_stop();
	         delay_us(10);
	         return SWI2C_STATUS_FAIL;
	     }

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
	CHAR8S ack=0,busy=0,data_number;



	      if(n_byte>255){
						return SWI2C_STATUS_FAIL; //over max set 255!!
	      				  }
	      i2c_stop();
	      busy = i2c_start();
	      if(busy)
	       {
	        delay_us(50);
	           ack=i2c_write(slave_address | WRITE_BIT);
	           if(ack == 1)
	            {
	             ack=i2c_write(reg_high_addr);
	                 if(ack == 1)
	                   {
		             ack=i2c_write(reg_low_addr);
	                       if(ack == 1)
	                       {
	                        busy = i2c_start();
	                         if(busy)
	                           {
	                         	ack = i2c_write(slave_address | READ_BIT);
	                           if(ack == 1)
	                                 {

						for(data_number=0;data_number<n_byte;data_number++)
						    {

							if(data_number == n_byte)
								{

								r_data_stream[data_number] = i2c_read(MASTER_NACK);
                                delay_us(3); // a little delay.
								break;

								}
							else
								{

								 r_data_stream[data_number] = i2c_read(MASTER_ACK);
                                 delay_us(3); // a little delay.

								}
						     }
							      // ALL read finish!!
				                           i2c_stop();
				                           delay_us(10);
				                           return SWI2C_STATUS_OK;

	                              	}
						else
	                           		{
	                             	printf("address error\r\n");
	                             	goto EXIT;
	                            	}
	                           }
	                           else
	                           {
	                             printf("register error\r\n");
	                             goto EXIT;
	                            }
				   }
	                       else
	                      {
	                           printf("register error\r\n");
	                           goto EXIT;
	                      }
	                   }
	                   else
	                   {
	                        printf("register error\r\n");
	                        goto EXIT;
	                   }
	            }
	            else
	            {
	                printf("address error\r\n");
	                goto EXIT;

	            }
	     }
	     else
	     {
	    EXIT:
	         i2c_stop();
	         delay_us(10);
		     return SWI2C_STATUS_FAIL;
	     }


}
//--------------------------------------------------------------------------------------------------


#endif		//#ifndef __PORTING_LAYER_FUNCTION___ 
